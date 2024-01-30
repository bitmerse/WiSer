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
 * @file app_espnow.c
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief Application espnow communication module which communicates with peer device over ESPNOW
 */

/**
 * @defgroup app_espnow Application espnow peer communication Module
 * @brief Module for managing application peer communication
 * @{
 */

/**
 * @addtogroup app_espnow_include
 * @{
 */
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <esp_private/wifi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "config.h"
#include "nvs_flash.h"
#include "commons.h"
#include "led.h"
#include "app_tusb.h"
#include "app_uart.h"
#include "app_conn.h"
#include "app_espnow.h"

/** @} */ // End of app_espnow_include group

/**
 * @addtogroup app_espnow_define
 * @{
 */

#define APP_ESPNOW_ENCYYPTION_ENABLE    1

// 0 for default peer mac, 1 for peer mac from NVS memory
#define APP_ESPNOW_USE_NVS_PEER_MAC    1

#define ESPNOW_MAXDELAY 512

// range 1 13
#define CONFIG_ESPNOW_CHANNEL   5
#define CONFIG_ESPNOW_LMK   "REPLACE_WITH_LMK_KEY"
#define CONFIG_ESPNOW_PMK   "REPLACE_WITH_PMK_KEY"

#define CONFIG_ESPNOW_LMK_LEN   13
// range 1 65535
#define CONFIG_ESPNOW_SEND_COUNT    100
// range 0 65535
#define CONFIG_ESPNOW_SEND_DELAY    1000
// range 10 250
#define CONFIG_ESPNOW_SEND_LEN  10

#define APP_ESPNOW_TX_SER_COUNT_DEFAULT 1
#define APP_ESPNOW_RX_SER_COUNT_DEFAULT 0

#define APP_ESPNOW_SEND_TIMEOUT_DEFAULT 20  // in ms

/** @} */ // End of app_espnow_define group

/**
 * @addtogroup app_espnow_static_vars
 * @{
 */

/**
 * @brief Tag used for logging in this module
 */
static const char *TAG = "app_espnow";

/**
 * @brief queue handler
 */
static QueueHandle_t s_app_espnow_queue;
static SemaphoreHandle_t xSemaphoreEspnowAck = NULL;
static SemaphoreHandle_t xSemaphoreEspnowSend = NULL;

static data_ack_t last_data_ack;

static bool esp_now_send_status = false;

/**
 * @brief default peer mac address
 */
static uint8_t s_app_peer_mac[ESP_NOW_ETH_ALEN] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

static uint8_t app_espnow_tx_ser_count = APP_ESPNOW_TX_SER_COUNT_DEFAULT;
static uint8_t app_espnow_rx_ser_count = APP_ESPNOW_RX_SER_COUNT_DEFAULT;

/** @} */ // End of app_espnow_static_vars group

/**
 * @addtogroup app_espnow_global_vars
 * @{
 */

// Define any global variables here
volatile uint32_t app_espnow_send_timeout = APP_ESPNOW_SEND_TIMEOUT_DEFAULT;

/** @} */ // End of app_espnow_global_vars group

/**
 * @addtogroup app_espnow_static_funcs
 * @{
 */

/**
 * @brief initialize wifi for espnow communication between peers
 *
 */
static void app_espnow_wifi_init(void);

/**
 * @brief data send callback for espnow communication between peers
 *
 * @param mac_addr address of peer mac address
 * @param status status of send event
 */
static void app_espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status);

/**
 * @brief data receive callback for espnow communication between peers
 *
 * @param recv_info address of peer info
 * @param data  data received
 * @param len length of data received
 */
static void app_espnow_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len);

/**
 * @brief task which handles espnow communication between peers
 *
 * @param pvParameter task parameters
 */
static void app_espnow_task(void *pvParameter);

/**
 * @brief create small data chunk to send over espnow
 *
 * @param data data buffer
 * @param len total length of data
 */
static void app_espnow_data_send_chunks(const uint8_t *data, size_t len);
/**
 * @brief handles sending acknowledgement on new ser packet received on espnow
 *
 * @param type data packet type
 * @param ser_count serial count of data packet
 */
static void app_espnow_ser_count_received(uint8_t type, uint8_t ser_count) ;

/**
 * @brief initialize module low level drivers for espnow communication between peers
 *
 * @return  esp error code
 */
