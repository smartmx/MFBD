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
 *
 */

#include "mfbd.h"

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
    #define MFBD_FILTER_TIME_IN_FUC         (_pbtn->btn_info->filter_time)
    /* long_time */
    #define MFBD_LONG_TIME_IN_FUC           (_pbtn->btn_info->long_time)
    /* repeat_time */
    #define MFBD_REPEAT_TIME_IN_FUC         (_pbtn->btn_info->repeat_time)
    /* multiclick_time */
    #define MFBD_MULTICLICK_TIME_IN_FUC     (_pbtn->btn_info->multiclick_time)

#endif /*MFBD_PARAMS_SAME_IN_GROUP*/


#if MFBD_USE_TINY_BUTTON
/**
 * @brief scan all tiny buttons, and report button event value if event happened.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_tbtn_scan(const mfbd_group_t *_pbtn_group)
{
    mfbd_tbtn_t **_ppbtn = _pbtn_group->tbtns;
    mfbd_tbtn_t *_pbtn = *_ppbtn;

    while (_pbtn != NULL)
    {
        if (_pbtn_group->is_btn_down_func(_pbtn->btn_info->btn_index) != 0)
        {
            if (_pbtn->filter_count < (MFBD_FILTER_TIME_IN_FUC))
            {
                _pbtn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
            }
            else if (_pbtn->filter_count < ((MFBD_FILTER_TIME_IN_FUC) * 2))
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
            if (_pbtn->filter_count > (MFBD_FILTER_TIME_IN_FUC))
            {
                _pbtn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
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
        _ppbtn++;
        _pbtn = *_ppbtn;
    }
}

/**
 * @brief reset all tiny buttons' params.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_tbtn_reset(const mfbd_group_t *_pbtn_group)
{
    mfbd_tbtn_t **_ppbtn = _pbtn_group->tbtns;
    mfbd_tbtn_t *_pbtn = *_ppbtn;
    while (_pbtn != NULL)
    {
        _pbtn->filter_count = 0;
        _pbtn->state = 0;
        _ppbtn++;
        _pbtn = *_ppbtn;
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
void mfbd_nbtn_scan(const mfbd_group_t *_pbtn_group)
{
    mfbd_nbtn_t **_ppbtn = _pbtn_group->nbtns;
    mfbd_nbtn_t *_pbtn = *_ppbtn;

    while (_pbtn != NULL)
    {
        if (_pbtn_group->is_btn_down_func(_pbtn->btn_info->btn_index) != 0)
        {
            if (_pbtn->filter_count < (MFBD_FILTER_TIME_IN_FUC))
            {
                _pbtn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
            }
            else if (_pbtn->filter_count < ((MFBD_FILTER_TIME_IN_FUC) * 2))
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
                    if (((MFBD_LONG_TIME_IN_FUC) > 0) && (_pbtn->btn_info->btn_long_code != 0))
                    {
                        /* if long_time is 0 or long_code is 0, disable long and repeat check. */
                        if (_pbtn->long_count < (MFBD_LONG_TIME_IN_FUC))
                        {
                            _pbtn->long_count++;
                            if (_pbtn->long_count >= (MFBD_LONG_TIME_IN_FUC))
                            {
                                /* it means the button is down for over long_time. */
                                _pbtn->repeat_count = 0; /* long event has happened, clear repeat_count. */
                                _pbtn_group->btn_value_report(_pbtn->btn_info->btn_long_code);
                            }
                        }
                        else
                        {
                            if (((MFBD_REPEAT_TIME_IN_FUC) > 0) && (_pbtn->btn_info->btn_down_code != 0))
                            {
                                _pbtn->repeat_count++;
                                if (_pbtn->repeat_count >= (MFBD_REPEAT_TIME_IN_FUC))
                                {
                                    /* repeat event has happened, clear repeat_count. */
                                    _pbtn->repeat_count = 0;
                                    _pbtn_group->btn_value_report(_pbtn->btn_info->btn_down_code);
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (_pbtn->filter_count > (MFBD_FILTER_TIME_IN_FUC))
            {
                _pbtn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
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
        _ppbtn++;
        _pbtn = *_ppbtn;
    }
}

/**
 * @brief reset all normal buttons' params.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_nbtn_reset(const mfbd_group_t *_pbtn_group)
{
    mfbd_nbtn_t **_ppbtn = _pbtn_group->nbtns;
    mfbd_nbtn_t *_pbtn = *_ppbtn;
    while (_pbtn != NULL)
    {
        _pbtn->filter_count = 0;
        _pbtn->long_count = 0;
        _pbtn->repeat_count = 0;
        _pbtn->state = 0;
        _ppbtn++;
        _pbtn = *_ppbtn;
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
void mfbd_mbtn_scan(const mfbd_group_t *_pbtn_group)
{
    mfbd_mbtn_t **_ppbtn = _pbtn_group->mbtns;
    mfbd_mbtn_t *_pbtn = *_ppbtn;
    while (_pbtn != NULL)
    {
        if (_pbtn_group->is_btn_down_func(_pbtn->btn_info->btn_index) != 0)
        {
            if (_pbtn->filter_count < (MFBD_FILTER_TIME_IN_FUC))
            {
                _pbtn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
            }
            else if (_pbtn->filter_count < ((MFBD_FILTER_TIME_IN_FUC) * 2))
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
                    if (_pbtn->multiclick_state == 0)
                    {
                        if (((MFBD_LONG_TIME_IN_FUC) > 0) && (_pbtn->btn_info->btn_long_code != 0))
                        {
                            /* if long_time is 0 or long_code is 0, disable long and repeat check. */
                            if (_pbtn->long_count < (MFBD_LONG_TIME_IN_FUC))
                            {
                                _pbtn->long_count++;
                                if (_pbtn->long_count >= (MFBD_LONG_TIME_IN_FUC))
                                {
                                    /* it means the button is down for over long_time. */
                                    _pbtn->repeat_count = 0; /* long event has happened, clear repeat_count. */
                                    _pbtn_group->btn_value_report(_pbtn->btn_info->btn_long_code);
                                }
                            }
                            else
                            {
                                if (((MFBD_REPEAT_TIME_IN_FUC) > 0) && (_pbtn->btn_info->btn_down_code[0] != 0))
                                {
                                    _pbtn->repeat_count++;
                                    if (_pbtn->repeat_count >= (MFBD_REPEAT_TIME_IN_FUC))
                                    {
                                        /* repeat event has happened, clear repeat_count. */
                                        _pbtn->repeat_count = 0;
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
            if (_pbtn->filter_count > (MFBD_FILTER_TIME_IN_FUC))
            {
                _pbtn->filter_count = (MFBD_FILTER_TIME_IN_FUC);
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
                    if (((MFBD_MULTICLICK_TIME_IN_FUC) != 0)  \
                            && (_pbtn->multiclick_state < _pbtn->btn_info->max_multiclick_state)  \
                            && (!((((MFBD_LONG_TIME_IN_FUC) > 0) && (_pbtn->btn_info->btn_long_code != 0)) && (_pbtn->long_count >= (MFBD_LONG_TIME_IN_FUC)))))
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
                        if (_pbtn->multiclick_count >= (MFBD_MULTICLICK_TIME_IN_FUC))
                        {
                            _pbtn->multiclick_state = 0;
                        }
                    }
                }
            }
        }
        _ppbtn++;
        _pbtn = *_ppbtn;
    }
}

