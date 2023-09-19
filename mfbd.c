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
 * 2023-03-15     smartmx      add state declaration.
 * 2023-07-03     smartmx      add Section Definition option.
 * 2023-07-15     smartmx      add skip function, to reduce calling of scan functions.
 * 2023-07-22     smartmx      add MFBD_MBTN_MULTICLICK_LONG_EVT and MFBD_MBTN_CONTINUE_LONG_COUNT option.
 * 2023-09-19     smartmx      improve performance, add MFBD_BTN_STATE_SKIP.
 *
 */

#include "mfbd.h"

#if (MFBD_USE_SECTION_DEFINITION == 0)

#define MFBD_BTN_IN_FUC                     (_pbtn)
#define MFBD_BTN_INFO_IN_FUC                (_pbtn->btn_info)

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
    #define MFBD_FILTER_TIME_IN_FUC         (MFBD_BTN_INFO_IN_FUC->filter_time)
    /* long_time */
    #define MFBD_LONG_TIME_IN_FUC           (MFBD_BTN_INFO_IN_FUC->long_time)
    /* repeat_time */
    #define MFBD_REPEAT_TIME_IN_FUC         (MFBD_BTN_INFO_IN_FUC->repeat_time)
    /* multiclick_time */
    #define MFBD_MULTICLICK_TIME_IN_FUC     (MFBD_BTN_INFO_IN_FUC->multiclick_time)

