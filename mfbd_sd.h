/*
 * Copyright (c) 2022-2023, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-03     smartmx      the first version, Multi-Function Button Dectection with Section Definition.
 * 2023-07-15     smartmx      add skip function, to reduce calling of scan functions.
 * 2023-09-19     smartmx      improve performance, add MFBD_BTN_STATE_SKIP.
 *
 */

#ifndef _MFBD_SD_H_
#define _MFBD_SD_H_

#include "mfbd_cfg.h"

#if MFBD_USE_SECTION_DEFINITION

typedef enum
{
    MFBD_BTN_STATE_UP = 0,
    MFBD_BTN_STATE_DOWN,
    MFBD_BTN_STATE_LONG,
    MFBD_BTN_STATE_SKIP = 0xff,
} MFBD_BTN_STATE_t;

#define MFBD_DOWN_CODE_NAME(NAME)                       NAME##_DOWN_CODE                /* when using tbtn/nbtn default define api, this is down-code name. */
#define MFBD_UP_CODE_NAME(NAME)                         NAME##_UP_CODE                  /* when using tbtn/nbtn/mbtn default define api, this is up-code name. */
#define MFBD_LONG_CODE_NAME(NAME)                       NAME##_LONG_CODE                /* when using nbtn/mbtn default define api, this is long-code name. */
#define MFBD_DOWN_CODES_NAME(NAME)                      NAME##_DOWN_CODES               /* when using mbtn default define api, this is down-codes name. */

#if defined(__CC_ARM) || defined(__CLANG_ARM)           /* ARM Compiler */
#define __MFBD_SECTION(x)                               __attribute__((section(x)))
#define MFBD_USED                                       __attribute__((used))
#define _MFBD_SECTION_START(X)                          X##$$Base
#define _MFBD_SECTION_END(X)                            X##$$Limit
#elif defined (__IAR_SYSTEMS_ICC__)                     /* IAR Compiler */
#define __MFBD_SECTION(x)                               @ x
#define MFBD_USED                                       __root
#define _MFBD_SECTION_START(X)                          __section_begin(#X)
#define _MFBD_SECTION_END(X)                            __section_end(#X)
#elif defined (__GNUC__)                                /* GNU GCC Compiler */
#define __MFBD_SECTION(x)                               __attribute__((section(x)))
#define MFBD_USED                                       __attribute__((used))
#define _MFBD_SECTION_START(X)                          X##_start
#define _MFBD_SECTION_END(X)                            X##_end
#else
#error "not supported tool chain..."
#endif

#define _MFBD_SECTION(X)                                __MFBD_SECTION(#X)
#define MFBD_SECTION(GROUP, BTN_KIND)                   _MFBD_SECTION(GROUP##_##BTN_KIND)

#define MFBD_SECTION_START(GROUP, BTN_KIND)             _MFBD_SECTION_START(GROUP##_##BTN_KIND)
#define MFBD_SECTION_END(GROUP, BTN_KIND)               _MFBD_SECTION_END(GROUP##_##BTN_KIND)


/* tiny button definitions, tiny button functions only support down and up event. */
typedef struct _mfbd_tiny_btn_struct
{
    mfbd_btn_count_t                filter_count;   /* filter time count when button state changed. */
    unsigned char                   state;          /* the state of button, up or down. */
} mfbd_tbtn_t;

#if MFBD_PARAMS_SAME_IN_GROUP

typedef struct _mfbd_tbtn_info_struct
{
    mfbd_tbtn_t        *btn;                        /* a pointer to mfbd_tbtn_t. */
    mfbd_btn_code_t    btn_down_code;               /* keyCode when button down, set to 0 will not report it. */
    mfbd_btn_code_t    btn_up_code;                 /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_index_t   btn_index;                   /* parameter when calling is_btn_down_func. */
} mfbd_tbtn_info_t;

#else

typedef struct _mfbd_tbtn_info_struct
{
    mfbd_tbtn_t        *btn;                        /* a pointer to mfbd_tbtn_t. */
    mfbd_btn_code_t    btn_down_code;               /* keyCode when button down, set to 0 will not report it. */
    mfbd_btn_code_t    btn_up_code;                 /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_count_t   filter_time;                 /* filter time when button state changed, please do not use 0. */
    mfbd_btn_index_t   btn_index;                   /* parameter when calling is_btn_down_func. */
} mfbd_tbtn_info_t;

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

#if MFBD_PARAMS_SAME_IN_GROUP

#define MFBD_TBTN_DEFINE(GROUP, NAME, BTN_INDEX, BTN_DOWN_CODE, BTN_UP_CODE)            \
    MFBD_USED mfbd_tbtn_t NAME = {                                                      \
        0,                                                                              \
        0,                                                                              \
    };                                                                                  \
    MFBD_USED static const mfbd_tbtn_info_t NAME##_info MFBD_SECTION(GROUP, tbtn)= {    \
        &NAME,                                                                          \
        BTN_DOWN_CODE,                                                                  \
        BTN_UP_CODE,                                                                    \
        BTN_INDEX,                                                                      \
    }

