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
 * @file app_tusb.c
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief Application tinyUSB (USBCDC) communication module which communicates with USB host device
 */

/**
 * @defgroup app_tusb Application usbcdc device Module
 * @brief Module for managing application USB communication
 * @{
 */

/**
 * @addtogroup app_tusb_include
 * @{
 */

#include <stdint.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#if CONFIG_IDF_TARGET_ESP32S2
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#endif
#include "sdkconfig.h"
#include "config.h"
#include "commons.h"
#include "led.h"
#include "app_espnow.h"
#include "app_conn.h"
#include "app_tusb.h"

/** @} */ // End of app_tusb_include group

/**
 * @addtogroup app_tusb_define
 * @{
 */
#if DEVICE_WISER_USB

#define APP_TUSB_CDC_RX_BUFSIZE CONFIG_TINYUSB_CDC_RX_BUFSIZE
#define APP_TUSB_CDC_TX_BUFSIZE CONFIG_TINYUSB_CDC_TX_BUFSIZE
#define APP_TUSB_CDC_DEFAULT_BITRATE    9600

/** @} */ // End of app_tusb_define group

/**
 * @addtogroup app_tusb_static_vars
 * @{
 */
static const char *TAG = "app_tusb";
static config_settings_t s_app_config_settings = {
    .bitrate = APP_TUSB_CDC_DEFAULT_BITRATE,
    .data_bits = 8,
    .parity = 0,
    .stop_bits = 0,
    .hw_flow_status = APP_TUSB_HW_FLOW_DISABLE
};

static QueueHandle_t s_app_tusb_config_queue;

static int last_dtr = APP_TUSB_HW_FLOW_LINE_STATE_UNKNOWN;
static int last_rts = APP_TUSB_HW_FLOW_LINE_STATE_UNKNOWN;

/** @} */ // End of app_tusb_static_vars group

/**
 * @addtogroup app_tusb_global_vars
 * @{
 */
size_t usb_rx_size = 0;
uint32_t bit_rate_last=APP_TUSB_CDC_DEFAULT_BITRATE;
uint8_t app_tusb_hw_flow_status = APP_TUSB_HW_FLOW_DISABLE;

SemaphoreHandle_t xSemaphoreUsbRead = NULL;
uint8_t evt_rx = APP_TUSB_TYPE_DATA;
app_tusb_data_t evt;
/** @} */ // End of app_tusb_global_vars group

/**
 * @addtogroup app_tusb_static_funcs
 * @{
 */
static void app_tusb_cdc_rx_callback(int itf, cdcacm_event_t *event);
static void app_tusb_cdc_line_state_changed_callback(int itf, cdcacm_event_t *event);
static void app_tusb_cdc_line_coding_changed_callback(int itf, cdcacm_event_t *event);
static void app_tusb_data_read_task(void *pvParameter);
static void app_tusb_read();
static void app_tusb_config_hw_line_update(void);
/** @} */ // End of app_tusb_static_funcs group

/**
 * @addtogroup app_tusb_static_funcs
 * @{
 */

/**
 * @brief tinyusb receive callback function
 * @param itf tinyusb instance
 * @param event receive callback event info
 * 
 */
static void IRAM_ATTR app_tusb_cdc_rx_callback(int itf, cdcacm_event_t *event)
{
    xSemaphoreGive(xSemaphoreUsbRead);
}

/**
 * @brief tinyusb rts/dtr line state callback function
 * @param itf tinyusb instance
 * @param event rts/dtr state callback event info
 * 
 */
static void app_tusb_cdc_line_state_changed_callback(int itf, cdcacm_event_t *event)
{
    ESP_LOGE(TAG, "DTR: %d, RTS: %d", event->line_state_changed_data.dtr, event->line_state_changed_data.rts);
    app_tusb_evt_config_t evt;

    int dtr = event->line_state_changed_data.dtr;
    int rts = event->line_state_changed_data.rts;
    
    if(last_dtr != dtr || last_rts != rts) {
        last_dtr = dtr;
        evt.config_hw_line.dtr = dtr;

        last_rts = rts;
        evt.config_hw_line.rts = rts;
        evt.type = APP_TUSB_TYPE_DTR_RTS;

        if (xQueueSend(s_app_tusb_config_queue, &evt, 0) != pdTRUE) {
            ESP_LOGW(TAG, "Send dtr queue fail");
        }
    }
}

/**
 * @brief tinyusb configuration callback function
 * @param itf tinyusb instance
 * @param event tinyusb configuration callback event info
 * 
 */
