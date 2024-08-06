//
// Created by Administrator on 2024/7/22.
//

#include "FFDemuxCacheControl.h"

void FFDemuxCacheControl::resetDemuxCacheControl() {
    min_frames                = DEFAULT_MIN_FRAMES;
    max_buffer_size           = MAX_QUEUE_SIZE;
    high_water_mark_in_bytes  = DEFAULT_HIGH_WATER_MARK_IN_BYTES;

    first_high_water_mark_in_ms    = DEFAULT_FIRST_HIGH_WATER_MARK_IN_MS;
    next_high_water_mark_in_ms     = DEFAULT_NEXT_HIGH_WATER_MARK_IN_MS;
    last_high_water_mark_in_ms     = DEFAULT_LAST_HIGH_WATER_MARK_IN_MS;
    current_high_water_mark_in_ms  = DEFAULT_FIRST_HIGH_WATER_MARK_IN_MS;

}
