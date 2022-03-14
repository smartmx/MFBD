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

#include "mfbd.h"

/**
 * @brief scan all tiny buttons, and report button event value if event happened.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t. Buttons in the list should be defined by mfbd_tbtn_t.
 * @return None.
 */
void mfbd_tbtn_scan(const mfbd_group_t *_pbtn_group)
{
    mfbd_tbtn_t *_pbtn = _pbtn_group->btn_list_head;

#if MFBD_USE_BTN_SCAN_PRE_FUNC
    if (_pbtn_group->btn_scan_prepare != NULL)
    {
        /* call prepare function */
        _pbtn_group->btn_scan_prepare();
    }
#endif
    while (_pbtn != NULL)
    {
        if (_pbtn_group->is_btn_down_func(_pbtn->btn_info->btn_index) != 0)
        {
            if (_pbtn->filter_count < _pbtn->btn_info->filter_time)
            {
                _pbtn->filter_count = _pbtn->btn_info->filter_time;
            }
            else if (_pbtn->filter_count < (_pbtn->btn_info->filter_time * 2))
            {
                _pbtn->filter_count++;
            }
            else
            {
                /* it means the button is down for over filter_time. */
                if (_pbtn->state == 0)
                {
                    _pbtn->state = 1;
                    if (_pbtn->btn_info->btn_down_code > 0)
                    {
                        _pbtn_group->btn_value_report(_pbtn->btn_info->btn_down_code);
                    }
                }
            }
        }
        else
        {
            if (_pbtn->filter_count > _pbtn->btn_info->filter_time)
            {
                _pbtn->filter_count = _pbtn->btn_info->filter_time;
            }
            else if (_pbtn->filter_count != 0)
            {
                _pbtn->filter_count--;
            }
            else
            {
                if (_pbtn->state == 1)
                {
                    _pbtn->state = 0;
                    if (_pbtn->btn_info->btn_up_code > 0)
                    {
                        _pbtn_group->btn_value_report(_pbtn->btn_info->btn_up_code);
                    }
                }
            }
        }
        _pbtn = _pbtn->next;
    }

#if MFBD_USE_BTN_SCAN_AFTER_FUNC
    if (_pbtn_group->btn_scan_after != NULL)
    {
        /* call prepare function */
        _pbtn_group->btn_scan_after();
    }
#endif
}

/**
 * @brief scan all normal buttons, and report button event value if event happened.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t. Buttons in the list should be defined by mfbd_nbtn_t.
 * @return None
 */
