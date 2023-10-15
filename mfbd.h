/*
 * Copyright (c) 2022-2023, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-22     smartmx      the first version.
 * 2022-03-15     smartmx      each mbtn has it's own max multi-click times.
 * 2022-04-16     smartmx      drop list definitions, use array, each group has all btn types.
 * 2022-08-05     smartmx      add reset params function.
 * 2022-08-15     smartmx      fix bugs.
 * 2022-09-07     smartmx      add default define apis.
 * 2023-03-15     smartmx      add state declaration.
 * 2023-07-03     smartmx      add Section Definition option.
 * 2023-07-15     smartmx      add skip function, to reduce calling of scan functions.
 * 2023-09-19     smartmx      improve performance, add MFBD_BTN_STATE_SKIP.
 *
 */

#ifndef _MFBD_H_
#define _MFBD_H_

#include "mfbd_cfg.h"

#if (MFBD_USE_SECTION_DEFINITION == 0)

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

/* tiny button definitions, tiny button functions only support down and up event. */
#if MFBD_PARAMS_SAME_IN_GROUP

typedef struct _mfbd_tbtn_info_struct
{
    mfbd_btn_code_t    btn_down_code;               /* keyCode when button down, set to 0 will not report it. */
    mfbd_btn_code_t    btn_up_code;                 /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_index_t   btn_index;                   /* parameter when calling is_btn_down_func. */
} mfbd_tbtn_info_t;

#else

typedef struct _mfbd_tbtn_info_struct
{
    mfbd_btn_code_t    btn_down_code;               /* keyCode when button down, set to 0 will not report it. */
    mfbd_btn_code_t    btn_up_code;                 /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_count_t   filter_time;                 /* filter time when button state changed, please do not use 0. */
    mfbd_btn_index_t   btn_index;                   /* parameter when calling is_btn_down_func. */
} mfbd_tbtn_info_t;

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

typedef struct _mfbd_tiny_btn_struct
{
    const   mfbd_tbtn_info_t        *btn_info;      /* a pointer to mfbd_tbtn_info_t. */
    mfbd_btn_count_t                filter_count;   /* filter time count when button state changed. */
    unsigned char                   state;          /* the state of button, up or down. */
} mfbd_tbtn_t;

#if MFBD_PARAMS_SAME_IN_GROUP

#define MFBD_TBTN_DEFINE(NAME, BTN_INDEX, BTN_DOWN_CODE, BTN_UP_CODE)   \
static const mfbd_tbtn_info_t NAME##_info = {                           \
        BTN_DOWN_CODE,                                                  \
        BTN_UP_CODE,                                                    \
        BTN_INDEX,                                                      \
};                                                                      \
mfbd_tbtn_t NAME = {                                                    \
        &NAME##_info,                                                   \
        0,                                                              \
        0,                                                              \
}

#define MFBD_TBTN_DEFAULT_DEFINE(NAME, BTN_INDEX)                       \
static const mfbd_tbtn_info_t NAME##_info = {                           \
        NAME##_DOWN_CODE,                                               \
        NAME##_UP_CODE,                                                 \
        BTN_INDEX,                                                      \
};                                                                      \
mfbd_tbtn_t NAME = {                                                    \
        &NAME##_info,                                                   \
        0,                                                              \
        0,                                                              \
}

#else

#define MFBD_TBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, BTN_DOWN_CODE, BTN_UP_CODE)    \
static const mfbd_tbtn_info_t NAME##_info = {                                         \
        BTN_DOWN_CODE,                                                                \
        BTN_UP_CODE,                                                                  \
        FILTER_TIME,                                                                  \
        BTN_INDEX,                                                                    \
};                                                                                    \
mfbd_tbtn_t NAME = {                                                                  \
        &NAME##_info,                                                                 \
        0,                                                                            \
        0,                                                                            \
}

#define MFBD_TBTN_DEFAULT_DEFINE(NAME, BTN_INDEX, FILTER_TIME)          \
static const mfbd_tbtn_info_t NAME##_info = {                           \
        NAME##_DOWN_CODE,                                               \
        NAME##_UP_CODE,                                                 \
        FILTER_TIME,                                                    \
        BTN_INDEX,                                                      \
};                                                                      \
mfbd_tbtn_t NAME = {                                                    \
        &NAME##_info,                                                   \
        0,                                                              \
        0,                                                              \
}

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

