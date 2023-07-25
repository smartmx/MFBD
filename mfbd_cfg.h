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
 *
 */

#ifndef _MFBD_CFG_H_
#define _MFBD_CFG_H_

/* add type definitions of your chips in your compiler environment. */
#include "stdint.h"
/* if you are not use mfbd in rt-thread, delete #include "rtconfig.h" below */
#include "rtconfig.h"

#ifndef NULL
    #define NULL 0
#endif

/* use section definitions can simplify the code, but limited with the complier. */
#ifdef PKG_MFBD_USE_SECTION_DEFINITION
    #define MFBD_USE_SECTION_DEFINITION     1
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    #define MFBD_USE_SECTION_DEFINITION     0
#endif

/* mfbd_btn_code_t is the type of value for every button event. */
#ifdef PKG_MFBD_BTN_CODE_SIZE
    #if PKG_MFBD_BTN_CODE_SIZE==1
        typedef uint8_t mfbd_btn_code_t;
    #elif PKG_MFBD_BTN_CODE_SIZE==2
        typedef uint16_t mfbd_btn_code_t;
    #elif PKG_MFBD_BTN_CODE_SIZE==4
        typedef uint32_t mfbd_btn_code_t;
    #elif PKG_MFBD_BTN_CODE_SIZE==8
        typedef uint64_t mfbd_btn_code_t;
    #else
        typedef uint16_t mfbd_btn_code_t;
    #endif
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    typedef uint16_t mfbd_btn_code_t;
#endif

/* mfbd_btn_count_t is the type of count time for button scanning. */
#ifdef PKG_MFBD_BTN_COUNT_SIZE
    #if PKG_MFBD_BTN_COUNT_SIZE==1
        typedef uint8_t mfbd_btn_count_t;
    #elif PKG_MFBD_BTN_COUNT_SIZE==2
        typedef uint16_t mfbd_btn_count_t;
    #elif PKG_MFBD_BTN_COUNT_SIZE==4
        typedef uint32_t mfbd_btn_count_t;
    #elif PKG_MFBD_BTN_COUNT_SIZE==8
        typedef uint64_t mfbd_btn_count_t;
    #else
        typedef uint16_t mfbd_btn_count_t;
    #endif
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    typedef uint16_t mfbd_btn_count_t;
#endif

/* mfbd_btn_index_t is the type of params when calling is_btn_down_func in mfbd_group_t. */
#ifdef PKG_MFBD_BTN_INDEX_SIZE
    #if PKG_MFBD_BTN_INDEX_SIZE==1
        typedef uint8_t mfbd_btn_index_t;
    #elif PKG_MFBD_BTN_INDEX_SIZE==2
        typedef uint16_t mfbd_btn_index_t;
    #elif PKG_MFBD_BTN_INDEX_SIZE==4
        typedef uint32_t mfbd_btn_index_t;
    #elif PKG_MFBD_BTN_INDEX_SIZE==8
        typedef uint64_t mfbd_btn_index_t;
    #elif PKG_MFBD_BTN_INDEX_SIZE==0
        typedef void *mfbd_btn_index_t;
    #else
        typedef uint32_t mfbd_btn_index_t;
    #endif
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    typedef uint32_t mfbd_btn_index_t;
#endif

/* set MFBD_USE_TINY_BUTTON to 1 will enable tiny button functions. */
#ifdef PKG_MFBD_USE_TINY_BUTTON
    #define MFBD_USE_TINY_BUTTON                1
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    #define MFBD_USE_TINY_BUTTON                0
#endif

/* set MFBD_USE_NORMAL_BUTTON to 1 will enable normal button functions. */
#ifdef PKG_MFBD_USE_NORMAL_BUTTON
    #define MFBD_USE_NORMAL_BUTTON              1
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    #define MFBD_USE_NORMAL_BUTTON              0
#endif

/* set MFBD_USE_MULTIFUCNTION_BUTTON to 1 will enable multifunction button functions. */
#ifdef PKG_MFBD_USE_MULTIFUCNTION_BUTTON
    #define MFBD_USE_MULTIFUCNTION_BUTTON       1
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    #define MFBD_USE_MULTIFUCNTION_BUTTON       0
#endif

/* set MFBD_USE_BTN_SCAN_PRE_FUNC to 1 will enable running prepare_function before run button detection function */
#ifdef PKG_MFBD_USE_BTN_SCAN_PRE_FUNC
    #define MFBD_USE_BTN_SCAN_PRE_FUNC          1
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    #define MFBD_USE_BTN_SCAN_PRE_FUNC          0
#endif

/* set MFBD_USE_BTN_SCAN_AFTER_FUNC to 1 will enable running after_function after run button detection function */
#ifdef PKG_MFBD_USE_BTN_SCAN_AFTER_FUNC
    #define MFBD_USE_BTN_SCAN_AFTER_FUNC        1
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    #define MFBD_USE_BTN_SCAN_AFTER_FUNC        0
#endif

/*
 * @Note:
 * set MFBD_PARAMS_SAME_IN_GROUP to 1 means all key's filter_time/repeat_time/long_time/multiclick_time are same,
 * it will not save filter_time/repeat_time/long_time/multiclick_time in btn_info struct.
 * if MFBD_PARAMS_SAME_IN_GROUP is 1, btns cannot disable repeat count alone.
 */
#ifdef PKG_MFBD_PARAMS_SAME_IN_GROUP
    #define MFBD_PARAMS_SAME_IN_GROUP           1
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    #define MFBD_PARAMS_SAME_IN_GROUP           0
#endif

/* set MFBD_MULTICLICK_STATE_AUTO_RESET to 1 will auto reset multiclick state to 0 when reach to max multicick state. */
#ifdef PKG_MFBD_MULTICLICK_STATE_AUTO_RESET
    #define MFBD_MULTICLICK_STATE_AUTO_RESET    1
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    #define MFBD_MULTICLICK_STATE_AUTO_RESET    0
#endif

/* set MFBD_MBTN_CONTINUE_LONG_COUNT to 1 will continue count to change state to long after when multiclick state is not 0. */
#ifdef PKG_MFBD_MBTN_CONTINUE_LONG_COUNT
    #define MFBD_MBTN_CONTINUE_LONG_COUNT       1
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    #define MFBD_MBTN_CONTINUE_LONG_COUNT       0
#endif

#if MFBD_MBTN_CONTINUE_LONG_COUNT
/*
 * @Note:
 * MFBD_MBTN_MULTICLICK_LONG_EVT only valid when MFBD_MBTN_CONTINUE_LONG_COUNT is not 0.
 * When MFBD_MBTN_MULTICLICK_LONG_EVT is 1, it will still report long code and repeat downcodes after multiclick.
 */
#ifdef PKG_MFBD_MBTN_MULTICLICK_LONG_EVT
    #define MFBD_MBTN_MULTICLICK_LONG_EVT       1
#else
    /* if you are not use mfbd in rt-thread, you can change this instead. */
    #define MFBD_MBTN_MULTICLICK_LONG_EVT       0
#endif
#else
    #define MFBD_MBTN_MULTICLICK_LONG_EVT       0
#endif

#endif /* _MFBD_CFG_H_ */
