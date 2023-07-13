/*
 * Copyright (c) 2022-2023, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-03     smartmx      the first version, Multi-Function Button Dectection with Section Definition.
 *
 */

#include "mfbd_sd.h"

#if MFBD_USE_SECTION_DEFINITION

#if MFBD_PARAMS_SAME_IN_GROUP

/* filter_time */
#define MFBD_FILTER_TIME_IN_FUC         (_pbtn_group->filter_time)
/* long_time */
#define MFBD_LONG_TIME_IN_FUC           (_pbtn_group->long_time)
/* repeat_time */
#define MFBD_REPEAT_TIME_IN_FUC         (_pbtn_group->repeat_time)
/* multiclick_time */
#define MFBD_MULTICLICK_TIME_IN_FUC     (_pbtn_group->multiclick_time)

#else

/* filter_time */
#define MFBD_FILTER_TIME_IN_FUC         (_pbtn_info->filter_time)
/* long_time */
#define MFBD_LONG_TIME_IN_FUC           (_pbtn_info->long_time)
/* repeat_time */
#define MFBD_REPEAT_TIME_IN_FUC         (_pbtn_info->repeat_time)
/* multiclick_time */
#define MFBD_MULTICLICK_TIME_IN_FUC     (_pbtn_info->multiclick_time)

#endif /*MFBD_PARAMS_SAME_IN_GROUP*/


#if MFBD_USE_TINY_BUTTON
/**
 * @brief scan all tiny buttons, and report button event value if event happened.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_tbtn_scan(const mfbd_group_t *_pbtn_group, const mfbd_tbtn_info_t *_pbtn_info_start, const mfbd_tbtn_info_t *_pbtn_info_end)
{
    const mfbd_tbtn_info_t *_pbtn_info = _pbtn_info_start;

    while (1)
    {
        if (_pbtn_info_end <= _pbtn_info)
        {
            break;
        }
        if (_pbtn_group->is_btn_down_func(_pbtn_info->btn_index) != MFBD_BTN_STATE_UP)
        {
            if (_pbtn_info->btn->filter_count < (MFBD_FILTER_TIME_IN_FUC))
            {
                _pbtn_info->btn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
            }
            else if (_pbtn_info->btn->filter_count < ((MFBD_FILTER_TIME_IN_FUC) * 2))
            {
                _pbtn_info->btn->filter_count++;
            }
            else
            {
                /* it means the button is down for over filter_time. */
                if (_pbtn_info->btn->state == MFBD_BTN_STATE_UP)
                {
                    _pbtn_info->btn->state = MFBD_BTN_STATE_DOWN;
                    if (_pbtn_info->btn_down_code > 0)
                    {
                        _pbtn_group->btn_value_report(_pbtn_info->btn_down_code);
                    }
                }
            }
        }
        else
        {
            if (_pbtn_info->btn->filter_count > (MFBD_FILTER_TIME_IN_FUC))
            {
                _pbtn_info->btn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
            }
            else if (_pbtn_info->btn->filter_count != 0)
            {
                _pbtn_info->btn->filter_count--;
            }
            else
            {
                if (_pbtn_info->btn->state != MFBD_BTN_STATE_UP)
                {
                    if (_pbtn_info->btn_up_code > 0)
                    {
                        _pbtn_group->btn_value_report(_pbtn_info->btn_up_code);
                    }
                    _pbtn_info->btn->state = MFBD_BTN_STATE_UP;
                }
            }
        }
        _pbtn_info++;
    }
}

/**
 * @brief reset all tiny buttons' params.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_tbtn_reset(const mfbd_tbtn_info_t *_pbtn_info_start, const mfbd_tbtn_info_t *_pbtn_info_end)
{
    const mfbd_tbtn_info_t *_pbtn_info = _pbtn_info_start;
    while (1)
    {
        if (_pbtn_info_end <= _pbtn_info)
        {
            break;
        }
        _pbtn_info->btn->filter_count = 0;
        _pbtn_info->btn->state = MFBD_BTN_STATE_UP;

        _pbtn_info++;
    }
}

#endif


#if MFBD_USE_NORMAL_BUTTON

/**
 * @brief scan all normal buttons, and report button event value if event happened.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None
 */
