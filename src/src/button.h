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
 * @file button.h
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief button module header
 */

#ifndef BUTTON_H
#define BUTTON_H

/**
 * @defgroup button Button control Module
 * @brief Module for controlling Buttons on WiSer-USB device
 * @{
 */

/**
 * @addtogroup button_include
 * @{
 */

/** @} */ // End of button_include group

/**
 * @addtogroup button_define
 * @{
 */

#define BUTTON_CONN_GPIO GPIO_NUM_36

typedef enum {
    BUTTON_GPIO_PRESSED = 0,
    BUTTON_GPIO_RELEASED = 1
} button_status_t;

/** @} */ // End of button_define group

/**
 * @addtogroup button_types
 * @{
 */

/** @} */ // End of button_types group

/**
 * @addtogroup button_global_vars
 * @{
 */
extern QueueHandle_t s_button_queue;
/** @} */ // End of button_global_vars group


/**
 * @addtogroup button_global_funcs
 * @{
 */

/**
 * @brief initialize button module
 *
 */
void button_init(void);

/**
 * @brief reads button gpio level/status
 *
 * @return button_status_t button status
 */
button_status_t button_read(void);

/** @} */ // End of button_global_funcs group
/** @} */ // End of button group
#endif
