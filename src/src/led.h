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
 * @file led.h
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief led control module header
 */

#ifndef LED_H
#define LED_H

/**
 * @defgroup led control  Module
 * @brief Module for controlling Tx, Rx and Conn LEDs
 * @{
 */

/**
 * @addtogroup led_include
 * @{
 */

/** @} */ // End of led_include group

/**
 * @addtogroup led_define
 * @{
 */

/** @} */ // End of led_define group


/**
 * @addtogroup led_types
 * @{
 */

/** @} */ // End of led_types group

/**
 * @addtogroup led_global_funcs
 * @{
 */

/**
 * @brief initialize led module
 *
 */
void led_init(void);

/**
 * @brief turns on Tx LED
 *
 */
void led_tx_on(void);

/**
 * @brief turns off Tx LED
 *
 */
void led_tx_off(void);

/**
 * @brief turns on Rx LED
 *
 */
void led_rx_on(void);

/**
 * @brief turns off Rx LED
 *
 */
void led_rx_off(void);

/**
 * @brief turns on Conn LED
 *
 */
void led_conn_on(void);

/**
 * @brief turns off Conn LED
 *
 */
void led_conn_off(void);

/** @} */ // End of led_global_funcs group

/** @} */ // End of led group
#endif


