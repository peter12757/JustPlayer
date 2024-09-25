//
// Created by Peter Xi on 2022/5/29.
//

#ifndef ANDROIDTEST_ITIMERHANDLER_H
#define ANDROIDTEST_ITIMERHANDLER_H

namespace XUtil {

    class ITimerHandler {
    public:
        virtual ~ITimerHandler() {};

        virtual void onTimeout() = 0;
    };
}

#endif //ANDROIDTEST_ITIMERHANDLER_H