static esp_err_t app_espnow_ll_init(void);

/**
 * @brief initialize module tasks for espnow communication between peers
 *
 * @return  esp error code
 */
static esp_err_t app_espnow_tasks_init(void);

/**
 * @brief resets the serial packet counts
 *
 */
static void app_espnow_ser_count_reset(void);

/**
 * @brief send acknowledgement for last packet received over espnow
 *
 * @param  data_ack data acknowledgement packet
 */
static void app_espnow_data_ack_send(const data_ack_t data_ack);

/**
 * @brief sends data packet over espnow
 *
 * @param  data data packet bytes
 * @param len length of data bytes
 */
static void app_espnow_send(uint8_t *data, size_t len);

/** @} */ // End of app_espnow_static_funcs group

/**
 * @addtogroup app_espnow_static_funcs
 * @{
 */
/* WiFi should start before using ESPNOW */
/**
 * @brief initialize wifi for espnow communication between peers
 *
 */
static void app_espnow_wifi_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_wifi_stop();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    cfg.ampdu_tx_enable = 0;                             //... and modify only what we want.
    cfg.ampdu_rx_enable = 0;                             //... and modify only what we want.


    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(ESPNOW_WIFI_MODE) );
 
    //disable power saving
    esp_wifi_set_ps (WIFI_PS_NONE);
 
    ESP_ERROR_CHECK( esp_wifi_start());
    ESP_ERROR_CHECK( esp_wifi_set_channel(CONFIG_ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE));

    esp_wifi_internal_set_fix_rate(WIFI_IF_STA, true, WIFI_PHY_RATE_54M);
    esp_wifi_config_espnow_rate(WIFI_IF_STA, WIFI_PHY_RATE_54M);

    int8_t tx_power=0;
    esp_wifi_get_max_tx_power(&tx_power);
    esp_wifi_set_max_tx_power(tx_power);
    esp_wifi_get_max_tx_power(&tx_power);
    ESP_LOGE(TAG, "tx_power: %d", tx_power);

#if CONFIG_ESPNOW_ENABLE_LONG_RANGE
    ESP_ERROR_CHECK( esp_wifi_set_protocol(ESPNOW_WIFI_IF, WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N|WIFI_PROTOCOL_LR) );
#endif
}

/* ESPNOW sending or receiving callback function is called in WiFi task.
 * Users should not do lengthy operations from this task. Instead, post
 * necessary data to a queue and handle it from a lower priority task. */
/**
 * @brief data send callback for espnow communication between peers
 *
 * @param mac_addr address of peer mac address
 * @param status status of send event
 */
static void app_espnow_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    // not required for this application
}

/**
 * @brief data receive callback for espnow communication between peers
 *
 * @param recv_info address of peer info
 * @param data  data received
 * @param len length of data received
 */
static void app_espnow_recv_cb(const esp_now_recv_info_t *recv_info, const uint8_t *data, int len)
{
    app_espnow_event_t evt;
    app_espnow_event_recv_cb_t *recv_cb = &evt.info.recv_cb;
    uint8_t *mac_addr = recv_info->src_addr;

    if (mac_addr == NULL || data == NULL || len < 2) {
        ESP_LOGE(TAG, "Receive cb arg error");
        return;
    }

    evt.id = APP_ESPNOW_RECV_CB;
    memcpy(recv_cb->mac_addr, mac_addr, ESP_NOW_ETH_ALEN);
    recv_cb->data = pvPortMalloc(len-1);
    if (recv_cb->data == NULL) {
        ESP_LOGE(TAG, "Malloc receive data fail");
        return;
    }

    recv_cb->type = data[0];
    recv_cb->ser_count = data[1];

    memcpy(recv_cb->data, &data[2], len-2);
    recv_cb->data_len = len-2;

#if TEST_RF_RSSI_ENABLE
    ESP_LOGE(TAG, "rssi: %d", recv_info->rx_ctrl->rssi);
#endif

    // if ack, then release the ack semaphore
    if(data[0] == APP_ESPNOW_TYPE_ACK) {
        data_ack_t data_ack;
        memcpy(&data_ack, &recv_cb->data[0], sizeof(data_ack));
        if(data_ack.type == last_data_ack.type && data_ack.ser_count == last_data_ack.ser_count) {
            esp_now_send_status = true;
            xSemaphoreGive(xSemaphoreEspnowAck);
        }
        vPortFree(recv_cb->data);
        return;
    } 

    // fill the s_app_espnow_queue data queue
    if (xQueueSend(s_app_espnow_queue, &evt, portMAX_DELAY) != pdTRUE) {
        ESP_LOGE(TAG, "Send receive queue fail");
        vPortFree(recv_cb->data);
    } else {
// if WiSer USB device, then send ack from here
#if DEVICE_WISER_USB
        if(data[0] == APP_ESPNOW_TYPE_DATA) {
            app_espnow_ser_count_received(recv_cb->type, recv_cb->ser_count);
        }
#endif        
    }
}