static void app_tusb_cdc_line_coding_changed_callback(int itf, cdcacm_event_t *event)
{
    app_tusb_evt_config_t evt;
    uint32_t bit_rate = event->line_coding_changed_data.p_line_coding->bit_rate;
    bit_rate_last = bit_rate;
    ESP_LOGE(TAG, "Line state changed on channel %d: bit_rate:%lu", itf, bit_rate);

    evt.type = APP_TUSB_TYPE_CONFIG;
    evt.config_settings.bitrate = bit_rate;
    evt.config_settings.data_bits = event->line_coding_changed_data.p_line_coding->data_bits;
    evt.config_settings.parity = event->line_coding_changed_data.p_line_coding->parity;
    evt.config_settings.stop_bits = event->line_coding_changed_data.p_line_coding->stop_bits;
    evt.config_settings.hw_flow_status = app_tusb_hw_flow_status;
    // evt.config_settings.hw_flow_status = APP_TUSB_HW_FLOW_DISABLE;

    if (xQueueSend(s_app_tusb_config_queue, &evt, 0) != pdTRUE) {
        ESP_LOGW(TAG, "Send receive queue fail");
    }
}

/**
 * @brief receives data over usb cdc and sends data over espnow
 * 
 */
static void IRAM_ATTR app_tusb_read(void)
{
    size_t rx_size = 0;
    uint8_t *buf = pvPortMalloc(APP_TUSB_CDC_RX_BUFSIZE);
    /* read */
    esp_err_t ret = tinyusb_cdcacm_read(TINYUSB_CDC_ACM_0, buf, CONFIG_TINYUSB_CDC_RX_BUFSIZE, &rx_size);
    if (ret == ESP_OK && rx_size != 0) {
        usb_rx_size = usb_rx_size + rx_size;
        // ESP_LOGI(TAG, "Data from channel %d:", TINYUSB_CDC_ACM_0);
        // ESP_LOG_BUFFER_HEXDUMP(TAG, buf, rx_size, ESP_LOG_INFO);
        // ESP_LOGE(TAG, "%u, %u", rx_size, usb_rx_size);

        evt.type = APP_TUSB_TYPE_DATA;
        evt.data = buf;
        evt.len = rx_size;  

        app_espnow_data_send(buf, rx_size);
    } else {
        ESP_LOGI(TAG, "Read error");
    }
    vPortFree(buf);
}

/**
 * @brief task which receives data over usb cdc
 * @param pvParameter task arguments
 * 
 */
static void IRAM_ATTR app_tusb_data_read_task(void *pvParameter)
{
    uint8_t evt = APP_TUSB_TYPE_DATA;
    while(xSemaphoreTake(xSemaphoreUsbRead, portMAX_DELAY)) {    
        switch(evt) {
            case APP_TUSB_TYPE_DATA: {
                app_tusb_read();
            } break;
            default: {

            } break;
        }
        taskYIELD();
    }
}

/**
 * @brief task which receives configuration data over usb cdc
 * @param pvParameter task arguments
 * 
 */
static void app_tusb_config_task(void *pvParameter)
{
    app_tusb_evt_config_t evt;
    while (xQueueReceive(s_app_tusb_config_queue, &evt, portMAX_DELAY) == pdTRUE) {
        switch(evt.type) {
            case APP_TUSB_TYPE_CONFIG: {
                memcpy(&s_app_config_settings, &evt.config_settings, sizeof(s_app_config_settings));
                app_espnow_send_timeout = (uint32_t)((APP_ESPNOW_SEND_DATA_SIZE*8*1000)/s_app_config_settings.bitrate) + (uint32_t)(1000/s_app_config_settings.bitrate) + 20;
                ESP_LOGE(TAG, "app_espnow_send_timeout: %lu", app_espnow_send_timeout);
                app_espnow_config_settings_send(evt.config_settings);
            } break;
            case APP_TUSB_TYPE_DTR_RTS: {
                app_espnow_config_hw_line_send(evt.config_hw_line);
            } break;
            default: {

            } break;
        }
        
    }
}

/**
 * @brief sends updated state of hardware lines over espnow
 * 
 */
static void app_tusb_config_hw_line_update(void)
{
    config_hw_line_t config_hw_line;
    if(last_dtr != APP_TUSB_HW_FLOW_LINE_STATE_UNKNOWN && last_rts != APP_TUSB_HW_FLOW_LINE_STATE_UNKNOWN) {
        config_hw_line.dtr = last_dtr;
        config_hw_line.rts = last_rts;

        app_espnow_config_hw_line_send(config_hw_line);
    }
}

/** @} */ // End of app_tusb_static_funcs group

/**
 * @addtogroup app_tusb_global_funcs
 * @{
 */

/**
 * @brief initialize the usb cdc devices
 * 
 */