#endif  /* MFBD_PARAMS_SAME_IN_GROUP */


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
    MFBD_BTN_STATE_t btn_state;

    while (MFBD_BTN_IN_FUC != NULL)
    {
        btn_state = _pbtn_group->is_btn_down_func(MFBD_BTN_INFO_IN_FUC->btn_index);
        if (btn_state == MFBD_BTN_STATE_DOWN)
        {
            if(MFBD_BTN_IN_FUC->filter_count >= ((MFBD_FILTER_TIME_IN_FUC) * 2))
            {
                /* it means the button is down for over filter_time. */
                if (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_UP)
                {
                    if (MFBD_BTN_INFO_IN_FUC->btn_down_code > 0)
                    {
                        _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_down_code);
                    }
                    MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_DOWN;
                }
            }
            else if(MFBD_BTN_IN_FUC->filter_count >= (MFBD_FILTER_TIME_IN_FUC))
            {
                MFBD_BTN_IN_FUC->filter_count++;
            }
            else
            {
                MFBD_BTN_IN_FUC->filter_count = (MFBD_FILTER_TIME_IN_FUC);
            }
        }
        else if (btn_state == MFBD_BTN_STATE_UP)
        {
            if (MFBD_BTN_IN_FUC->filter_count == 0)
            {
                if (MFBD_BTN_IN_FUC->state != MFBD_BTN_STATE_UP)
                {
                    if (MFBD_BTN_INFO_IN_FUC->btn_up_code > 0)
                    {
                        _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_up_code);
                    }
                    MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_UP;
                }
            }
            else
            {
                if (MFBD_BTN_IN_FUC->filter_count > (MFBD_FILTER_TIME_IN_FUC))
                {
                    MFBD_BTN_IN_FUC->filter_count = (MFBD_FILTER_TIME_IN_FUC);
                }
                else if (MFBD_BTN_IN_FUC->filter_count != 0)
                {
                    MFBD_BTN_IN_FUC->filter_count--;
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
    while (MFBD_BTN_IN_FUC != NULL)
    {
        MFBD_BTN_IN_FUC->filter_count = 0;
        MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_UP;
        _ppbtn++;
        _pbtn = *_ppbtn;
    }
}

#endif  /* MFBD_USE_TINY_BUTTON */


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
    MFBD_BTN_STATE_t btn_state;

    while (MFBD_BTN_IN_FUC != NULL)
    {
        btn_state = _pbtn_group->is_btn_down_func(MFBD_BTN_INFO_IN_FUC->btn_index);
        if (btn_state == MFBD_BTN_STATE_DOWN)
        {
          if(MFBD_BTN_IN_FUC->filter_count >= ((MFBD_FILTER_TIME_IN_FUC) * 2))
          {
              /* it means the button is down for over filter_time. */
              if (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_LONG)
              {
                  /* MFBD_BTN_STATE_LONG */
                  if (MFBD_BTN_IN_FUC->repeat_count > 0)
                  {
                      MFBD_BTN_IN_FUC->repeat_count++;
                      if (MFBD_BTN_IN_FUC->repeat_count > (MFBD_REPEAT_TIME_IN_FUC))
                      {
                          /* repeat event has happened, reset repeat_count to 1. */
                          MFBD_BTN_IN_FUC->repeat_count = 1;
                          _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_down_code);
                      }
                  }
              }
              else if (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_DOWN)
              {
                  if (MFBD_BTN_IN_FUC->long_count > 0)
                  {
                      /* if long_time is 0 or long_code is 0, disable long and repeat check. */
                      if (MFBD_BTN_IN_FUC->long_count <= (MFBD_LONG_TIME_IN_FUC))
                      {
                          MFBD_BTN_IN_FUC->long_count++;
                          if (MFBD_BTN_IN_FUC->long_count > (MFBD_LONG_TIME_IN_FUC))
                          {
                              /* it means the button is down for over long_time. */
                              if (((MFBD_REPEAT_TIME_IN_FUC) > 0) && (MFBD_BTN_INFO_IN_FUC->btn_down_code != 0))
                              {
                                  /* repeat event is enabled in this btn. */
                                  MFBD_BTN_IN_FUC->repeat_count = 1;
                              }
                              else
                              {
                                  /* repeat event is disabled in this btn. */
                                  MFBD_BTN_IN_FUC->repeat_count = 0;
                              }
                              _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_long_code);
                              MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_LONG;
                          }
                      }
                  }
              }
              else
              {
                  /* MFBD_BTN_STATE_UP */
                  /* clear long_count. */
                  if (((MFBD_LONG_TIME_IN_FUC) > 0) && (MFBD_BTN_INFO_IN_FUC->btn_long_code != 0))
                  {
                      /* long event is enabled in this btn. */
                      MFBD_BTN_IN_FUC->long_count = 1;
                  }
                  else
                  {
                      /* long event is disabled in this btn. */
                      MFBD_BTN_IN_FUC->long_count = 0;
                  }
                  if (MFBD_BTN_INFO_IN_FUC->btn_down_code > 0)
                  {
                      _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_down_code);
                  }
                  MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_DOWN;
              }
            }
            else if(MFBD_BTN_IN_FUC->filter_count >= (MFBD_FILTER_TIME_IN_FUC))
            {
                MFBD_BTN_IN_FUC->filter_count++;
            }
            else
            {
                MFBD_BTN_IN_FUC->filter_count = (MFBD_FILTER_TIME_IN_FUC);
            }
        }
        else if (btn_state == MFBD_BTN_STATE_UP)
        {
            if (MFBD_BTN_IN_FUC->filter_count == 0)
            {
                if (MFBD_BTN_IN_FUC->state != MFBD_BTN_STATE_UP)
                {
                    if (MFBD_BTN_INFO_IN_FUC->btn_up_code > 0)
                    {
                        _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_up_code);
                    }
                    MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_UP;
                }
            }
            else
            {
                if (MFBD_BTN_IN_FUC->filter_count > (MFBD_FILTER_TIME_IN_FUC))
                {
                    MFBD_BTN_IN_FUC->filter_count = (MFBD_FILTER_TIME_IN_FUC);
                }
                else if (MFBD_BTN_IN_FUC->filter_count != 0)
                {
                    MFBD_BTN_IN_FUC->filter_count--;
                }
            }
        }
        _ppbtn++;
        _pbtn = *_ppbtn;
    }
}

/**
 * @brief skip some times of mfbd_btn_count_t with last state.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 * @param times is times need to skip.
 *
 * @return None.
 */
