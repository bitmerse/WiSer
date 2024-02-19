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
 * @file nvs_peer.h
 * @author Dhrumil Doshi
 * @date 25 January 2024
 * @brief nvs_peer module header
 */

#ifndef NVS_PEER_H
#define NVS_PEER_H

/**
 * @defgroup nvs_peer NVS memory control Module
 * @brief Module for accessing NVS memory
 * @{
 */

/**
 * @addtogroup nvs_peer_include
 * @{
 */

/** @} */ // End of nvs_peer_include group


/**
 * @addtogroup nvs_peer_define
 * @{
 */

/** @} */ // End of nvs_peer_define group

/**
 * @addtogroup nvs_peer_types
 * @{
 */

/** @} */ // End of nvs_peer_types group

/**
 * @addtogroup nvs_peer_global_funcs
 * @{
 */

/**
 * @brief initialize nvs_peer module
 *
 */
void nvs_peer_init();

/**
 * @brief deinitialize nvs_peer module
 *
 */
void nvs_peer_deinit();

/**
 * @brief opens nvs memory storage handle
 *
 */
void nvs_peer_open();

/**
 * @brief closes nvs memory storage handle
 *
 */
void nvs_peer_close();

/**
 * @brief reads peer MAC address from NVS memory
 *
 * @param mac pointer to peer mac address
 */
void nvs_peer_read(uint8_t *mac);

/**
 * @brief writes peer MAC address to NVS memory
 *
 * @param mac pointer to peer mac address
 */
void nvs_peer_write(uint8_t *mac);

/** @} */ // End of nvs_peer_global_funcs group
/** @} */ // End of nvs_peer group
#endif

