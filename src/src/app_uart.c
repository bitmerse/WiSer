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
 * @file app_uart.c
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief UART communication implementation
 */

/**
 * @defgroup app_uart Application uart communication Module
 * @brief Module for managing application uart communication
 * @{
 */

/**
 * @addtogroup app_uart_include
 * @{
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_intr_alloc.h"
#if CONFIG_IDF_TARGET_ESP32
    #include "esp32/rom/uart.h"
#elif CONFIG_IDF_TARGET_ESP32S2
    #include "esp32s2/rom/uart.h"
#endif
#include "config.h"
#include "commons.h"
#include "app_espnow.h"
#include "app_conn.h"
#include "app_uart.h"

/** @} */ // End of app_uart_include group
#if DEVICE_WISER_UART
/**
 * @addtogroup app_uart_define
 * @{
 */
/** @brief UART number */
#define APP_UART_NUM    UART_NUM_1
/** @brief UART baud rate */
#define APP_UART_BAUD_RATE  9600

// WiSer-UART
#define APP_UART_GPIO_RX  GPIO_NUM_37
#define APP_UART_GPIO_CTS GPIO_NUM_35
#define APP_UART_GPIO_TX  GPIO_NUM_38
#define APP_UART_GPIO_RTS GPIO_NUM_36
#define APP_UART_GPIO_DTR GPIO_NUM_34

/** @brief GPIO logic level to set */
#define APP_UART_GPIO_SET     1
/** @brief GPIO logic level to reset */
#define APP_UART_GPIO_RESET   0

/** @brief UART test configuration */
#define ECHO_TEST_TXD (CONFIG_EXAMPLE_UART_TXD)
#define ECHO_TEST_RXD (CONFIG_EXAMPLE_UART_RXD)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

/** @brief UART buffer size */
#define BUF_SIZE (2048)

/** @} */ // End of app_uart_define group


/**
 * @addtogroup app_uart_static_vars
 * @{
 */
/** @brief Tag for logging */
static const char *TAG = "app_uart";

/** @brief Queue handle for UART */
static QueueHandle_t uart1_queue;

/** @} */ // End of app_uart_static_vars group

/**
 * @addtogroup app_uart_global_vars
 * @{
 */
uint8_t app_uart_hw_flow_status = APP_UART_HW_FLOW_DISABLE;

size_t total_uart_intr_received_len = 0;
size_t total_espnow_sent_len = 0;