#define MFBD_TBTN_EXTERN(NAME) extern mfbd_tbtn_t NAME

#define MFBD_TBTN_ARRAYLIST(NAME, ...) mfbd_tbtn_t* NAME[] = {__VA_ARGS__, NULL}



/* normal button definitions, normal button functions support down, up and long-press event. */
#if MFBD_PARAMS_SAME_IN_GROUP

typedef struct _mfbd_nbtn_info_struct
{
    mfbd_btn_code_t    btn_down_code;               /* keyCode when button down, set to 0 will not report it. */
    mfbd_btn_code_t    btn_up_code;                 /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_code_t    btn_long_code;               /* keyCode when button down for long_time, set 0 will not report it ,but report btn_down_code instead. */
    mfbd_btn_index_t   btn_index;                   /* parameter when calling is_btn_down_func. */
} mfbd_nbtn_info_t;

#else

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

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

typedef struct _mfbd_normal_btn_struct
{
    const   mfbd_nbtn_info_t          *btn_info;        /* a pointer to mfbd_nbtn_info_t. */
    mfbd_btn_count_t                  filter_count;     /* filter time count when button state changed. */
    mfbd_btn_count_t                  long_count;       /* long time count when button still down. */
    mfbd_btn_count_t                  repeat_count;     /* repeat time count when button still down. */
    unsigned char                     state;            /* the state of button, up or down. */
} mfbd_nbtn_t;


#if MFBD_PARAMS_SAME_IN_GROUP

#define MFBD_NBTN_DEFINE(NAME, BTN_INDEX, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE)    \
static const mfbd_nbtn_info_t NAME##_info = {                                           \
        BTN_DOWN_CODE,                                                                  \
        BTN_UP_CODE,                                                                    \
        BTN_LONG_CODE,                                                                  \
        BTN_INDEX,                                                                      \
};                                                                                      \
mfbd_nbtn_t NAME = {                                                                    \
        &NAME##_info,                                                                   \
        0,                                                                              \
        0,                                                                              \
        0,                                                                              \
        0,                                                                              \
}

#define MFBD_NBTN_DEFAULT_DEFINE(NAME, BTN_INDEX)                                                   \
static const mfbd_nbtn_info_t NAME##_info = {                                                       \
        NAME##_DOWN_CODE,                                                                           \
        NAME##_UP_CODE,                                                                             \
        NAME##_LONG_CODE,                                                                           \
        BTN_INDEX,                                                                                  \
};                                                                                                  \
mfbd_nbtn_t NAME = {                                                                                \
        &NAME##_info,                                                                               \
        0,                                                                                          \
        0,                                                                                          \
}

#else

#define MFBD_NBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE)   \
static const mfbd_nbtn_info_t NAME##_info = {                                                                               \
        FILTER_TIME,                                                                                                        \
        REPEAT_TIME,                                                                                                        \
        LONG_TIME,                                                                                                          \
        BTN_DOWN_CODE,                                                                                                      \
        BTN_UP_CODE,                                                                                                        \
        BTN_LONG_CODE,                                                                                                      \
        BTN_INDEX,                                                                                                          \
};                                                                                                                          \
mfbd_nbtn_t NAME = {                                                                                                        \
        &NAME##_info,                                                                                                       \
        0,                                                                                                                  \
        0,                                                                                                                  \
        0,                                                                                                                  \
        0,                                                                                                                  \
}

#define MFBD_NBTN_DEFAULT_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME)              \
static const mfbd_nbtn_info_t NAME##_info = {                                                       \
        FILTER_TIME,                                                                                \
        REPEAT_TIME,                                                                                \
        LONG_TIME,                                                                                  \
        NAME##_DOWN_CODE,                                                                           \
        NAME##_UP_CODE,                                                                             \
        NAME##_LONG_CODE,                                                                           \
        BTN_INDEX,                                                                                  \
};                                                                                                  \
mfbd_nbtn_t NAME = {                                                                                \
        &NAME##_info,                                                                               \
        0,                                                                                          \
        0,                                                                                          \
}

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

#define MFBD_NBTN_EXTERN(NAME) extern mfbd_nbtn_t NAME