#define MFBD_TBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX)                                \
    MFBD_USED mfbd_tbtn_t NAME = {                                                      \
        0,                                                                              \
        0,                                                                              \
    };                                                                                  \
    MFBD_USED static const mfbd_tbtn_info_t NAME##_info MFBD_SECTION(GROUP, tbtn)= {    \
        &NAME,                                                                          \
        NAME##_DOWN_CODE,                                                               \
        NAME##_UP_CODE,                                                                 \
        BTN_INDEX,                                                                      \
    }

#else

#define MFBD_TBTN_DEFINE(GROUP, NAME, BTN_INDEX, FILTER_TIME, BTN_DOWN_CODE, BTN_UP_CODE)       \
    MFBD_USED mfbd_tbtn_t NAME = {                                                              \
        0,                                                                                      \
        0,                                                                                      \
    };                                                                                          \
    MFBD_USED static const mfbd_tbtn_info_t NAME##_info MFBD_SECTION(GROUP, tbtn)= {            \
        &NAME,                                                                                  \
        BTN_DOWN_CODE,                                                                          \
        BTN_UP_CODE,                                                                            \
        FILTER_TIME,                                                                            \
        BTN_INDEX,                                                                              \
    }

#define MFBD_TBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX, FILTER_TIME)                           \
    MFBD_USED mfbd_tbtn_t NAME = {                                                              \
        0,                                                                                      \
        0,                                                                                      \
    };                                                                                          \
    MFBD_USED static const mfbd_tbtn_info_t NAME##_info MFBD_SECTION(GROUP, tbtn)= {            \
        &NAME,                                                                                  \
        NAME##_DOWN_CODE,                                                                       \
        NAME##_UP_CODE,                                                                         \
        FILTER_TIME,                                                                            \
        BTN_INDEX,                                                                              \
    }

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

#define MFBD_TBTN_EXTERN(NAME) extern mfbd_tbtn_t NAME


/* normal button definitions, normal button functions support down, up and long-press event. */
typedef struct _mfbd_normal_btn_struct
{
    mfbd_btn_count_t                  filter_count;     /* filter time count when button state changed. */
    mfbd_btn_count_t                  long_count;       /* long time count when button still down. */
    mfbd_btn_count_t                  repeat_count;     /* repeat time count when button still down. */
    unsigned char                     state;            /* the state of button, up or down. */
} mfbd_nbtn_t;

#if MFBD_PARAMS_SAME_IN_GROUP

typedef struct _mfbd_nbtn_info_struct
{
    mfbd_nbtn_t        *btn;                        /* a pointer to mfbd_nbtn_t. */
    mfbd_btn_code_t    btn_down_code;               /* keyCode when button down, set to 0 will not report it. */
    mfbd_btn_code_t    btn_up_code;                 /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_code_t    btn_long_code;               /* keyCode when button down for long_time, set 0 will not report it ,but report btn_down_code instead. */
    mfbd_btn_index_t   btn_index;                   /* parameter when calling is_btn_down_func. */
} mfbd_nbtn_info_t;

#else

typedef struct _mfbd_nbtn_info_struct
{
    mfbd_nbtn_t        *btn;                        /* a pointer to mfbd_nbtn_t. */
    mfbd_btn_count_t   filter_time;                 /* filter time when button state changed, please do not use 0. */
    mfbd_btn_count_t   repeat_time;                 /* repeat time when button still down for over long_time, set 0 will disable repeat time count. */
    mfbd_btn_count_t   long_time;                   /* long time when button still down, set 0 will disable long time and repeat time count. */
    mfbd_btn_code_t    btn_down_code;               /* keyCode when button down, set to 0 will not report it. */
    mfbd_btn_code_t    btn_up_code;                 /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_code_t    btn_long_code;               /* keyCode when button down for long_time, set 0 will not report it ,but report btn_down_code instead. */
    mfbd_btn_index_t   btn_index;                   /* parameter when calling is_btn_down_func. */
} mfbd_nbtn_info_t;

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */


#if MFBD_PARAMS_SAME_IN_GROUP

#define MFBD_NBTN_DEFINE(GROUP, NAME, BTN_INDEX, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE)     \
    MFBD_USED mfbd_nbtn_t NAME = {                                                              \
        0,                                                                                      \
        0,                                                                                      \
        0,                                                                                      \
        0,                                                                                      \
    };                                                                                          \
    MFBD_USED static const mfbd_nbtn_info_t NAME##_info MFBD_SECTION(GROUP, nbtn)= {            \
        &NAME,                                                                                  \
        BTN_DOWN_CODE,                                                                          \
        BTN_UP_CODE,                                                                            \
        BTN_LONG_CODE,                                                                          \
        BTN_INDEX,                                                                              \
    }

#define MFBD_NBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX)                                        \
    mfbd_nbtn_t NAME = {                                                                        \
        0,                                                                                      \
        0,                                                                                      \
        0,                                                                                      \
        0,                                                                                      \
    };                                                                                          \
    MFBD_USED static const mfbd_nbtn_info_t NAME##_info MFBD_SECTION(GROUP, nbtn)= {            \
        &NAME,                                                                                  \
        NAME##_DOWN_CODE,                                                                       \
        NAME##_UP_CODE,                                                                         \
        NAME##_LONG_CODE,                                                                       \
        BTN_INDEX,                                                                              \
    }

