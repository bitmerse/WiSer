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
 * @file app_tusb.h
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief Application tusb usb cdc device header
 */
#ifndef APP_TUSB_H
#define APP_TUSB_H

/**
 * @defgroup app_tusb Application tinyusb communication  Module
 * @brief Module for managing application USB CDC communication
 * @{
 */

/**
 * @addtogroup app_tusb_define
 * @{
 */

/* 1 for WiSer-USB and 0 for WiSer-TTL */
#if (DEVICE_CONFIG_MODE == DEVICE_CONFIG_MODE_USB)
#define DEVICE_WISER_USB    1
#else
#define DEVICE_WISER_USB    0
#endif

/** @} */ // End of app_tusb_define group

#if DEVICE_WISER_USB

/**
 * @addtogroup app_tusb_types
 * @{
 */
typedef enum {
    APP_TUSB_TYPE_DATA=0,
    APP_TUSB_TYPE_CONFIG,
    APP_TUSB_TYPE_DTR,
    APP_TUSB_TYPE_RTS,
    APP_TUSB_TYPE_DTR_RTS,
} app_tusb_frame_type_t;

typedef enum {
    APP_TUSB_HW_FLOW_DISABLE=0,
    APP_TUSB_HW_FLOW_ENABLE,
} app_tusb_hw_flow_status_t;

typedef enum {
    APP_TUSB_HW_FLOW_LINE_STATE_LOW=0,
    APP_TUSB_HW_FLOW_LINE_STATE_HIGH=1,
    APP_TUSB_HW_FLOW_LINE_STATE_UNKNOWN=2,
} app_tusb_hw_flow_line_state_t;

typedef struct {
    uint8_t type;
    size_t len;
    uint8_t *data;
} app_tusb_data_t;


typedef struct {
    uint8_t type;
    union {
        config_settings_t config_settings;
        config_hw_line_t config_hw_line;
    };
} app_tusb_evt_config_t;

/** @} */ // End of app_tusb_types group

/**
 * @addtogroup app_tusb_global_funcs
 * @{
 */

/**
 * @brief initialize the usb cdc devices
 * 
 */
int app_tusb_init(void);
/**
 * @brief write to device over usb cdc
 * @param tx_buf transmit buffer
 * @param tx_size buffer size
 * 
 */
void app_tusb_write(const uint8_t *tx_buf, size_t tx_size);
/**
 * @brief sends serial connection configuration over espnow
 * 
 */
void app_tusb_config_request (void);
/**
 * @brief sends event of enabling hardware flow control over espnow
 * 
 */
void app_tusb_config_hw_flow_enable (void);
/**
 * @brief sends event of disabling hardware flow control over espnow
 * 
 */
void app_tusb_config_hw_flow_disble (void);
/**
 * @brief toggles the hardware flow control state
 * 
 */
void app_tusb_config_hw_flow_toggle (void);

/** @} */ // End of app_tusb_global_funcs group
#endif

/** @} */ // End of app_tusb group

#endif  // End of APP_TUSB_H