//
// Created by Administrator on 2024/7/22.
//

#ifndef JUSTPLAYER_FFDEMUXCACHECONTROL_H
#define JUSTPLAYER_FFDEMUXCACHECONTROL_H

#include "config/FFdef.h"

class FFDemuxCacheControl {
public:
    int min_frames;
    int max_buffer_size;
    int high_water_mark_in_bytes;

    int first_high_water_mark_in_ms;
    int next_high_water_mark_in_ms;
    int last_high_water_mark_in_ms;
    int current_high_water_mark_in_ms;

    void resetDemuxCacheControl();
};


#endif //JUSTPLAYER_FFDEMUXCACHECONTROL_H
