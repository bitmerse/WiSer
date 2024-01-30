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
 * @file app.c
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief application module inializes all application modules
 */

/**
 * @defgroup app application module
 * @brief Module for inializing all application modules
 * @{
 */


/**
 * @addtogroup app_include
 * @{
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "esp_check.h"
#include "config.h"
#include "commons.h"
#include "app_uart.h"
#include "app_tusb.h"
#include "app_espnow.h"
#include "app_conn.h"
#include "led.h"
#include "app.h"

/** @} */ // End of app_include group

/**
 * @addtogroup app_define
 * @{
 */

#if (((DEVICE_WISER_USB) && (DEVICE_WISER_UART)) || (!(DEVICE_WISER_USB) && !(DEVICE_WISER_UART)))
    #error Device mode configuration is not correct!
#endif

/** @} */ // End of app_define group

/**
 * @addtogroup app_static_vars
 * @{
 */

/** @} */ // End of app_static_vars group

/**
 * @addtogroup app_global_vars
 * @{
 */

/** @} */ // End of app_global_vars group

/**
 * @addtogroup app_static_funcs
 * @{
 */

/** @} */ // End of app_static_funcs group


/**
 * @addtogroup app_global_funcs
 * @{
 */

/**
 * @brief initialize application modules
 * 
 * @return error code
 */
int app_init(void) 
{
    led_init();
    
    vTaskDelay(50);
    app_espnow_init();
    vTaskDelay(100);

#if DEVICE_WISER_UART
    app_uart_init();
#endif
#if DEVICE_WISER_USB
    app_tusb_init();
#endif
    app_conn_init();
    return ESP_OK;
}

/** @} */ // End of app_global_funcs group

/** @} */ // End of app module