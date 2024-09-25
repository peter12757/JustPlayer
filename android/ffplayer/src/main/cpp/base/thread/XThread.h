#pragma once
#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#endif
#include <stdint.h>
#include "../util/Util.h"

const uint32_t kThreadNameSize = 100;
const uint32_t kMaxRunUseArraySize = 16;
const uint32_t kRunUseThreshold[kMaxRunUseArraySize] =
  {5, 10, 15, 20, 25, 30, 40, 50, 60, 70, 80, 100, 200, 500, 1000, (uint32_t)-1};

enum ThreadMode {
    UNCONDITIONAL,  //无限循环
    URGENT, //快速的，使用线程锁
    TIMER,  //使用select
};

class XThread
{
 public:
  XThread(const char* name, uint32_t interval, ThreadMode mode = TIMER);
  virtual ~XThread();

 public:
#ifdef _WIN32
  static unsigned __stdcall threadFunc(void* p);
  static void __stdcall timerRoutine(PVOID lpParameter, BOOLEAN timerOrWaitFired);
#else
  static void* threadFunc(void* p);
#endif

 public:
  virtual void loop();
  virtual void onThreadRun(uint32_t now) = 0;
  virtual void onCreate();
  virtual void onStop();

 public:
  void stopThread();
  void startThread();
  void wakeUp();
  void resetInterval(uint32_t interval);
  bool isQuit() const;
  void resetWakeUpEvent();
  void setMode(ThreadMode mode);


 private:
  void onUnconditionalLoop();
  void onTimerLoop();
  void onUrgentLoop();
  void onTimerRoutine();
  void checkPerformance(uint32_t now);
  void monitorWakeupTimes(uint32_t now, uint32_t runUse);
  void showThreadPriority();

 protected:
  uint32_t m_interval;
  ThreadMode m_mode;

 private:
  uint32_t m_lastTimeoutStamp;
  uint32_t m_lastWakeupStamp;
  uint32_t m_wakeupTimes;
  uint32_t m_callWakeUpCount;
  uint32_t m_callResetWakeUpCount;
  uint32_t m_runUseArray[kMaxRunUseArraySize];
  char m_name[kThreadNameSize];
  bool m_bQuit;
#ifdef _WIN32
  HANDLE m_hThread;
  HANDLE m_event;
  uint32_t m_exitMaxWaitTime;
#else
  int m_pipeFd[2];
  pthread_t m_hThread;
  pthread_cond_t m_condition;
  pthread_mutex_t m_mutex;
#endif
};

//
class ITask
{
 public:
  virtual void proc() = 0;
};

// generic task as a delegate template

template <class XSubject> class TGenericTask : public ITask
{
 public:
  typedef void (XSubject::*Proc_PMF)();

  TGenericTask() : m_pSubject(NULL), m_pmfProc(NULL) {}

  void delegateBy(XSubject* pSubject, Proc_PMF pmfProc)
  {
    m_pSubject = pSubject;
    m_pmfProc = pmfProc;
  }

  virtual void proc() { (m_pSubject->*m_pmfProc)(); }

 private:
  XSubject* m_pSubject;
  Proc_PMF m_pmfProc;
};

#ifdef __OBJC__
class TObjcTask : public ITask
{
 public:
  TObjcTask() : m_subject(nil), m_proc(nil) {}

  void delegateBy(id subject, SEL procSelector)
  {
    m_subject = subject;
    m_proc = procSelector;
  }

  virtual void proc() { [m_subject performSelector:m_proc]; }

 private:
  id m_subject;
  SEL m_proc;
};
#endif

class XTaskThread : public XThread
{
 public:
  XTaskThread(const char* name, uint32_t interval, ITask* task = 0);
  void setTask(ITask* task) { m_task = task; }

  virtual void onThreadRun(uint32_t now);

 private:
  ITask* m_task;
};
