/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_mifare_ultralight.h
 * @brief     driver mifare_ultralight header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-06-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/06/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_MIFARE_ULTRALIGHT_H
#define DRIVER_MIFARE_ULTRALIGHT_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup mifare_ultralight_driver mifare ultralight driver function
 * @brief    mifare ultralight driver modules
 * @{
 */

/**
 * @addtogroup mifare_ultralight_basic_driver
 * @{
 */

/**
 * @brief mifare ultralight bool enumeration definition
 */
typedef enum
{
    MIFARE_ULTRALIGHT_BOOL_FALSE = 0x00,        /**< false */
    MIFARE_ULTRALIGHT_BOOL_TRUE  = 0x01,        /**< true */
} mifare_ultralight_bool_t;

/**
 * @brief mifare ultralight type enumeration definition
 */
typedef enum
{
    MIFARE_ULTRALIGHT_TYPE_INVALID    = 0x00,     /**< invalid */
    MIFARE_ULTRALIGHT_TYPE_ULTRALIGHT = 0x01,     /**< ultralight */
} mifare_ultralight_type_t;

/**
 * @brief mifare ultralight storage enumeration definition
 */
typedef enum
{
    MIFARE_ULTRALIGHT_STORAGE_MF0UL11 = 0x13,     /**< 20 pages */
    MIFARE_ULTRALIGHT_STORAGE_MF0UL21 = 0x28,     /**< 41 pages */
} mifare_ultralight_storage_t;

/**
 * @brief mifare_ultralight access enumeration definition
 */
typedef enum
{
    MIFARE_ULTRALIGHT_ACCESS_READ_PROTECTION      = 7,        /**< enable or disable read access protection */
    MIFARE_ULTRALIGHT_ACCESS_USER_CONF_PROTECTION = 6,        /**< enable or disable user configuration protection */
} mifare_ultralight_access_t;

/**
 * @brief mifare ultralight modulation mode enumeration definition
 */
typedef enum
{
    MIFARE_ULTRALIGHT_MODULATION_MODE_NORMAL = 0x00,        /**< normal */
    MIFARE_ULTRALIGHT_MODULATION_MODE_STRONG = 0x01,        /**< strong */
} mifare_ultralight_modulation_mode_t;

/**
 * @brief mifare ultralight version structure definition
 */
typedef struct mifare_ultralight_version_s
{
    uint8_t fixed_header;                 /**< fixed header */
    uint8_t vendor_id;                    /**< vendor id */
    uint8_t product_type;                 /**< product type */
    uint8_t product_subtype;              /**< product subtype */
    uint8_t major_product_version;        /**< major product version */
    uint8_t minor_product_version;        /**< minor product version */
    uint8_t storage_size;                 /**< storage size */
    uint8_t protocol_type;                /**< protocol type */
} mifare_ultralight_version_t;

/**
 * @brief mifare ultralight handle structure definition
 */
typedef struct mifare_ultralight_handle_s
{
    uint8_t (*contactless_init)(void);                                             /**< point to a contactless_init function address */
    uint8_t (*contactless_deinit)(void);                                           /**< point to a contactless_deinit function address */
    uint8_t (*contactless_transceiver)(uint8_t *in_buf, uint8_t in_len, 
                                       uint8_t *out_buf, uint8_t *out_len);        /**< point to a contactless_transceiver function address */
    void (*delay_ms)(uint32_t ms);                                                 /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                               /**< point to a debug_print function address */
    uint8_t end_page;                                                              /**< end page */
    uint8_t type;                                                                  /**< type */
    uint8_t inited;                                                                /**< inited flag */
} mifare_ultralight_handle_t;

/**
 * @brief mifare ultralight information structure definition
 */
typedef struct mifare_ultralight_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} mifare_ultralight_info_t;

/**
 * @}
 */

/**
 * @defgroup mifare_ultralight_link_driver mifare ultralight link driver function
 * @brief    mifare ultralight link driver modules
 * @ingroup  mifare_ultralight_driver
 * @{
 */

