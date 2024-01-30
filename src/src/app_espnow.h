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
 * @file app_espnow.h
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief Application espnow peer communication header
 */

#ifndef APP_ESPNOW_H
#define APP_ESPNOW_H

/**
 * @defgroup app_espnow Application espnow peer communication  Module
 * @brief Module for managing application peer communication
 * @{
 */

/**
 * @addtogroup app_espnow_include
 * @{
 */

#include "esp_random.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_now.h"
#include "esp_crc.h"
#include "nvs_peer.h"
/** @} */ // End of app_espnow_include group

/**
 * @addtogroup app_espnow_define
 * @{
 */
/* ESPNOW can work in both station and softap mode. It is configured in menuconfig. */
#define CONFIG_ESPNOW_WIFI_MODE_STATION 1
#if CONFIG_ESPNOW_WIFI_MODE_STATION
#define ESPNOW_WIFI_MODE WIFI_MODE_STA
#define ESPNOW_WIFI_IF   ESP_IF_WIFI_STA
#else
#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF   ESP_IF_WIFI_AP
#endif

#define CONFIG_ESPNOW_ENABLE_LONG_RANGE 0

#define APP_ESPNOW_QUEUE_SIZE           200

#define APP_ESPNOW_SEND_DATA_SIZE     240

#define APP_ESPNOW_HW_FLOW_OFF   0
#define APP_ESPNOW_HW_FLOW_ON   1
typedef enum {
    APP_ESPNOW_TYPE_DATA=0,
    APP_ESPNOW_TYPE_CONFIG_SETTINGS,
    APP_ESPNOW_TYPE_CONFIG_HW_LINE,
    APP_ESPNOW_TYPE_DEVICE_CONN,
    APP_ESPNOW_TYPE_CONFIG_REQ,
    APP_ESPNOW_TYPE_ACK,
} app_espnow_type_t;

// #define IS_BROADCAST_ADDR(addr) (memcmp(addr, s_app_broadcast_mac, ESP_NOW_ETH_ALEN) == 0)

typedef enum {
    APP_ESPNOW_SEND_CB,
    APP_ESPNOW_RECV_CB,
} app_espnow_event_id_t;

/** @} */ // End of app_conn_define group

/**
 * @addtogroup app_espnow_global_vars
 * @{
 */
extern volatile uint32_t app_espnow_send_timeout;
/** @} */ // End of app_espnow_global_vars group


/**
 * @addtogroup app_espnow_types
 * @{
 */
typedef struct {
    uint8_t mac_addr[ESP_NOW_ETH_ALEN];
    esp_now_send_status_t status;
} app_espnow_event_send_cb_t;

typedef struct {
    uint8_t mac_addr[ESP_NOW_ETH_ALEN];
    uint8_t type;
    uint8_t ser_count;
    size_t data_len;
    uint8_t *data;
} app_espnow_event_recv_cb_t;

typedef union {
    app_espnow_event_send_cb_t send_cb;
    app_espnow_event_recv_cb_t recv_cb;
} app_espnow_event_info_t;

/* When ESPNOW sending or receiving callback function is called, post event to ESPNOW task. */
typedef struct {
    app_espnow_event_id_t id;
    app_espnow_event_info_t info;
} app_espnow_event_t;

typedef struct {
    uint8_t *data;
    size_t len;
} app_espnow_data_send_t;

/** @} */ // End of app_espnow_types group

/**
 * @addtogroup app_espnow_global_funcs
 * @{
 */

/**
 * @brief initialize module espnow for communication between peers
 *
 */
void app_espnow_init(void);

/**
 * @brief deinitialize module
 *
 */
void app_espnow_deinit(void);

/**
 * @brief sends serial data over espnow
 *
 * @param  data data packet bytes
 * @param len length of data bytes
 */
void app_espnow_data_send(const uint8_t *data, size_t len);

/**
 * @brief sends serial config settings to peer over espnow
 * @param config_settings config settings to be sent
 */
void app_espnow_config_settings_send(const config_settings_t config_settings);

/**
 * @brief sends serial hw line state to peer over espnow
 * @param config_hw_line config hw line state to be sent
 */
void app_espnow_config_hw_line_send(const config_hw_line_t config_hw_line);

/**
 * @brief sends connection indication event to peer over espnow
 * @param device_conn connection indication parameters
 */
void app_espnow_device_conn_send(const device_conn_t device_conn);

/**
 * @brief sends serial configuration request to peer over espnow
 * 
 */
void app_espnow_config_req_send(void);

/** @} */ // End of app_espnow_global_funcs group

/** @} */ // End of app_espnow group

#endif