#define MFBD_NBTN_ARRAYLIST(NAME, ...) mfbd_nbtn_t* NAME[] = {__VA_ARGS__, NULL}



/* multi-function button definitions, multi-function button functions support down, up, long-press and multi-click event. */
#if MFBD_PARAMS_SAME_IN_GROUP

typedef struct _mfbd_mbtn_info_struct
{
    const mfbd_btn_code_t   *btn_down_code;           /* pointer to multi-click keyCodes when button down. */
    mfbd_btn_code_t         btn_up_code;              /* keyCode when button up, set to 0 will not report it. */
    mfbd_btn_code_t         btn_long_code;            /* keyCode when button down for long_time, set 0 will not report it ,but report btn_down_code[0] instead. */
    mfbd_btn_index_t        btn_index;                /* parameter when calling is_btn_down_func. */
    unsigned char           max_multiclick_state;     /* max multiclick states. */
} mfbd_mbtn_info_t;

#else

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
    unsigned char      max_multiclick_state;    /* max multiclick states. */
} mfbd_mbtn_info_t;

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */
/*
 * @Note:
 * repeat_count and long_count are conflict to multi-click.
 * repeat_count and long_count only check in the first button down event, and will disable in next multi-click event.
 * also, if repeat_count and long_count event has happened in the first down event, it will reset multiclick_state.
 */
typedef struct _mfbd_multi_fuction_btn_struct
{
    const   mfbd_mbtn_info_t                  *btn_info;              /* a pointer to mfbd_mbtn_info_t. */
    mfbd_btn_count_t                          filter_count;           /* filter time count when button state changed. */
    mfbd_btn_count_t                          long_count;             /* long time count when button still down. */
    mfbd_btn_count_t                          repeat_count;           /* repeat time count when button still down. */
    mfbd_btn_count_t                          multiclick_count;       /* multi-click time count when button is up. */
    unsigned char                             multiclick_state;       /* multi-click count when button is in multi-click state. */
    unsigned char                             state;                  /* the state of button, up or down. */
} mfbd_mbtn_t;

#if MFBD_PARAMS_SAME_IN_GROUP

#define MFBD_MBTN_DEFINE(NAME, BTN_INDEX, MAX_MULTICLICK_STATE, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE, ...)   \
static const mfbd_btn_code_t NAME##_down_codes[MAX_MULTICLICK_STATE + 1] = {BTN_DOWN_CODE, __VA_ARGS__};          \
static const mfbd_mbtn_info_t NAME##_info = {                                                                     \
        NAME##_down_codes,                                                                                        \
        BTN_UP_CODE,                                                                                              \
        BTN_LONG_CODE,                                                                                            \
        BTN_INDEX,                                                                                                \
        MAX_MULTICLICK_STATE,                                                                                     \
};                                                                                                                \
mfbd_mbtn_t NAME = {                                                                                              \
        &NAME##_info,                                                                                             \
        0,                                                                                                        \
        0,                                                                                                        \
        0,                                                                                                        \
        0,                                                                                                        \
        0,                                                                                                        \
        0,                                                                                                        \
}

#define MFBD_MBTN_DEFAULT_DEFINE(NAME, BTN_INDEX, MAX_MULTICLICK_STATE)                             \
static const mfbd_mbtn_info_t NAME##_info = {                                                       \
        NAME##_DOWN_CODES,                                                                          \
        NAME##_UP_CODE,                                                                             \
        NAME##_LONG_CODE,                                                                           \
        BTN_INDEX,                                                                                  \
        MAX_MULTICLICK_STATE,                                                                       \
};                                                                                                  \
mfbd_mbtn_t NAME = {                                                                                \
        &NAME##_info,                                                                               \
        0,                                                                                          \
        0,                                                                                          \
        0,                                                                                          \
        0,                                                                                          \
        0,                                                                                          \
        0,                                                                                          \
}

#else

