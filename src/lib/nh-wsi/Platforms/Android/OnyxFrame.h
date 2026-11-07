#ifndef NH_WSI_ANDROID_ONYX_FRAME_H
#define NH_WSI_ANDROID_ONYX_FRAME_H

#include <stdbool.h>
#include <stdint.h>

// Odd generations have a pen down; even generations await a finalized app frame.
typedef struct nh_wsi_OnyxFrame {
    uint64_t generation;
    uint64_t posted;
} nh_wsi_OnyxFrame;

static inline void nh_wsi_beginOnyxStroke(nh_wsi_OnyxFrame *frame)
{
    uint64_t generation = __atomic_load_n(&frame->generation, __ATOMIC_SEQ_CST);
    if (!(generation & 1))
        __atomic_add_fetch(&frame->generation, 1, __ATOMIC_SEQ_CST);
}

static inline void nh_wsi_endOnyxStroke(nh_wsi_OnyxFrame *frame)
{
    uint64_t generation = __atomic_load_n(&frame->generation, __ATOMIC_SEQ_CST);
    if (generation & 1)
        __atomic_add_fetch(&frame->generation, 1, __ATOMIC_SEQ_CST);
}

static inline bool nh_wsi_canPostOnyxFrame(nh_wsi_OnyxFrame *frame, uint64_t generation)
{
    return generation && !(generation & 1) &&
        generation == __atomic_load_n(&frame->generation, __ATOMIC_SEQ_CST) &&
        generation != __atomic_load_n(&frame->posted, __ATOMIC_SEQ_CST);
}

#endif
