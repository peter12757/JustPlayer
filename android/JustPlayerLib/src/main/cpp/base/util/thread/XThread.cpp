#include "XThread.h"
#include "../Util.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sstream>
#include <string>
#ifdef _WIN32
#include <process.h>
#include <MMSystem.h>
// #include <timeapi.h>
#else
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#endif

XThread::XThread(const char* name, uint32_t interval,ThreadMode mode)
  : m_interval(interval)
  , m_lastTimeoutStamp(0)
  , m_lastWakeupStamp(0)
  , m_wakeupTimes(0)
  , m_callWakeUpCount(0)
  , m_callResetWakeUpCount(0)
  , m_bQuit(true)
  , m_hThread(0)
  ,m_mode(mode)
#ifdef _WIN32
  , m_event(NULL)
  , m_exitMaxWaitTime(INFINITE)
#endif
{
  strncpy(m_name, name, kThreadNameSize);
  memset(m_runUseArray, 0, sizeof(m_runUseArray));

#ifndef _WIN32
  int result = pthread_mutex_init(&m_mutex, 0);
  if (result != 0)
  {
    LOGE("thread(%s) failed to crease mutex", m_name);
    return;
  }

  result = pthread_cond_init(&m_condition, 0);
  if (result != 0)
  {
    LOGW("thread(%s) failed to crease condition", m_name);
  }

  if (pipe(m_pipeFd) < 0)
  {
    LOGW("thread(%s) pipe error", m_name);
  }

  fcntl(m_pipeFd[0], F_SETFL, O_NONBLOCK);
  fcntl(m_pipeFd[1], F_SETFL, O_NONBLOCK);
#else
  // 注: m_interval=0在移动端是一直阻塞，对应WIN的是INFINITE
  if (m_interval == 0)
  {
    m_interval = INFINITE;
  }
#endif

  LOGI("thread(%s) %u %u construct", m_name, interval,mode);
}

XThread::~XThread()
{
  stopThread();
#ifndef _WIN32
  pthread_cond_destroy(&m_condition);
  pthread_mutex_destroy(&m_mutex);
  close(m_pipeFd[0]);
  close(m_pipeFd[1]);
#endif
}

#ifdef _WIN32
unsigned __stdcall XThread::threadFunc(void* pParam)
{
  XThread* pThread = static_cast<XThread*>(pParam);
  if (pThread)
  {
    pThread->loop();
  }
  return 0;
}

void _stdcall XThread::timerRoutine(PVOID lpParameter, BOOLEAN timerOrWaitFired)
{
  XThread* pThread = (XThread*)lpParameter;
  if (NULL != pThread)
  {
    pThread->onTimerRoutine();
  }
}
#else
void* XThread::threadFunc(void* pParam)
{
  XThread* pThread = static_cast<XThread*>(pParam);
  if (pThread)
  {
    pThread->loop();
  }
  return pParam;
}
#endif
void XThread::loop()
{
  onCreate();
  switch (m_mode) {
    case UNCONDITIONAL:{
      onUnconditionalLoop();
      break;
    }
    case URGENT:{
      // #ifdef ANDROID
      //    SetAndroidThreadPriority(m_name, THREAD_PRIORITY_URGENT_AUDIO);
      // #endif
      onUrgentLoop();
      break;
    }
    case TIMER:{
      onTimerLoop();
      break;
    }

  }
  showThreadPriority();
  onStop();
  resetWakeUpEvent();
}

void XThread::onUnconditionalLoop()
{
  while (!m_bQuit)
  {
    uint32_t now = Util::getTickCount();
    onThreadRun(now);
  }
}

