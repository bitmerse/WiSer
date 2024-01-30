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
 * @file main.c
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief main module inializes all application modules and runs application
 */

/**
 * @defgroup main application module
 * @brief Module for inializing all application modules and run application tasks
 * @{
 */


/**
 * @addtogroup main_include
 * @{
 */
#include "app.h"

/** @} */ // End of main_include group

/**
 * @addtogroup main_define
 * @{
 */

/** @} */ // End of main_define group


/**
 * @addtogroup main_global_funcs
 * @{
 */

/**
 * @brief initializes application modules and runs application
 *
 */
void app_main() {
    app_init();
}

/** @} */ // End of main_global_funcs group

/** @} */ // End of main module