/**
 * @brief task which handles espnow communication between peers
 *
 * @param pvParameter task parameters
 */
static void IRAM_ATTR app_espnow_task(void *pvParameter)
{
    app_espnow_event_t evt;

    vTaskDelay(1000);
    ESP_LOGI(TAG, "Start esp now task");

    while (xQueueReceive(s_app_espnow_queue, &evt, portMAX_DELAY) == pdTRUE) {
        switch (evt.id) {
            case APP_ESPNOW_SEND_CB:
            {
                // app_espnow_event_send_cb_t *send_cb = &evt.info.send_cb;
                // ESP_LOGD(TAG, "Send data to "MACSTR", status1: %d", MAC2STR(send_cb->mac_addr), send_cb->status);
                break;
            }
            case APP_ESPNOW_RECV_CB:
            {
                app_espnow_event_recv_cb_t *recv_cb = &evt.info.recv_cb;
                
                // ESP_LOGI(TAG, "Receive error data from: "MACSTR"", MAC2STR(recv_cb->mac_addr));    
                if(memcmp(recv_cb->mac_addr, s_app_peer_mac, 6) == 0) {
                    switch(recv_cb->type) {
                        case APP_ESPNOW_TYPE_DATA: {
                            #if DEVICE_WISER_USB
                                led_rx_on();
                            #endif    
                            if(recv_cb->ser_count != app_espnow_rx_ser_count) {
                                #if DEVICE_WISER_USB
                                    // ESP_LOGI(TAG, "Receive data from: size: %d, data: %s", recv_cb->data_len, recv_cb->data);
                                    app_tusb_write(recv_cb->data, recv_cb->data_len);
                                #elif DEVICE_WISER_UART
                                    app_uart_write(recv_cb->data, recv_cb->data_len);
                                #endif
                            }
#if DEVICE_WISER_UART
                            app_espnow_ser_count_received(recv_cb->type, recv_cb->ser_count);
#endif
                            app_espnow_rx_ser_count = recv_cb->ser_count;
                            #if DEVICE_WISER_USB
                                led_rx_off();
                            #endif    
                        } break;
                        case APP_ESPNOW_TYPE_CONFIG_SETTINGS: {
                            config_settings_t config_settings;
                            memcpy(&config_settings, &recv_cb->data[0], sizeof(config_settings));
                            // ESP_LOGE(TAG, "Receive bitrate data from: %lu", config_settings.bitrate);
                            // ESP_LOGI(TAG, "Receive hw flow status from: %d", config_settings.hw_flow_status);
                            // ESP_LOGI(TAG, "Receive data_bit: %d, parity: %d, stop_bit: %d", config_settings.data_bits, config_settings.parity, config_settings.stop_bits);
                            #if DEVICE_WISER_UART
                                app_uart_config_reset(config_settings);
                            #endif
                        } break;
                        case APP_ESPNOW_TYPE_CONFIG_HW_LINE: {
                            config_hw_line_t config_hw_line;
                            memcpy(&config_hw_line, &recv_cb->data[0], sizeof(config_hw_line));
                            // ESP_LOGI(TAG, "Receive dtr: %d rts: %d", config_hw_line.dtr, config_hw_line.rts);
                            #if DEVICE_WISER_UART
                                app_uart_dtr_set(config_hw_line.dtr);
                                app_uart_rts_set(config_hw_line.rts);
                            #endif
                            app_espnow_ser_count_received(recv_cb->type, recv_cb->ser_count);
                        } break;
                        case APP_ESPNOW_TYPE_DEVICE_CONN: {
                            app_espnow_ser_count_received(recv_cb->type, recv_cb->ser_count);
                            device_conn_t device_conn;
                            memcpy(&device_conn, &recv_cb->data[0], sizeof(device_conn));
                            // ESP_LOGI(TAG, "Receive conn from: period: %d", device_conn.conn_on_period);
                            app_conn_on(device_conn.conn_on_period, device_conn.conn_off_period, device_conn.conn_on_count);
                        } break;
                        case APP_ESPNOW_TYPE_CONFIG_REQ: {
                            app_espnow_ser_count_received(recv_cb->type, recv_cb->ser_count);
                            #if DEVICE_WISER_USB
                                app_tusb_config_request();
                            #endif
                        } break;
                        default: {
                        } break;
                    }
                }
                vPortFree(recv_cb->data);
                break;
            }
            default:
                ESP_LOGE(TAG, "Callback type error: %d", evt.id);
                break;
        }
        taskYIELD();
    }
}

