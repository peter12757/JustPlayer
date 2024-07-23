//
// Created by Administrator on 2024/7/22.
//

#include "FFStatistic.h"

FFStatistic::FFStatistic() {
    //todo

}

FFStatistic::~FFStatistic() {
    //todo
}

void FFStatistic::resetStatistic() {

    memset(this, 0, sizeof(FFStatistic));
    tcp_read_sampler.resetSpeedSampler(FFP_TCP_READ_SAMPLE_RANGE);
}