#else

#define MFBD_NBTN_DEFINE(GROUP, NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE)        \
    mfbd_nbtn_t NAME = {                                                                                                                \
        0,                                                                                                                              \
        0,                                                                                                                              \
        0,                                                                                                                              \
        0,                                                                                                                              \
    };                                                                                                                                  \
    MFBD_USED static const mfbd_nbtn_info_t NAME##_info MFBD_SECTION(GROUP, nbtn)= {        \
        &NAME,                                                                              \
        FILTER_TIME,                                                                        \
        REPEAT_TIME,                                                                        \
        LONG_TIME,                                                                          \
        BTN_DOWN_CODE,                                                                      \
        BTN_UP_CODE,                                                                        \
        BTN_LONG_CODE,                                                                      \
        BTN_INDEX,                                                                          \
    }

#define MFBD_NBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME)           \
    mfbd_nbtn_t NAME = {                                                                                \
        0,                                                                                              \
        0,                                                                                              \
        0,                                                                                              \
        0,                                                                                              \
    };                                                                                                  \
    MFBD_USED static const mfbd_nbtn_info_t NAME##_info MFBD_SECTION(GROUP, nbtn)= {                    \
        &NAME,                                                                                          \
        FILTER_TIME,                                                                                    \
        REPEAT_TIME,                                                                                    \
        LONG_TIME,                                                                                      \
        NAME##_DOWN_CODE,                                                                               \
        NAME##_UP_CODE,                                                                                 \
        NAME##_LONG_CODE,                                                                               \
        BTN_INDEX,                                                                                      \
    }

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

#define MFBD_NBTN_EXTERN(NAME) extern mfbd_nbtn_t NAME


/* multi-function button definitions, multi-function button functions support down, up, long-press and multi-click event. */

/*
 * @Note:
 * repeat_count and long_count are conflict to multi-click.
 * repeat_count and long_count only check in the first button down event, and will disable in next multi-click event.
 * also, if repeat_count and long_count event has happened in the first down event, it will reset multiclick_state.
 */
typedef struct _mfbd_multifuction_btn_struct
{
    mfbd_btn_count_t                        filter_count;           /* filter time count when button state changed. */
    mfbd_btn_count_t                        long_count;             /* long time count when button still down. */
    mfbd_btn_count_t                        repeat_count;           /* repeat time count when button still down. */
    mfbd_btn_count_t                        multiclick_count;       /* multi-click time count when button is up. */
    unsigned char                           multiclick_state;       /* multi-click count when button is in multi-click state. */
    unsigned char                           state;                  /* the state of button, up or down. */
} mfbd_mbtn_t;

#if MFBD_PARAMS_SAME_IN_GROUP

typedef struct _mfbd_mbtn_info_struct
{
    mfbd_mbtn_t             *btn;                       /* a pointer to mfbd_mbtn_t. */
    const mfbd_btn_code_t   *btn_down_code;             /* pointer to multi-click keyCodes when button down. */
    mfbd_btn_code_t         btn_up_code;                /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_code_t         btn_long_code;              /* keyCode when button down for long_time, set 0 will not report it ,but report btn_down_code[0] instead. */
    mfbd_btn_index_t        btn_index;                  /* parameter when calling is_btn_down_func. */
    unsigned char           max_multiclick_state;       /* max multiclick states. */
} mfbd_mbtn_info_t;

#else

typedef struct _mfbd_mbtn_info_struct
{
    mfbd_mbtn_t             *btn;                       /* a pointer to mfbd_mbtn_t. */
    const mfbd_btn_code_t   *btn_down_code;             /* pointer to multi-click keyCodes when button down. */
    mfbd_btn_count_t        filter_time;                /* filter time when button state changed, please do not use 0. */
    mfbd_btn_count_t        repeat_time;                /* repeat time when button still down for over long_time, set 0 will disable repeat time count. */
    mfbd_btn_count_t        long_time;                  /* long time when button still down, set 0 will disable long time and repeat time count. */
    mfbd_btn_count_t        multiclick_time;            /* multi-click time when button still up, set 0 will disable multi-click time count. */
    mfbd_btn_code_t         btn_up_code;                /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_code_t         btn_long_code;              /* keyCode when button down for long_time, set 0 will not report it ,but report btn_down_code[0] instead. */
    mfbd_btn_index_t        btn_index;                  /* parameter when calling is_btn_down_func. */
    unsigned char           max_multiclick_state;       /* max multiclick states. */
} mfbd_mbtn_info_t;

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

#if MFBD_PARAMS_SAME_IN_GROUP

#define MFBD_MBTN_DEFINE(GROUP, NAME, BTN_INDEX, MAX_MULTICLICK_STATE, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE, ...)          \
    static const mfbd_btn_code_t NAME##_down_codes[MAX_MULTICLICK_STATE + 1] = {BTN_DOWN_CODE, __VA_ARGS__};                    \
    mfbd_mbtn_t NAME = {                                                                                                        \
        0,                                                                                                                      \
        0,                                                                                                                      \
        0,                                                                                                                      \
        0,                                                                                                                      \
        0,                                                                                                                      \
        0,                                                                                                                      \
    };                                                                                                                          \
    MFBD_USED static const mfbd_mbtn_info_t NAME##_info MFBD_SECTION(GROUP, mbtn)= {                    \
        &NAME,                                                                                          \
        NAME##_down_codes,                                                                              \
        BTN_UP_CODE,                                                                                    \
        BTN_LONG_CODE,                                                                                  \
        BTN_INDEX,                                                                                      \
        MAX_MULTICLICK_STATE,                                                                           \
    }

