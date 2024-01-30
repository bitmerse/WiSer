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
 * @file app_uart.h
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief Application uart communication header
 */

#ifndef APP_UART_H
#define APP_UART_H

/**
 * @defgroup app_uart Application UART communication  Module
 * @brief Module for managing application UART communication
 * @{
 */

/**
 * @addtogroup app_uart_define
 * @{
 */

/* 1 for WiSer-TTL and 0 for WiSer-USB */
#if (DEVICE_CONFIG_MODE == DEVICE_CONFIG_MODE_UART)
#define DEVICE_WISER_UART   1
#else
#define DEVICE_WISER_UART   0
#endif

#if DEVICE_WISER_UART
typedef enum {
    APP_UART_TYPE_DATA=0,
    APP_UART_TYPE_CONFIG_SETTINGS,
    APP_UART_TYPE_CONFIG_HW_LINE,
} app_uart_evt_type_t;

typedef enum {
    APP_UART_HW_FLOW_DISABLE=0,
    APP_UART_HW_FLOW_ENABLE,
} app_usb_hw_flow_status_t;

/** @} */ // End of app_uart_define group


/**
 * @addtogroup app_uart_types
 * @{
 */
typedef struct {
    uint8_t type;
    size_t len;
    uint8_t *data;
} app_uart_evt_data_t;

typedef struct {
    uint8_t type;
    union {
        config_settings_t config_settings;
        config_hw_line_t config_hw_line;
    };
} app_uart_evt_config_t;

/** @} */ // End of app_uart_types group

/**
 * @addtogroup app_uart_global_funcs
 * @{
 */

/**
 * @brief initialize app UART interface.
 * 
 */
int app_uart_init(void);
/**
 * @brief reconfigures UART configuration.
 * @param config_settings UART configuration settings
 * 
 */
void app_uart_config_reset(config_settings_t config_settings);
/**
 * @brief write to device over UART
 * @param tx_buf transmit buffer
 * @param tx_size buffer size
 * 
 */
void app_uart_write(const uint8_t *tx_buf, size_t tx_size);
/**
 * @brief set DTR level
 * @param dtr_state DTR level
 * 
 */
void app_uart_dtr_set(uint8_t dtr_state);
/**
 * @brief set RTS level
 * @param rts_state RTS level
 * 
 */
void app_uart_rts_set(uint8_t rts_state);

/** @} */ // End of app_uart_global_funcs group

#endif

/** @} */ // End of app_uart group

#endif  // End of APP_UART_H