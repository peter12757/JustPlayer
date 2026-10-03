#pragma once
#include <deque>
#include <map>
#include "IThreadHandler.h"
#include "XThread.h"
#include "TimerPool.h"

/**
 * class HandleThread
 * @brief
 * 在这条线程中，实现了系统定时器、处理大部分外部的调用以及所有的网络消息。
 * 基本上系统的绝大部分主要任务都是在这条线程完成的，因此这样一来也少处理很
 * 多线程安全问题。
 */
class HandleThread : public XThread
{
 public:
  HandleThread(IThreadHandler* pHandler);
  virtual ~HandleThread();

 public:
  virtual void stopHandleThread();

 private:
  virtual void onCreate();
  virtual void onStop();
  virtual void onThreadRun(uint32_t now);

 private:
  void checkTimer(uint32_t now);
  void checkResetWakeupEvent();

 private:
  IThreadHandler* m_pHandler;
  uint32_t m_lastTimerStamp;
};