/**
 * @brief     initialize mifare_ultralight_handle_t structure
 * @param[in] HANDLE points to a mifare_ultralight handle structure
 * @param[in] STRUCTURE is mifare_ultralight_handle_t
 * @note      none
 */
#define DRIVER_MIFARE_ULTRALIGHT_LINK_INIT(HANDLE, STRUCTURE)                  memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link contactless_init function
 * @param[in] HANDLE points to a mifare_ultralight handle structure
 * @param[in] FUC points to a contactless_init function address
 * @note      none
 */
#define DRIVER_MIFARE_ULTRALIGHT_LINK_CONTACTLESS_INIT(HANDLE, FUC)           (HANDLE)->contactless_init = FUC

/**
 * @brief     link contactless_deinit function
 * @param[in] HANDLE points to a mifare_ultralight handle structure
 * @param[in] FUC points to a contactless_deinit function address
 * @note      none
 */
#define DRIVER_MIFARE_ULTRALIGHT_LINK_CONTACTLESS_DEINIT(HANDLE, FUC)         (HANDLE)->contactless_deinit = FUC

/**
 * @brief     link contactless_transceiver function
 * @param[in] HANDLE points to a mifare_ultralight handle structure
 * @param[in] FUC points to a contactless_transceiver function address
 * @note      none
 */
