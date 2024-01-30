/**********************************************************************************
 * MIT License                                                                    *
 *                                                                                *
 * Copyright (c) 2024 Bitmerse LLP                                                *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 *********************************************************************************/
/**
 * @file app_conn.h
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief Application connection indication header
 */

#ifndef APP_CONN_H
#define APP_CONN_H

/**
 * @defgroup app_conn Application Connection indication  Module
 * @brief Module for managing application connection indication
 * @{
 */

/**
 * @addtogroup app_conn_define
 * @{
 */

#define APP_CONN_ON_PERIOD  5000   /**< Connection indication period in seconds */
#define APP_CONN_OFF_PERIOD  10   /**< Connection indication period in seconds */
#define APP_CONN_ON_COUNT  1   /**< Connection indication flash count */

#define APP_CONN_FIND_PAIR_LONG_PRESS   2000

typedef enum {
    APP_CONN_LED_OFF = 0,
    APP_CONN_LED_ON = 1
} app_conn_led_state_t;

/** @} */ // End of app_conn_define group

/**
 * @addtogroup app_conn_global_funcs
 * @{
 */

/**
 * @brief Initialize the application connection module
 */
void app_conn_init(void);

/**
 * @brief Turn on the application connection indication  for the specified period
 *
 * @param conn_period Connection indication period in seconds
 */
void app_conn_on(int conn_on_period, int conn_off_period, int count);

/**
 * @brief Turn on the application connection led
 */
void app_conn_led_off(void);

/**
 * @brief Turn off the application connection led
 */
void app_conn_led_off(void);

/** @} */ // End of app_conn_global_funcs group

/** @} */ // End of app_conn group

#endif