#define MFBD_MBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX, MAX_MULTICLICK_STATE)                          \
    mfbd_mbtn_t NAME = {                                                                                \
        0,                                                                                              \
        0,                                                                                              \
        0,                                                                                              \
        0,                                                                                              \
        0,                                                                                              \
        0,                                                                                              \
    };                                                                                                  \
    MFBD_USED static const mfbd_mbtn_info_t NAME##_info MFBD_SECTION(GROUP, mbtn)= {                    \
        &NAME,                                                                                          \
        NAME##_DOWN_CODES,                                                                              \
        NAME##_UP_CODE,                                                                                 \
        NAME##_LONG_CODE,                                                                               \
        BTN_INDEX,                                                                                      \
        MAX_MULTICLICK_STATE,                                                                           \
    }

#else

#define MFBD_MBTN_DEFINE(GROUP, NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, MULTICLICK_TIME, MAX_MULTICLICK_STATE, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE, ...)    \
    static const mfbd_btn_code_t NAME##_down_codes[MAX_MULTICLICK_STATE + 1] = {BTN_DOWN_CODE, __VA_ARGS__};                                                                    \
    mfbd_mbtn_t NAME = {                                                                                                                                                        \
        0,                                                                                                                                                                      \
        0,                                                                                                                                                                      \
        0,                                                                                                                                                                      \
        0,                                                                                                                                                                      \
        0,                                                                                                                                                                      \
        0,                                                                                                                                                                      \
   };                                                                                                                                                                           \
    MFBD_USED static const mfbd_mbtn_info_t NAME##_info MFBD_SECTION(GROUP, mbtn)= {                                                                                            \
        &NAME,                                                                                                                                                                  \
        NAME##_down_codes,                                                                                                                                                      \
        FILTER_TIME,                                                                                                                                                            \
        REPEAT_TIME,                                                                                                                                                            \
        LONG_TIME,                                                                                                                                                              \
        MULTICLICK_TIME,                                                                                                                                                        \
        BTN_UP_CODE,                                                                                                                                                            \
        BTN_LONG_CODE,                                                                                                                                                          \
        BTN_INDEX,                                                                                                                                                              \
        MAX_MULTICLICK_STATE,                                                                                                                                                   \
    }

#define MFBD_MBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, MULTICLICK_TIME, MAX_MULTICLICK_STATE)            \
    mfbd_mbtn_t NAME = {                                                                                                                        \
        0,                                                                                                                                      \
        0,                                                                                                                                      \
        0,                                                                                                                                      \
        0,                                                                                                                                      \
        0,                                                                                                                                      \
        0,                                                                                                                                      \
    };                                                                                                                                          \
    MFBD_USED static const mfbd_mbtn_info_t NAME##_info MFBD_SECTION(GROUP, mbtn)= {                                                            \
        &NAME,                                                                                                                                  \
        NAME##_DOWN_CODES,                                                                                                                      \
        FILTER_TIME,                                                                                                                            \
        REPEAT_TIME,                                                                                                                            \
        LONG_TIME,                                                                                                                              \
        MULTICLICK_TIME,                                                                                                                        \
        NAME##_UP_CODE,                                                                                                                         \
        NAME##_LONG_CODE,                                                                                                                       \
        BTN_INDEX,                                                                                                                              \
        MAX_MULTICLICK_STATE,                                                                                                                   \
    }

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

#define MFBD_MBTN_EXTERN(NAME) extern mfbd_mbtn_t NAME


/* mfbd group struct */
typedef struct _mfbd_group_struct
{
    /* used to read whether button is down. */
    unsigned char (*is_btn_down_func)(mfbd_btn_index_t btn_index);

    /* used to report btn_value, must have a legal value, must not be NULL. */
    void (*btn_value_report)(mfbd_btn_code_t btn_value);

    /* if set MFBD_PARAMS_SAME_IN_GROUP to 1, all btns in group has same params. */
#if MFBD_PARAMS_SAME_IN_GROUP

#if MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON
    mfbd_btn_count_t   filter_time;             /* filter time when button state changed, please do not use 0. */
#endif /*  MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON

    mfbd_btn_count_t   repeat_time;             /* repeat time when button still down for over long_time, set 0 will disable repeat time count. */

    mfbd_btn_count_t   long_time;               /* long time when button still down, set 0 will disable long time and repeat time count. */

#endif /* MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON

    mfbd_btn_count_t   multiclick_time;         /* multi-click time when button still up, set 0 will disable multi-click time count. */

#endif /* MFBD_USE_MULTIFUCNTION_BUTTON */

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

#if MFBD_USE_BTN_SCAN_PRE_FUNC
    /* prepare function when start to scan buttons for each group. */
    void (*btn_scan_prepare)(void);
#endif  /* MFBD_USE_BTN_SCAN_PRE_FUNC */

#if MFBD_USE_BTN_SCAN_AFTER_FUNC
    /* function after scanning buttons for each group. */
    void (*btn_scan_after)(void);
#endif  /* MFBD_USE_BTN_SCAN_AFTER_FUNC */

} mfbd_group_t;

