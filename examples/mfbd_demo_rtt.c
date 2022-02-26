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

#include <rtthread.h>
#include <board.h>

#include "mfbd.h"

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

/* MFBD_TBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, BTN_DOWN_CODE, BTN_UP_CODE, NEXT) */
MFBD_TBTN_DEFINE(test_tbtn, 1, 3, 0x1201, 0x1200, NULL);

const mfbd_group_t test_tbtn_group =
{
    bsp_btn_check,
    bsp_btn_value_report,
    &test_tbtn,
};

/* nbtn test */
/* MFBD_NBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, BTN_DOWN_CODE, BTN_UP_CODE, BTN_LONG_CODE, NEXT) */
MFBD_NBTN_DEFINE(test_nbtn1, 3, 3, 0, 150, 0x1401, 0x1400, 0x1402, NULL);

MFBD_NBTN_DEFINE(test_nbtn, 2, 3, 30, 150, 0x1301, 0x1300, 0, &test_nbtn1);

const mfbd_group_t test_nbtn_group =
{
    bsp_btn_check,
    bsp_btn_value_report,
    &test_nbtn,
};

/* mbtn test */
/* MFBD_MBTN_DEFINE(NAME, BTN_INDEX, FILTER_TIME, REPEAT_TIME, LONG_TIME, MULTICLICK_TIME, BTN_UP_CODE, BTN_LONG_CODE, NEXT, BTN_DOWN_CODE, ...) */
MFBD_MBTN_DEFINE(test_mbtn, 4, 3, 30, 150, 75, 0x1500, 0, NULL, 0x1501, 0x1511, 0x1521, 0x1531);

const mfbd_group_t test_mbtn_group =
{
    bsp_btn_check,
    bsp_btn_value_report,
    &test_mbtn,
};


unsigned char bsp_btn_check(mfbd_btn_index_t btn_index)
{
    switch (btn_index)
    {
    case 1:
        if (rt_pin_read(BTN_KEY0) == 0)
        {
            return 1;
        }
        break;
    case 2:
        if (rt_pin_read(BTN_KEY1) == 0)
        {
            return 1;
        }
        break;
    case 3:
        if (rt_pin_read(BTN_KEY2) == 0)
        {
            return 1;
        }
        break;
    case 4:
        if (rt_pin_read(BTN_WK_UP) == 1)
        {
            return 1;
        }
        break;
    default:
        break;
    }
    return 0;
}

void bsp_btn_value_report(mfbd_btn_code_t btn_value)
{
    rt_kprintf("%04x\n", btn_value);
}

static void mfbd_scan(void *arg)
{
    while (1)
    {
        mfbd_tbtn_scan(&test_tbtn_group); /* scan tiny button group */
        mfbd_nbtn_scan(&test_nbtn_group); /* scan normal button group */
        mfbd_mbtn_scan(&test_mbtn_group); /* scan multi-function button group */
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
#ifdef FINSH_USING_MSH
    INIT_APP_EXPORT(mfbd_main);
#endif
