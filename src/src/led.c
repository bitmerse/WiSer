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
 * @file led.c
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief LED control module which turns on/off CONN, Tx and Rx LEDs
 */

/**
 * @defgroup led LED control Module
 * @brief Module for controlling LEDs on WiSer-USB and WiSer-TTL device
 * @{
 */

/**
 * @addtogroup led_include
 * @{
 */

#include "driver/gpio.h"
#include "config.h"
#include "commons.h"
#include "app_tusb.h"
#include "app_uart.h"
#include "led.h"

/** @} */ // End of led_include group

/**
 * @addtogroup led_define
 * @{
 */

#if DEVICE_WISER_USB
#define LED_TX_GPIO GPIO_NUM_35
#define LED_RX_GPIO GPIO_NUM_34
#define LED_CONN_GPIO GPIO_NUM_35
#endif

#if DEVICE_WISER_UART
#define LED_CONN_GPIO GPIO_NUM_21
#endif

#define LED_GPIO_ON     0
#define LED_GPIO_OFF    1

/** @} */ // End of led_define group

/**
 * @addtogroup led_static_vars
 * @{
 */

static bool led_conn_on_flag = false;

/** @} */ // End of led_static_vars group

/**
 * @addtogroup led_global_vars
 * @{
 */

/** @} */ // End of led_global_vars group

/**
 * @addtogroup led_static_funcs
 * @{
 */

/** @} */ // End of led_static_funcs group

/**
 * @addtogroup led_global_funcs
 * @{
 */

/**
 * @brief initialize led module
 *
 */
void led_init(void)
{
#if DEVICE_WISER_USB
    gpio_set_direction(LED_TX_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_RX_GPIO, GPIO_MODE_OUTPUT);
    led_tx_off();
    led_rx_off();
#endif

    gpio_set_direction(LED_CONN_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(LED_CONN_GPIO, GPIO_PULLUP_ONLY);    
    led_conn_off();
}

/**
 * @brief turns on Tx LED
 *
 */
void led_tx_on(void)
{
#if DEVICE_WISER_USB  
    gpio_set_level(LED_TX_GPIO, LED_GPIO_ON);
#endif
}

/**
 * @brief turns off Tx LED
 *
 */
void led_tx_off(void)
{
#if DEVICE_WISER_USB
    if(led_conn_on_flag == false) {
        gpio_set_level(LED_TX_GPIO, LED_GPIO_OFF);
    }
#endif
}

/**
 * @brief turns on Rx LED
 *
 */
void led_rx_on(void)
{
#if DEVICE_WISER_USB
    gpio_set_level(LED_RX_GPIO, LED_GPIO_ON);
#endif
}

/**
 * @brief turns off Rx LED
 *
 */
void led_rx_off(void)
{
#if DEVICE_WISER_USB
    gpio_set_level(LED_RX_GPIO, LED_GPIO_OFF);
#endif
}

/**
 * @brief turns on Conn LED
 *
 */
void led_conn_on(void)
{
    led_conn_on_flag = true;
    gpio_set_level(LED_CONN_GPIO, LED_GPIO_ON);
}

/**
 * @brief turns off Conn LED
 *
 */
void led_conn_off(void)
{
    led_conn_on_flag = false;
    gpio_set_level(LED_CONN_GPIO, LED_GPIO_OFF);
}

/** @} */ // End of led_global_funcs group

/** @} */ // End of led module