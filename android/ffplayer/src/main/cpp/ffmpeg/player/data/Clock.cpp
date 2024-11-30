//
// Created by Administrator on 2024/8/8.
//

#include "Clock.h"

Clock::Clock(int *queue_serial)
:speed(1.0)
,paused(false)
,queue_serial(queue_serial)
{
    set_clock(NAN,-1);
}

Clock::~Clock() {

}