void XThread::onUrgentLoop()
{
#ifdef _WIN32
  onTimerLoop();
#else
  for (;;)
  {
    const long int E6 = 1000000;
    const long int E9 = 1000 * E6;

    unsigned long timeout = m_interval;

    pthread_mutex_lock(&m_mutex);
    timespec end_at;
    timeval value;
    struct timezone time_zone;
    time_zone.tz_minuteswest = 0;
    time_zone.tz_dsttime = 0;
    gettimeofday(&value, &time_zone);
    end_at.tv_sec = value.tv_sec;
    end_at.tv_nsec = value.tv_usec * 1000;

    end_at.tv_sec += timeout / 1000;
    end_at.tv_nsec += (timeout - (timeout / 1000) * 1000) * E6;

    if (end_at.tv_nsec >= E9)
    {
      end_at.tv_sec++;
      end_at.tv_nsec -= E9;
    }
    pthread_cond_timedwait(&m_condition, &m_mutex, &end_at);
    pthread_mutex_unlock(&m_mutex);

    if (m_bQuit)
    {
      break;
    }

    uint32_t now = Util::getTickCount();
    checkPerformance(now);
    onThreadRun(now);

    m_lastTimeoutStamp = Util::getTickCount();
    monitorWakeupTimes(m_lastTimeoutStamp, m_lastTimeoutStamp - now);

    if (m_bQuit)
    {
      break;
    }
  }
#endif
}

void XThread::setMode(ThreadMode mode) {
  m_mode = mode;
}

// todo test pthread_cond_timedwait
void XThread::onTimerLoop()
{
  // 防止函数内地址越界
  char tmp[2048] = {0};
  memset(tmp, 0, 2048);

#ifdef _WIN32
  HANDLE hTimer = NULL;
  MMRESULT periodRet = timeBeginPeriod(1);
  uint32_t minInterval = (uint32_t)m_interval;
  uint32_t maxInterval = (uint32_t)(m_interval << 1);
  if (!CreateTimerQueueTimer(&hTimer, NULL, (WAITORTIMERCALLBACK)timerRoutine, this, minInterval, minInterval, 0))
  {
    LOGW("%s CreateTimerQueueTimer failed %u", m_name, GetLastError());
  }
#else
  fd_set fdSetRead;
#endif

  for (;;)
  {
#ifdef _WIN32
    if (minInterval != m_interval)
    {
      if (hTimer != NULL)
      {
        DeleteTimerQueueTimer(NULL, hTimer, NULL);
      }
      minInterval = (uint32_t)m_interval;
      maxInterval = (uint32_t)(m_interval << 1);
      if (!CreateTimerQueueTimer(&hTimer, NULL, (WAITORTIMERCALLBACK)timerRoutine, this, minInterval, minInterval, 0))
      {
        LOGW("%s CreateTimerQueueTimer failed %u", m_name, GetLastError());
      }
    }
    WaitForSingleObject(m_event, maxInterval);
#else
    FD_ZERO(&fdSetRead);
    FD_SET(m_pipeFd[0], &fdSetRead);
    int maxFd = 0;
    if (m_pipeFd[0] > maxFd)
    {
      maxFd = m_pipeFd[0];
    }

    struct timeval tv;
    tv.tv_sec = m_interval / 1000;
    tv.tv_usec = (m_interval % 1000) * 1000;

    int ret = select(maxFd + 1, &fdSetRead, NULL, NULL, m_interval == 0 ? NULL : &tv);
    if (ret < 0)
    {
      LOGE("%s thread onLoop select error %d", m_name, ret);
      ::usleep(20 * 1000);
      continue;
    }
    else if (ret > 0)
    {
      if (FD_ISSET(m_pipeFd[0], &fdSetRead))
      {
        resetWakeUpEvent();
      }
    }
#endif
    if (m_bQuit)
    {
      break;
    }

    uint32_t now = Util::getTickCount();
    if (m_interval != 0)
    {
      checkPerformance(now);
    }

    onThreadRun(now);

    m_lastTimeoutStamp = Util::getTickCount();
    monitorWakeupTimes(m_lastTimeoutStamp, m_lastTimeoutStamp - now);

    if (m_bQuit)
    {
      break;
    }
  }

#ifdef _WIN32
  if (NULL != hTimer)
  {
    DeleteTimerQueueTimer(NULL, hTimer, NULL);
    hTimer = NULL;
  }
  if (TIMERR_NOERROR == periodRet)
  {
    timeEndPeriod(1);
  }
#endif
}