void mfbd_nbtn_scan(const mfbd_group_t *_pbtn_group, const mfbd_nbtn_info_t *_pbtn_info_start, const mfbd_nbtn_info_t *_pbtn_info_end)
{
    const mfbd_nbtn_info_t *_pbtn_info = _pbtn_info_start;
    while (1)
    {
        if (_pbtn_info_end <= _pbtn_info)
        {
            break;
        }
        if (_pbtn_group->is_btn_down_func(_pbtn_info->btn_index) != MFBD_BTN_STATE_UP)
            {
                if (_pbtn_info->btn->filter_count < (MFBD_FILTER_TIME_IN_FUC))
                {
                    _pbtn_info->btn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
                }
                else if (_pbtn_info->btn->filter_count < ((MFBD_FILTER_TIME_IN_FUC) * 2))
                {
                    _pbtn_info->btn->filter_count++;
                }
                else
                {
                    /* it means the button is down for over filter_time. */
                    if (_pbtn_info->btn->state == MFBD_BTN_STATE_UP)
                    {
                        _pbtn_info->btn->state = MFBD_BTN_STATE_DOWN;
                        /* clear long_count. */
                        _pbtn_info->btn->long_count = 0;
                        if (_pbtn_info->btn_down_code > 0)
                        {
                            _pbtn_group->btn_value_report(_pbtn_info->btn_down_code);
                        }
                    }
                    else
                    {
                        if (((MFBD_LONG_TIME_IN_FUC) > 0) && (_pbtn_info->btn_long_code != 0))
                        {
                            /* if long_time is 0 or long_code is 0, disable long and repeat check. */
                            if (_pbtn_info->btn->long_count < (MFBD_LONG_TIME_IN_FUC))
                            {
                                _pbtn_info->btn->long_count++;
                                if (_pbtn_info->btn->long_count >= (MFBD_LONG_TIME_IN_FUC))
                                {
                                    /* it means the button is down for over long_time. */
                                    _pbtn_info->btn->state = MFBD_BTN_STATE_LONG;
                                    _pbtn_info->btn->repeat_count = 0; /* long event has happened, clear repeat_count. */
                                    _pbtn_group->btn_value_report(_pbtn_info->btn_long_code);
                                }
                            }
                            else
                            {
                                if (((MFBD_REPEAT_TIME_IN_FUC) > 0) && (_pbtn_info->btn_down_code != 0))
                                {
                                    _pbtn_info->btn->repeat_count++;
                                    if (_pbtn_info->btn->repeat_count >= (MFBD_REPEAT_TIME_IN_FUC))
                                    {
                                        /* repeat event has happened, clear repeat_count. */
                                        _pbtn_info->btn->repeat_count = 0;
                                        _pbtn_group->btn_value_report(_pbtn_info->btn_down_code);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (_pbtn_info->btn->filter_count > (MFBD_FILTER_TIME_IN_FUC))
                {
                    _pbtn_info->btn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
                }
                else if (_pbtn_info->btn->filter_count != 0)
                {
                    _pbtn_info->btn->filter_count--;
                }
                else
                {
                    if (_pbtn_info->btn->state != MFBD_BTN_STATE_UP)
                    {
                        if (_pbtn_info->btn_up_code > 0)
                        {
                            _pbtn_group->btn_value_report(_pbtn_info->btn_up_code);
                        }
                        _pbtn_info->btn->state = MFBD_BTN_STATE_UP;
                    }
                }
            }
        _pbtn_info++;
    }
}

/**
 * @brief reset all normal buttons' params.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_nbtn_reset(const mfbd_nbtn_info_t *_pbtn_info_start, const mfbd_nbtn_info_t *_pbtn_info_end)
{
    const mfbd_nbtn_info_t *_pbtn_info = _pbtn_info_start;
    while (1)
    {
        if (_pbtn_info_end <= _pbtn_info)
        {
            break;
        }
        _pbtn_info->btn->filter_count = 0;
        _pbtn_info->btn->long_count = 0;
        _pbtn_info->btn->repeat_count = 0;
        _pbtn_info->btn->state = MFBD_BTN_STATE_UP;

        _pbtn_info++;
    }
}

#endif


#if MFBD_USE_MULTIFUCNTION_BUTTON

/**
 * @brief scan all multi-function buttons, and report button event value if event happened.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_mbtn_scan(const mfbd_group_t *_pbtn_group, const mfbd_mbtn_info_t *_pbtn_info_start, const mfbd_mbtn_info_t *_pbtn_info_end)
{
    const mfbd_mbtn_info_t *_pbtn_info = _pbtn_info_start;
    while (1)
    {
        if (_pbtn_info_end <= _pbtn_info)
        {
            break;
        }

        if (_pbtn_group->is_btn_down_func(_pbtn_info->btn_index) != MFBD_BTN_STATE_UP)
        {
            if (_pbtn_info->btn->filter_count < (MFBD_FILTER_TIME_IN_FUC))
            {
                _pbtn_info->btn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
            }
            else if (_pbtn_info->btn->filter_count < ((MFBD_FILTER_TIME_IN_FUC) * 2))
            {
                _pbtn_info->btn->filter_count++;
            }
            else
            {
                /* it means the button is down for over filter_time. */
                if (_pbtn_info->btn->state == MFBD_BTN_STATE_UP)
                {
                    _pbtn_info->btn->state = MFBD_BTN_STATE_DOWN;
                    /* clear long_count. */
                    _pbtn_info->btn->long_count = 0;
                    if (_pbtn_info->btn_down_code[_pbtn_info->btn->multiclick_state] > 0)
                    {
                        _pbtn_group->btn_value_report(_pbtn_info->btn_down_code[_pbtn_info->btn->multiclick_state]);
                    }
                }
                else
                {
                    if (_pbtn_info->btn->multiclick_state == 0)
                    {
                        if (((MFBD_LONG_TIME_IN_FUC) > 0) && (_pbtn_info->btn_long_code != 0))
                        {
                            /* if long_time is 0 or long_code is 0, disable long and repeat check. */
                            if (_pbtn_info->btn->long_count < (MFBD_LONG_TIME_IN_FUC))
                            {
                                _pbtn_info->btn->long_count++;
                                if (_pbtn_info->btn->long_count >= (MFBD_LONG_TIME_IN_FUC))
                                {
                                    /* it means the button is down for over long_time. */
                                    _pbtn_info->btn->state = MFBD_BTN_STATE_LONG;
                                    _pbtn_info->btn->repeat_count = 0; /* long event has happened, clear repeat_count. */
                                    _pbtn_group->btn_value_report(_pbtn_info->btn_long_code);
                                }
                            }
                            else
                            {
                                if (((MFBD_REPEAT_TIME_IN_FUC) > 0) && (_pbtn_info->btn_down_code[0] != 0))
                                {
                                    _pbtn_info->btn->repeat_count++;
                                    if (_pbtn_info->btn->repeat_count >= (MFBD_REPEAT_TIME_IN_FUC))
                                    {
                                        /* repeat event has happened, clear repeat_count. */
                                        _pbtn_info->btn->repeat_count = 0;
                                        _pbtn_group->btn_value_report(_pbtn_info->btn_down_code[0]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (_pbtn_info->btn->filter_count > (MFBD_FILTER_TIME_IN_FUC))
            {
                _pbtn_info->btn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
            }
            else if (_pbtn_info->btn->filter_count != 0)
            {
                _pbtn_info->btn->filter_count--;
            }
            else
            {
                if (_pbtn_info->btn->state != MFBD_BTN_STATE_UP)
                {
                    if (_pbtn_info->btn_up_code > 0)
                    {
                        _pbtn_group->btn_value_report(_pbtn_info->btn_up_code);
                    }
                    _pbtn_info->btn->state = MFBD_BTN_STATE_UP;
#if MFBD_MULTICLICK_STATE_AUTO_RESET
                    /* if multiclick_state is not 0 and less than max_multiclick_state, inc multiclick_state */
                    if (((MFBD_MULTICLICK_TIME_IN_FUC) != 0)  \
                            && (_pbtn_info->btn->multiclick_state < _pbtn_info->max_multiclick_state)  \
                            && (!((((MFBD_LONG_TIME_IN_FUC) > 0) && (_pbtn_info->btn_long_code != 0)) && (_pbtn_info->btn->long_count >= (MFBD_LONG_TIME_IN_FUC)))))
                    {
                        _pbtn_info->btn->multiclick_state++;
                        _pbtn_info->btn->multiclick_count = 0;
                    }
#else
                    /* if multiclick_state is not 0 and less than max_multiclick_state, inc multiclick_state */
                    if (((MFBD_MULTICLICK_TIME_IN_FUC) != 0)  \
                            && (!((((MFBD_LONG_TIME_IN_FUC) > 0) && (_pbtn_info->btn_long_code != 0)) && (_pbtn_info->btn->long_count >= (MFBD_LONG_TIME_IN_FUC)))))
                    {
                        if(_pbtn_info->btn->multiclick_state < _pbtn_info->max_multiclick_state)
                        {
                            _pbtn_info->btn->multiclick_state++;
                        }
                        _pbtn_info->btn->multiclick_count = 0;
                    }
#endif
                    else
                    {
                        /* over max multi-click times or (long event and repeat event) happened, reset to 0. */
                        _pbtn_info->btn->multiclick_state = 0;
                    }
                }
                else
                {
                    if (_pbtn_info->btn->multiclick_state != 0)
                    {
                        _pbtn_info->btn->multiclick_count++;
                        if (_pbtn_info->btn->multiclick_count >= (MFBD_MULTICLICK_TIME_IN_FUC))
                        {
                            _pbtn_info->btn->multiclick_state = 0;
                        }
                    }
                }
            }
        }
        _pbtn_info++;
    }
}

/**
 * @brief reset all multi-function buttons' params.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_mbtn_reset(const mfbd_mbtn_info_t *_pbtn_info_start, const mfbd_mbtn_info_t *_pbtn_info_end)
{
    const mfbd_mbtn_info_t *_pbtn_info = _pbtn_info_start;
    while (1)
    {
        if (_pbtn_info_end <= _pbtn_info)
        {
            break;
        }
        _pbtn_info->btn->filter_count = 0;
        _pbtn_info->btn->long_count = 0;
        _pbtn_info->btn->multiclick_count = 0;
        _pbtn_info->btn->multiclick_state = 0;
        _pbtn_info->btn->repeat_count = 0;
        _pbtn_info->btn->state = MFBD_BTN_STATE_UP;

        _pbtn_info++;
    }
}

#endif

#endif /*MFBD_USE_SECTION_DEFINITION*/
