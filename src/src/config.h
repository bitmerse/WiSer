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
 * @file config.h
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief config module header
 */
#ifndef CONFIG_H
#define CONFIG_H

/**
 * @defgroup config application Module
 * @brief Module for configuration of device
 * @{
 */

/**
 * @addtogroup config_include
 * @{
 */

/** @} */ // End of config_include group

/**
 * @addtogroup config_define
 * @{
 */

#define DEVICE_CONFIG_MODE_NONE 0
#define DEVICE_CONFIG_MODE_USB  1
#define DEVICE_CONFIG_MODE_UART 2


/* select device mode to either DEVICE_CONFIG_MODE_USB or DEVICE_CONFIG_MODE_UART */
#define DEVICE_CONFIG_MODE  DEVICE_CONFIG_MODE_NONE

/** @} */ // End of config_define group

/**
 * @addtogroup config_types
 * @{
 */

/** @} */ // End of config_types group

/**
 * @addtogroup config_global_vars
 * @{
 */

/** @} */ // End of config_global_vars group


/**
 * @addtogroup config_global_funcs
 * @{
 */

/** @} */ // End of config_global_funcs group
/** @} */ // End of config group
#endif  // End CONFIG_H
