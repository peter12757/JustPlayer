#include "HandleThread.h"
#include "../util/Util.h"

using namespace XUtil;

HandleThread::HandleThread(IThreadHandler* pHandler)
  : XThread("HandleThread", 30)
  , m_pHandler(pHandler)
  , m_lastTimerStamp(0)
{
}

HandleThread::~HandleThread()
{
  stopThread();
  m_pHandler = NULL;
  LOGI("end of link thread destruction");
}

void HandleThread::onCreate()
{
  LOGI( "handle thread created");
  m_pHandler->onThreadStart();
}

void HandleThread::onStop() { m_pHandler->onThreadStop(); }

void HandleThread::stopHandleThread()
{
  LOGI( "exit link thread");
  stopThread();
}

void HandleThread::onThreadRun(uint32_t now)
{
  m_pHandler->onThreadRun(now);
  checkResetWakeupEvent();
  checkTimer(now);
}

void HandleThread::checkTimer(uint32_t now)
{
  if (m_lastTimerStamp == 0)
  {
    m_lastTimerStamp = now;
    return;
  }

  const uint32_t kTimerTimeout = 50;
  if (Util::isBiggerUint32(now, m_lastTimerStamp) && now - m_lastTimerStamp >= kTimerTimeout)
  {
    TimerPool::instance()->onTimeout(now);
    m_lastTimerStamp = now;
  }
}

void HandleThread::checkResetWakeupEvent()
{
  if (m_pHandler->isThreadAbleToSleep())
  {
    resetWakeUpEvent();
  }
}
