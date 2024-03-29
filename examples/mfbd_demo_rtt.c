/*
 * Copyright (c) 2022, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-22     smartmx      the first version
 * 2022-03-15     smartmx      each mbtn has it's own max multi-click times
 * 2022-04-16     smartmx      drop list definitions, use arraylist, each group has all btn types.
 * 2022-08-05     smartmx      add reset params function.
 * 2022-08-15     smartmx      fix bugs.
 * 2023-07-03     smartmx      add Section Definition option.
 *
 */

#include <rtthread.h>
#include <board.h>
#include "mfbd.h"
#include "mfbd_sd.h"

#define MFBD_DEMO_USE_DEFAULT_DEFINE            1       /* set to 1, you can study how to use default define APIs. */

void bsp_btn_value_report(mfbd_btn_code_t btn_value);
unsigned char bsp_btn_check(mfbd_btn_index_t btn_index);

#ifndef BTN_KEY0
    #define BTN_KEY0 GET_PIN(H, 3)
#endif

#ifndef BTN_KEY1
    #define BTN_KEY1 GET_PIN(H, 2)
#endif

#ifndef BTN_KEY2
    #define BTN_KEY2 GET_PIN(C, 13)
#endif

#ifndef BTN_WK_UP
    #define BTN_WK_UP GET_PIN(A, 0)
#endif

/* default definition example */
#if MFBD_DEMO_USE_DEFAULT_DEFINE

enum
{
    MFBD_DOWN_CODE_NAME(test_tbtn)      = 0x1201,
    MFBD_UP_CODE_NAME(test_tbtn)        = 0x1200,
    MFBD_LONG_CODE_NAME(test_tbtn)      = 0x1202,

    MFBD_DOWN_CODE_NAME(test_nbtn)      = 0x1301,
    MFBD_UP_CODE_NAME(test_nbtn)        = 0x1300,
    MFBD_LONG_CODE_NAME(test_nbtn)      = 0x1301,

    MFBD_DOWN_CODE_NAME(test_nbtn1)     = 0x1401,
    MFBD_UP_CODE_NAME(test_nbtn1)       = 0x1400,
    MFBD_LONG_CODE_NAME(test_nbtn1)     = 0x1402,

    MFBD_UP_CODE_NAME(test_mbtn)        = 0x1500,
    MFBD_LONG_CODE_NAME(test_mbtn)      = 0x1502,
};

mfbd_btn_code_t MFBD_DOWN_CODES_NAME(test_mbtn)[4] = {0x1501, 0x1511, 0x1521, 0x1531};

#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */

#if MFBD_USE_SECTION_DEFINITION
/* use section definition. */

#if MFBD_PARAMS_SAME_IN_GROUP

/* tbtn test */
#if MFBD_USE_TINY_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_TBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX) */
MFBD_TBTN_DEFAULT_DEFINE(test_btns, test_tbtn, 1);
#else
/* MFBD_TBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, BTN_DOWN_CODE, BTN_UP_CODE) */
MFBD_TBTN_DEFINE(test_btns, test_tbtn, 1, 0x1201, 0x1200);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_TINY_BUTTON */

/* nbtn test */
#if MFBD_USE_NORMAL_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_NBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME) */
MFBD_NBTN_DEFAULT_DEFINE(test_btns, test_nbtn1, 3);
MFBD_NBTN_DEFAULT_DEFINE(test_btns, test_nbtn, 2);
#else
/* MFBD_NBTN_DEFINE(NAME, BTN_INDEX, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE) */
MFBD_NBTN_DEFINE(test_btns, test_nbtn1, 3,  0x1401, 0x1400, 0x1402);
MFBD_NBTN_DEFINE(test_btns, test_nbtn, 2, 0x1301, 0x1300, 0x1301);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_NORMAL_BUTTON */

/* mbtn test */
#if MFBD_USE_MULTIFUCNTION_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_MBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX, MAX_MULTICLICK_STATE) */
MFBD_MBTN_DEFAULT_DEFINE(test_btns, test_mbtn, 4, 3);
#else
/* MFBD_MBTN_DEFINE(NAME, BTN_INDEX, MAX_MULTICLICK_STATE, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE, ...) */
MFBD_MBTN_DEFINE(test_btns, test_mbtn, 4, 3, 0x1501, 0x1500, 0x1501, 0x1511, 0x1521, 0x1531);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

#else

/* tbtn test */
#if MFBD_USE_TINY_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_TBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX, FILTER_TIME) */
MFBD_TBTN_DEFAULT_DEFINE(test_btns, test_tbtn, 1, 3);
#else
/* MFBD_TBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, BTN_DOWN_CODE, BTN_UP_CODE) */
MFBD_TBTN_DEFINE(test_btns, test_tbtn, 1, 3, 0x1201, 0x1200);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_TINY_BUTTON */