#define MFBD_GROUP_NAME(GROUP)              mfbd_group_##GROUP
#define MFBD_GROUP_EXTERN(GROUP)            extern const mfbd_group_t MFBD_GROUP_NAME(GROUP)

#define MFBD_GROUP_DEFINE(GROUP, IS_BTN_DOWN_FUNC, BTN_VALUE_REPORT_FUNC, ...)      \
    const mfbd_group_t MFBD_GROUP_NAME(GROUP) = {                                   \
        IS_BTN_DOWN_FUNC,                                                           \
        BTN_VALUE_REPORT_FUNC,                                                      \
        __VA_ARGS__                                                                 \
    }

extern void mfbd_tbtn_scan(const mfbd_group_t *_pbtn_group, const mfbd_tbtn_info_t *_pbtn_info_start, const mfbd_tbtn_info_t *_pbtn_info_end);
extern void mfbd_tbtn_reset(const mfbd_tbtn_info_t *_pbtn_info_start, const mfbd_tbtn_info_t *_pbtn_info_end);

extern void mfbd_nbtn_scan(const mfbd_group_t *_pbtn_group, const mfbd_nbtn_info_t *_pbtn_info_start, const mfbd_nbtn_info_t *_pbtn_info_end);
extern void mfbd_nbtn_skip(const mfbd_group_t *_pbtn_group, const mfbd_nbtn_info_t *_pbtn_info_start, const mfbd_nbtn_info_t *_pbtn_info_end, mfbd_btn_count_t times);
extern void mfbd_nbtn_reset(const mfbd_nbtn_info_t *_pbtn_info_start, const mfbd_nbtn_info_t *_pbtn_info_end);

extern void mfbd_mbtn_scan(const mfbd_group_t *_pbtn_group, const mfbd_mbtn_info_t *_pbtn_info_start, const mfbd_mbtn_info_t *_pbtn_info_end);
extern void mfbd_mbtn_skip(const mfbd_group_t *_pbtn_group, const mfbd_mbtn_info_t *_pbtn_info_start, const mfbd_mbtn_info_t *_pbtn_info_end, mfbd_btn_count_t times);
extern void mfbd_mbtn_reset(const mfbd_mbtn_info_t *_pbtn_info_start, const mfbd_mbtn_info_t *_pbtn_info_end);

#if defined(__CC_ARM) || defined(__CLANG_ARM)           /* ARM Compiler */

#if MFBD_USE_TINY_BUTTON
#define MFBD_GROUP_SCAN_TBTN(GROUP)                                                                                         \
    do                                                                                                                      \
    {                                                                                                                       \
        extern const int MFBD_SECTION_START(GROUP, tbtn);                                                                   \
        extern const int MFBD_SECTION_END(GROUP, tbtn);                                                                     \
        mfbd_tbtn_scan((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_tbtn_info_t *)&(MFBD_SECTION_START(GROUP, tbtn)), (const mfbd_tbtn_info_t *)&(MFBD_SECTION_END(GROUP, tbtn)));    \
    } while (0)
#define MFBD_GROUP_RESET_TBTN(GROUP)                                                                \
    do                                                                                              \
    {                                                                                               \
        extern const int MFBD_SECTION_START(GROUP, tbtn);                                           \
        extern const int MFBD_SECTION_END(GROUP, tbtn);                                             \
        mfbd_tbtn_reset((const mfbd_tbtn_info_t *)&(MFBD_SECTION_START(GROUP, tbtn)), (const mfbd_tbtn_info_t *)&(MFBD_SECTION_END(GROUP, tbtn)));      \
    } while (0)
#else
#define MFBD_GROUP_SCAN_TBTN(GROUP)                 do{} while(0)
#define MFBD_GROUP_RESET_TBTN(GROUP)                do{} while(0)
#endif  /* MFBD_USE_TINY_BUTTON */

#if MFBD_USE_NORMAL_BUTTON
#define MFBD_GROUP_SCAN_NBTN(GROUP)                                                                                         \
    do                                                                                                                      \
    {                                                                                                                       \
        extern const int MFBD_SECTION_START(GROUP, nbtn);                                                                   \
        extern const int MFBD_SECTION_END(GROUP, nbtn);                                                                     \
        mfbd_nbtn_scan((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_nbtn_info_t *)&(MFBD_SECTION_START(GROUP, nbtn)), (const mfbd_nbtn_info_t *)&(MFBD_SECTION_END(GROUP, nbtn)));    \
    } while (0)
#define MFBD_GROUP_SKIP_NBTN(GROUP, TIMES)                                                          \
    do                                                                                              \
    {                                                                                               \
        extern const int MFBD_SECTION_START(GROUP, nbtn);                                           \
        extern const int MFBD_SECTION_END(GROUP, nbtn);                                             \
        mfbd_nbtn_skip((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_nbtn_info_t *)&(MFBD_SECTION_START(GROUP, nbtn)), (const mfbd_nbtn_info_t *)&(MFBD_SECTION_END(GROUP, nbtn)), TIMES);   \
    } while (0)