void XThread::stopThread()
{
  if (m_bQuit)
  {
     LOGE("faild to stop thread, thread(%s) has been stopped", m_name);
    return;
  }

  m_bQuit = true;

#ifdef _WIN32
  if (m_event == 0)
  {
    LOGE("!!!bug %s thread event is null when stop", m_name);
    return;
  }

  if (m_hThread == 0)
  {
    LOGE("!!!bug %s thread handle is null when stop", m_name);
    CloseHandle(m_event);
    m_event = 0;
    return;
  }

  LOGE("stop %s thread", m_name);
  SetEvent(m_event);

  DWORD ret = WaitForSingleObject(m_hThread, m_exitMaxWaitTime);
  if (WAIT_TIMEOUT == ret)
  {
    TerminateThread(m_hThread, -1);
  }

  CloseHandle(m_hThread);
  m_hThread = 0;
  CloseHandle(m_event);
  m_event = 0;
  m_lastTimeoutStamp = 0;
#else
  if (m_hThread == 0)
  {
    LOGD("!!!bug %s thread handle is null when stop", m_name);
    return;
  }

  LOGD("stop %s thread", m_name);
  wakeUp();

  pthread_join(m_hThread, NULL);
  m_hThread = 0;
#endif
  LOGI("thread(%s) stop successfully", m_name);
}

void XThread::startThread()
{
  if (!m_bQuit)
  {
    LOGW("failed to start thread, thread(%s) has been started", m_name);
    return;
  }

  m_bQuit = false;
#ifdef _WIN32
  m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
  if (m_event == 0)
  {
    m_bQuit = true;
    LOGI("!!!bug %s thread event is null when start", m_name);
    return;
  }

  unsigned threadID;
  m_hThread = (HANDLE)_beginthreadex(NULL, 0, XThread::threadFunc, this, 0, &threadID);
  if (m_hThread == 0)
  {
    LOGI("!!!bug %s thread handle is null when start, error %u", m_name, GetLastError());
    CloseHandle(m_event);
    m_event = 0;
    m_bQuit = true;
    return;
  }

  SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);
#else
  int err = 0;
  if (m_mode == URGENT)
  {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (pthread_attr_init(&attr) != 0)
    {
      LOGW("failed to call pthread_attr_init in thread(%s)", m_name);
    }

    // Set real-time round-robin policy.
    if (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0)
    {
      LOGW("failed to call pthread_attr_setschedpolicy in thread(%s)", m_name);
    }

    struct sched_param param;
    memset(&param, 0, sizeof(param));
    param.sched_priority = 6; // 6 = HIGH
    if (pthread_attr_setschedparam(&attr, &param) != 0)
    {
      LOGW("failed to call pthread_attr_setschedparam in thread(%s)", m_name);
    }

    err = pthread_create(&m_hThread, &attr, &threadFunc, this);
  }
  else
  {
    err = pthread_create(&m_hThread, NULL, &threadFunc, this);
  }

  if (err != 0)
  {
    m_hThread = 0;
    m_bQuit = true;
    LOGE("failed to createEngine thread(%s) %u", m_name, err);
    return;
  }
#endif
  LOGI("start thread(%s) %s interval %u", m_name, m_mode == URGENT ? "urgent" : "unurgent", m_interval);
}

void XThread::wakeUp()
{
#ifdef _WIN32
  if (m_event != 0)
  {
    SetEvent(m_event);
  }
#else
  const char* pCh = "a";
  if (write(m_pipeFd[1], pCh, 1) <= 0)
  {
    LOGI("thread(%s) wakeup error %s", m_name, strerror(errno));
  }
#endif
  ++m_callWakeUpCount;
}

void XThread::resetWakeUpEvent()
{
#ifndef _WIN32
  char buf[2048] = {0};
  if (read(m_pipeFd[0], buf, 2048) <= 0)
  {
    int errorCode = errno;
    if (errorCode != EAGAIN)
    {
      LOGI(
           "thread(%s) resetWakeUpEvent failed errCode %d info %s",
           m_name,
           errorCode,
           strerror(errorCode));
    }
  }
#endif
  ++m_callResetWakeUpCount;
}

