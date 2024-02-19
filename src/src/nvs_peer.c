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
 * @file nvs_peer.c
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief NVS memory module which allows to store and read back peer address in non-volatile memory
 */


/**
 * @defgroup nvs_peer NVS memory control Module
 * @brief Module for accessing NVS memory
 * @{
 */


/**
 * @addtogroup nvs_peer_include
 * @{
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "nvs_peer.h"

/** @} */ // End of nvs_peer_include group

/**
 * @addtogroup nvs_peer_define
 * @{
 */
#define NVS_PEER_MAC_ADDR_LEN   6
/** @} */ // End of nvs_peer_define group


/**
 * @addtogroup nvs_peer_static_vars
 * @{
 */

/** @} */ // End of nvs_peer_static_vars group


/**
 * @addtogroup nvs_peer_global_vars
 * @{
 */

nvs_handle_t my_handle;

/** @} */ // End of nvs_peer_global_vars group


/**
 * @addtogroup nvs_peer_static_funcs
 * @{
 */


/** @} */ // End of nvs_peer_static_funcs group


/**
 * @addtogroup nvs_peer_global_funcs
 * @{
 */

/**
 * @brief initialize nvs_peer module
 *
 */
void nvs_peer_init()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("NVS flash init failed");
    }
    ESP_ERROR_CHECK( err );
}

/**
 * @brief deinitialize nvs_peer module
 *
 */
void nvs_peer_deinit()
{
    nvs_peer_close();
}


/**
 * @brief reads peer MAC address from NVS memory
 *
 * @param mac pointer to peer mac address
 */
void nvs_peer_read(uint8_t *mac)
{
    esp_err_t err;
    size_t len=NVS_PEER_MAC_ADDR_LEN;
    if(my_handle != NULL) {
        // Read
        int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS
        err = nvs_get_blob(my_handle, "peer_mac", mac, &len);        
        switch (err) {
            case ESP_OK:
                printf("Done\n");
                printf("peer mac = %s \n", mac);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The value is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading!\n", esp_err_to_name(err));
        }
    }
}

/**
 * @brief opens nvs memory storage handle
 *
 */
void nvs_peer_open()
{
    // Open
    esp_err_t err;
     // Open
    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    err = nvs_open("peer_info", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        my_handle = NULL;
    } else {
        printf("Done\n");
    }
}

/**
 * @brief closes nvs memory storage handle
 *
 */
void nvs_peer_close()
{
    // Close
    esp_err_t err;
    if(my_handle != NULL) {
        nvs_close(my_handle);
        err = nvs_commit(my_handle);
    }
}

/**
 * @brief writes peer MAC address to NVS memory
 *
 * @param mac pointer to peer mac address
 */
void nvs_peer_write(uint8_t *mac)
{
     // Write
    if(my_handle != NULL) {
        printf("Updating peer mac address in NVS...");
        esp_err_t err = nvs_set_blob(my_handle, "peer_mac", mac, NVS_PEER_MAC_ADDR_LEN);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        printf("Committing updates in NVS ... ");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");
    }
}

/** @} */ // End of nvs_peer_global_funcs group

/** @} */ // End of nvs_peer module