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
 * @file      driver_mifare_ultralight_card_test.c
 * @brief     driver mifare_ultralight card test source file
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

#include "driver_mifare_ultralight_card_test.h"
#include <stdlib.h>

static mifare_ultralight_handle_t gs_handle;        /**< mifare_ultralight handle */

/**
 * @brief  card test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t mifare_ultralight_card_test(void)
{
    uint8_t res;
    uint8_t p;
    uint8_t limit_check;
    uint32_t i;
    uint32_t cnt;
    uint8_t id[4];
    uint8_t number[7];
    uint8_t signature[32];
    uint8_t data[16];
    uint8_t data_check[16];
    uint8_t page[32];
    uint8_t pack[2];
    uint8_t pwd[4];
    uint8_t lock[5];
    uint8_t lock_check[5];
    uint16_t len;
    uint8_t flag;
    uint8_t identifier;
    mifare_ultralight_info_t info;
    mifare_ultralight_type_t type;
    mifare_ultralight_version_t version;
    mifare_ultralight_modulation_mode_t mode;
    mifare_ultralight_bool_t enable;
    
    /* link functions */
    DRIVER_MIFARE_ULTRALIGHT_LINK_INIT(&gs_handle, mifare_ultralight_handle_t);
    DRIVER_MIFARE_ULTRALIGHT_LINK_CONTACTLESS_INIT(&gs_handle, mifare_ultralight_interface_contactless_init);
    DRIVER_MIFARE_ULTRALIGHT_LINK_CONTACTLESS_DEINIT(&gs_handle, mifare_ultralight_interface_contactless_deinit);
    DRIVER_MIFARE_ULTRALIGHT_LINK_CONTACTLESS_TRANSCEIVER(&gs_handle, mifare_ultralight_interface_contactless_transceiver);
    DRIVER_MIFARE_ULTRALIGHT_LINK_DELAY_MS(&gs_handle, mifare_ultralight_interface_delay_ms);
    DRIVER_MIFARE_ULTRALIGHT_LINK_DEBUG_PRINT(&gs_handle, mifare_ultralight_interface_debug_print);
    
    /* get information */
    res = mifare_ultralight_info(&info);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
        mifare_ultralight_interface_debug_print("mifare_ultralight: chip is %s.\n", info.chip_name);
        mifare_ultralight_interface_debug_print("mifare_ultralight: manufacturer is %s.\n", info.manufacturer_name);
        mifare_ultralight_interface_debug_print("mifare_ultralight: interface is %s.\n", info.interface);
        mifare_ultralight_interface_debug_print("mifare_ultralight: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        mifare_ultralight_interface_debug_print("mifare_ultralight: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        mifare_ultralight_interface_debug_print("mifare_ultralight: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        mifare_ultralight_interface_debug_print("mifare_ultralight: max current is %0.2fmA.\n", info.max_current_ma);
        mifare_ultralight_interface_debug_print("mifare_ultralight: max temperature is %0.1fC.\n", info.temperature_max);
        mifare_ultralight_interface_debug_print("mifare_ultralight: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    mifare_ultralight_interface_debug_print("mifare_ultralight: start card test.\n");
    
    /* init */
    res = mifare_ultralight_init(&gs_handle);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: init failed.\n");
        
        return 1;
    }
    
    /* wake up */
    res = mifare_ultralight_wake_up(&gs_handle, &type);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: wake up failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    if (type == MIFARE_ULTRALIGHT_TYPE_ULTRALIGHT)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: find ultralight card.\n");
    }
    else
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    
    /* anti collision cl1 */
    res = mifare_ultralight_anticollision_cl1(&gs_handle, id);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: anti collision cl1 failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    
    /* select cl1 */
    res = mifare_ultralight_select_cl1(&gs_handle, id);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: select cl1 failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: id1 is 0x%02X 0x%02X 0x%02X 0x%02X.\n", id[0], id[1], id[2], id[3]);
    
    /* anti collision cl2 */
    res = mifare_ultralight_anticollision_cl2(&gs_handle, id);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: anti collision cl2 failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    
    /* anti collision cl2 */
    res = mifare_ultralight_anticollision_cl2(&gs_handle, id);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: anti collision cl2 failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: id2 is 0x%02X 0x%02X 0x%02X 0x%02X.\n", id[0], id[1], id[2], id[3]);
    
    /* get serial number */
    res = mifare_ultralight_get_serial_number(&gs_handle, number);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get serial number failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: serial number is ");
    for (i = 0; i < 7; i++)
    {
        mifare_ultralight_interface_debug_print("0x%02X ", number[i]);
    }
    mifare_ultralight_interface_debug_print("\n");
    
    /* read signature */
    res = mifare_ultralight_read_signature(&gs_handle, signature);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: read signature failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: signature is ");
    for (i = 0; i < 32; i++)
    {
        mifare_ultralight_interface_debug_print("0x%02X ", signature[i]);
    }
    mifare_ultralight_interface_debug_print("\n");
    
    mifare_ultralight_interface_debug_print("mifare_ultralight: read four pages from page 0.\n");
    
    /* read four pages */
    res = mifare_ultralight_read_four_pages(&gs_handle, 0x00, data_check);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: read four pages failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < 16; i++)
    {
        mifare_ultralight_interface_debug_print("0x%02X ", data_check[i]);
    }
    mifare_ultralight_interface_debug_print("\n");
    
    mifare_ultralight_interface_debug_print("mifare_ultralight: read page 3.\n");
    
    /* read page */
    res = mifare_ultralight_read_page(&gs_handle, 0x3, data_check);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: read page failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < 4; i++)
    {
        mifare_ultralight_interface_debug_print("0x%02X ", data_check[i]);
    }
    mifare_ultralight_interface_debug_print("\n");
    
    mifare_ultralight_interface_debug_print("mifare_ultralight: fast read page from page 0 - page 5.\n");
    
    /* fast read page */
    len = 32;
    res = mifare_ultralight_fast_read_page(&gs_handle, 0x00, 0x5, page, &len);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: fast read page failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    for (i = 0; i < 32; i++)
    {
        mifare_ultralight_interface_debug_print("0x%02X ", page[i]);
    }
    mifare_ultralight_interface_debug_print("\n");
    
    mifare_ultralight_interface_debug_print("mifare_ultralight: compatibility write page 16.\n");
    for (i = 0; i < 4; i++)
    {
        data[i] = rand() % 256;
    }
    res = mifare_ultralight_compatibility_write_page(&gs_handle, 8, data);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: compatibility write page failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    res = mifare_ultralight_read_page(&gs_handle, 8, data_check);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: read page failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check page %s.\n", memcmp(data, data_check, 4) == 0 ? "ok" : "error");
    
    mifare_ultralight_interface_debug_print("mifare_ultralight: write page 17.\n");
    for (i = 0; i < 4; i++)
    {
        data[i] = rand() % 256;
    }
    res = mifare_ultralight_write_page(&gs_handle, 9, data);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: write page failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    res = mifare_ultralight_read_page(&gs_handle, 9, data_check);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: read page failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check page %s.\n", memcmp(data, data_check, 4) == 0 ? "ok" : "error");
    
    /* get version */
    res = mifare_ultralight_get_version(&gs_handle, &version);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get version failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: fixed_header is 0x%02X\n", version.fixed_header);
    mifare_ultralight_interface_debug_print("mifare_ultralight: vendor_id is 0x%02X\n", version.vendor_id);
    mifare_ultralight_interface_debug_print("mifare_ultralight: product_type is 0x%02X\n", version.product_type);
    mifare_ultralight_interface_debug_print("mifare_ultralight: product_subtype is 0x%02X\n", version.product_subtype);
    mifare_ultralight_interface_debug_print("mifare_ultralight: major_product_version is 0x%02X\n", version.major_product_version);
    mifare_ultralight_interface_debug_print("mifare_ultralight: minor_product_version is 0x%02X\n", version.minor_product_version);
    mifare_ultralight_interface_debug_print("mifare_ultralight: storage_size is 0x%02X\n", version.storage_size);
    mifare_ultralight_interface_debug_print("mifare_ultralight: protocol_type is 0x%02X\n", version.protocol_type);
    
    /* check the password */
    pwd[0] = 0xFF;
    pwd[1] = 0xFF;
    pwd[2] = 0xFF;
    pwd[3] = 0xFF;
    pack[0] = 0x00;
    pack[1] = 0x00;
    res = mifare_ultralight_set_password(&gs_handle, pwd);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: set password failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    res = mifare_ultralight_set_pack(&gs_handle, pack);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: set pack failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: authenticate.\n");
    res = mifare_ultralight_authenticate(&gs_handle, pwd, pack);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: authenticate failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set lock */
    lock[0] = 0x00;
    lock[1] = 0x00;
    lock[2] = 0x00;
    lock[3] = 0x00;
    lock[4] = 0x00;
    
    /* set the lock */
    res = mifare_ultralight_set_lock(&gs_handle, lock);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: set lock failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: set lock 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", 
                                            lock[0], lock[1], lock[2], lock[3], lock[4]);
    res = mifare_ultralight_get_lock(&gs_handle, lock_check);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get lock failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: set lock 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", 
                                            lock_check[0], lock_check[1], lock_check[2], lock_check[3], lock_check[4]);
    
    /* set normal mode */
    res = mifare_ultralight_set_modulation_mode(&gs_handle, MIFARE_ULTRALIGHT_MODULATION_MODE_NORMAL);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: set modulation mode failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: set normal mode.\n");
    res = mifare_ultralight_get_modulation_mode(&gs_handle, &mode);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get modulation mode failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check modulation mode %s.\n", mode == MIFARE_ULTRALIGHT_MODULATION_MODE_NORMAL ? "ok" : "error");
    
    /* set strong mode */
    res = mifare_ultralight_set_modulation_mode(&gs_handle, MIFARE_ULTRALIGHT_MODULATION_MODE_STRONG);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: set modulation mode failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: set strong mode.\n");
    res = mifare_ultralight_get_modulation_mode(&gs_handle, &mode);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get modulation mode failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check modulation mode %s.\n", mode == MIFARE_ULTRALIGHT_MODULATION_MODE_STRONG ? "ok" : "error");
    
    /* set protect start page */
    res = mifare_ultralight_set_protect_start_page(&gs_handle, 0xFF);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: set protect start page failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: set protect start page 0xFF.\n");
    res = mifare_ultralight_get_protect_start_page(&gs_handle, &p);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get protect start page failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check protect start page %s.\n", p == 0xFF ? "ok" : "error");
    
    /* read access protection */
    res = mifare_ultralight_set_access(&gs_handle, MIFARE_ULTRALIGHT_ACCESS_READ_PROTECTION, MIFARE_ULTRALIGHT_BOOL_FALSE);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: set access failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: read access protection disable.\n");
    res = mifare_ultralight_get_access(&gs_handle, MIFARE_ULTRALIGHT_ACCESS_READ_PROTECTION, &enable);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get access failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check read access protection %s.\n", enable == MIFARE_ULTRALIGHT_BOOL_FALSE ? "ok" : "error");
    
    /* user configuration protection */
    res = mifare_ultralight_set_access(&gs_handle, MIFARE_ULTRALIGHT_ACCESS_USER_CONF_PROTECTION, MIFARE_ULTRALIGHT_BOOL_FALSE);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: set access failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: user configuration protection disable.\n");
    res = mifare_ultralight_get_access(&gs_handle, MIFARE_ULTRALIGHT_ACCESS_USER_CONF_PROTECTION, &enable);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get access failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check user configuration protection %s.\n", enable == MIFARE_ULTRALIGHT_BOOL_FALSE ? "ok" : "error");
    
    /* set authenticate limitation  */
    res = mifare_ultralight_set_authenticate_limitation(&gs_handle, 7);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: set authenticate limitation failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: set authenticate limitation 7.\n");
    res = mifare_ultralight_get_authenticate_limitation(&gs_handle, &limit_check);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get authenticate limitation failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check authenticate limitation %s.\n", limit_check == 7 ? "ok" : "error");
    
    /* set virtual card type identifier */
    res = mifare_ultralight_set_virtual_card_type_identifier(&gs_handle, 0x05);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: set virtual card type identifier failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: set virtual card type identifier 5.\n");
    res = mifare_ultralight_get_virtual_card_type_identifier(&gs_handle, &identifier);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: get virtual card type identifier failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check virtual card type identifier %s.\n", identifier == 5 ? "ok" : "error");
    
    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = 0x00;
    data[3] = 0x00;
    res = mifare_ultralight_write_otp(&gs_handle, data);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: write otp failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: write otp 0x%02X 0x%02X 0x%02X 0x%02X.\n", data[0], data[1], data[2], data[3]);
    res = mifare_ultralight_read_otp(&gs_handle, data_check);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: read otp failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check opt %s.\n", (memcmp(data, data_check, 4) == 0) ? "ok" : "error");
    
    /* increment counter */
    res = mifare_ultralight_increment_counter(&gs_handle, 0x00, 1);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: increment counter failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: increment counter 1.\n");
    
    /* read counter */
    res = mifare_ultralight_read_counter(&gs_handle, 0x00, &cnt);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: read counter failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: read counter %d.\n", cnt);
    
    /* check_tearing_event */
    res = mifare_ultralight_check_tearing_event(&gs_handle, 0x00, &flag);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: check tearing event failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    mifare_ultralight_interface_debug_print("mifare_ultralight: check tearing event flag 0x%02X.\n", flag);
    
    /* halt */
    res = mifare_ultralight_halt(&gs_handle);
    if (res != 0)
    {
        mifare_ultralight_interface_debug_print("mifare_ultralight: halt failed.\n");
        (void)mifare_ultralight_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish register */
    mifare_ultralight_interface_debug_print("mifare_ultralight: finish card test.\n");
    (void)mifare_ultralight_deinit(&gs_handle);
    
    return 0;
}
