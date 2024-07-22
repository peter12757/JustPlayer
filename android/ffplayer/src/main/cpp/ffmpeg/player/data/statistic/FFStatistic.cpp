//
// Created by Administrator on 2024/7/22.
//

#include "FFStatistic.h"

void FFStatistic::resetStatistic(FFStatistic *dcc) {
    memset(this, 0, sizeof(FFStatistic));
    dcc->tcp_read_sampler.resetSpeedSampler(FFP_TCP_READ_SAMPLE_RANGE);
}