void mfbd_nbtn_skip(const mfbd_group_t *_pbtn_group, mfbd_btn_count_t times)
{
    mfbd_nbtn_t **_ppbtn = _pbtn_group->nbtns;
    mfbd_nbtn_t *_pbtn = *_ppbtn;

    while (MFBD_BTN_IN_FUC != NULL)
    {
        if(MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_DOWN)
        {
            if ((MFBD_BTN_IN_FUC->long_count > 0) && (MFBD_BTN_IN_FUC->long_count < (MFBD_LONG_TIME_IN_FUC)))
            {
                if(((MFBD_LONG_TIME_IN_FUC) - MFBD_BTN_IN_FUC->long_count) > times)
                {
                    MFBD_BTN_IN_FUC->long_count = MFBD_BTN_IN_FUC->long_count + times;
                }
                else
                {
                    MFBD_BTN_IN_FUC->long_count = MFBD_LONG_TIME_IN_FUC;
                }
            }
        }
        else if(MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_LONG)
        {
            if ((MFBD_BTN_IN_FUC->repeat_count > 0) && (MFBD_BTN_IN_FUC->repeat_count < (MFBD_REPEAT_TIME_IN_FUC)))
            {
                if(((MFBD_REPEAT_TIME_IN_FUC) - MFBD_BTN_IN_FUC->repeat_count) > times)
                {
                    MFBD_BTN_IN_FUC->repeat_count = MFBD_BTN_IN_FUC->repeat_count + times;
                }
                else
                {
                    MFBD_BTN_IN_FUC->repeat_count = MFBD_REPEAT_TIME_IN_FUC;
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
    while (MFBD_BTN_IN_FUC != NULL)
    {
        MFBD_BTN_IN_FUC->filter_count = 0;
        MFBD_BTN_IN_FUC->long_count = 0;
        MFBD_BTN_IN_FUC->repeat_count = 0;
        MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_UP;
        _ppbtn++;
        _pbtn = *_ppbtn;
    }
}

#endif  /* MFBD_USE_NORMAL_BUTTON */


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
    MFBD_BTN_STATE_t btn_state;

    while (MFBD_BTN_IN_FUC != NULL)
    {
        btn_state = _pbtn_group->is_btn_down_func(MFBD_BTN_INFO_IN_FUC->btn_index);
        if (btn_state == MFBD_BTN_STATE_DOWN)
        {
            if(MFBD_BTN_IN_FUC->filter_count >= ((MFBD_FILTER_TIME_IN_FUC) * 2))
            {
                /* it means the button is down for over filter_time. */
#if MFBD_MBTN_CONTINUE_LONG_COUNT
#if MFBD_MBTN_MULTICLICK_LONG_EVT
                if (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_LONG)
                {
                    /* MFBD_BTN_STATE_LONG */
                    if (MFBD_BTN_IN_FUC->repeat_count > 0)
                    {
                        MFBD_BTN_IN_FUC->repeat_count++;
                        if (MFBD_BTN_IN_FUC->repeat_count > (MFBD_REPEAT_TIME_IN_FUC))
                        {
                            /* repeat event has happened, clear repeat_count. */
                            MFBD_BTN_IN_FUC->repeat_count = 1;
                            _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_down_code[MFBD_BTN_IN_FUC->multiclick_state]);
                        }
                    }
                }
                else if (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_DOWN)
                {
                    if (MFBD_BTN_IN_FUC->long_count > 0)
                    {
                        /* if long_time is 0 or long_code is 0, disable long and repeat check. */
                        if (MFBD_BTN_IN_FUC->long_count <= (MFBD_LONG_TIME_IN_FUC))
                        {
                            MFBD_BTN_IN_FUC->long_count++;
                            if (MFBD_BTN_IN_FUC->long_count > (MFBD_LONG_TIME_IN_FUC))
                            {
                                /* it means the button is down for over long_time. */
                                if (((MFBD_REPEAT_TIME_IN_FUC) > 0) && (MFBD_BTN_INFO_IN_FUC->btn_down_code[MFBD_BTN_IN_FUC->multiclick_state] != 0))
                                {
                                  /* repeat event is enabled in this btn. */
                                    MFBD_BTN_IN_FUC->repeat_count = 1;
                                }
                                else
                                {
                                    /* repeat event is disabled in this btn. */
                                    MFBD_BTN_IN_FUC->repeat_count = 0;
                                }
                                _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_long_code);
                                MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_LONG;
                            }
                        }
                    }
                }
#else
                if (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_LONG)
                {
                    /* we don't support repeat event here.*/
                }
                else if (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_DOWN)
                {
                    if (MFBD_BTN_IN_FUC->long_count > 0)
                    {
                        /* if long_time is 0 or long_code is 0, disable long and repeat check. */
                        if (MFBD_BTN_IN_FUC->long_count <= (MFBD_LONG_TIME_IN_FUC))
                        {
                            MFBD_BTN_IN_FUC->long_count++;
                            if (MFBD_BTN_IN_FUC->long_count > (MFBD_LONG_TIME_IN_FUC))
                            {
                                /* it means the button is down for over long_time. */
                                if (MFBD_BTN_IN_FUC->multiclick_state == 0)
                                {
                                    _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_long_code);
                                }
                                MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_LONG;
                            }
                        }
                    }
                }
#endif  /* MFBD_MBTN_MULTICLICK_LONG_EVT */
#else
                if (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_LONG)
                {
                    /* MFBD_BTN_STATE_LONG */
                    if (MFBD_BTN_IN_FUC->repeat_count > 0)
                    {
                        MFBD_BTN_IN_FUC->repeat_count++;
                        if (MFBD_BTN_IN_FUC->repeat_count > (MFBD_REPEAT_TIME_IN_FUC))
                        {
                            /* repeat event has happened, clear repeat_count. */
                            MFBD_BTN_IN_FUC->repeat_count = 1;
                            _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_down_code[0]);
                        }
                    }
                }
                else if (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_DOWN)
                {
                    if (MFBD_BTN_IN_FUC->multiclick_state == 0)
                    {
                        if (MFBD_BTN_IN_FUC->long_count > 0)
                        {
                            /* if long_time is 0 or long_code is 0, disable long and repeat check. */
                            if (MFBD_BTN_IN_FUC->long_count <= (MFBD_LONG_TIME_IN_FUC))
                            {
                                MFBD_BTN_IN_FUC->long_count++;
                                if (MFBD_BTN_IN_FUC->long_count > (MFBD_LONG_TIME_IN_FUC))
                                {
                                    /* it means the button is down for over long_time. */
                                    if (((MFBD_REPEAT_TIME_IN_FUC) > 0) && (MFBD_BTN_INFO_IN_FUC->btn_down_code[0] != 0))
                                    {
                                      /* repeat event is enabled in this btn. */
                                        MFBD_BTN_IN_FUC->repeat_count = 1;
                                    }
                                    else
                                    {
                                        /* repeat event is disabled in this btn. */
                                        MFBD_BTN_IN_FUC->repeat_count = 0;
                                    }
                                    _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_long_code);
                                    MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_LONG;
                                }
                            }
                        }
                    }
                }
#endif  /* MFBD_MBTN_CONTINUE_LONG_COUNT */
                else
                {
                    /* MFBD_BTN_STATE_UP */
                    /* clear long_count. */
                    if (((MFBD_LONG_TIME_IN_FUC) > 0) && (MFBD_BTN_INFO_IN_FUC->btn_long_code != 0))
                    {
                        /* long event is enabled in this btn. */
                        MFBD_BTN_IN_FUC->long_count = 1;
                    }
                    else
                    {
                        /* long event is disabled in this btn. */
                        MFBD_BTN_IN_FUC->long_count = 0;
                    }
                    if (MFBD_BTN_INFO_IN_FUC->btn_down_code[MFBD_BTN_IN_FUC->multiclick_state] > 0)
                    {
                        _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_down_code[MFBD_BTN_IN_FUC->multiclick_state]);
                    }
                    MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_DOWN;
                }
            }
            else if(MFBD_BTN_IN_FUC->filter_count >= (MFBD_FILTER_TIME_IN_FUC))
            {
                MFBD_BTN_IN_FUC->filter_count++;
            }
            else
            {
                MFBD_BTN_IN_FUC->filter_count = (MFBD_FILTER_TIME_IN_FUC);
            }
        }
        else if (btn_state == MFBD_BTN_STATE_UP)
        {
            if (MFBD_BTN_IN_FUC->filter_count == 0)
            {
              if (MFBD_BTN_IN_FUC->state != MFBD_BTN_STATE_UP)
              {
#if MFBD_MULTICLICK_STATE_AUTO_RESET
                  /* if multiclick_state is not 0 and less than max_multiclick_state, inc multiclick_state */
                  if (((MFBD_MULTICLICK_TIME_IN_FUC) != 0)
                          && (MFBD_BTN_IN_FUC->multiclick_state < MFBD_BTN_INFO_IN_FUC->max_multiclick_state)
#if (MFBD_MBTN_CONTINUE_LONG_COUNT==0)
                          && (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_DOWN)
#endif  /* (MFBD_MBTN_CONTINUE_LONG_COUNT==0) */
                          )
                  {
                      MFBD_BTN_IN_FUC->multiclick_state++;
                      MFBD_BTN_IN_FUC->multiclick_count = 0;
                  }
#else
                  /* if multiclick_state is not 0 and less than max_multiclick_state, inc multiclick_state */
                  if (((MFBD_MULTICLICK_TIME_IN_FUC) != 0)
#if (MFBD_MBTN_CONTINUE_LONG_COUNT==0)
                          && (MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_DOWN)
#endif  /* (MFBD_MBTN_CONTINUE_LONG_COUNT==0) */
                          )
                  {
                      if(MFBD_BTN_IN_FUC->multiclick_state < MFBD_BTN_INFO_IN_FUC->max_multiclick_state)
                      {
                          MFBD_BTN_IN_FUC->multiclick_state++;
                      }
                      MFBD_BTN_IN_FUC->multiclick_count = 0;
                  }
#endif  /* MFBD_MULTICLICK_STATE_AUTO_RESET */
                  else
                  {
                      /* over max multi-click times or (long event and repeat event) happened, reset to 0. */
                      MFBD_BTN_IN_FUC->multiclick_state = 0;
                  }
                  if (MFBD_BTN_INFO_IN_FUC->btn_up_code > 0)
                  {
                      _pbtn_group->btn_value_report(MFBD_BTN_INFO_IN_FUC->btn_up_code);
                  }
                  MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_UP;
              }
              else
              {
                  if (MFBD_BTN_IN_FUC->multiclick_state != 0)
                  {
                      MFBD_BTN_IN_FUC->multiclick_count++;
                      if (MFBD_BTN_IN_FUC->multiclick_count >= (MFBD_MULTICLICK_TIME_IN_FUC))
                      {
                          MFBD_BTN_IN_FUC->multiclick_state = 0;
                      }
                  }
              }
            }
            else
            {
                if (MFBD_BTN_IN_FUC->filter_count > (MFBD_FILTER_TIME_IN_FUC))
                {
                    MFBD_BTN_IN_FUC->filter_count = (MFBD_FILTER_TIME_IN_FUC);
                }
                else if (MFBD_BTN_IN_FUC->filter_count != 0)
                {
                    MFBD_BTN_IN_FUC->filter_count--;
                }
            }
        }
        _ppbtn++;
        _pbtn = *_ppbtn;
    }
}