/**
 * @brief handles sending acknowledgement on new ser packet received on espnow
 *
 * @param type data packet type
 * @param ser_count serial count of data packet
 */
static void IRAM_ATTR app_espnow_ser_count_received(uint8_t type, uint8_t ser_count) 
{
    if(type != APP_ESPNOW_TYPE_ACK) {
        data_ack_t data_ack;
        data_ack.type = type;
        data_ack.ser_count = ser_count;
        app_espnow_data_ack_send(data_ack);
    }
}

/**
 * @brief initialize module low level drivers for espnow communication between peers
 *
 */
static esp_err_t app_espnow_ll_init(void)
{
    /* Initialize ESPNOW and register sending and receiving callback function. */
    ESP_ERROR_CHECK( esp_now_init() );
    ESP_ERROR_CHECK( esp_now_register_send_cb(app_espnow_send_cb) );
    ESP_ERROR_CHECK( esp_now_register_recv_cb(app_espnow_recv_cb) );
#if CONFIG_ESP_WIFI_STA_DISCONNECTED_PM_ENABLE
    ESP_ERROR_CHECK( esp_now_set_wake_window(65535) );
#endif
    /* Set primary master key. */
    ESP_ERROR_CHECK( esp_now_set_pmk((uint8_t *)CONFIG_ESPNOW_PMK) );

    /* Add broadcast peer information to peer list. */
    esp_now_peer_info_t *peer = malloc(sizeof(esp_now_peer_info_t));
    if (peer == NULL) {
        ESP_LOGE(TAG, "Malloc peer information fail");
        vSemaphoreDelete(s_app_espnow_queue);
        esp_now_deinit();
        return ESP_FAIL;
    }
    memset(peer, 0, sizeof(esp_now_peer_info_t));
    peer->channel = CONFIG_ESPNOW_CHANNEL;
    peer->ifidx = ESPNOW_WIFI_IF;
    peer->encrypt = false;
    
#if APP_ESPNOW_ENCYYPTION_ENABLE    
    for (uint8_t i = 0; i < CONFIG_ESPNOW_LMK_LEN; i++) {
        peer->lmk[i] = CONFIG_ESPNOW_LMK[i];
    }
    peer->encrypt = true;
#endif
    memcpy(peer->peer_addr, s_app_peer_mac, ESP_NOW_ETH_ALEN);
    ESP_ERROR_CHECK( esp_now_add_peer(peer) );
    free(peer);

    return ESP_OK;
}

/**
 * @brief initialize module tasks for espnow communication between peers
 *
 * @return  esp error code
 */
static esp_err_t app_espnow_tasks_init(void)
{
    xSemaphoreEspnowAck = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphoreEspnowAck);
    xSemaphoreTake(xSemaphoreEspnowAck, 20);

    xSemaphoreEspnowSend = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphoreEspnowSend);

#if DEVICE_WISER_USB
    s_app_espnow_queue = xQueueCreate(60, sizeof(app_espnow_event_t));
#else
    s_app_espnow_queue = xQueueCreate(9, sizeof(app_espnow_event_t));
#endif
    if (s_app_espnow_queue == NULL) {
        ESP_LOGE(TAG, "Create s_app_espnow_queue fail");
        return ESP_FAIL;
    }

    xTaskCreate(app_espnow_task, "app_espnow_task", 2048, NULL, 3, NULL);