#define MFBD_MBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, MULTICLICK_TIME, MAX_MULTICLICK_STATE, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE, ...)   \
static const mfbd_btn_code_t NAME##_down_codes[MAX_MULTICLICK_STATE + 1] = {BTN_DOWN_CODE, __VA_ARGS__};                                                                \
static const mfbd_mbtn_info_t NAME##_info = {                                                                                                                           \
        FILTER_TIME,                                                                                                                                                    \
        REPEAT_TIME,                                                                                                                                                    \
        LONG_TIME,                                                                                                                                                      \
        MULTICLICK_TIME,                                                                                                                                                \
        NAME##_down_codes,                                                                                                                                              \
        BTN_UP_CODE,                                                                                                                                                    \
        BTN_LONG_CODE,                                                                                                                                                  \
        BTN_INDEX,                                                                                                                                                      \
        MAX_MULTICLICK_STATE,                                                                                                                                           \
};                                                                                                                                                                      \
mfbd_mbtn_t NAME = {                                                                                                                                                    \
        &NAME##_info,                                                                                                                                                   \
        0,                                                                                                                                                              \
        0,                                                                                                                                                              \
        0,                                                                                                                                                              \
        0,                                                                                                                                                              \
        0,                                                                                                                                                              \
        0,                                                                                                                                                              \
}

#define MFBD_MBTN_DEFAULT_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, MULTICLICK_TIME, MAX_MULTICLICK_STATE)               \
static const mfbd_mbtn_info_t NAME##_info = {                                                                                               \
        FILTER_TIME,                                                                                                                        \
        REPEAT_TIME,                                                                                                                        \
        LONG_TIME,                                                                                                                          \
        MULTICLICK_TIME,                                                                                                                    \
        NAME##_DOWN_CODES,                                                                                                                  \
        NAME##_UP_CODE,                                                                                                                     \
        NAME##_LONG_CODE,                                                                                                                   \
        BTN_INDEX,                                                                                                                          \
        MAX_MULTICLICK_STATE,                                                                                                               \
};                                                                                                                                          \
mfbd_mbtn_t NAME = {                                                                                                                        \
        &NAME##_info,                                                                                                                       \
        0,                                                                                                                                  \
        0,                                                                                                                                  \
        0,                                                                                                                                  \
        0,                                                                                                                                  \
        0,                                                                                                                                  \
        0,                                                                                                                                  \
}

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */

#define MFBD_MBTN_EXTERN(NAME) extern mfbd_mbtn_t NAME

#define MFBD_MBTN_ARRAYLIST(NAME, ...) mfbd_mbtn_t* NAME[] = {__VA_ARGS__, NULL}



/* mfbd group struct */
typedef struct _mfbd_group_struct
{
    /* used to read whether button is down. */
    unsigned char (*is_btn_down_func)(mfbd_btn_index_t btn_index);

    /* used to report btn_value, must have a legal value, must not be NULL. */
    void (*btn_value_report)(mfbd_btn_code_t btn_value);

#if MFBD_USE_TINY_BUTTON
    /* pointer to the head of tiny buttons pointer array */
    mfbd_tbtn_t **tbtns;
#endif  /* MFBD_USE_TINY_BUTTON */

#if MFBD_USE_NORMAL_BUTTON
    /* pointer to the head of normal buttons pointer array */
    mfbd_nbtn_t **nbtns;
#endif  /* MFBD_USE_NORMAL_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
    /* pointer to the head of multifunction buttons pointer array */
    mfbd_mbtn_t **mbtns;
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

/* if set MFBD_PARAMS_SAME_IN_GROUP to 1, all btns in group has same params. */
#if MFBD_PARAMS_SAME_IN_GROUP

#if MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON
    mfbd_btn_count_t   filter_time;             /* filter time when button state changed, please do not use 0. */
#endif /*  MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON

    mfbd_btn_count_t   repeat_time;             /* repeat time when button still down for over long_time, set 0 will disable repeat time count. */

    mfbd_btn_count_t   long_time;               /* long time when button still down, set 0 will disable long time and repeat time count. */

#endif  /* MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON

    mfbd_btn_count_t   multiclick_time;         /* multi-click time when button still up, set 0 will disable multi-click time count. */

#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

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

extern void mfbd_group_scan(const mfbd_group_t *_pbtn_group);

extern void mfbd_group_skip(const mfbd_group_t *_pbtn_group, mfbd_btn_count_t times);

extern void mfbd_group_reset(const mfbd_group_t *_pbtn_group);

#endif  /* (MFBD_USE_SECTION_DEFINITION == 0) */

#endif  /* _MFBD_H_ */