/**
 * @brief skip some times of mfbd_btn_count_t with last state.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 * @param times is times need to skip.
 *
 * @return None.
 */
void mfbd_mbtn_skip(const mfbd_group_t *_pbtn_group, mfbd_btn_count_t times)
{
    mfbd_mbtn_t **_ppbtn = _pbtn_group->mbtns;
    mfbd_mbtn_t *_pbtn = *_ppbtn;

    while (MFBD_BTN_IN_FUC != NULL)
    {
        if(MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_UP)
        {
            if (MFBD_BTN_IN_FUC->multiclick_state != 0)
            {
                if (MFBD_BTN_IN_FUC->multiclick_count < (MFBD_MULTICLICK_TIME_IN_FUC))
                {
                    if(((MFBD_MULTICLICK_TIME_IN_FUC) - MFBD_BTN_IN_FUC->multiclick_count) > times)
                    {
                        MFBD_BTN_IN_FUC->multiclick_count = MFBD_BTN_IN_FUC->multiclick_count + times;
                    }
                    else
                    {
                        MFBD_BTN_IN_FUC->multiclick_state = 0;
                    }
                }
            }
        }
#if MFBD_MBTN_CONTINUE_LONG_COUNT
#if MFBD_MBTN_MULTICLICK_LONG_EVT
        else if(MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_DOWN)
        {
            if ((MFBD_BTN_IN_FUC->long_count > 0) && (MFBD_BTN_IN_FUC->long_count < (MFBD_LONG_TIME_IN_FUC)))
            {
                if(((MFBD_LONG_TIME_IN_FUC) - MFBD_BTN_IN_FUC->long_count) > times)
                {
                    MFBD_BTN_IN_FUC->long_count = MFBD_BTN_IN_FUC->long_count + times;
                }
                else
                {
                    MFBD_BTN_IN_FUC->long_count = MFBD_LONG_TIME_IN_FUC;
                }
            }
        }
        else
        {
            /* MFBD_BTN_STATE_LONG, if(MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_LONG) */
            if ((MFBD_BTN_IN_FUC->repeat_count > 0) && (MFBD_BTN_IN_FUC->repeat_count < (MFBD_REPEAT_TIME_IN_FUC)))
            {
                if(((MFBD_REPEAT_TIME_IN_FUC) - MFBD_BTN_IN_FUC->repeat_count) > times)
                {
                    MFBD_BTN_IN_FUC->repeat_count = MFBD_BTN_IN_FUC->repeat_count + times;
                }
                else
                {
                    MFBD_BTN_IN_FUC->repeat_count = MFBD_REPEAT_TIME_IN_FUC;
                }
            }
        }
#else
        else if(MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_DOWN)
        {
            if ((MFBD_BTN_IN_FUC->long_count > 0) && (MFBD_BTN_IN_FUC->long_count < (MFBD_LONG_TIME_IN_FUC)))
            {
                /* if long_time is 0 or long_code is 0, disable long and repeat check. */
                if(((MFBD_LONG_TIME_IN_FUC) - MFBD_BTN_IN_FUC->long_count) > times)
                {
                    MFBD_BTN_IN_FUC->long_count = MFBD_BTN_IN_FUC->long_count + times;
                }
                else
                {
                    MFBD_BTN_IN_FUC->long_count = MFBD_LONG_TIME_IN_FUC;
                }
            }
        }
        else
        {
            /* MFBD_BTN_STATE_LONG, if(MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_LONG) */
            /* we don't support repeat event here.*/
        }
#endif  /* MFBD_MBTN_MULTICLICK_LONG_EVT */
#else
        else if(MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_DOWN)
        {
            if ((MFBD_BTN_IN_FUC->long_count > 0) && (MFBD_BTN_IN_FUC->long_count < (MFBD_LONG_TIME_IN_FUC)))
            {
                if(((MFBD_LONG_TIME_IN_FUC) - MFBD_BTN_IN_FUC->long_count) > times)
                {
                    MFBD_BTN_IN_FUC->long_count = MFBD_BTN_IN_FUC->long_count + times;
                }
                else
                {
                    MFBD_BTN_IN_FUC->long_count = MFBD_LONG_TIME_IN_FUC;
                }
            }
        }
        else
        {
            /* MFBD_BTN_STATE_LONG, if(MFBD_BTN_IN_FUC->state == MFBD_BTN_STATE_LONG) */
            if ((MFBD_BTN_IN_FUC->repeat_count > 0) && (MFBD_BTN_IN_FUC->repeat_count < (MFBD_REPEAT_TIME_IN_FUC)))
            {
                if(((MFBD_REPEAT_TIME_IN_FUC) - MFBD_BTN_IN_FUC->repeat_count) > times)
                {
                    MFBD_BTN_IN_FUC->repeat_count = MFBD_BTN_IN_FUC->repeat_count + times;
                }
                else
                {
                    MFBD_BTN_IN_FUC->repeat_count = MFBD_REPEAT_TIME_IN_FUC;
                }
            }
        }
#endif  /* MFBD_MBTN_CONTINUE_LONG_COUNT */
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
    while (MFBD_BTN_IN_FUC != NULL)
    {
        MFBD_BTN_IN_FUC->filter_count = 0;
        MFBD_BTN_IN_FUC->long_count = 0;
        MFBD_BTN_IN_FUC->multiclick_count = 0;
        MFBD_BTN_IN_FUC->multiclick_state = 0;
        MFBD_BTN_IN_FUC->repeat_count = 0;
        MFBD_BTN_IN_FUC->state = MFBD_BTN_STATE_UP;
        _ppbtn++;
        _pbtn = *_ppbtn;
    }
}

#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */


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
#endif  /* MFBD_USE_BTN_SCAN_PRE_FUNC */

#if MFBD_USE_TINY_BUTTON
    if (_pbtn_group->tbtns != NULL)
    {
        /*scan tiny buttons in group.*/
        mfbd_tbtn_scan(_pbtn_group);
    }
#endif  /* MFBD_USE_TINY_BUTTON */

#if MFBD_USE_NORMAL_BUTTON
    if (_pbtn_group->nbtns != NULL)
    {
        /*scan normal buttons in group.*/
        mfbd_nbtn_scan(_pbtn_group);
    }
#endif  /* MFBD_USE_NORMAL_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
    if (_pbtn_group->mbtns != NULL)
    {
        /*scan multifunction buttons in group.*/
        mfbd_mbtn_scan(_pbtn_group);
    }
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

#if MFBD_USE_BTN_SCAN_AFTER_FUNC
    if (_pbtn_group->btn_scan_after != NULL)
    {
        /* call after scan function */
        _pbtn_group->btn_scan_after();
    }
#endif  /* MFBD_USE_BTN_SCAN_AFTER_FUNC */

}


