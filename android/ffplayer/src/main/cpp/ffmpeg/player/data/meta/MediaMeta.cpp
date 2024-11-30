//
// Created by Administrator on 2024/7/24.
//

#include "MediaMeta.h"

MediaMeta::MediaMeta() {
}

MediaMeta::~MediaMeta() {
    if (dict) {
        av_dict_free(&dict);
    }

    if (children) {
        for(int i = 0; i < children_count; ++i) {
            MediaMeta *child = children[i];
            if (child) {
                SafeDelete(child);
            }
        }
        free(children);
        children = NULL;
    }
}

void MediaMeta::reset() {
    if (dict)
        av_dict_free(&dict);
}

void MediaMeta::appendChild(MediaMeta *child) {
    if (!child)
        return;

    if (!children) {
        children = (MediaMeta **)calloc(IJK_META_INIT_CAPACITY, sizeof(MediaMeta *));
        if (!children)
            return;
        children_count    = 0;
        children_capacity = IJK_META_INIT_CAPACITY;
    } else if (children_count >= children_capacity) {
        size_t new_capacity = children_capacity * 2;
        MediaMeta **new_children = (MediaMeta **)calloc(new_capacity, sizeof(MediaMeta *));
        if (!new_children)
            return;

        memcpy(new_children, children, children_capacity * sizeof(MediaMeta *));
        free(children);
        children          = new_children;
        children_capacity = new_capacity;
    }

    children[children_count] = child;
    children_count++;

}

void MediaMeta::set_int64_l(const char *name, int64_t value) {
    av_dict_set_int(&dict, name, value, 0);
}

void MediaMeta::set_string_l(const char *name, const char *value) {
    av_dict_set(&dict, name, value, 0);
}

static int64_t get_bit_rate(AVCodecParameters *codecpar)
{
    int64_t bit_rate;
    int bits_per_sample;

    switch (codecpar->codec_type) {
        case AVMEDIA_TYPE_VIDEO:
        case AVMEDIA_TYPE_DATA:
        case AVMEDIA_TYPE_SUBTITLE:
        case AVMEDIA_TYPE_ATTACHMENT:
            bit_rate = codecpar->bit_rate;
            break;
        case AVMEDIA_TYPE_AUDIO:
            bits_per_sample = av_get_bits_per_sample(codecpar->codec_id);
            bit_rate = bits_per_sample ? codecpar->sample_rate * codecpar->channels * bits_per_sample : codecpar->bit_rate;
            break;
        default:
            bit_rate = 0;
            break;
    }
    return bit_rate;
}