bool XThread::isQuit() const { return m_bQuit; }

void XThread::onCreate() { LOGI("thread(%s) created %u", m_name, m_interval); }

void XThread::onStop() { LOGI("exit thread(%s) %u", m_name, m_interval); }

void XThread::checkPerformance(uint32_t now)
{
  if (m_lastTimeoutStamp == 0)
  {
    return;
  }

  if (Util::isBiggerUint32(m_lastTimeoutStamp, now))
  {
    LOGI(
         "thread(%s) system time has been modified, last %u cur %u diff %u",
         m_name,
         m_lastTimeoutStamp,
         now,
         m_lastTimeoutStamp - now);
    return;
  }

  const uint32_t kMaxInterval = 100;
  uint32_t deltaT = now - m_lastTimeoutStamp;
  if (deltaT > kMaxInterval + m_interval)
  {
    LOGW("thread(%s) process spend too long %u %u", m_name, m_interval, deltaT);
  }
}

void XThread::monitorWakeupTimes(uint32_t now, uint32_t runUse)
{
  ++m_wakeupTimes;
  for (uint32_t i = 0; i < kMaxRunUseArraySize; ++i)
  {
    if (runUse <= kRunUseThreshold[i])
    {
      ++m_runUseArray[i];
      break;
    }
  }

  if (m_lastWakeupStamp == 0)
  {
    m_lastWakeupStamp = now;
    return;
  }

  const uint32_t kTimeout = 32 * 1000;
  if (Util::isBiggerUint32(m_lastWakeupStamp + kTimeout, now))
  {
    return;
  }

  std::ostringstream oss;
  for (uint32_t i = 0; i < kMaxRunUseArraySize; ++i)
  {
    oss << " " << (int)kRunUseThreshold[i] << ":" << m_runUseArray[i];
  }

  LOGI(
       "thread(%s) in past %u ms, wakeup %u times callWakeupCount %u "
       "callResetWakeUpCount %u runUse(%s)",
       m_name,
       now - m_lastWakeupStamp,
       m_wakeupTimes,
       m_callWakeUpCount,
       m_callResetWakeUpCount,
       oss.str().c_str());

  m_lastWakeupStamp = now;
  m_wakeupTimes = 0;
  m_callWakeUpCount = 0;
  m_callResetWakeUpCount = 0;
  memset(m_runUseArray, 0, sizeof(m_runUseArray));
}

void XThread::showThreadPriority()
{
#ifndef _WIN32
  pthread_attr_t attr;
  int ret = pthread_attr_init(&attr);
  if (ret != 0)
  {
    LOGW("failed to call pthread_attr_init in thread(%s)", m_name);
    return;
  }

  int policy = 0;
  ret = pthread_attr_getschedpolicy(&attr, &policy);
  if (ret != 0)
  {
    LOGW("failed to call pthread_attr_getschedpolicy in thread(%s)", m_name);
    return;
  }

  struct sched_param param;
  memset(&param, 0, sizeof(param));
  ret = pthread_attr_getschedparam(&attr, &param);
  if (ret != 0)
  {
    LOGW("failed to call pthread_attr_getschedparam in thread(%s)", m_name);
    return;
  }

  int maxPriority = sched_get_priority_max(policy);
  int minPriority = sched_get_priority_min(policy);

  LOGI(
       "thread priority in thread(%s), policy %d minPriority %d maxPriority %d "
       "curPriority %u",
       m_name,
       policy,
       minPriority,
       maxPriority,
       param.sched_priority);
#endif
}

void XThread::resetInterval(uint32_t interval) { m_interval = interval; }

void XThread::onTimerRoutine()
{
#ifdef _WIN32
  SetEvent(m_event);
#endif
}

XTaskThread::XTaskThread(const char* name, uint32_t interval, ITask* task) : XThread(name, interval), m_task(task)
{

}

void XTaskThread::onThreadRun(uint32_t now)
{
  if (!m_task)
  {
    LOGE("error!!!!!!!!");
    return;
  }

  m_task->proc();
}
