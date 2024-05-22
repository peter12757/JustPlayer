#pragma once
#include "IntTypes.h"

class IThreadHandler
{
 public:
  virtual void onThreadStart() = 0;
  virtual void onThreadRun(uint32_t now) = 0;
  virtual void onThreadStop() = 0;
  virtual bool isThreadAbleToSleep() = 0;
};