#if DEVICE_WISER_UART
    vTaskDelay(1000);
    app_espnow_config_req_send();
#endif
    return ESP_OK;
}

/**
 * @brief resets the serial packet counts
 *
 */
static void app_espnow_ser_count_reset(void) {
    app_espnow_tx_ser_count = APP_ESPNOW_TX_SER_COUNT_DEFAULT;
    app_espnow_rx_ser_count = APP_ESPNOW_RX_SER_COUNT_DEFAULT;
}

/**
 * @brief send acknowledgement for last packet received over espnow
 *
 * @param  data_ack data acknowledgement packet
 */
static void app_espnow_data_ack_send(const data_ack_t data_ack)
{
    uint8_t *data_tosend = (uint8_t *)pvPortMalloc(sizeof(data_ack)+2);
    size_t len_tosend = sizeof(data_ack)+2;

    // prepare data
    data_tosend[0] = APP_ESPNOW_TYPE_ACK;
    data_tosend[1] = 0;
    memcpy(&data_tosend[2], &data_ack, sizeof(data_ack));

    if(xSemaphoreTake(xSemaphoreEspnowSend, portMAX_DELAY) == pdTRUE) {
        if (esp_now_send(s_app_peer_mac, data_tosend, len_tosend) != ESP_OK) {
            ESP_LOGE(TAG, "Send ack error");
        }
        xSemaphoreGive(xSemaphoreEspnowSend);
    }

    vPortFree(data_tosend);
}

/**
 * @brief sends data packet over espnow
 *
 * @param  data data packet bytes
 * @param len length of data bytes
 */
static void app_espnow_send(uint8_t *data, size_t len) {
    uint8_t retry_count = 3;
    if(len >= 2 && data[0] != APP_ESPNOW_TYPE_ACK) {
        if(data[0] == APP_ESPNOW_TYPE_DATA) {
#if DEVICE_WISER_USB
            led_tx_on();
#endif
        }

        esp_now_send_status = false;
        last_data_ack.type = data[0];
        last_data_ack.ser_count = data[1];
        
        do {
            if(xSemaphoreTake(xSemaphoreEspnowSend, portMAX_DELAY) == pdTRUE) {
                if (esp_now_send(s_app_peer_mac, data, len) != ESP_OK) {
                    xSemaphoreGive(xSemaphoreEspnowSend);
                    ESP_LOGE(TAG, "Send error");
#if DEVICE_WISER_USB
                        led_tx_off();
#endif
                } else {
                    xSemaphoreGive(xSemaphoreEspnowSend);
                    if(data[0] == APP_ESPNOW_TYPE_DATA) {
#if DEVICE_WISER_USB
                        led_tx_off();
#endif
                    }
                    if(data[0] == APP_ESPNOW_TYPE_CONFIG_SETTINGS) {
                        esp_now_send_status = true;
                    } else {
                        xSemaphoreTake(xSemaphoreEspnowAck, app_espnow_send_timeout);
                    }
                }
                retry_count--;
                if(esp_now_send_status != true) {
                    ESP_LOGE(TAG, "retry");
                }
            }
            taskYIELD();
        } while(esp_now_send_status == false && retry_count > 0);
    }
}

/**
 * @brief create small data chunk to send over espnow
 *
 * @param data data buffer
 * @param len total length of data
 */
static void app_espnow_data_send_chunks(const uint8_t *data, size_t len)
{
    uint8_t *data_tosend = (uint8_t *)pvPortMalloc(len+2);
    size_t len_tosend = 0;
    // prepare data        
    data_tosend[0] = APP_ESPNOW_TYPE_DATA;
    data_tosend[1] = app_espnow_tx_ser_count++;

    memcpy(&data_tosend[2], data, len);
    len_tosend = len+2;
    app_espnow_send(data_tosend, len_tosend);
    vPortFree(data_tosend);
}

/** @} */ // End of app_espnow_static_funcs group

/**
 * @addtogroup app_espnow_global_funcs
 * @{
 */

/**
 * @brief sends serial data over espnow
 *
 * @param  data data packet bytes
 * @param len length of data bytes
 */