/**
 * @brief reset all multi-function buttons' params.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_mbtn_reset(const mfbd_group_t *_pbtn_group)
{
    mfbd_mbtn_t **_ppbtn = _pbtn_group->mbtns;
    mfbd_mbtn_t *_pbtn = *_ppbtn;
    while (_pbtn != NULL)
    {
        _pbtn->filter_count = 0;
        _pbtn->long_count = 0;
        _pbtn->multiclick_count = 0;
        _pbtn->multiclick_state = 0;
        _pbtn->repeat_count = 0;
        _pbtn->state = 0;
        _ppbtn++;
        _pbtn = *_ppbtn;
    }
}

#endif


/**
 * @brief scan all buttons in the group, and report button event value if event happened.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_group_scan(const mfbd_group_t *_pbtn_group)
{

#if MFBD_USE_BTN_SCAN_PRE_FUNC
    if (_pbtn_group->btn_scan_prepare != NULL)
    {
        /* call prepare scan function */
        _pbtn_group->btn_scan_prepare();
    }
#endif

#if MFBD_USE_TINY_BUTTON
    if (_pbtn_group->tbtns != NULL)
    {
        /*scan tiny buttons in group.*/
        mfbd_tbtn_scan(_pbtn_group);
    }
#endif

#if MFBD_USE_NORMAL_BUTTON
    if (_pbtn_group->nbtns != NULL)
    {
        /*scan normal buttons in group.*/
        mfbd_nbtn_scan(_pbtn_group);
    }
#endif

#if MFBD_USE_MULTIFUCNTION_BUTTON
    if (_pbtn_group->mbtns != NULL)
    {
        /*scan multifunction buttons in group.*/
        mfbd_mbtn_scan(_pbtn_group);
    }
#endif

#if MFBD_USE_BTN_SCAN_AFTER_FUNC
    if (_pbtn_group->btn_scan_after != NULL)
    {
        /* call after scan function */
        _pbtn_group->btn_scan_after();
    }
#endif

}


/**
 * @brief reset all buttons params in the group.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 *
 * @return None.
 */
void mfbd_group_reset(const mfbd_group_t *_pbtn_group)
{

#if MFBD_USE_TINY_BUTTON
    if (_pbtn_group->tbtns != NULL)
    {
        /*reset tiny buttons in group.*/
        mfbd_tbtn_reset(_pbtn_group);
    }
#endif

#if MFBD_USE_NORMAL_BUTTON
    if (_pbtn_group->nbtns != NULL)
    {
        /*reset normal buttons in group.*/
        mfbd_nbtn_reset(_pbtn_group);
    }
#endif

#if MFBD_USE_MULTIFUCNTION_BUTTON
    if (_pbtn_group->mbtns != NULL)
    {
        /*reset multifunction buttons in group.*/
        mfbd_mbtn_reset(_pbtn_group);
    }
#endif

}
