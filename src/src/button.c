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
 * @file button.c
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief Button control module which controls press events for buttons on WiSer-USB
 */

/**
 * @defgroup button Button control Module
 * @brief Module for controlling Buttons on WiSer-USB device
 * @{
 */


/**
 * @addtogroup button_include
 * @{
 */
#include <stdio.h>
#include "commons.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "config.h"
#include "app_tusb.h"
#include "button.h"


#if DEVICE_WISER_USB

/** @} */ // End of button_include group

/**
 * @addtogroup button_define
 * @{
 */

#define BUTTON_QUEUE_SIZE   1

/** @} */ // End of button_define group

/**
 * @addtogroup button_static_vars
 * @{
 */

/** @} */ // End of button_static_vars group


/**
 * @addtogroup button_global_vars
 * @{
 */
int state = 0;
QueueHandle_t s_button_queue;

/** @} */ // End of button_global_vars group

/**
 * @addtogroup button_static_funcs
 * @{
 */

/**
 * @brief interrupt handler for button gpio
 *
 */
static void button_gpio_interrupt_handler(void *args);

/**
 * @brief interrupt handler for button gpio
 *
 */
static void IRAM_ATTR button_gpio_interrupt_handler(void *args)
{
    int pin_number = (int)args;
    xQueueSendFromISR(s_button_queue, &pin_number, NULL);
}

/** @} */ // End of button_static_funcs group


/**
 * @addtogroup button_global_funcs
 * @{
 */

/**
 * @brief initialize button module
 *
 */
void button_init(void)
{
    s_button_queue = xQueueCreate(BUTTON_QUEUE_SIZE, sizeof(int));

    gpio_set_direction(BUTTON_CONN_GPIO, GPIO_MODE_INPUT);
    gpio_set_intr_type(BUTTON_CONN_GPIO, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_CONN_GPIO, button_gpio_interrupt_handler, (void *)BUTTON_CONN_GPIO);
}

/**
 * @brief reads button gpio level/status
 *
 * @return button_status_t button status
 */
button_status_t button_read(void)
{
    return gpio_get_level(BUTTON_CONN_GPIO);

}

/** @} */ // End of button_global_funcs group
#endif

/** @} */ // End of button module