void MediaMeta::set_avformat_context_l(struct AVFormatContext *ic) {
    if (!ic)
        return;

    if (ic->iformat && ic->iformat->name)
        set_string_l(IJKM_KEY_FORMAT, ic->iformat->name);

    if (ic->duration != AV_NOPTS_VALUE)
        set_int64_l(IJKM_KEY_DURATION_US, ic->duration);

    if (ic->start_time != AV_NOPTS_VALUE)
        set_int64_l(IJKM_KEY_START_US, ic->start_time);

    if (ic->bit_rate)
        set_int64_l(IJKM_KEY_BITRATE, ic->bit_rate);
    MediaMeta *stream_meta = NULL;
    for (int i = 0; i < ic->nb_streams; i++) {
        stream_meta->reset();
        AVStream *st = ic->streams[i];
        if (!st || !st->codecpar)
            continue;
        stream_meta = new MediaMeta();
        AVCodecParameters *codecpar = st->codecpar;
        const char *codec_name = avcodec_get_name(codecpar->codec_id);
        if (codec_name)
            stream_meta->set_string_l( IJKM_KEY_CODEC_NAME, codec_name);
        if (codecpar->profile != FF_PROFILE_UNKNOWN) {
            const AVCodec *codec = avcodec_find_decoder(codecpar->codec_id);
            if (codec) {
                stream_meta->set_int64_l(IJKM_KEY_CODEC_PROFILE_ID, codecpar->profile);
                const char *profile = av_get_profile_name(codec, codecpar->profile);
                if (profile)
                    stream_meta->set_string_l(IJKM_KEY_CODEC_PROFILE, profile);
                if (codec->long_name)
                    stream_meta->set_string_l(IJKM_KEY_CODEC_LONG_NAME, codec->long_name);
                stream_meta->set_int64_l(IJKM_KEY_CODEC_LEVEL, codecpar->level);
                if (codecpar->format != AV_PIX_FMT_NONE)
                    stream_meta->set_string_l(IJKM_KEY_CODEC_PIXEL_FORMAT, av_get_pix_fmt_name((AVPixelFormat)(codecpar->format)));
            }
        }

        int64_t bitrate = get_bit_rate(codecpar);
        if (bitrate > 0) {
            stream_meta->set_int64_l(IJKM_KEY_BITRATE, bitrate);
        }

        switch (codecpar->codec_type) {
            case AVMEDIA_TYPE_VIDEO: {
                stream_meta->set_string_l(IJKM_KEY_TYPE, IJKM_VAL_TYPE__VIDEO);

                if (codecpar->width > 0)
                    stream_meta->set_int64_l(IJKM_KEY_WIDTH, codecpar->width);
                if (codecpar->height > 0)
                    stream_meta->set_int64_l(IJKM_KEY_HEIGHT, codecpar->height);
                if (st->sample_aspect_ratio.num > 0 && st->sample_aspect_ratio.den > 0) {
                    stream_meta->set_int64_l(IJKM_KEY_SAR_NUM, codecpar->sample_aspect_ratio.num);
                    stream_meta->set_int64_l(IJKM_KEY_SAR_DEN, codecpar->sample_aspect_ratio.den);
                }

                if (st->avg_frame_rate.num > 0 && st->avg_frame_rate.den > 0) {
                    stream_meta->set_int64_l(IJKM_KEY_FPS_NUM, st->avg_frame_rate.num);
                    stream_meta->set_int64_l(IJKM_KEY_FPS_DEN, st->avg_frame_rate.den);
                }
                if (st->r_frame_rate.num > 0 && st->r_frame_rate.den > 0) {
                    stream_meta->set_int64_l(IJKM_KEY_TBR_NUM, st->avg_frame_rate.num);
                    stream_meta->set_int64_l(IJKM_KEY_TBR_DEN, st->avg_frame_rate.den);
                }
                break;
            }
            case AVMEDIA_TYPE_AUDIO: {
                stream_meta->set_string_l(IJKM_KEY_TYPE, IJKM_VAL_TYPE__AUDIO);

                if (codecpar->sample_rate)
                    stream_meta->set_int64_l(IJKM_KEY_SAMPLE_RATE, codecpar->sample_rate);
                if (codecpar->channel_layout)
                    stream_meta->set_int64_l(IJKM_KEY_CHANNEL_LAYOUT, codecpar->channel_layout);
                break;
            }
            case AVMEDIA_TYPE_SUBTITLE: {
                stream_meta->set_string_l(IJKM_KEY_TYPE, IJKM_VAL_TYPE__TIMEDTEXT);
                break;
            }
            default: {
                stream_meta->set_string_l(IJKM_KEY_TYPE, IJKM_VAL_TYPE__UNKNOWN);
                break;
            }
        }

        AVDictionaryEntry *lang = av_dict_get(st->metadata, "language", NULL, 0);
        if (lang && lang->value)
            stream_meta->set_string_l(IJKM_KEY_LANGUAGE, lang->value);

        appendChild(stream_meta);
        SafeDelete(stream_meta);
    }
}

const char *MediaMeta::get_string_l(const char *name) {
    if (!dict)
        return NULL;

    AVDictionaryEntry *entry = av_dict_get(dict, name, NULL, 0);
    if (!entry)
        return NULL;

    return entry->value;
}

int64_t MediaMeta::get_int64_l(const char *name, int64_t defaultValue) {
    if (!dict)
        return defaultValue;

    AVDictionaryEntry *entry = av_dict_get(dict, name, NULL, 0);
    if (!entry || !entry->value)
        return defaultValue;

    return atoll(entry->value);
}

size_t MediaMeta::get_children_count_l() {
    if (!children)
        return 0;

    return children_count;
}

MediaMeta *MediaMeta::get_child_l(size_t index) {
    if (index >= children_count)
        return nullptr;

    return children[index];
}