int app_tusb_init(void)
{
    ESP_LOGI(TAG, "USB initialization");

    xSemaphoreUsbRead = xSemaphoreCreateCounting(80, 0);
    xSemaphoreGive(xSemaphoreUsbRead);

    s_app_tusb_config_queue = xQueueCreate(APP_ESPNOW_QUEUE_SIZE, sizeof(app_tusb_evt_config_t));
    if (s_app_tusb_config_queue == NULL) {
        ESP_LOGE(TAG, "Create mutex fail");
        return ESP_FAIL;
    }

    xTaskCreate(app_tusb_data_read_task, "app_tusb_read_task", 4096, NULL, 3, NULL);
    xTaskCreate(app_tusb_config_task, "app_tusb_config_task", 2048, NULL, 6, NULL);

    const tinyusb_config_t tusb_cfg = {
        .string_descriptor = NULL,
        .external_phy = false,
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    tinyusb_config_cdcacm_t acm_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = APP_TUSB_CDC_RX_BUFSIZE,
        .callback_rx = &app_tusb_cdc_rx_callback, // the first way to register a callback
        .callback_rx_wanted_char = NULL,
        .callback_line_state_changed = NULL,
        .callback_line_coding_changed = &app_tusb_cdc_line_coding_changed_callback   
    };

    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));
    /* the second way to register a callback */
    ESP_ERROR_CHECK(tinyusb_cdcacm_register_callback(
                        TINYUSB_CDC_ACM_0,
                        CDC_EVENT_LINE_STATE_CHANGED,
                        &app_tusb_cdc_line_state_changed_callback));

#if (CONFIG_TINYUSB_CDC_COUNT > 1)
    acm_cfg.cdc_port = TINYUSB_CDC_ACM_1;
    ESP_ERROR_CHECK(tusb_cdc_acm_init(&acm_cfg));
    ESP_ERROR_CHECK(tinyusb_cdcacm_register_callback(
                        TINYUSB_CDC_ACM_1,
                        CDC_EVENT_LINE_STATE_CHANGED,
                        &tinyusb_cdc_line_state_changed_callback));
#endif

    ESP_LOGI(TAG, "USB initialization DONE");

    return ESP_OK;
}

/**
 * @brief write to device over usb cdc
 * @param tx_buf transmit buffer
 * @param tx_size buffer size
 * 
 */
void IRAM_ATTR app_tusb_write(const uint8_t *tx_buf, size_t tx_size)
{
    /* write */
    size_t tx_len = tx_size;
    do {
        int tx_bytes = tinyusb_cdcacm_write_queue(TINYUSB_CDC_ACM_0, &tx_buf[tx_size-tx_len], tx_len);
        tinyusb_cdcacm_write_flush(TINYUSB_CDC_ACM_0, 0);
        tx_len = tx_len - tx_bytes;
        if(tx_len != 0) {
            taskYIELD();
        }
    } while(0 != tx_len);
}

/**
 * @brief sends serial connection configuration over espnow
 * 
 */
void app_tusb_config_request (void) {
    app_espnow_config_settings_send(s_app_config_settings);
}

/**
 * @brief sends event of enabling hardware flow control over espnow
 * 
 */
void app_tusb_config_hw_flow_enable (void) {
    app_tusb_hw_flow_status = APP_TUSB_HW_FLOW_ENABLE;
    s_app_config_settings.hw_flow_status = app_tusb_hw_flow_status;
    app_espnow_config_settings_send(s_app_config_settings);
}

/**
 * @brief sends event of disabling hardware flow control over espnow
 * 
 */
void app_tusb_config_hw_flow_disble (void) {
    app_tusb_hw_flow_status = APP_TUSB_HW_FLOW_DISABLE;
    s_app_config_settings.hw_flow_status = app_tusb_hw_flow_status;
    app_espnow_config_settings_send(s_app_config_settings);
}

/**
 * @brief toggles the hardware flow control state
 * 
 */
void app_tusb_config_hw_flow_toggle (void) {
    if(app_tusb_hw_flow_status == APP_TUSB_HW_FLOW_ENABLE) {
        app_tusb_config_hw_flow_disble();
        app_conn_on(HW_FLOW_DIS_CONN_ON_PERIOD, HW_FLOW_DIS_CONN_OFF_PERIOD, HW_FLOW_DIS_APP_CONN_ON_COUNT);
        app_tusb_config_hw_line_update();
    } else {
        app_tusb_config_hw_flow_enable();
        app_conn_on(HW_FLOW_EN_CONN_ON_PERIOD, HW_FLOW_EN_CONN_OFF_PERIOD, HW_FLOW_EN_APP_CONN_ON_COUNT);
    }
}

#endif

/** @} */ // End of app_tusb_global_funcs group

/** @} */ // End of app_tusb group

/** @} */ // End of app_tusb module