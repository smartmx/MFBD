/*
 * Copyright (c) 2022, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-22     smartmx      the first version
 *
 */

#ifndef _MFBD_H_
#define _MFBD_H_

#include "mfbd_cfg.h"

typedef struct _mfbd_group_struct
{

    /* used to read whether button is down. */
    unsigned char (*is_btn_down_func)(mfbd_btn_index_t btn_index);

    /* used to report btn_value, must have a legal value, must not be NULL. */
    void (*btn_value_report)(mfbd_btn_code_t btn_value);

#if MFBD_USE_BTN_SCAN_PRE_FUNC
    /* prepare function when start to scan buttons for each group. */
    void (*btn_scan_prepare)(void);
#endif

#if MFBD_USE_BTN_SCAN_AFTER_FUNC
    /* function after scanning buttons for each group. */
    void (*btn_scan_after)(void);
#endif

    /* pointer to the head of key info list */
    void *btn_list_head;
} mfbd_group_t;

#define MFBD_GROUP_DEFINE(NAME,...) const mfbd_group_t NAME = {__VA_ARGS__}

/* tiny button definitions, tiny button functions only support down and up event. */

typedef struct _mfbd_tbtn_info_struct
{
    mfbd_btn_code_t    btn_down_code;               /* keyCode when button down, set to 0 will not report it. */
    mfbd_btn_code_t    btn_up_code;                 /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_count_t   filter_time;                 /* filter time when button state changed, please do not use 0. */
    mfbd_btn_index_t   btn_index;                   /* parameter when calling is_btn_down_func. */
} mfbd_tbtn_info_t;

typedef struct _mfbd_tiny_btn_struct
{
    struct  _mfbd_tiny_btn_struct   *next;          /* next pointer in the button group. */
    const   mfbd_tbtn_info_t        *btn_info;      /* a pointer to mfbd_tbtn_info_t. */
    mfbd_btn_count_t                filter_count;   /* filter time count when button state changed. */
    unsigned char                   state;          /* the state of button, up or down. */
} mfbd_tbtn_t;

extern void mfbd_tbtn_scan(const mfbd_group_t *_pbtn_group);

/* use #define to declare the mfbd button object easily. */
#define MFBD_TBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, BTN_DOWN_CODE, BTN_UP_CODE, NEXT)  \
static const mfbd_tbtn_info_t NAME##_info = { \
        BTN_DOWN_CODE, \
        BTN_UP_CODE, \
        FILTER_TIME, \
        BTN_INDEX, \
}; \
static mfbd_tbtn_t NAME = { \
        NEXT,\
        &NAME##_info, \
        0, \
        0, \
}

/* normal button definitions, normal button functions support down, up and long-press event. */

typedef struct _mfbd_nbtn_info_struct
{
    mfbd_btn_count_t   filter_time;                 /* filter time when button state changed, please do not use 0. */
    mfbd_btn_count_t   repeat_time;                 /* repeat time when button still down for over long_time, set 0 will disable repeat time count. */
    mfbd_btn_count_t   long_time;                   /* long time when button still down, set 0 will disable long time and repeat time count. */
    mfbd_btn_code_t    btn_down_code;               /* keyCode when button down, set to 0 will not report it. */
    mfbd_btn_code_t    btn_up_code;                 /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_code_t    btn_long_code;               /* keyCode when button down for long_time, set 0 will not report it ,but report btn_down_code instead. */
    mfbd_btn_index_t   btn_index;                   /* parameter when calling is_btn_down_func. */
} mfbd_nbtn_info_t;

/* normal btn_info scan up, down and long press event */
typedef struct _mfbd_normal_btn_struct
{
    struct _mfbd_normal_btn_struct    *next;            /* next pointer in the button group. */
    const   mfbd_nbtn_info_t          *btn_info;        /* a pointer to mfbd_nbtn_info_t. */
    mfbd_btn_count_t                  filter_count;     /* filter time count when button state changed. */
    mfbd_btn_count_t                  long_count;       /* long time count when button still down. */
    mfbd_btn_count_t                  repeat_count;     /* repeat time count when button still down. */
    unsigned char                     state;            /* the state of button, up or down. */
} mfbd_nbtn_t;