void app_espnow_data_send(const uint8_t *data, size_t len)
{
    size_t tx_len = 0;
    while(len != tx_len) {
        if((len - tx_len) > APP_ESPNOW_SEND_DATA_SIZE) {
            app_espnow_data_send_chunks(&data[tx_len], APP_ESPNOW_SEND_DATA_SIZE);
            tx_len = tx_len + APP_ESPNOW_SEND_DATA_SIZE;
        } else {
            app_espnow_data_send_chunks(&data[tx_len], len - tx_len);                
            tx_len = tx_len + (len - tx_len);
        }
    }
}

/**
 * @brief sends serial config settings to peer over espnow
 * @param config_settings config settings to be sent
 */
void app_espnow_config_settings_send(const config_settings_t config_settings)
{
    uint8_t *data_tosend = (uint8_t *)pvPortMalloc(sizeof(config_settings)+2);
    size_t len_tosend = sizeof(config_settings)+2;

    // prepare data
    data_tosend[0] = APP_ESPNOW_TYPE_CONFIG_SETTINGS;
    data_tosend[1] = app_espnow_tx_ser_count;
    memcpy(&data_tosend[2], &config_settings, sizeof(config_settings));

    app_espnow_send(data_tosend, len_tosend);
    vPortFree(data_tosend);
}

/**
 * @brief sends serial hw line state to peer over espnow
 * @param config_hw_line config hw line state to be sent
 */
void app_espnow_config_hw_line_send(const config_hw_line_t config_hw_line)
{
    uint8_t *data_tosend = (uint8_t *)pvPortMalloc(sizeof(config_hw_line)+2);
    size_t len_tosend = sizeof(config_hw_line)+2;

    // prepare data
    data_tosend[0] = APP_ESPNOW_TYPE_CONFIG_HW_LINE;
    data_tosend[1] = app_espnow_tx_ser_count;
    memcpy(&data_tosend[2], &config_hw_line, sizeof(config_hw_line));

    app_espnow_send(data_tosend, len_tosend);
    vPortFree(data_tosend);
}

/**
 * @brief sends connection indication event to peer over espnow
 * @param device_conn connection indication parameters
 */
void app_espnow_device_conn_send(const device_conn_t device_conn)
{
    uint8_t *data_tosend = (uint8_t *)pvPortMalloc(sizeof(device_conn)+2);
    size_t len_tosend = sizeof(device_conn)+2;

    // prepare data
    data_tosend[0] = APP_ESPNOW_TYPE_DEVICE_CONN;
    data_tosend[1] = app_espnow_tx_ser_count;
    memcpy(&data_tosend[2], &device_conn, sizeof(device_conn));

    app_espnow_send(data_tosend, len_tosend);
    vPortFree(data_tosend);
}

/**
 * @brief sends serial configuration request to peer over espnow
 * 
 */
void app_espnow_config_req_send(void)
{
    uint8_t *data_tosend = (uint8_t *)pvPortMalloc(2);
    size_t len_tosend = 2;

    // prepare data
    data_tosend[0] = APP_ESPNOW_TYPE_CONFIG_REQ;
    data_tosend[1] = app_espnow_tx_ser_count;

    app_espnow_send(data_tosend, len_tosend);
    vPortFree(data_tosend);
}

/**
 * @brief initialize module espnow for communication between peers
 *
 */
void app_espnow_init(void)
{
    // Initialize NVS
    nvs_peer_init();
#if APP_ESPNOW_USE_NVS_PEER_MAC
    nvs_peer_read(s_app_peer_mac);
    nvs_peer_deinit();
#endif
    ESP_LOGE(TAG, "peer mac address - %02x:%02x:%02x:%02x:%02x:%02x", s_app_peer_mac[0], s_app_peer_mac[1],s_app_peer_mac[2],s_app_peer_mac[3],s_app_peer_mac[4],s_app_peer_mac[5]);

    app_espnow_wifi_init();
    app_espnow_ll_init();
    app_espnow_tasks_init();
}

/**
 * @brief deinitialize module espnow
 *
 */
void app_espnow_deinit(void)
{
    vSemaphoreDelete(xSemaphoreEspnowAck);
    vSemaphoreDelete(xSemaphoreEspnowSend);
    vQueueDelete(s_app_espnow_queue);
    esp_now_deinit();
}

/** @} */ // End of app_espnow_global_funcs group

/** @} */ // End of app_espnow group

/** @} */ // End of app_espnow module