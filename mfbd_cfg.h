/*
 * Copyright (c) 2022-2023, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-22     smartmx      the first version.
 * 2022-03-15     smartmx      each mbtn has it's own max multi-click times.
 * 2022-04-16     smartmx      drop list definitions, use arraylist, each group has all btn types.
 * 2022-08-05     smartmx      add reset params function.
 * 2022-12-04     smartmx      change some definitions, add rtconfig.h.
 * 2023-03-25     smartmx      add some comment.
 * 2023-07-03     smartmx      add Section Definition option.
 * 2023-07-22     smartmx      add MFBD_MBTN_MULTICLICK_LONG_EVT and MFBD_MBTN_CONTINUE_LONG_COUNT option.
 * 2026-10-11     codex        make config macros overridable from compiler command line,
 *                             guard rtconfig.h include for non RT-Thread ports.
 *
 */

#ifndef _MFBD_CFG_H_
#define _MFBD_CFG_H_

/* add type definitions of your chips in your compiler environment. */
#include <stdint.h>
#include <stddef.h>

/*
 * If you are not using mfbd in RT-Thread, you can define MFBD_NOT_USE_RT_THREAD
 * in your compiler command line to skip including rtconfig.h,
 * or just delete the include below.
 */
#ifndef MFBD_NOT_USE_RT_THREAD
#include "rtconfig.h"
#endif

/* use section definitions can simplify the code, but limited with the complier. */
#ifndef MFBD_USE_SECTION_DEFINITION
    #ifdef PKG_MFBD_USE_SECTION_DEFINITION
        #define MFBD_USE_SECTION_DEFINITION     1
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_USE_SECTION_DEFINITION     0
    #endif
#endif

/*
 * mfbd_btn_code_t is the type of value for every button event.
 * define MFBD_BTN_CODE_SIZE to 1/2/4/8 to select uint8_t/uint16_t/uint32_t/uint64_t.
 */
#ifndef MFBD_BTN_CODE_SIZE
    #ifdef PKG_MFBD_BTN_CODE_SIZE
        #define MFBD_BTN_CODE_SIZE              PKG_MFBD_BTN_CODE_SIZE
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_BTN_CODE_SIZE              2
    #endif
#endif
#if MFBD_BTN_CODE_SIZE == 1
    typedef uint8_t mfbd_btn_code_t;
#elif MFBD_BTN_CODE_SIZE == 2
    typedef uint16_t mfbd_btn_code_t;
#elif MFBD_BTN_CODE_SIZE == 4
    typedef uint32_t mfbd_btn_code_t;
#elif MFBD_BTN_CODE_SIZE == 8
    typedef uint64_t mfbd_btn_code_t;
#else
    typedef uint16_t mfbd_btn_code_t;
#endif

/*
 * mfbd_btn_count_t is the type of count time for button scanning.
 * define MFBD_BTN_COUNT_SIZE to 1/2/4/8 to select uint8_t/uint16_t/uint32_t/uint64_t.
 *
 * @Note:
 * filter_time must be less than or equal to the half of the max value of this type,
 * long_time and repeat_time must be less than the max value of this type,
 * or the counters will overflow and the events will never be reported.
 * For example, when this type is uint8_t, filter_time <= 127,
 * long_time <= 254 and repeat_time <= 254.
 */
#ifndef MFBD_BTN_COUNT_SIZE
    #ifdef PKG_MFBD_BTN_COUNT_SIZE
        #define MFBD_BTN_COUNT_SIZE             PKG_MFBD_BTN_COUNT_SIZE
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_BTN_COUNT_SIZE             2
    #endif
#endif
#if MFBD_BTN_COUNT_SIZE == 1
    typedef uint8_t mfbd_btn_count_t;
#elif MFBD_BTN_COUNT_SIZE == 2
    typedef uint16_t mfbd_btn_count_t;
#elif MFBD_BTN_COUNT_SIZE == 4
    typedef uint32_t mfbd_btn_count_t;
#elif MFBD_BTN_COUNT_SIZE == 8
    typedef uint64_t mfbd_btn_count_t;
#else
    typedef uint16_t mfbd_btn_count_t;
#endif

/*
 * mfbd_btn_index_t is the type of params when calling is_btn_down_func in mfbd_group_t.
 * define MFBD_BTN_INDEX_SIZE to 0/1/2/4/8 to select void pointer or uint8_t/uint16_t/uint32_t/uint64_t.
 */
#ifndef MFBD_BTN_INDEX_SIZE
    #ifdef PKG_MFBD_BTN_INDEX_SIZE
        #define MFBD_BTN_INDEX_SIZE             PKG_MFBD_BTN_INDEX_SIZE
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_BTN_INDEX_SIZE             4
    #endif
#endif
#if MFBD_BTN_INDEX_SIZE == 1
    typedef uint8_t mfbd_btn_index_t;
#elif MFBD_BTN_INDEX_SIZE == 2
    typedef uint16_t mfbd_btn_index_t;
#elif MFBD_BTN_INDEX_SIZE == 4
    typedef uint32_t mfbd_btn_index_t;
