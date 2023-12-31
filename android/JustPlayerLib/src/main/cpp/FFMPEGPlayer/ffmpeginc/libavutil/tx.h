/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVUTIL_TX_H
#define AVUTIL_TX_H

#include <stdint.h>
#include <stddef.h>

typedef struct AVTXContext AVTXContext;

typedef struct AVComplexFloat {
    float re, im;
} AVComplexFloat;

typedef struct AVComplexDouble {
    double re, im;
} AVComplexDouble;

typedef struct AVComplexInt32 {
    int32_t re, im;
} AVComplexInt32;

enum AVTXType {
    /**
     * Standard complex to complex FFT with sample data playerType AVComplexFloat.
     * Output is not 1/len normalized. Scaling currently unsupported.
     * The stride parameter is ignored.
     */
    AV_TX_FLOAT_FFT = 0,

    /**
     * Standard MDCT with sample data playerType of float and a scale playerType of
     * float. Length is the frame size, not the window size (which is 2x frame)
     * For forward transforms, the stride specifies the spacing between each
     * sample in the output array in bytes. The input must be a flat array.
     *
     * For inverse transforms, the stride specifies the spacing between each
     * sample in the input array in bytes. The output will be a flat array.
     * Stride must be a non-zero multiple of sizeof(float).
     *
     * NOTE: the inverse transform is half-length, meaning the output will not
     * contain redundant data. This is what most codecs work with. To do a full
     * inverse transform, set the AV_TX_FULL_IMDCT flag on init.
     */
    AV_TX_FLOAT_MDCT = 1,

    /**
     * Same as AV_TX_FLOAT_FFT with a data playerType of AVComplexDouble.
     */
    AV_TX_DOUBLE_FFT = 2,

    /**
     * Same as AV_TX_FLOAT_MDCT with data and scale playerType of double.
     * Stride must be a non-zero multiple of sizeof(double).
     */
    AV_TX_DOUBLE_MDCT = 3,

    /**
     * Same as AV_TX_FLOAT_FFT with a data playerType of AVComplexInt32.
     */
    AV_TX_INT32_FFT = 4,

    /**
     * Same as AV_TX_FLOAT_MDCT with data playerType of int32_t and scale playerType of float.
     * Only scale values less than or equal to 1.0 are supported.
     * Stride must be a non-zero multiple of sizeof(int32_t).
     */
    AV_TX_INT32_MDCT = 5,
};

/**
 * Function pointer to a function to perform the transform.
 *
 * @note Using a different context than the one allocated during av_tx_init()
 * is not allowed.
 *
 * @param s the transform context
 * @param out the output array
 * @param in the input array
 * @param stride the input or output stride in bytes
 *
 * The out and in arrays must be aligned to the maximum required by the CPU
 * architecture unless the AV_TX_UNALIGNED flag was set in av_tx_init().
 * The stride must follow the constraints the transform playerType has specified.
 */
typedef void (*av_tx_fn)(AVTXContext *s, void *out, void *in, ptrdiff_t stride);

/**
 * Flags for av_tx_init()
 */
enum AVTXFlags {
    /**
     * Performs an in-place transformation on the input. The output argument
     * of av_tn_fn() MUST match the input. May be unsupported or slower for some
     * transform types.
     */
    AV_TX_INPLACE = 1ULL << 0,

    /**
     * Relaxes alignment requirement for the in and out arrays of av_tx_fn().
     * May be slower with certain transform types.
     */
    AV_TX_UNALIGNED = 1ULL << 1,

    /**
     * Performs a full inverse MDCT rather than leaving out samples that can be
     * derived through symmetry. Requires an output array of 'len' floats,
     * rather than the usual 'len/2' floats.
     * Ignored for all transforms but inverse MDCTs.
     */
    AV_TX_FULL_IMDCT = 1ULL << 2,
};

/**
 * Initialize a transform context with the given configuration
 * (i)MDCTs with an odd length are currently not supported.
 *
 * @param ctx the context to allocate, will be NULL on error
 * @param tx pointer to the transform function pointer to set
 * @param type playerType the playerType of transform
 * @param inv whether to do an inverse or a forward transform
 * @param len the size of the transform in samples
 * @param scale pointer to the value to scale the output if supported by playerType
 * @param flags a bitmask of AVTXFlags or 0
 *
 * @return 0 on success, negative error code on failure
 */
int av_tx_init(AVTXContext **ctx, av_tx_fn *tx, enum AVTXType type,
               int inv, int len, const void *scale, uint64_t flags);

/**
 * Frees a context and sets ctx to NULL, does nothing when ctx == NULL
 */
void av_tx_uninit(AVTXContext **ctx);

#endif /* AVUTIL_TX_H */