extern void mfbd_nbtn_scan(const mfbd_group_t *_pbtn_group);

/* use #define to declare the mfbd button object easily. */
#define MFBD_NBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE, NEXT)  \
static const mfbd_nbtn_info_t NAME##_info = { \
        FILTER_TIME, \
        REPEAT_TIME, \
        LONG_TIME, \
        BTN_DOWN_CODE, \
        BTN_UP_CODE, \
        BTN_LONG_CODE, \
        BTN_INDEX, \
}; \
static mfbd_nbtn_t NAME = { \
        NEXT,\
        &NAME##_info, \
        0, \
        0, \
        0, \
        0, \
}


/* multi-function button definitions, multi-function button functions support down, up, long-press and multi-click event. */

typedef struct _mfbd_mbtn_info_struct
{
    mfbd_btn_count_t   filter_time;             /* filter time when button state changed, please do not use 0. */
    mfbd_btn_count_t   repeat_time;             /* repeat time when button still down for over long_time, set 0 will disable repeat time count. */
    mfbd_btn_count_t   long_time;               /* long time when button still down, set 0 will disable long time and repeat time count. */
    mfbd_btn_count_t   multiclick_time;         /* multi-click time when button still up, set 0 will disable multi-click time count. */
    const mfbd_btn_code_t    *btn_down_code;    /* pointer to multi-click keyCodes when button down. */
    mfbd_btn_code_t    btn_up_code;             /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_code_t    btn_long_code;           /* keyCode when button down for long_time, set 0 will not report it ,but report btn_down_code[0] instead. */
    mfbd_btn_index_t   btn_index;               /* parameter when calling is_btn_down_func. */
} mfbd_mbtn_info_t;

/*
 * @Note:
 * repeat_count and long_count are conflict to multi-click.
 * repeat_count and long_count only check in the first button down event, and will disable in next multi-click event.
 * also, if repeat_count and long_count event has happened in the first down event, it will reset multiclick_state.
 */
typedef struct _mfbd_multi_fuction_btn_struct
{
    struct _mfbd_multi_fuction_btn_struct     *next;                  /* next pointer in the button group. */
    const   mfbd_mbtn_info_t                  *btn_info;              /* a pointer to mfbd_mbtn_info_t. */
    mfbd_btn_count_t                          filter_count;           /* filter time count when button state changed. */
    mfbd_btn_count_t                          long_count;             /* long time count when button still down. */
    mfbd_btn_count_t                          repeat_count;           /* repeat time count when button still down. */
    mfbd_btn_count_t                          multiclick_count;       /* multi-click time count when button is up. */
    unsigned char                             multiclick_state;       /* multi-click count when button is in multi-click state. */
    unsigned char                             state;                  /* the state of button, up or down. */
} mfbd_mbtn_t;

extern void mfbd_mbtn_scan(const mfbd_group_t *_pbtn_group);

/* use #define to declare the mfbd button object easily. */
#define MFBD_MBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, MULTICLICK_TIME, BTN_UP_CODE, BTN_LONG_CODE, NEXT, BTN_DOWN_CODE,...)  \
static const mfbd_btn_code_t NAME##_down_codes[MFBD_MULTI_MAX_CLICK + 1] = {BTN_DOWN_CODE,__VA_ARGS__};    \
static const mfbd_mbtn_info_t NAME##_info = { \
        FILTER_TIME, \
        REPEAT_TIME, \
        LONG_TIME, \
        MULTICLICK_TIME, \
        NAME##_down_codes, \
        BTN_UP_CODE, \
        BTN_LONG_CODE, \
        BTN_INDEX, \
};\
static mfbd_mbtn_t NAME = { \
        NEXT,\
        &NAME##_info, \
        0, \
        0, \
        0, \
        0, \
        0, \
        0, \
}

#endif