/* nbtn test */
#if MFBD_USE_NORMAL_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_NBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME) */
MFBD_NBTN_DEFAULT_DEFINE(test_btns, test_nbtn1, 3, 3, 0, 150);
MFBD_NBTN_DEFAULT_DEFINE(test_btns, test_nbtn, 2, 3, 30, 150);
#else
/* MFBD_NBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE) */
MFBD_NBTN_DEFINE(test_btns, test_nbtn1, 3, 3, 0, 150, 0x1401, 0x1400, 0x1402);
MFBD_NBTN_DEFINE(test_btns, test_nbtn, 2, 3, 30, 150, 0x1301, 0x1300, 0x1301);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_NORMAL_BUTTON */

/* mbtn test */
#if MFBD_USE_MULTIFUCNTION_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_MBTN_DEFAULT_DEFINE(GROUP, NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, MULTICLICK_TIME, MAX_MULTICLICK_STATE) */
MFBD_MBTN_DEFAULT_DEFINE(test_btns, test_mbtn, 4, 3, 30, 150, 75, 3);
#else
/* MFBD_MBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, MULTICLICK_TIME, MAX_MULTICLICK_STATE, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE, ...) */
MFBD_MBTN_DEFINE(test_btns, test_mbtn, 4, 3, 30, 150, 75, 3, 0x1501, 0x1500, 0x1501, 0x1511, 0x1521, 0x1531);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

#endif /*MFBD_PARAMS_SAME_IN_GROUP*/

//MFBD_GROUP_DEFINE(test_btns, bsp_btn_check, bsp_btn_value_report, 3, 30, 150, 75);
//MFBD_GROUP_DEFINE(test_btns, bsp_btn_check, bsp_btn_value_report);

const mfbd_group_t MFBD_GROUP_NAME(test_btns) = {
    bsp_btn_check,
    bsp_btn_value_report,
#if MFBD_PARAMS_SAME_IN_GROUP

#if MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON
    3,
#endif /*  MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON
    30,
    150,
#endif /* MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
    75,
#endif /* MFBD_USE_MULTIFUCNTION_BUTTON */

#endif /*MFBD_PARAMS_SAME_IN_GROUP*/
};

#else

#if MFBD_PARAMS_SAME_IN_GROUP

/* tbtn test */
#if MFBD_USE_TINY_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_TBTN_DEFAULT_DEFINE(NAME, BTN_INDEX) */
MFBD_TBTN_DEFAULT_DEFINE(test_tbtn, 1);
#else
/* MFBD_TBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, BTN_DOWN_CODE, BTN_UP_CODE) */
MFBD_TBTN_DEFINE(test_tbtn, 1, 0x1201, 0x1200);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_TINY_BUTTON */

/* nbtn test */
#if MFBD_USE_NORMAL_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_NBTN_DEFAULT_DEFINE(NAME, BTN_INDEX) */
MFBD_NBTN_DEFAULT_DEFINE(test_nbtn1, 3);
MFBD_NBTN_DEFAULT_DEFINE(test_nbtn, 2);
#else
/* MFBD_NBTN_DEFINE(NAME, BTN_INDEX, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE) */
MFBD_NBTN_DEFINE(test_nbtn1, 3,  0x1401, 0x1400, 0x1402);
MFBD_NBTN_DEFINE(test_nbtn, 2, 0x1301, 0x1300, 0x1301);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_NORMAL_BUTTON */

/* mbtn test */
#if MFBD_USE_MULTIFUCNTION_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_MBTN_DEFAULT_DEFINE(NAME, BTN_INDEX, MAX_MULTICLICK_STATE) */
MFBD_MBTN_DEFAULT_DEFINE(test_mbtn, 4, 3);
#else
/* FBD_MBTN_DEFINE(NAME, BTN_INDEX, MAX_MULTICLICK_STATE, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE, ...) */
MFBD_MBTN_DEFINE(test_mbtn, 4, 3, 0x1501, 0x1500, 0x1501, 0x1511, 0x1521, 0x1531);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

#else

/* tbtn test */
#if MFBD_USE_TINY_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_TBTN_DEFAULT_DEFINE(NAME, BTN_INDEX, FILTER_TIME) */
MFBD_TBTN_DEFAULT_DEFINE(test_tbtn, 1, 3);
#else
/* MFBD_TBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, BTN_DOWN_CODE, BTN_UP_CODE) */
MFBD_TBTN_DEFINE(test_tbtn, 1, 3, 0x1201, 0x1200);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_TINY_BUTTON */

/* nbtn test */
#if MFBD_USE_NORMAL_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_NBTN_DEFAULT_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME) */
MFBD_NBTN_DEFAULT_DEFINE(test_nbtn1, 3, 3, 0, 150);
MFBD_NBTN_DEFAULT_DEFINE(test_nbtn, 2, 3, 30, 150);
#else
/* MFBD_NBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE) */
MFBD_NBTN_DEFINE(test_nbtn1, 3, 3, 0, 150, 0x1401, 0x1400, 0x1402);
MFBD_NBTN_DEFINE(test_nbtn, 2, 3, 30, 150, 0x1301, 0x1300, 0x1301);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_NORMAL_BUTTON */