#define MFBD_GROUP_RESET_NBTN(GROUP)                                                                \
    do                                                                                              \
    {                                                                                               \
        extern const int MFBD_SECTION_START(GROUP, nbtn);                                           \
        extern const int MFBD_SECTION_END(GROUP, nbtn);                                             \
        mfbd_nbtn_reset((const mfbd_nbtn_info_t *)&(MFBD_SECTION_START(GROUP, nbtn)), (const mfbd_nbtn_info_t *)&(MFBD_SECTION_END(GROUP, nbtn)));      \
    } while (0)
#else
#define MFBD_GROUP_SCAN_NBTN(GROUP)                 do{} while(0)
#define MFBD_GROUP_SKIP_NBTN(GROUP, TIMES)          do{} while(0)
#define MFBD_GROUP_RESET_NBTN(GROUP)                do{} while(0)
#endif  /* MFBD_USE_NORMAL_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
#define MFBD_GROUP_SCAN_MBTN(GROUP)                                                                                         \
    do                                                                                                                      \
    {                                                                                                                       \
        extern const int MFBD_SECTION_START(GROUP, mbtn);                                                                   \
        extern const int MFBD_SECTION_END(GROUP, mbtn);                                                                     \
        mfbd_mbtn_scan((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_mbtn_info_t *)&(MFBD_SECTION_START(GROUP, mbtn)), (const mfbd_mbtn_info_t *)&(MFBD_SECTION_END(GROUP, mbtn)));    \
    } while (0)
#define MFBD_GROUP_SKIP_MBTN(GROUP, TIMES)                                                                                  \
    do                                                                                                                      \
    {                                                                                                                       \
        extern const int MFBD_SECTION_START(GROUP, mbtn);                                                                   \
        extern const int MFBD_SECTION_END(GROUP, mbtn);                                                                     \
        mfbd_mbtn_skip((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_mbtn_info_t *)&(MFBD_SECTION_START(GROUP, mbtn)), (const mfbd_mbtn_info_t *)&(MFBD_SECTION_END(GROUP, mbtn)), TIMES);   \
    } while (0)
#define MFBD_GROUP_RESET_MBTN(GROUP)                                                                \
    do                                                                                              \
    {                                                                                               \
        extern const int MFBD_SECTION_START(GROUP, mbtn);                                           \
        extern const int MFBD_SECTION_END(GROUP, mbtn);                                             \
        mfbd_mbtn_reset((const mfbd_mbtn_info_t *)&(MFBD_SECTION_START(GROUP, mbtn)), (const mfbd_mbtn_info_t *)&(MFBD_SECTION_END(GROUP, mbtn)));      \
    } while (0)
#else
#define MFBD_GROUP_SCAN_MBTN(GROUP)                 do{} while(0)
#define MFBD_GROUP_SKIP_MBTN(GROUP, TIMES)          do{} while(0)
#define MFBD_GROUP_RESET_MBTN(GROUP)                do{} while(0)
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

#elif defined (__IAR_SYSTEMS_ICC__)                     /* IAR Compiler */

#if MFBD_USE_TINY_BUTTON
#define MFBD_GROUP_SCAN_TBTN(GROUP)                                                                                 \
    do                                                                                                              \
    {                                                                                                               \
        mfbd_tbtn_scan((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_tbtn_info_t *)MFBD_SECTION_START(GROUP, tbtn), (const mfbd_tbtn_info_t *)MFBD_SECTION_END(GROUP, tbtn));  \
    } while (0)
#define MFBD_GROUP_RESET_TBTN(GROUP)                                                        \
    do                                                                                      \
    {                                                                                       \
        mfbd_tbtn_reset((const mfbd_tbtn_info_t *)MFBD_SECTION_START(GROUP, nbtn), (const mfbd_tbtn_info_t *)MFBD_SECTION_END(GROUP, nbtn));    \
    } while (0)
#else
#define MFBD_GROUP_SCAN_TBTN(GROUP)                 do{} while(0)
#endif  /* MFBD_USE_TINY_BUTTON */

#if MFBD_USE_NORMAL_BUTTON
#define MFBD_GROUP_SCAN_NBTN(GROUP)                                                                                 \
    do                                                                                                              \
    {                                                                                                               \
        mfbd_nbtn_scan((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_nbtn_info_t *)MFBD_SECTION_START(GROUP, nbtn), (const mfbd_nbtn_info_t *)MFBD_SECTION_END(GROUP, nbtn));  \
    } while (0)
#define MFBD_GROUP_SKIP_NBTN(GROUP, TIMES)                                                                          \
    do                                                                                                              \
    {                                                                                                               \
        mfbd_nbtn_skip((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_nbtn_info_t *)MFBD_SECTION_START(GROUP, nbtn), (const mfbd_nbtn_info_t *)MFBD_SECTION_END(GROUP, nbtn), TIMES);     \
    } while (0)
#define MFBD_GROUP_RESET_NBTN(GROUP)                                                        \
    do                                                                                      \
    {                                                                                       \
        mfbd_nbtn_reset((const mfbd_nbtn_info_t *)MFBD_SECTION_START(GROUP, nbtn), (const mfbd_nbtn_info_t *)MFBD_SECTION_END(GROUP, nbtn));    \
    } while (0)
