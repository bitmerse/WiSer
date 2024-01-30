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
 * @file commons.h
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief commons module header
 */
#ifndef COMMONS_H
#define COMMONS_H

/**
 * @defgroup commons application Module
 * @brief Module for all common types and constants used in multiple application modules
 * @{
 */

/**
 * @addtogroup commons_include
 * @{
 */

/** @} */ // End of commons_include group

/**
 * @addtogroup commons_define
 * @{
 */

#define TEST_RF_RSSI_ENABLE 0

#define HW_FLOW_EN_CONN_ON_PERIOD  100   /**< Connection indication period in milliseconds */
#define HW_FLOW_EN_CONN_OFF_PERIOD  100   /**< Connection indication period in milliseconds */
#define HW_FLOW_EN_APP_CONN_ON_COUNT  25   /**< Connection indication flash count */

#define HW_FLOW_DIS_CONN_ON_PERIOD  500   /**< Connection indication period in milliseconds */
#define HW_FLOW_DIS_CONN_OFF_PERIOD  500   /**< Connection indication period in milliseconds */
#define HW_FLOW_DIS_APP_CONN_ON_COUNT  5   /**< Connection indication flash count */

/** @} */ // End of commons_define group

/**
 * @addtogroup commons_types
 * @{
 */

typedef struct {
    uint8_t dtr;
    uint8_t rts;
} config_hw_line_t;

typedef struct {
    uint32_t bitrate;
    uint8_t data_bits;
    uint8_t parity;
    uint8_t stop_bits;
    uint8_t hw_flow_status;
} config_settings_t;

typedef struct {
    int conn_on_period;
    int conn_off_period;
    int conn_on_count;
} device_conn_t;

typedef struct {
    uint8_t type;
    uint8_t ser_count;
} data_ack_t;
/** @} */ // End of commons_types group

/**
 * @addtogroup commons_global_vars
 * @{
 */

/** @} */ // End of commons_global_vars group


/**
 * @addtogroup commons_global_funcs
 * @{
 */

/** @} */ // End of commons_global_funcs group
/** @} */ // End of commons group
#endif  // End COMMONS_H