#define DRIVER_MIFARE_ULTRALIGHT_LINK_CONTACTLESS_TRANSCEIVER(HANDLE, FUC)    (HANDLE)->contactless_transceiver = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a mifare_ultralight handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_MIFARE_ULTRALIGHT_LINK_DELAY_MS(HANDLE, FUC)                   (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a mifare_ultralight handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_MIFARE_ULTRALIGHT_LINK_DEBUG_PRINT(HANDLE, FUC)                (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup mifare_ultralight_basic_driver mifare ultralight basic driver function
 * @brief    mifare ultralight basic driver modules
 * @ingroup  mifare_ultralight_driver
 * @{
 */

/**
 * @brief      get chip information
 * @param[out] *info points to a mifare_ultralight info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mifare_ultralight_info(mifare_ultralight_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @return    status code
 *            - 0 success
 *            - 1 contactless initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t mifare_ultralight_init(mifare_ultralight_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @return    status code
 *            - 0 success
 *            - 1 contactless deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mifare_ultralight_deinit(mifare_ultralight_handle_t *handle);

/**
 * @brief     set the storage
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] storage is the set storage
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mifare_ultralight_set_storage(mifare_ultralight_handle_t *handle, mifare_ultralight_storage_t storage);

/**
 * @brief      get the storage
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *storage points to a set storage buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mifare_ultralight_get_storage(mifare_ultralight_handle_t *handle, mifare_ultralight_storage_t *storage);

/**
 * @brief      mifare_ultralight request
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *type points to a type buffer
 * @return     status code
 *             - 0 success
 *             - 1 request failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 type is invalid
 * @note       none
 */
uint8_t mifare_ultralight_request(mifare_ultralight_handle_t *handle, mifare_ultralight_type_t *type);

/**
 * @brief      mifare_ultralight wake up
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *type points to a type buffer
 * @return     status code
 *             - 0 success
 *             - 1 wake up failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 type is invalid
 * @note       none
 */
uint8_t mifare_ultralight_wake_up(mifare_ultralight_handle_t *handle, mifare_ultralight_type_t *type);

/**
 * @brief      mifare_ultralight halt
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @return     status code
 *             - 0 success
 *             - 1 halt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mifare_ultralight_halt(mifare_ultralight_handle_t *handle);

/**
 * @brief      mifare_ultralight anti collision cl1
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 anti collision cl1 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 check error
 * @note       none
 */
uint8_t mifare_ultralight_anticollision_cl1(mifare_ultralight_handle_t *handle, uint8_t id[4]);

/**
 * @brief      mifare_ultralight anti collision cl2
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 anti collision cl2 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 check error
 * @note       none
 */
uint8_t mifare_ultralight_anticollision_cl2(mifare_ultralight_handle_t *handle, uint8_t id[4]);

/**
 * @brief     mifare_ultralight select cl1
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] *id points to an id buffer
 * @return    status code
 *            - 0 success
 *            - 1 select cl1 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 sak error
 * @note      none
 */
uint8_t mifare_ultralight_select_cl1(mifare_ultralight_handle_t *handle, uint8_t id[4]);

/**
 * @brief     mifare_ultralight select cl2
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] *id points to an id buffer
 * @return    status code
 *            - 0 success
 *            - 1 select cl2 failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 sak error
 * @note      none
 */
uint8_t mifare_ultralight_select_cl2(mifare_ultralight_handle_t *handle, uint8_t id[4]);

/**
 * @brief      mifare_ultralight get the version
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *version points to a version structure
 * @return     status code
 *             - 0 success
 *             - 1 get version failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t mifare_ultralight_get_version(mifare_ultralight_handle_t *handle, mifare_ultralight_version_t *version);

/**
 * @brief      mifare_ultralight read the counter
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[in]  addr is the read counter address
 * @param[out] *cnt points to a counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 read counter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 *             - 6 addr > 0x2
 * @note       0 <= addr <= 2
 */
uint8_t mifare_ultralight_read_counter(mifare_ultralight_handle_t *handle, uint8_t addr, uint32_t *cnt);

/**
 * @brief     mifare_ultralight increment the counter
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] addr is the read counter address
 * @param[in] cnt is the increment counter
 * @return    status code
 *            - 0 success
 *            - 1 increment counter failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 *            - 6 addr > 0x2
 * @note      0 <= addr <= 2
 */
uint8_t mifare_ultralight_increment_counter(mifare_ultralight_handle_t *handle, uint8_t addr, uint32_t cnt);

/**
 * @brief      mifare_ultralight check the tearing event
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[in]  addr is the check address
 * @param[out] *flag points to a flag buffer
 * @return     status code
 *             - 0 success
 *             - 1 check tearing event failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 *             - 6 addr > 0x2
 * @note       0 <= addr <= 2
 */
uint8_t mifare_ultralight_check_tearing_event(mifare_ultralight_handle_t *handle, uint8_t addr, uint8_t *flag);

/**
 * @brief      mifare_ultralight vcsl command
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[in]  *installation_identifier points to an installation identifier buffer
 * @param[in]  *pcd_capabilities points to a pcd capabilities buffer
 * @param[out] *identifier points to an identifier buffer
 * @return     status code
 *             - 0 success
 *             - 1 vcsl failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t mifare_ultralight_vcsl(mifare_ultralight_handle_t *handle, uint8_t installation_identifier[16],
                               uint8_t pcd_capabilities[4], uint8_t *identifier);

/**
 * @brief      mifare_ultralight read the signature
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *signature points to a signature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read signature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t mifare_ultralight_read_signature(mifare_ultralight_handle_t *handle, uint8_t signature[32]);

/**
 * @brief      mifare_ultralight get the serial number
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *number points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t mifare_ultralight_get_serial_number(mifare_ultralight_handle_t *handle, uint8_t number[7]);

/**
 * @brief      mifare_ultralight read four pages
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[in]  start_page is the start page of read
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read four pages failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t mifare_ultralight_read_four_pages(mifare_ultralight_handle_t *handle, uint8_t start_page, uint8_t data[16]);

/**
 * @brief      mifare_ultralight read page
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[in]  page is the page of read
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read page failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t mifare_ultralight_read_page(mifare_ultralight_handle_t *handle, uint8_t page, uint8_t data[4]);

/**
 * @brief         mifare_ultralight fast read page
 * @param[in]     *handle points to a mifare_ultralight handle structure
 * @param[in]     start_page is the start page
 * @param[in]     stop_page is the stop page
 * @param[out]    *data points to a data buffer
 * @param[in,out] *len points to a data length buffer
 * @return        status code
 *                - 0 success
 *                - 1 fast read page failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 *                - 4 stop_page < start_page
 *                - 5 stop_page - start_page + 1 is over 15
 *                - 6 len is invalid
 *                - 7 output_len is invalid
 *                - 8 crc error
 * @note          stop_page - start_page + 1 <= 15
 *                4 * (stop_page - start_page + 1) < len
 */
uint8_t mifare_ultralight_fast_read_page(mifare_ultralight_handle_t *handle, uint8_t start_page, uint8_t stop_page, uint8_t *data, uint16_t *len);

/**
 * @brief     mifare_ultralight compatibility write page
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] page is the page of write
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 compatibility write page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t mifare_ultralight_compatibility_write_page(mifare_ultralight_handle_t *handle, uint8_t page, uint8_t data[4]);

/**
 * @brief     mifare_ultralight write page
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] page is the page of write
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t mifare_ultralight_write_page(mifare_ultralight_handle_t *handle, uint8_t page, uint8_t data[4]);

/**
 * @brief     mifare_ultralight authenticate
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] *pwd points to a pwd buffer
 * @param[in] *pack points to a pack buffer
 * @return    status code
 *            - 0 success
 *            - 1 authenticate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 crc error
 *            - 6 pack check failed
 * @note      none
 */
uint8_t mifare_ultralight_authenticate(mifare_ultralight_handle_t *handle, uint8_t pwd[4], uint8_t pack[2]);

/**
 * @brief     mifare_ultralight set the password
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] *pwd points to a password buffer
 * @return    status code
 *            - 0 success
 *            - 1 set pwd failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t mifare_ultralight_set_password(mifare_ultralight_handle_t *handle, uint8_t pwd[4]);

/**
 * @brief     mifare_ultralight set the pack
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] *pack points to a pack buffer
 * @return    status code
 *            - 0 success
 *            - 1 set pack failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t mifare_ultralight_set_pack(mifare_ultralight_handle_t *handle, uint8_t pack[2]);

/**
 * @brief     mifare_ultralight set the modulation mode
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] mode is the modulation mode
 * @return    status code
 *            - 0 success
 *            - 1 set modulation mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mifare_ultralight_set_modulation_mode(mifare_ultralight_handle_t *handle, mifare_ultralight_modulation_mode_t mode);

/**
 * @brief      mifare_ultralight get the modulation mode
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *mode points to a modulation mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get modulation mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mifare_ultralight_get_modulation_mode(mifare_ultralight_handle_t *handle, mifare_ultralight_modulation_mode_t *mode);

/**
 * @brief     mifare_ultralight set the start page of protection
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] page is the start page
 * @return    status code
 *            - 0 success
 *            - 1 set protect start page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mifare_ultralight_set_protect_start_page(mifare_ultralight_handle_t *handle, uint8_t page);

/**
 * @brief      mifare_ultralight get the start page of protection
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *page points to a start page buffer
 * @return     status code
 *             - 0 success
 *             - 1 get protect start page failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mifare_ultralight_get_protect_start_page(mifare_ultralight_handle_t *handle, uint8_t *page);

/**
 * @brief     mifare_ultralight enable or disable access
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] access is the set access
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set access failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mifare_ultralight_set_access(mifare_ultralight_handle_t *handle, mifare_ultralight_access_t access, mifare_ultralight_bool_t enable);

/**
 * @brief      mifare_ultralight get the access status
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[in]  access is the set access
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get access failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mifare_ultralight_get_access(mifare_ultralight_handle_t *handle, mifare_ultralight_access_t access, mifare_ultralight_bool_t *enable);

/**
 * @brief     mifare_ultralight set the authenticate limitation
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] limit is the authenticate limitation
 * @return    status code
 *            - 0 success
 *            - 1 set authenticate limitation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 limit > 7
 * @note      none
 */
uint8_t mifare_ultralight_set_authenticate_limitation(mifare_ultralight_handle_t *handle, uint8_t limit);

/**
 * @brief      mifare_ultralight get the authenticate limitation
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *limit points to an authenticate limitation buffer
 * @return     status code
 *             - 0 success
 *             - 1 get authenticate limitation failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mifare_ultralight_get_authenticate_limitation(mifare_ultralight_handle_t *handle, uint8_t *limit);

/**
 * @brief     mifare_ultralight set the virtual card type identifier
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] identifier is the virtual card type identifier
 * @return    status code
 *            - 0 success
 *            - 1 set virtual card type identifier failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t mifare_ultralight_set_virtual_card_type_identifier(mifare_ultralight_handle_t *handle, uint8_t identifier);

/**
 * @brief      mifare_ultralight get the virtual card type identifier
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *identifier points to a virtual card type identifier buffer
 * @return     status code
 *             - 0 success
 *             - 1 get virtual card type identifier failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t mifare_ultralight_get_virtual_card_type_identifier(mifare_ultralight_handle_t *handle, uint8_t *identifier);

/**
 * @brief     mifare_ultralight set the lock
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] *lock points to a lock buffer
 * @return    status code
 *            - 0 success
 *            - 1 set lock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note                                                      byte0                                                                               
 *            bit7            bit6            bit5            bit4             bit3             bit2             bit1             bit0            
 *            lock_page_7     lock_page_6     lock_page_5     lock_page_4      lock_otp         block_page_15_10 block_page_9_4   bock_otp        
 *                                                            byte1                                                                               
 *            bit7            bit6            bit5            bit4             bit3             bit2             bit1             bit0            
 *            lock_page_15    lock_page_14    lock_page_13    lock_page_12     lock_page_11     lock_page_10     lock_page_9      lock_page_8     
 *                                                            byte2                                                                               
 *            bit7            bit6            bit5            bit4             bit3             bit2             bit1             bit0            
 *            lock_page_30-31 lock_page_28-29 lock_page_26-27 lock_page_24-25  lock_page_22-23  lock_page_20-21  lock_page_18-19  lock_page_16-17 
 *                                                            byte3                                                                               
 *            bit7            bit6            bit5            bit4             bit3             bit2             bit1             bit0            
 *            rfui            rfui            rfui            rfui             rfui             rfui             lock_page_34-35  lock_page_32-22 
 *                                                            byte4                                                                               
 *            bit7            bit6            bit5            bit4             bit3             bit2             bit1             bit0            
 *            rfui            rfui            rfui            block_page_32_35 block_page_28_31 block_page_24_27 block_page_20_23 block_page_16_19
 */
uint8_t mifare_ultralight_set_lock(mifare_ultralight_handle_t *handle, uint8_t lock[5]);

/**
 * @brief      mifare_ultralight get the lock
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *lock points to a lock buffer
 * @return     status code
 *             - 0 success
 *             - 1 get lock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t mifare_ultralight_get_lock(mifare_ultralight_handle_t *handle, uint8_t lock[5]);

/**
 * @brief      mifare_ultralight read otp page
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read otp page failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 output_len is invalid
 *             - 5 crc error
 * @note       none
 */
uint8_t mifare_ultralight_read_otp(mifare_ultralight_handle_t *handle, uint8_t data[4]);

/**
 * @brief     mifare_ultralight write otp page
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write otp page failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 output_len is invalid
 *            - 5 ack error
 * @note      none
 */
uint8_t mifare_ultralight_write_otp(mifare_ultralight_handle_t *handle, uint8_t data[4]);

/**
 * @}
 */

/**
 * @defgroup mifare_ultralight_extern_driver mifare ultralight extern driver function
 * @brief    mifare ultralight extern driver modules
 * @ingroup  mifare_ultralight_driver
 * @{
 */

/**
 * @brief         transceiver data
 * @param[in]     *handle points to a mifare_ultralight handle structure
 * @param[in]     *in_buf points to an input buffer
 * @param[in]     in_len is the input length
 * @param[out]    *out_buf points to an output buffer
 * @param[in,out] *out_len points to an output length buffer
 * @return        status code
 *                - 0 success
 *                - 1 transceiver failed
 * @note          none
 */
uint8_t mifare_ultralight_transceiver(mifare_ultralight_handle_t *handle, uint8_t *in_buf, uint8_t in_len, uint8_t *out_buf, uint8_t *out_len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