#else
#define MFBD_GROUP_SCAN_NBTN(GROUP)                 do{} while(0)
#define MFBD_GROUP_SKIP_NBTN(GROUP, TIMES)          do{} while(0)
#define MFBD_GROUP_RESET_NBTN(GROUP)                do{} while(0)
#endif  /* MFBD_USE_NORMAL_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
#define MFBD_GROUP_SCAN_MBTN(GROUP)                                                                                 \
    do                                                                                                              \
    {                                                                                                               \
        mfbd_mbtn_scan((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_mbtn_info_t *)MFBD_SECTION_START(GROUP, mbtn), (const mfbd_mbtn_info_t *)MFBD_SECTION_END(GROUP, mbtn));    \
    } while (0)
#define MFBD_GROUP_SKIP_MBTN(GROUP, TIMES)                                                                          \
    do                                                                                                              \
    {                                                                                                               \
        mfbd_mbtn_skip((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_mbtn_info_t *)MFBD_SECTION_START(GROUP, mbtn), (const mfbd_mbtn_info_t *)MFBD_SECTION_END(GROUP, mbtn), TIMES);     \
    } while (0)
#define MFBD_GROUP_RESET_MBTN(GROUP)                                                        \
    do                                                                                      \
    {                                                                                       \
        mfbd_mbtn_reset((const mfbd_mbtn_info_t *)MFBD_SECTION_START(GROUP, mbtn), (const mfbd_mbtn_info_t *)MFBD_SECTION_END(GROUP, mbtn));    \
    } while (0)
#else
#define MFBD_GROUP_SCAN_MBTN(GROUP)                 do{} while(0)
#define MFBD_GROUP_SKIP_MBTN(GROUP, TIMES)          do{} while(0)
#define MFBD_GROUP_RESET_MBTN(GROUP)                do{} while(0)
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

#elif defined (__GNUC__)                                /* GNU GCC Compiler */

#if MFBD_USE_TINY_BUTTON
#define MFBD_GROUP_SCAN_TBTN(GROUP)                                                                                         \
    do                                                                                                                      \
    {                                                                                                                       \
        extern const int MFBD_SECTION_START(GROUP, tbtn);                                                                   \
        extern const int MFBD_SECTION_END(GROUP, tbtn);                                                                     \
        mfbd_tbtn_scan((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_tbtn_info_t *)&(MFBD_SECTION_START(GROUP, tbtn)), (const mfbd_tbtn_info_t *)&(MFBD_SECTION_END(GROUP, tbtn)));  \
    } while (0)
#define MFBD_GROUP_RESET_TBTN(GROUP)                                                                \
    do                                                                                              \
    {                                                                                               \
        extern const int MFBD_SECTION_START(GROUP, tbtn);                                           \
        extern const int MFBD_SECTION_END(GROUP, tbtn);                                             \
        mfbd_tbtn_reset((const mfbd_tbtn_info_t *)&(MFBD_SECTION_START(GROUP, tbtn)), (const mfbd_tbtn_info_t *)&(MFBD_SECTION_END(GROUP, tbtn)));  \
    } while (0)
#else
#define MFBD_GROUP_SCAN_TBTN(GROUP)                 do{} while(0)
#define MFBD_GROUP_RESET_TBTN(GROUP)                do{} while(0)
#endif  /* MFBD_USE_TINY_BUTTON */

#if MFBD_USE_NORMAL_BUTTON
#define MFBD_GROUP_SCAN_NBTN(GROUP)                                                                                         \
    do                                                                                                                      \
    {                                                                                                                       \
        extern const int MFBD_SECTION_START(GROUP, nbtn);                                                                   \
        extern const int MFBD_SECTION_END(GROUP, nbtn);                                                                     \
        mfbd_nbtn_scan((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_nbtn_info_t *)&(MFBD_SECTION_START(GROUP, nbtn)), (const mfbd_nbtn_info_t *)&(MFBD_SECTION_END(GROUP, nbtn)));      \
    } while (0)
#define MFBD_GROUP_SKIP_NBTN(GROUP, TIMES)                                                                                  \
    do                                                                                                                      \
    {                                                                                                                       \
        extern const int MFBD_SECTION_START(GROUP, nbtn);                                                                   \
        extern const int MFBD_SECTION_END(GROUP, nbtn);                                                                     \
        mfbd_nbtn_skip((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_nbtn_info_t *)&(MFBD_SECTION_START(GROUP, nbtn)), (const mfbd_nbtn_info_t *)&(MFBD_SECTION_END(GROUP, nbtn)), TIMES);   \
    } while (0)
#define MFBD_GROUP_RESET_NBTN(GROUP)                                                                \
    do                                                                                              \
    {                                                                                               \
        extern const int MFBD_SECTION_START(GROUP, nbtn);                                           \
        extern const int MFBD_SECTION_END(GROUP, nbtn);                                             \
        mfbd_nbtn_reset((const mfbd_nbtn_info_t *)&(MFBD_SECTION_START(GROUP, nbtn)), (const mfbd_nbtn_info_t *)&(MFBD_SECTION_END(GROUP, nbtn)));  \
    } while (0)
