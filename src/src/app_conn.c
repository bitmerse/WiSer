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
 * @file app_conn.c
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief Application connection indication module which turns on CONN LED on both paired devices on Button Press
 */

/**
 * @defgroup app_conn Application Connection Indication Module
 * @brief Module for indicating paired/connected WiSer-USB and WiSer-TTL device through CONN LED 
 * @{
 */

/**
 * @addtogroup app_conn_include
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "config.h"
#include "commons.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "app_tusb.h"
#include "led.h"
#include "button.h"
#include "app_espnow.h"
#include "app_conn.h"
#include "app_conn.h"

/** @} */ // End of app_conn_include group

/**
 * @addtogroup app_conn_static_vars
 * @{
 */

/**
 * @brief Tag used for logging in this module
 */
static const char *TAG = "app_conn";

/**
 * @brief Timer handle for the connection timer
 */
static esp_timer_handle_t app_conn_timer_handler;

static int app_conn_led_on_period = 0;
static int app_conn_led_off_period = 0;
static int app_conn_led_on_count = 0;
static uint8_t app_conn_led_last_state = APP_CONN_LED_OFF;

/** @} */ // End of app_conn_static_vars group

#if DEVICE_WISER_USB
/**
 * @addtogroup app_conn_static_funcs
 * @{
 */

/**
 * @brief Task for handling the application connection indication
 *
 * @param params Task parameters
 */
static void app_conn_task(void *params);

/** @} */ // End of app_conn_static_funcs group
#endif

/**
 * @addtogroup app_conn_static_funcs
 * @{
 */

/**
 * @brief Callback function for the connection timer for LED ON
 *
 * @param param Timer parameter
 */
static void app_conn_timer_callback(void *param);

/** @} */ // End of app_conn_static_funcs group

/**
 * @addtogroup app_conn_global_vars
 * @{
 */

// Define any global variables here

/** @} */ // End of app_conn_global_vars group

#if DEVICE_WISER_USB
/**
 * @addtogroup app_conn_static_funcs
 * @{
 */

/**
 * @brief Task for handling the application connection indication
 *
 * @param params Task parameters
 */
static void app_conn_task(void *params)
{
    int pin_number;
    device_conn_t device_conn;
    device_conn.conn_on_period = APP_CONN_ON_PERIOD; // in seconds
    device_conn.conn_off_period = APP_CONN_OFF_PERIOD; // in seconds
    device_conn.conn_on_count = APP_CONN_ON_COUNT; // in seconds

    while (true)
    {
        if (xQueueReceive(s_button_queue, &pin_number, portMAX_DELAY))
        {
            if (pin_number == BUTTON_CONN_GPIO)
            {
                int button_press_time = 0;

                app_espnow_device_conn_send(device_conn);
                app_conn_on(device_conn.conn_on_period, device_conn.conn_off_period, device_conn.conn_on_count);
                while(button_read() == BUTTON_GPIO_PRESSED)
                {
                    vTaskDelay(50);
                    button_press_time = button_press_time + 50;

                    if (button_press_time >= APP_CONN_FIND_PAIR_LONG_PRESS)
                    {
                        app_tusb_config_hw_flow_toggle();
                        break;
                    }
                }
            }
        }
    }
}

/** @} */ // End of app_conn_static_funcs group
#endif

/**
 * @addtogroup app_conn_static_funcs
 * @{
 */

/**
 * @brief Callback function for the connection timer
 *
 * @param param Timer parameter
 */
static void app_conn_timer_callback(void *param)
{
    app_conn_led_off();
    app_conn_led_on_count--;
    if(app_conn_led_on_count > 0) {
        vTaskDelay(app_conn_led_off_period);
        app_conn_on (app_conn_led_on_period, app_conn_led_off_period, app_conn_led_on_count);
    }
}
/** @} */ // End of app_conn_static_funcs group

/**
 * @addtogroup app_conn_global_funcs
 * @{
 */

/**
 * @brief Initialize the application connection module
 */
void app_conn_init(void)
{
    const esp_timer_create_args_t app_conn_timer_args = {
      .callback = &app_conn_timer_callback,
      .name = "app_conn_timer_callback"};
    ESP_ERROR_CHECK(esp_timer_create(&app_conn_timer_args, &app_conn_timer_handler));
#if DEVICE_WISER_USB
    button_init();
    xTaskCreate(app_conn_task, "app_conn_task", 2048, NULL, 1, NULL);
#endif
}

/**
 * @brief Turn on the application connection indication for the specified period
 *
 * @param conn_period Connection indication period in seconds
 */
void app_conn_on(int conn_on_period, int conn_off_period, int count)
{
    esp_timer_stop(app_conn_timer_handler);
    app_conn_led_on_count = count;
    app_conn_led_on_period = conn_on_period;
    app_conn_led_off_period = conn_off_period;
    if(app_conn_led_on_count > 0 && app_conn_led_on_period > 0 && app_conn_led_off_period >= 0) {     
        app_conn_led_last_state = APP_CONN_LED_ON;   
        led_conn_on();
        ESP_ERROR_CHECK(esp_timer_start_once(app_conn_timer_handler, (app_conn_led_on_period * 1000)));
    } else {
        app_conn_led_on_count = 0;
        app_conn_led_on_period = 0;
        app_conn_led_off_period = 0;
        app_conn_led_last_state = APP_CONN_LED_OFF; 
    }
}

/**
 * @brief Turn on the application connection led
 */
void app_conn_led_on(void)
{
    led_conn_on();
}

/**
 * @brief Turn off the application connection led
 */
void app_conn_led_off(void)
{
    led_conn_off();
}

/** @} */ // End of app_conn_global_funcs group

/** @} */ // End of app_conn module