/**
 * @brief skip some times with last state.
 *
 * @param _pbtn_group is a pointer of mfbd_group_t.
 * @param times is times need to skip.
 *
 * @return None.
 */
void mfbd_group_skip(const mfbd_group_t *_pbtn_group, mfbd_btn_count_t times)
{
    /* tbtn doesn't need to skip times. */

#if MFBD_USE_NORMAL_BUTTON
    if (_pbtn_group->nbtns != NULL)
    {
        /* skip times of normal buttons in group.*/
        mfbd_nbtn_skip(_pbtn_group, times);
    }
#endif  /* MFBD_USE_NORMAL_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
    if (_pbtn_group->mbtns != NULL)
    {
        /* skip times of multifunction buttons in group.*/
        mfbd_mbtn_skip(_pbtn_group, times);
    }
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

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
#endif  /* MFBD_USE_TINY_BUTTON */

#if MFBD_USE_NORMAL_BUTTON
    if (_pbtn_group->nbtns != NULL)
    {
        /*reset normal buttons in group.*/
        mfbd_nbtn_reset(_pbtn_group);
    }
#endif  /* MFBD_USE_NORMAL_BUTTON */

#if MFBD_USE_MULTIFUCNTION_BUTTON
    if (_pbtn_group->mbtns != NULL)
    {
        /*reset multifunction buttons in group.*/
        mfbd_mbtn_reset(_pbtn_group);
    }
#endif  /* MFBD_USE_MULTIFUCNTION_BUTTON */

}

#endif  /* (MFBD_USE_SECTION_DEFINITION == 0) */