#else
#define MFBD_GROUP_SCAN_NBTN(GROUP)                 do{} while(0)
#define MFBD_GROUP_SKIP_NBTN(GROUP, TIMES)          do{} while(0)
#define MFBD_GROUP_RESET_NBTN(GROUP)                do{} while(0)
#endif  /* MFBD_USE_NORMAL_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
#define MFBD_GROUP_SCAN_MBTN(GROUP)                                                                                         \
    do                                                                                                                      \
    {                                                                                                                       \
        extern const int MFBD_SECTION_START(GROUP, mbtn);                                                                   \
        extern const int MFBD_SECTION_END(GROUP, mbtn);                                                                     \
        mfbd_mbtn_scan((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_mbtn_info_t *)&(MFBD_SECTION_START(GROUP, mbtn)), (const mfbd_mbtn_info_t *)&(MFBD_SECTION_END(GROUP, mbtn)));  \
    } while (0)
#define MFBD_GROUP_SKIP_MBTN(GROUP, TIMES)                                                                                  \
    do                                                                                                                      \
    {                                                                                                                       \
        extern const int MFBD_SECTION_START(GROUP, mbtn);                                                                   \
        extern const int MFBD_SECTION_END(GROUP, mbtn);                                                                     \
        mfbd_mbtn_skip((const mfbd_group_t *)&(MFBD_GROUP_NAME(GROUP)), (const mfbd_mbtn_info_t *)&(MFBD_SECTION_START(GROUP, mbtn)), (const mfbd_mbtn_info_t *)&(MFBD_SECTION_END(GROUP, mbtn)), TIMES);   \
    } while (0)
#define MFBD_GROUP_RESET_MBTN(GROUP)                                                                \
    do                                                                                              \
    {                                                                                               \
        extern const int MFBD_SECTION_START(GROUP, mbtn);                                           \
        extern const int MFBD_SECTION_END(GROUP, mbtn);                                             \
        mfbd_mbtn_reset((const mfbd_mbtn_info_t *)&(MFBD_SECTION_START(GROUP, mbtn)), (const mfbd_mbtn_info_t *)&(MFBD_SECTION_END(GROUP, mbtn)));  \
    } while (0)
#else
#define MFBD_GROUP_SCAN_MBTN(GROUP)                 do{} while(0)
#define MFBD_GROUP_SKIP_MBTN(GROUP, TIMES)          do{} while(0)
#define MFBD_GROUP_RESET_MBTN(GROUP)                do{} while(0)
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

#else
#error "not supported tool chain..."
#endif

#if MFBD_USE_BTN_SCAN_PRE_FUNC
#define MFBD_GROUP_SCAN_PREPARE(GROUP)                        \
    do                                                        \
    {                                                         \
        if(MFBD_GROUP_NAME(GROUP).btn_scan_prepare!= NULL)    \
        {                                                     \
          MFBD_GROUP_NAME(GROUP).btn_scan_prepare();          \
        }                                                     \
    } while(0)
#else
#define MFBD_GROUP_SCAN_PREPARE(GROUP)              do{} while(0)
#endif  /* MFBD_USE_BTN_SCAN_PRE_FUNC */

#if MFBD_USE_BTN_SCAN_AFTER_FUNC
#define MFBD_GROUP_SCAN_AFTER(GROUP)                          \
    do                                                        \
    {                                                         \
        if(MFBD_GROUP_NAME(GROUP).btn_scan_after!= NULL)      \
        {                                                     \
          MFBD_GROUP_NAME(GROUP).btn_scan_after();            \
        }                                                     \
    } while(0)
#else
#define MFBD_GROUP_SCAN_AFTER(GROUP)                do{} while(0)
#endif  /* MFBD_USE_BTN_SCAN_AFTER_FUNC */

/*
 * @Note:
 * this in a example for how to scan or reset the mfbd group,
 * if some group has not all btn types, you should write code by yourself.
 */
#define MFBD_GROUP_SCAN(GROUP)                      \
    do                                              \
    {                                               \
        MFBD_GROUP_SCAN_PREPARE(GROUP);             \
        MFBD_GROUP_SCAN_TBTN(GROUP);                \
        MFBD_GROUP_SCAN_NBTN(GROUP);                \
        MFBD_GROUP_SCAN_MBTN(GROUP);                \
        MFBD_GROUP_SCAN_AFTER(GROUP);               \
    } while (0)

#define MFBD_GROUP_SKIP(GROUP, TIMES)               \
    do                                              \
    {                                               \
        MFBD_GROUP_SKIP_NBTN(GROUP, TIMES);         \
        MFBD_GROUP_SKIP_MBTN(GROUP, TIMES);         \
    } while (0)

#define MFBD_GROUP_RESET(GROUP)                     \
    do                                              \
    {                                               \
        MFBD_GROUP_RESET_TBTN(GROUP);               \
        MFBD_GROUP_RESET_NBTN(GROUP);               \
        MFBD_GROUP_RESET_MBTN(GROUP);               \
    } while (0)

#endif  /* MFBD_USE_SECTION_DEFINITION */

#endif  /* _MFBD_SD_H_ */
