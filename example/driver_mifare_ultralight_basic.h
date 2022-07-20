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
 * @file      driver_mifare_ultralight_basic.h
 * @brief     driver mifare_ultralight basic header file
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

#ifndef DRIVER_MIFARE_ULTRALIGHT_BASIC_H
#define DRIVER_MIFARE_ULTRALIGHT_BASIC_H

#include "driver_mifare_ultralight_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup mifare_ultralight_example_driver mifare ultralight example driver function
 * @brief    mifare ultralight example driver modules
 * @ingroup  mifare_ultralight_driver
 * @{
 */

/**
 * @brief mifare_ultralight basic example default definition
 */
#define MIFARE_MIFARE_ULTRALIGHT_DEFAULT_SEARCH_DELAY_MS        200        /**< 5Hz */

/**
 * @brief  basic example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t mifare_ultralight_basic_init(void);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t mifare_ultralight_basic_deinit(void);

/**
 * @brief  basic example halt
 * @return status code
 *         - 0 success
 *         - 1 halt failed
 * @note   none
 */
uint8_t mifare_ultralight_basic_halt(void);

/**
 * @brief  basic example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t mifare_ultralight_basic_wake_up(void);

/**
 * @brief      basic example search
 * @param[out] *type points to a type buffer
 * @param[out] *id points to an id buffer
 * @param[in]  timeout is the check times
 * @return     status code
 *             - 0 success
 *             - 1 timeout
 * @note       none
 */
uint8_t mifare_ultralight_basic_search(mifare_ultralight_storage_t *type, uint8_t id[8], int32_t timeout);

/**
 * @brief      basic example read
 * @param[in]  page is the read page
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mifare_ultralight_basic_read(uint8_t page, uint8_t data[4]);

/**
 * @brief      basic example read four pages
 * @param[in]  start_page is the start page
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t mifare_ultralight_basic_read_four_pages(uint8_t start_page, uint8_t data[16]);

/**
 * @brief         basic example read pages
 * @param[in]     start_page is the start page
 * @param[in]     stop_page is the stop page
 * @param[out]    *data points to a data buffer
 * @param[in,out] *len points to a data length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 * @note          stop_page - start_page + 1 <= 15
 *                4 * (stop_page - start_page + 1) < len
 */
uint8_t mifare_ultralight_basic_read_pages(uint8_t start_page, uint8_t stop_page, uint8_t *data, uint16_t *len);

/**
 * @brief     basic example write
 * @param[in] page is the written page
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t mifare_ultralight_basic_write(uint8_t page, uint8_t data[4]);

/**
 * @brief     basic example write otp
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write otp failed
 * @note      none
 */
uint8_t mifare_ultralight_basic_write_otp(uint8_t data[4]);

/**
 * @brief      basic example read otp
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read otp failed
 * @note       none
 */
uint8_t mifare_ultralight_basic_read_otp(uint8_t data[4]);

/**
 * @brief      basic example get version
 * @param[out] *version points to a version structure
 * @return     status code
 *             - 0 success
 *             - 1 get version failed
 * @note       none
 */
uint8_t mifare_ultralight_basic_get_version(mifare_ultralight_version_t *version);

/**
 * @brief      basic example read counter
 * @param[in]  addr is the read counter address
 * @param[out] *cnt points to a counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 read counter failed
 * @note       0 <= addr <= 2
 */
uint8_t mifare_ultralight_basic_read_counter(uint8_t addr, uint32_t *cnt);

/**
 * @brief     basic example increment counter
 * @param[in] addr is the increment counter address
 * @param[in] cnt is the increment counter
 * @return    status code
 *            - 0 success
 *            - 1 increment counter failed
 * @note      0 <= addr <= 2
 */
uint8_t mifare_ultralight_basic_increment_counter(uint8_t addr, uint32_t cnt);

/**
 * @brief      basic example check the tearing event
 * @param[in]  addr is the increment counter address
 * @param[out] *flag points to a flag buffer
 * @return     status code
 *             - 0 success
 *             - 1 check tearing event failed
 * @note       0 <= addr <= 2
 */
uint8_t mifare_ultralight_basic_check_tearing_event(uint8_t addr, uint8_t *flag);

/**
 * @brief      basic example read signature
 * @param[out] *signature points to a signature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read signature failed
 * @note       none
 */
uint8_t mifare_ultralight_basic_read_signature(uint8_t signature[32]);

/**
 * @brief      basic example get serial number
 * @param[out] *number points to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial number failed
 * @note       none
 */
uint8_t mifare_ultralight_basic_get_serial_number(uint8_t number[7]);

/**
 * @brief     basic example set password and pack
 * @param[in] *pwd points to a password buffer
 * @param[in] *pack points to a pack buffer
 * @return    status code
 *            - 0 success
 *            - 1 set password pack failed
 * @note      none
 */
uint8_t mifare_ultralight_basic_set_password_pack(uint8_t pwd[4], uint8_t pack[2]);

/**
 * @brief     basic example authenticate
 * @param[in] *pwd points to a password buffer
 * @param[in] *pack points to a pack buffer
 * @return    status code
 *            - 0 success
 *            - 1 authenticate failed
 * @note      none
 */
uint8_t mifare_ultralight_basic_authenticate(uint8_t pwd[4], uint8_t pack[2]);

/**
 * @brief     basic example set lock
 * @param[in] *lock points to a lock buffer
 * @return    status code
 *            - 0 success
 *            - 1 set lock failed
 * @note      none
 */
uint8_t mifare_ultralight_basic_set_lock(uint8_t lock[5]);

/**
 * @brief     basic example set modulation mode
 * @param[in] mode is the modulation mode
 * @return    status code
 *            - 0 success
 *            - 1 set modulation mode failed
 * @note      none
 */
uint8_t mifare_ultralight_basic_set_modulation_mode(mifare_ultralight_modulation_mode_t mode);

/**
 * @brief     basic example set protect start page
 * @param[in] page is the start page
 * @return    status code
 *            - 0 success
 *            - 1 set protect start page failed
 * @note      none
 */
uint8_t mifare_ultralight_basic_set_protect_start_page(uint8_t page);

/**
 * @brief     basic example set the authenticate limitation
 * @param[in] limit is the authenticate limitation
 * @return    status code
 *            - 0 success
 *            - 1 set authenticate limitation failed
 * @note      none
 */
uint8_t mifare_ultralight_basic_set_authenticate_limitation(uint8_t limit);

/**
 * @brief     basic example set access
 * @param[in] access is the set access
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set access failed
 * @note      none
 */
uint8_t mifare_ultralight_basic_set_access(mifare_ultralight_access_t access, mifare_ultralight_bool_t enable);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
