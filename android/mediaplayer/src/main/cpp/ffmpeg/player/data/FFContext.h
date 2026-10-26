//
// Created by Administrator on 2026/10/27.
//

#ifndef JUSTPLAYER_FFCONTEXT_H
#define JUSTPLAYER_FFCONTEXT_H


#include <avformat.h>
#include "FFdef.h"

class FFContext {
/* options specified by the user */
    const AVInputFormat *file_iformat;
    const char *input_filename;
    const char *window_title;
    int default_width  = 640;
    int default_height = 480;
    int screen_width  = 0;
    int screen_height = 0;
    int screen_left = 0;
    int screen_top = 0;
    int audio_disable;
    int video_disable;
    int subtitle_disable;
    const char* wanted_stream_spec[AVMEDIA_TYPE_NB] = {0};
    int seek_by_bytes = -1;
    float seek_interval = 10;
    int display_disable;
    int borderless;
    int alwaysontop;
    int startup_volume = 100;
    int show_status = -1;
    int av_sync_type = AV_SYNC_AUDIO_MASTER;
    int64_t start_time = AV_NOPTS_VALUE;
    int64_t duration = AV_NOPTS_VALUE;
    int fast = 0;
    int genpts = 0;
    int lowres = 0;
    int decoder_reorder_pts = -1;
    int autoexit;
    int exit_on_keydown;
    int exit_on_mousedown;
    int loop = 1;
    int framedrop = -1;
    int infinite_buffer = -1;
    enum ShowMode show_mode = SHOW_MODE_NONE;
    const char *audio_codec_name;
    const char *subtitle_codec_name;
    const char *video_codec_name;
    double rdftspeed = 0.02;
    int64_t cursor_last_shown;
    int cursor_hidden = 0;
    const char **vfilters_list = NULL;
    int nb_vfilters = 0;
    char *afilters = NULL;
    int autorotate = 1;
    int find_stream_info = 1;
    int filter_nbthreads = 0;
    int enable_vulkan = 0;
    char *vulkan_params = NULL;
    char *video_background = NULL;
    const char *hwaccel = NULL;

/* current context */
    int is_full_screen;
    int64_t audio_callback_time;

#define FF_QUIT_EVENT    (SDL_USEREVENT + 2)

//    SDL_Window *window;
//    SDL_Renderer *renderer;
//    SDL_RendererInfo renderer_info = {0};
//    SDL_AudioDeviceID audio_dev;
//
//    VkRenderer *vk_renderer;

//    const TextureFormatEntry sdl_texture_format_map[19] = {
//            { AV_PIX_FMT_RGB8,           SDL_PIXELFORMAT_RGB332 },
//            { AV_PIX_FMT_RGB444,         SDL_PIXELFORMAT_RGB444 },
//            { AV_PIX_FMT_RGB555,         SDL_PIXELFORMAT_RGB555 },
//            { AV_PIX_FMT_BGR555,         SDL_PIXELFORMAT_BGR555 },
//            { AV_PIX_FMT_RGB565,         SDL_PIXELFORMAT_RGB565 },
//            { AV_PIX_FMT_BGR565,         SDL_PIXELFORMAT_BGR565 },
//            { AV_PIX_FMT_RGB24,          SDL_PIXELFORMAT_RGB24 },
//            { AV_PIX_FMT_BGR24,          SDL_PIXELFORMAT_BGR24 },
//            { AV_PIX_FMT_0RGB32,         SDL_PIXELFORMAT_RGB888 },
//            { AV_PIX_FMT_0BGR32,         SDL_PIXELFORMAT_BGR888 },
//            { AV_PIX_FMT_NE(RGB0, 0BGR), SDL_PIXELFORMAT_RGBX8888 },
//            { AV_PIX_FMT_NE(BGR0, 0RGB), SDL_PIXELFORMAT_BGRX8888 },
//            { AV_PIX_FMT_RGB32,          SDL_PIXELFORMAT_ARGB8888 },
//            { AV_PIX_FMT_RGB32_1,        SDL_PIXELFORMAT_RGBA8888 },
//            { AV_PIX_FMT_BGR32,          SDL_PIXELFORMAT_ABGR8888 },
//            { AV_PIX_FMT_BGR32_1,        SDL_PIXELFORMAT_BGRA8888 },
//            { AV_PIX_FMT_YUV420P,        SDL_PIXELFORMAT_IYUV },
//            { AV_PIX_FMT_YUYV422,        SDL_PIXELFORMAT_YUY2 },
//            { AV_PIX_FMT_UYVY422,        SDL_PIXELFORMAT_UYVY },
//    };
};


#endif //JUSTPLAYER_FFCONTEXT_H
