/*
 * Copyright (c) 2022, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-22     smartmx      the first version
 * 2022-03-15     smartmx      each mbtn has it's own max multi-click times
 *
 */

#ifndef _MFBD_CFG_H_
#define _MFBD_CFG_H_

/* add type definitions of your chips in your compiler environment */
#include "stdint.h"

#ifndef NULL
    #define NULL 0
#endif

/* mfbd_btn_code_t is the type of value of every button event. */
typedef uint16_t    mfbd_btn_code_t;

/* mfbd_btn_count_t is the type of count time for button scanning. */
typedef uint16_t    mfbd_btn_count_t;

/* mfbd_btn_index_t is the type of params when calling is_btn_down_func in mfbd_group_t. */
typedef uint32_t    mfbd_btn_index_t;

/* set MFBD_USE_BTN_SCAN_PRE_FUNC to 1 will enable running prepare_function before run button detection function */
#define MFBD_USE_BTN_SCAN_PRE_FUNC       0

/* set MFBD_USE_BTN_SCAN_AFTER_FUNC to 1 will enable running after_function after run button detection function */
#define MFBD_USE_BTN_SCAN_AFTER_FUNC     0

#endif /* _MFBD_CFG_H_ */