/* mbtn test */
#if MFBD_USE_MULTIFUCNTION_BUTTON
#if MFBD_DEMO_USE_DEFAULT_DEFINE
/* MFBD_MBTN_DEFAULT_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, MULTICLICK_TIME, MAX_MULTICLICK_STATE) */
MFBD_MBTN_DEFAULT_DEFINE(test_mbtn, 4, 3, 30, 150, 75, 3);
#else
/* MFBD_MBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, MULTICLICK_TIME, MAX_MULTICLICK_STATE, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE, ...) */
MFBD_MBTN_DEFINE(test_mbtn, 4, 3, 30, 150, 75, 3, 0x1501, 0x1500, 0x1501, 0x1511, 0x1521, 0x1531);
#endif  /* MFBD_DEMO_USE_DEFAULT_DEFINE */
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

#endif /*MFBD_PARAMS_SAME_IN_GROUP*/

#if MFBD_USE_TINY_BUTTON
MFBD_TBTN_ARRAYLIST(test_tbtn_list, &test_tbtn);
#endif /* MFBD_USE_TINY_BUTTON */

#if MFBD_USE_NORMAL_BUTTON
MFBD_NBTN_ARRAYLIST(test_nbtn_list, &test_nbtn1, &test_nbtn);
#endif /* MFBD_USE_NORMAL_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
MFBD_MBTN_ARRAYLIST(test_mbtn_list, &test_mbtn);
#endif /* MFBD_USE_MULTIFUCNTION_BUTTON */

const mfbd_group_t test_btn_group =
{
    bsp_btn_check,
    bsp_btn_value_report,

#if MFBD_USE_TINY_BUTTON
    test_tbtn_list,
#endif /* MFBD_USE_TINY_BUTTON */

#if MFBD_USE_NORMAL_BUTTON
    test_nbtn_list,
#endif /* MFBD_USE_NORMAL_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
    test_mbtn_list,
#endif /* MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_PARAMS_SAME_IN_GROUP

#if MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON
    3,
#endif /*  MFBD_USE_TINY_BUTTON || MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON
    30,
    150,
#endif /* MFBD_USE_NORMAL_BUTTON || MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
    75,
#endif /* MFBD_USE_MULTIFUCNTION_BUTTON */

#endif /*MFBD_PARAMS_SAME_IN_GROUP*/

};

#endif  /*MFBD_USE_SECTION_DEFINITION*/

unsigned char bsp_btn_check(mfbd_btn_index_t btn_index)
{
    switch (btn_index)
    {
    case 1:
        if (rt_pin_read(BTN_KEY0) == 0)
        {
            return MFBD_BTN_STATE_DOWN;
        }
        break;
    case 2:
        if (rt_pin_read(BTN_KEY1) == 0)
        {
            return MFBD_BTN_STATE_DOWN;
        }
        break;
    case 3:
        if (rt_pin_read(BTN_KEY2) == 0)
        {
            return MFBD_BTN_STATE_DOWN;
        }
        break;
    case 4:
        if (rt_pin_read(BTN_WK_UP) == 1)
        {
            return MFBD_BTN_STATE_DOWN;
        }
        break;
    default:
        break;
    }
    return MFBD_BTN_STATE_UP;
}

void bsp_btn_value_report(mfbd_btn_code_t btn_value)
{
    rt_kprintf("%04x\n", btn_value);
}

static void mfbd_scan(void *arg)
{
    while (1)
    {
        #if MFBD_USE_SECTION_DEFINITION
        /* use section definition. */

        MFBD_GROUP_SCAN(test_btns);

        #else

        mfbd_group_scan(&test_btn_group); /* scan button group */

        #endif  /*MFBD_USE_SECTION_DEFINITION*/

        rt_thread_mdelay(10); /* scan period: 10ms */
    }
}

static void user_button_init(void)
{

    rt_pin_mode(BTN_KEY0, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */
    rt_pin_mode(BTN_KEY1, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */
    rt_pin_mode(BTN_KEY2, PIN_MODE_INPUT_PULLUP); /* set KEY pin mode to input */
    rt_pin_mode(BTN_WK_UP, PIN_MODE_INPUT_PULLDOWN); /* set KEY pin mode to input */

}

int mfbd_main(void)
{
    rt_thread_t tid = RT_NULL;

    user_button_init();

    /* Create background ticks thread */
    tid = rt_thread_create("mfbd", mfbd_scan, RT_NULL, 1024, 10, 10);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }

    return 0;
}

INIT_APP_EXPORT(mfbd_main);