void mfbd_nbtn_scan(const mfbd_group_t *_pbtn_group)
{
    mfbd_nbtn_t *_pbtn = _pbtn_group->btn_list_head;

#if MFBD_USE_BTN_SCAN_PRE_FUNC
    if (_pbtn_group->btn_scan_prepare != NULL)
    {
        /* call prepare function */
        _pbtn_group->btn_scan_prepare();
    }
#endif

    while (_pbtn != NULL)
    {
        if (_pbtn_group->is_btn_down_func(_pbtn->btn_info->btn_index) != 0)
        {
            if (_pbtn->filter_count < _pbtn->btn_info->filter_time)
            {
                _pbtn->filter_count = _pbtn->btn_info->filter_time;
            }
            else if (_pbtn->filter_count < (_pbtn->btn_info->filter_time * 2))
            {
                _pbtn->filter_count++;
            }
            else
            {
                /* it means the button is down for over filter_time. */
                if (_pbtn->state == 0)
                {
                    _pbtn->state = 1;
                    /* clear long_count. */
                    _pbtn->long_count = 0;
                    if (_pbtn->btn_info->btn_down_code > 0)
                    {
                        _pbtn_group->btn_value_report(_pbtn->btn_info->btn_down_code);
                    }
                }
                else
                {
                    if (_pbtn->btn_info->long_time != 0)
                    {
                        if (_pbtn->long_count < _pbtn->btn_info->long_time)
                        {
                            _pbtn->long_count++;
                            if (_pbtn->long_count >= _pbtn->btn_info->long_time)
                            {
                                /* long event has happened, clear repeat_count. */
                                _pbtn->repeat_count = 0;
                                if (_pbtn->btn_info->btn_long_code > 0)
                                {
                                    _pbtn_group->btn_value_report(_pbtn->btn_info->btn_long_code);
                                }
                                else
                                {
                                    goto report_btn_down_code;
                                }
                            }
                        }
                        else
                        {

                            if (_pbtn->btn_info->repeat_time > 0)
                            {
                                _pbtn->repeat_count++;
                                if (_pbtn->repeat_count >= _pbtn->btn_info->repeat_time)
                                {
                                    /* repeat event has happened, clear repeat_count. */
                                    _pbtn->repeat_count = 0;
report_btn_down_code:
                                    if (_pbtn->btn_info->btn_down_code > 0)
                                    {
                                        _pbtn_group->btn_value_report(_pbtn->btn_info->btn_down_code);
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
            if (_pbtn->filter_count > _pbtn->btn_info->filter_time)
            {
                _pbtn->filter_count = _pbtn->btn_info->filter_time;
            }
            else if (_pbtn->filter_count != 0)
            {
                _pbtn->filter_count--;
            }
            else
            {
                if (_pbtn->state == 1)
                {
                    _pbtn->state = 0;
                    if (_pbtn->btn_info->btn_up_code > 0)
                    {
                        _pbtn_group->btn_value_report(_pbtn->btn_info->btn_up_code);
                    }
                }
            }
        }
        _pbtn = _pbtn->next;
    }

#if MFBD_USE_BTN_SCAN_AFTER_FUNC
    if (_pbtn_group->btn_scan_after != NULL)
    {
        /* call prepare function */
        _pbtn_group->btn_scan_after();
    }
#endif
}


/**
 * @brief scan all multi-function buttons, and report button event value if event happened.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t. Buttons in the list should be defined by mfbd_mbtn_t.
 * @return None.
 */
void mfbd_mbtn_scan(const mfbd_group_t *_pbtn_group)
{
    mfbd_mbtn_t *_pbtn = _pbtn_group->btn_list_head;

#if MFBD_USE_BTN_SCAN_PRE_FUNC
    if (_pbtn_group->btn_scan_prepare != NULL)
    {
        /* call prepare function */
        _pbtn_group->btn_scan_prepare();
    }
#endif

    while (_pbtn != NULL)
    {
        if (_pbtn_group->is_btn_down_func(_pbtn->btn_info->btn_index) != 0)
        {
            if (_pbtn->filter_count < _pbtn->btn_info->filter_time)
            {
                _pbtn->filter_count = _pbtn->btn_info->filter_time;
            }
            else if (_pbtn->filter_count < (_pbtn->btn_info->filter_time * 2))
            {
                _pbtn->filter_count++;
            }
            else
            {
                /* it means the button is down for over filter_time. */
                if (_pbtn->state == 0)
                {
                    _pbtn->state = 1;
                    /* clear long_count. */
                    _pbtn->long_count = 0;
                    if (_pbtn->btn_info->btn_down_code[_pbtn->multiclick_state] > 0)
                    {
                        _pbtn_group->btn_value_report(_pbtn->btn_info->btn_down_code[_pbtn->multiclick_state]);
                    }
                }
                else
                {
                    if ((_pbtn->multiclick_state == 0) && (_pbtn->btn_info->long_time > 0))
                    {
                        /* check long and repeat event only in first click process. */
                        if (_pbtn->long_count < _pbtn->btn_info->long_time)
                        {
                            _pbtn->long_count++;
                            if (_pbtn->long_count >= _pbtn->btn_info->long_time)
                            {
                                /* long event has happened, clear repeat_count. */
                                _pbtn->repeat_count = 0;
                                if (_pbtn->btn_info->btn_long_code > 0)
                                {
                                    _pbtn_group->btn_value_report(_pbtn->btn_info->btn_long_code);
                                }
                                else
                                {
                                    goto report_btn_down_code;
                                }
                            }
                        }
                        else
                        {
                            if (_pbtn->btn_info->repeat_time > 0)
                            {
                                _pbtn->repeat_count++;
                                if (_pbtn->repeat_count >= _pbtn->btn_info->repeat_time)
                                {
                                    /* repeat event has happened, clear repeat_count. */
                                    _pbtn->repeat_count = 0;
report_btn_down_code:
                                    if (_pbtn->btn_info->btn_down_code[0] > 0)
                                    {
                                        _pbtn_group->btn_value_report(_pbtn->btn_info->btn_down_code[0]);
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
            if (_pbtn->filter_count > _pbtn->btn_info->filter_time)
            {
                _pbtn->filter_count = _pbtn->btn_info->filter_time;
            }
            else if (_pbtn->filter_count != 0)
            {
                _pbtn->filter_count--;
            }
            else
            {
                if (_pbtn->state == 1)
                {
                    _pbtn->state = 0;
                    if (_pbtn->btn_info->btn_up_code > 0)
                    {
                        _pbtn_group->btn_value_report(_pbtn->btn_info->btn_up_code);
                    }

                    /* if multiclick_state is not 0 and less than max_multiclick_state, inc multiclick_state */
                    if (((_pbtn->multiclick_state < _pbtn->btn_info->max_multiclick_state) && (_pbtn->btn_info->multiclick_time != 0)) \
                            && (!((_pbtn->btn_info->long_time != 0) && (_pbtn->long_count >= _pbtn->btn_info->long_time))))
                    {
                        _pbtn->multiclick_state++;
                        _pbtn->multiclick_count = 0;
                    }
                    else
                    {
                        /* over max multi-click times or (long event and repeat event) happened, reset to 0. */
                        _pbtn->multiclick_state = 0;
                    }
                }
                else
                {
                    if (_pbtn->multiclick_state != 0)
                    {
                        _pbtn->multiclick_count++;
                        if (_pbtn->multiclick_count >= _pbtn->btn_info->multiclick_time)
                        {
                            _pbtn->multiclick_state = 0;
                        }
                    }
                }
            }
        }
        _pbtn = _pbtn->next;
    }

#if MFBD_USE_BTN_SCAN_AFTER_FUNC
    if (_pbtn_group->btn_scan_after != NULL)
    {
        /* call prepare function */
        _pbtn_group->btn_scan_after();
    }
#endif
}