#elif MFBD_BTN_INDEX_SIZE == 8
    typedef uint64_t mfbd_btn_index_t;
#elif MFBD_BTN_INDEX_SIZE == 0
    typedef void *mfbd_btn_index_t;
#else
    typedef uint32_t mfbd_btn_index_t;
#endif

/* set MFBD_USE_TINY_BUTTON to 1 will enable tiny button functions. */
#ifndef MFBD_USE_TINY_BUTTON
    #ifdef PKG_MFBD_USE_TINY_BUTTON
        #define MFBD_USE_TINY_BUTTON                1
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_USE_TINY_BUTTON                0
    #endif
#endif

/* set MFBD_USE_NORMAL_BUTTON to 1 will enable normal button functions. */
#ifndef MFBD_USE_NORMAL_BUTTON
    #ifdef PKG_MFBD_USE_NORMAL_BUTTON
        #define MFBD_USE_NORMAL_BUTTON              1
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_USE_NORMAL_BUTTON              0
    #endif
#endif

/* set MFBD_USE_MULTIFUCNTION_BUTTON to 1 will enable multifunction button functions. */
#ifndef MFBD_USE_MULTIFUCNTION_BUTTON
    #ifdef PKG_MFBD_USE_MULTIFUCNTION_BUTTON
        #define MFBD_USE_MULTIFUCNTION_BUTTON       1
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_USE_MULTIFUCNTION_BUTTON       0
    #endif
#endif

/* set MFBD_USE_BTN_SCAN_PRE_FUNC to 1 will enable running prepare_function before run button detection function */
#ifndef MFBD_USE_BTN_SCAN_PRE_FUNC
    #ifdef PKG_MFBD_USE_BTN_SCAN_PRE_FUNC
        #define MFBD_USE_BTN_SCAN_PRE_FUNC          1
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_USE_BTN_SCAN_PRE_FUNC          0
    #endif
#endif

/* set MFBD_USE_BTN_SCAN_AFTER_FUNC to 1 will enable running after_function after run button detection function */
#ifndef MFBD_USE_BTN_SCAN_AFTER_FUNC
    #ifdef PKG_MFBD_USE_BTN_SCAN_AFTER_FUNC
        #define MFBD_USE_BTN_SCAN_AFTER_FUNC        1
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_USE_BTN_SCAN_AFTER_FUNC        0
    #endif
#endif

/*
 * @Note:
 * set MFBD_PARAMS_SAME_IN_GROUP to 1 means all key's filter_time/repeat_time/long_time/multiclick_time are same,
 * it will not save filter_time/repeat_time/long_time/multiclick_time in btn_info struct.
 * if MFBD_PARAMS_SAME_IN_GROUP is 1, btns cannot disable repeat count alone.
 */
#ifndef MFBD_PARAMS_SAME_IN_GROUP
    #ifdef PKG_MFBD_PARAMS_SAME_IN_GROUP
        #define MFBD_PARAMS_SAME_IN_GROUP           1
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_PARAMS_SAME_IN_GROUP           0
    #endif
#endif

/* set MFBD_MULTICLICK_STATE_AUTO_RESET to 1 will auto reset multiclick state to 0 when reach to max multicick state. */
#ifndef MFBD_MULTICLICK_STATE_AUTO_RESET
    #ifdef PKG_MFBD_MULTICLICK_STATE_AUTO_RESET
        #define MFBD_MULTICLICK_STATE_AUTO_RESET    1
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_MULTICLICK_STATE_AUTO_RESET    0
    #endif
#endif

/* set MFBD_MBTN_CONTINUE_LONG_COUNT to 1 will continue count to change state to long after when multiclick state is not 0. */
#ifndef MFBD_MBTN_CONTINUE_LONG_COUNT
    #ifdef PKG_MFBD_MBTN_CONTINUE_LONG_COUNT
        #define MFBD_MBTN_CONTINUE_LONG_COUNT       1
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_MBTN_CONTINUE_LONG_COUNT       0
    #endif
#endif

#if MFBD_MBTN_CONTINUE_LONG_COUNT
/*
 * @Note:
 * MFBD_MBTN_MULTICLICK_LONG_EVT only valid when MFBD_MBTN_CONTINUE_LONG_COUNT is not 0.
 * When MFBD_MBTN_MULTICLICK_LONG_EVT is 1, it will still report long code and repeat downcodes after multiclick.
 */
#ifndef MFBD_MBTN_MULTICLICK_LONG_EVT
    #ifdef PKG_MFBD_MBTN_MULTICLICK_LONG_EVT
        #define MFBD_MBTN_MULTICLICK_LONG_EVT       1
    #else
        /* if you are not use mfbd in rt-thread, you can change this instead. */
        #define MFBD_MBTN_MULTICLICK_LONG_EVT       0
    #endif
#endif
#else
#undef MFBD_MBTN_MULTICLICK_LONG_EVT
#define MFBD_MBTN_MULTICLICK_LONG_EVT               0
#endif

#endif /* _MFBD_CFG_H_ */