/** @brief UART configuration structure */
uart_config_t uart_config = {
        .baud_rate = APP_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

/** @} */ // End of app_uart_global_vars group

/**
 * @addtogroup app_uart_static_funcs
 * @{
 */

/** @brief Initialize UART low-level configuration */
static void app_uart_ll_init(void);

/**
 * @brief Initialize uart pins and enable RTS/CTS hardware flow control.
 * 
 */
static void app_uart_hw_flow_enable(void);

/**
 * @brief Initialize uart pins and disable RTS/CTS hardware flow control.
 * 
 */
static void app_uart_hw_flow_disable(void);

/** @brief Initialize DTR pin for UART communication */
static void app_uart_dtr_init(void);

/** @brief Task to handle UART events */
static void app_uart_event_task(void *pvParameters);

/**
 * @brief initialize app UART tasks.
 * 
 * @return error code
 */
static int app_uart_tasks_init(void);

/** @} */ // End of app_espnow_static_funcs group

/**
 * @addtogroup app_uart_static_funcs
 * @{
 */

/**
 * @brief Initialize UART.
 * 
 */
static void app_uart_ll_init(void)
{
    int intr_alloc_flags = 0;

    app_espnow_send_timeout = (uint32_t)((APP_ESPNOW_SEND_DATA_SIZE*8*1000)/uart_config.baud_rate) + (uint32_t)(1000/uart_config.baud_rate) + 20;
    ESP_LOGE(TAG, "app_espnow_send_timeout: %lu", app_espnow_send_timeout);
    gpio_pullup_dis(APP_UART_GPIO_CTS);

    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
    
    ESP_ERROR_CHECK(uart_driver_install(APP_UART_NUM, BUF_SIZE*8, BUF_SIZE, 200, &uart1_queue, intr_alloc_flags));
    uart_config.rx_flow_ctrl_thresh = UART_FIFO_LEN - 1;
    ESP_ERROR_CHECK(uart_param_config(APP_UART_NUM, &uart_config));
    
    app_uart_hw_flow_disable();
}


/**
 * @brief Initialize uart pins and enable RTS/CTS hardware flow control.
 * 
 */
static void app_uart_hw_flow_enable(void) 
{
    //for uart with flow control
    ESP_ERROR_CHECK(uart_set_pin(APP_UART_NUM, APP_UART_GPIO_TX, APP_UART_GPIO_RX, APP_UART_GPIO_RTS, APP_UART_GPIO_CTS));

    gpio_pullup_en(APP_UART_GPIO_RTS);
    gpio_set_pull_mode(APP_UART_GPIO_RTS, GPIO_PULLUP_ONLY);
    gpio_pullup_en(APP_UART_GPIO_CTS);
    gpio_set_pull_mode(APP_UART_GPIO_CTS, GPIO_PULLUP_ONLY);
}

/**
 * @brief Initialize uart pins and disable RTS/CTS hardware flow control.
 * 
 */
static void app_uart_hw_flow_disable(void) 
{
    //for uart with flow control
    ESP_ERROR_CHECK(uart_set_pin(APP_UART_NUM, APP_UART_GPIO_TX, APP_UART_GPIO_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    gpio_pullup_en(APP_UART_GPIO_RTS);
    gpio_set_direction(APP_UART_GPIO_RTS, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(APP_UART_GPIO_RTS, GPIO_PULLUP_ONLY);
    gpio_set_level(APP_UART_GPIO_RTS, 1);
    gpio_pullup_en(APP_UART_GPIO_CTS);
    gpio_set_direction(APP_UART_GPIO_CTS, GPIO_MODE_INPUT);
    gpio_set_pull_mode(APP_UART_GPIO_CTS, GPIO_PULLUP_ONLY);
}

/**
 * @brief Initialize DTR pin.
 * 
 */
static void app_uart_dtr_init(void) 
{
    gpio_set_direction(APP_UART_GPIO_DTR, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(APP_UART_GPIO_DTR, GPIO_PULLUP_ONLY);
    app_uart_dtr_set(0);
}

/**
 * @brief Task which receives data on UART interrupt.
 * @param pvParameters task parameter
 * 
 */
static void IRAM_ATTR app_uart_event_task(void *pvParameters)
{
    static DRAM_ATTR app_uart_evt_data_t evt;
    static DRAM_ATTR uart_event_t event;
    size_t buffered_size;
    uint8_t *dtmp = (uint8_t *) pvPortMalloc(BUF_SIZE*20);
    while (1) {
        /* Waiting for UART event.
           If it happens then print out information what is it */
        if (xQueueReceive(uart1_queue, (void * )&event, portMAX_DELAY)) {
            // ESP_LOGI(TAG, "uart[%d] event:", APP_UART_NUM);
            switch (event.type) {
            case UART_DATA:
                /* Event of UART receiving data
                 * We'd better handler data event fast, there would be much more data events
                 * than other types of events.
                 * If we take too much time on data event, the queue might be full.
                 * In this example, we don't process data in event, but read data outside.
                 */
                uart_get_buffered_data_len(APP_UART_NUM, &buffered_size);
                // ESP_LOGI(TAG, "data, len: %d; buffered len: %d", event.size, buffered_size);
                evt.type = APP_UART_TYPE_DATA;
                evt.len = buffered_size;
                if (evt.len > 0) {
                    const size_t rx_bytes = uart_read_bytes(APP_UART_NUM, dtmp, evt.len, 1000);
                    app_espnow_data_send(dtmp, rx_bytes);
                }
                break;
            case UART_FIFO_OVF:
                ESP_LOGE(TAG, "hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // We can read data out out the buffer, or directly flush the Rx buffer.
                uart_flush(APP_UART_NUM);
                break;
            case UART_BUFFER_FULL:
                ESP_LOGE(TAG, "ring buffer full");
                // If buffer full happened, you should consider increasing your buffer size
                // We can read data out out the buffer, or directly flush the Rx buffer.
                uart_flush(APP_UART_NUM);
                break;
            case UART_BREAK:
                ESP_LOGI(TAG, "uart rx break detected");
                break;
            case UART_PARITY_ERR:
                ESP_LOGE(TAG, "uart parity error");
                break;
            case UART_FRAME_ERR:
                ESP_LOGE(TAG, "uart frame error");
                break;
            default:
                ESP_LOGE(TAG, "not serviced uart event type: %d\n", event.type);
                break;
            }
        }
    }
    vPortFree(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

/**
 * @brief initialize app UART tasks.
 * 
 * @return error code
 */
static int app_uart_tasks_init(void)
{
    // Create a task to handle uart event from ISR
    xTaskCreate(app_uart_event_task, "app_uart_event_task", 14096, NULL, 3, NULL);

    return ESP_OK;

}

/** @} */ // End of app_uart_static_funcs group

/**
 * @addtogroup app_uart_global_funcs
 * @{
 */

/**
 * @brief initialize app UART interface.
 * 
 */
int app_uart_init(void)
{
    app_uart_dtr_init();
    app_uart_ll_init();
    if(app_uart_tasks_init() != ESP_OK) {
        return ESP_FAIL;
    }
    return ESP_OK;

}

/**
 * @brief reconfigures UART configuration.
 * @param config_settings UART configuration settings
 * 
 */
void app_uart_config_reset(config_settings_t config_settings)
{
    uart_config.baud_rate = config_settings.bitrate;
    switch(config_settings.data_bits) {
        case 5:
        {
            uart_config.data_bits = UART_DATA_5_BITS;
        } break;
        case 6:
        {
            uart_config.data_bits = UART_DATA_6_BITS;
        } break;
        case 7:
        {
            uart_config.data_bits = UART_DATA_7_BITS;
        } break;
        case 8:
        {
            uart_config.data_bits = UART_DATA_8_BITS;
        } break;
        case 16:
        {
            uart_config.data_bits = UART_DATA_BITS_MAX;
        } break;
        default:
        {
            uart_config.data_bits = UART_DATA_8_BITS;
        } break;
    }

    switch(config_settings.parity) {
        case 0:
        {
            uart_config.parity = UART_PARITY_DISABLE;
        } break;
        case 1:
        {
            uart_config.parity = UART_PARITY_ODD;
        } break;
        case 2:
        {
            uart_config.parity = UART_PARITY_EVEN;
        } break;
        default:
        {
            uart_config.parity = UART_PARITY_DISABLE;
        } break;
    }

    switch(config_settings.stop_bits) {
        case 0:
        {
            uart_config.stop_bits = UART_STOP_BITS_1;
        } break;
        case 1:
        {
            uart_config.stop_bits = UART_STOP_BITS_1_5;
        } break;
        case 2:
        {
            uart_config.stop_bits = UART_STOP_BITS_2;
        } break;
        default:
        {
            uart_config.stop_bits = UART_STOP_BITS_1;
        } break;
    }
    if(config_settings.hw_flow_status == APP_UART_HW_FLOW_ENABLE) {
        uart_config.flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS;
    } else {
        uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    }

    app_espnow_send_timeout = (uint32_t)((APP_ESPNOW_SEND_DATA_SIZE*8*1000)/uart_config.baud_rate) + (uint32_t)(1000/uart_config.baud_rate) + 20;
    ESP_LOGE(TAG, "app_espnow_send_timeout: %lu", app_espnow_send_timeout);
    ESP_ERROR_CHECK(uart_param_config(APP_UART_NUM, &uart_config));

    if(app_uart_hw_flow_status != config_settings.hw_flow_status) {
        app_uart_hw_flow_status = config_settings.hw_flow_status;
        if(app_uart_hw_flow_status == APP_UART_HW_FLOW_ENABLE) {
            app_uart_hw_flow_enable();
            app_conn_on(HW_FLOW_EN_CONN_ON_PERIOD, HW_FLOW_EN_CONN_OFF_PERIOD, HW_FLOW_EN_APP_CONN_ON_COUNT);
        } else {
            app_uart_hw_flow_disable();
            app_conn_on(HW_FLOW_DIS_CONN_ON_PERIOD, HW_FLOW_DIS_CONN_OFF_PERIOD, HW_FLOW_DIS_APP_CONN_ON_COUNT);
        }
    }

}

/**
 * @brief write to device over UART
 * @param tx_buf transmit buffer
 * @param tx_size buffer size
 * 
 */
void app_uart_write(const uint8_t *tx_buf, size_t tx_size)
{
    /* write */
    size_t tx_len = tx_size;
    do {
        int tx_bytes = uart_write_bytes(APP_UART_NUM, &tx_buf[tx_size-tx_len], tx_len);
        if(tx_bytes != -1) {
            tx_len = tx_len - tx_bytes;
        }
        if(tx_len != 0) {
            taskYIELD();
        }
    } while(0 != tx_len);
    taskYIELD();
}

/**
 * @brief set DTR level
 * @param dtr_state DTR level
 * 
 */
void app_uart_dtr_set(uint8_t dtr_state) 
{
    static uint8_t dtr_state_last = 2;
    if(dtr_state_last != dtr_state) {
        dtr_state_last = dtr_state;
        if(dtr_state) {
            gpio_set_level(APP_UART_GPIO_DTR, APP_UART_GPIO_RESET);
        } else {
            gpio_set_level(APP_UART_GPIO_DTR, APP_UART_GPIO_SET);
        }
    }
}

/**
 * @brief set RTS level
 * @param rts_state RTS level
 * 
 */
void app_uart_rts_set(uint8_t rts_state) 
{
    static uint8_t rts_state_last = 2;
    if(rts_state_last != rts_state) {
        rts_state_last = rts_state;
        if(app_uart_hw_flow_status == APP_UART_HW_FLOW_DISABLE) {
            if(rts_state) {
                gpio_set_level(APP_UART_GPIO_RTS, APP_UART_GPIO_RESET);
            } else {
                gpio_set_level(APP_UART_GPIO_RTS, APP_UART_GPIO_SET);
            }
        }
    }
}

/** @} */ // End of app_uart_global_funcs group

/** @} */ // End of app_uart group
#endif
/** @} */ // End of app_uart module