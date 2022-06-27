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
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-06-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/06/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_mifare_ultralight_basic.h"
#include "driver_mifare_ultralight_card_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
uint16_t g_len;                            /**< uart buffer length */

/**
 * @brief     mifare_ultralight full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t mifare_ultralight(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            mifare_ultralight_info_t info;
            
            /* print mifare_ultralight info */
            mifare_ultralight_info(&info);
            mifare_ultralight_interface_debug_print("mifare_ultralight: chip is %s.\n", info.chip_name);
            mifare_ultralight_interface_debug_print("mifare_ultralight: manufacturer is %s.\n", info.manufacturer_name);
            mifare_ultralight_interface_debug_print("mifare_ultralight: interface is %s.\n", info.interface);
            mifare_ultralight_interface_debug_print("mifare_ultralight: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
            mifare_ultralight_interface_debug_print("mifare_ultralight: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            mifare_ultralight_interface_debug_print("mifare_ultralight: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            mifare_ultralight_interface_debug_print("mifare_ultralight: max current is %0.2fmA.\n", info.max_current_ma);
            mifare_ultralight_interface_debug_print("mifare_ultralight: max temperature is %0.1fC.\n", info.temperature_max);
            mifare_ultralight_interface_debug_print("mifare_ultralight: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            mifare_ultralight_interface_debug_print("mifare_ultralight: SPI interface SCK connected to GPIOA PIN5.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight: SPI interface MISO connected to GPIOA PIN6.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight: SPI interface MOSI connected to GPIOA PIN7.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight: SPI interface CS connected to GPIOA PIN4.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight: IIC interface SCL connected to GPIOB PIN8.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight: IIC interface SDA connected to GPIOB PIN9.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight: UART interface TX connected to GPIOA PIN2.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight: UART interface RX connected to GPIOA PIN3.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight: INT connected to GPIOB PIN0.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight: RESET connected to GPIOA PIN8.\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show mifare_ultralight help */
            help:
            
            mifare_ultralight_interface_debug_print("mifare_ultralight -i\n\tshow mifare_ultralight chip and driver information.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -h\n\tshow mifare_ultralight help.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -p\n\tshow mifare_ultralight pin connections of the current board.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -t card\n\trun mifare_ultralight card test.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c halt\n\tchip halt.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c wake_up\n\tchip wake up.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c read <page>\n\tchip read page.page is the read page.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c read_pages <startpage> <stoppage>\n\tchip read pages.startpage is the start page,stoppage is the stop page.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c read4 <startpage>\n\tchip read four pages.startpage is the start page.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c write <page>\n\tchip write page.page is the written page.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c version\n\tget the version.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c counter <addr>\n\tget the chip read counter.addr is the read counter address.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c inc <addr> <cnt>\n\tincrement the chip counter.addr is the increment counter address.cnt is the increment counter.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c otp -read\n\tread the otp data.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c otp -write <data>\n\twrite the otp data.data is the written data.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c check <addr>\n\tcheck the tearing event.addr is the check address.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c signature\n\tget the chip signature.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c serial_number\n\tget the chip serial number.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c set -pwd <pwd> <pack>\n\tset the password.pwd is the password,pack is the checked pack.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c set -lock <lock>\n\tset the lock.lock is the set lock.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c set -mode <mode>\n\tset the mode.mode can be \"NORMAL\" or \"STRONG\".\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c set -protect <page>\n\tset the start protect page.page is the start page.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c set -limitation <limit>\n\tset the authenticate limitation.limit is the set limit and it can be 0 - 7.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c set -access <access> <enable>\n\tset the access.access can be \"READ_PROTECTION\", \"USER_CONF_PROTECTION\"."
                                                    "enable is the set bool and it can be 0 or 1.\n");
            mifare_ultralight_interface_debug_print("mifare_ultralight -c authenticate <pwd> <pack>\n\tpassword authenticate.pwd is the password,pack is the checked pack.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("card", argv[2]) == 0)
            {
                uint8_t res;
                
                /* run the card test */
                res = mifare_ultralight_card_test();
                if (res != 0)
                {
                    return 1;
                }
                
                return 0;
            }
            else
            {
                return 5;
            }
        }
        else if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("halt", argv[2]) == 0)
            {
                uint8_t res;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* halt */
                res = mifare_ultralight_basic_halt();
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: halt.\n");
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("wake_up", argv[2]) == 0)
            {
                uint8_t res;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* wake_up */
                res = mifare_ultralight_basic_wake_up();
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: wake up.\n");
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("version", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                mifare_ultralight_storage_t type;
                mifare_ultralight_version_t version;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* get the version */
                res = mifare_ultralight_basic_get_version(&version);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
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
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("signature", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                mifare_ultralight_storage_t type;
                uint8_t signature[32];
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* read the signature */
                res = mifare_ultralight_basic_read_signature(signature);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: signature is ");
                for (i = 0; i < 32; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", signature[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("serial_number", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                mifare_ultralight_storage_t type;
                uint8_t number[7];
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* get the serial number */
                res = mifare_ultralight_basic_get_serial_number(number);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: serial number is ");
                for (i = 0; i < 7; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("read", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                uint8_t data[4];
                mifare_ultralight_storage_t type;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* read data */
                res = mifare_ultralight_basic_read((uint8_t)atoi(argv[3]), data);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: read page %d: ", atoi(argv[3]));
                for (i = 0; i < 4; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("read4", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                uint8_t data[16];
                mifare_ultralight_storage_t type;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* read data */
                res = mifare_ultralight_basic_read_four_pages((uint8_t)atoi(argv[3]), data);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: read page %d: ", atoi(argv[3]));
                for (i = 0; i < 16; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("counter", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                mifare_ultralight_storage_t type;
                uint32_t cnt;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* read the counter */
                res = mifare_ultralight_basic_read_counter((uint8_t)atoi(argv[3]), &cnt);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("addr %d read counter %d.\n", atoi(argv[3]), cnt);
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("check", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                mifare_ultralight_storage_t type;
                uint8_t flag;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* check the tearing event */
                res = mifare_ultralight_basic_check_tearing_event((uint8_t)atoi(argv[3]), &flag);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("addr %d check the tearing event 0x%02X.\n", atoi(argv[3]), flag);
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("otp", argv[2]) == 0)
            {
                if (strcmp("-read", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    uint8_t data[4];
                    mifare_ultralight_storage_t type;
                    
                    /* init */
                    res = mifare_ultralight_basic_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    /* search */
                    res = mifare_ultralight_basic_search(&type, id, 50);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    /* ouput the type */
                    if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                    }
                    else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                    }
                    else
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                    }
                    mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                    for (i = 0; i < 8; i++)
                    {
                        mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                    }
                    mifare_ultralight_interface_debug_print("\n");
                    
                    /* read otp */
                    res = mifare_ultralight_basic_read_otp(data);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    mifare_ultralight_interface_debug_print("read otp 0x%02X 0x%02X 0x%02X 0x%02X.\n", data[0], data[1], data[2], data[3]);
                    
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("write", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint32_t size;
                uint8_t id[8];
                uint8_t data[4];
                mifare_ultralight_storage_t type;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* get the data */
                size = (uint8_t)strlen(argv[4]);
                for (i = 0; i < size; i += 2)
                {
                    if (('A' <= argv[4][i]) && (argv[4][i] <= 'Z'))
                    {
                        data[i / 2] = argv[4][i] - 'A' + 10;
                        data[i / 2] *= 16;
                    }
                    else if (('a' <= argv[4][i]) && (argv[4][i] <= 'z'))
                    {
                        data[i / 2] = argv[4][i] - 'a' + 10;
                        data[i / 2] *= 16;
                    }
                    else if (('0' <= argv[4][i]) && (argv[4][i] <= '9'))
                    {
                        data[i / 2] = argv[4][i] - '0';
                        data[i / 2] *= 16;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (('A' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'Z'))
                    {
                        data[i / 2 ] += argv[4][i + 1] - 'A' + 10;
                    }
                    else if (('a' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'z'))
                    {
                        data[i / 2 ] += argv[4][i + 1] - 'a' + 10;
                    }
                    else if (('0' <= argv[4][i + 1]) && (argv[4][i + 1] <= '9'))
                    {
                        data[i / 2 ] += argv[4][i + 1] - '0';
                    }
                    else
                    {
                        return 5;
                    }
                }

                /* write data */
                res = mifare_ultralight_basic_write((uint8_t)atoi(argv[3]), data);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: write page %d: ", atoi(argv[3]));
                for (i = 0; i < 4; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("read_pages", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                uint8_t data[60];
                uint16_t len;
                mifare_ultralight_storage_t type;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* read pages */
                len = 60;
                res = mifare_ultralight_basic_read_pages((uint8_t)atoi(argv[3]), (uint8_t)atoi(argv[4]), data, &len);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: read pages %d - %d: ", atoi(argv[3]), atoi(argv[4]));
                for (i = 0; i < len; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("otp", argv[2]) == 0)
            {
                if (strcmp("-write", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    uint8_t data[4];
                    uint32_t size;
                    mifare_ultralight_storage_t type;
                    
                    /* init */
                    res = mifare_ultralight_basic_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    /* search */
                    res = mifare_ultralight_basic_search(&type, id, 50);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    /* ouput the type */
                    if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                    }
                    else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                    }
                    else
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                    }
                    mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                    for (i = 0; i < 8; i++)
                    {
                        mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                    }
                    mifare_ultralight_interface_debug_print("\n");
                    
                    /* get the data */
                    size = (uint8_t)strlen(argv[4]);
                    for (i = 0; i < size; i += 2)
                    {
                        if (('A' <= argv[4][i]) && (argv[4][i] <= 'Z'))
                        {
                            data[i / 2] = argv[4][i] - 'A' + 10;
                            data[i / 2] *= 16;
                        }
                        else if (('a' <= argv[4][i]) && (argv[4][i] <= 'z'))
                        {
                            data[i / 2] = argv[4][i] - 'a' + 10;
                            data[i / 2] *= 16;
                        }
                        else if (('0' <= argv[4][i]) && (argv[4][i] <= '9'))
                        {
                            data[i / 2] = argv[4][i] - '0';
                            data[i / 2] *= 16;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        if (('A' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'Z'))
                        {
                            data[i / 2 ] += argv[4][i + 1] - 'A' + 10;
                        }
                        else if (('a' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'z'))
                        {
                            data[i / 2 ] += argv[4][i + 1] - 'a' + 10;
                        }
                        else if (('0' <= argv[4][i + 1]) && (argv[4][i + 1] <= '9'))
                        {
                            data[i / 2 ] += argv[4][i + 1] - '0';
                        }
                        else
                        {
                            return 5;
                        }
                    }
                    
                    /* write otp */
                    res = mifare_ultralight_basic_write_otp(data);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    mifare_ultralight_interface_debug_print("write otp 0x%02X 0x%02X 0x%02X 0x%02X.\n", data[0], data[1], data[2], data[3]);
                    
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("inc", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                mifare_ultralight_storage_t type;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* increment the counter */
                res = mifare_ultralight_basic_increment_counter((uint8_t)atoi(argv[3]), atoi(argv[4]));
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                mifare_ultralight_interface_debug_print("addr %d increment counter %d.\n", atoi(argv[3]), atoi(argv[4]));
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else if (strcmp("set", argv[2]) == 0)
            {
                if (strcmp("-lock", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint32_t size;
                    uint8_t id[8];
                    uint8_t lock[5];
                    mifare_ultralight_storage_t type;
                    
                    /* init */
                    res = mifare_ultralight_basic_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    /* search */
                    res = mifare_ultralight_basic_search(&type, id, 50);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    /* ouput the type */
                    if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                    }
                    else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                    }
                    else
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                    }
                    mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                    for (i = 0; i < 8; i++)
                    {
                        mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                    }
                    mifare_ultralight_interface_debug_print("\n");
                    
                    /* get the data */
                    size = (uint8_t)strlen(argv[4]);
                    for (i = 0; i < size; i += 2)
                    {
                        if (('A' <= argv[4][i]) && (argv[4][i] <= 'Z'))
                        {
                            lock[i / 2] = argv[4][i] - 'A' + 10;
                            lock[i / 2] *= 16;
                        }
                        else if (('a' <= argv[4][i]) && (argv[4][i] <= 'z'))
                        {
                            lock[i / 2] = argv[4][i] - 'a' + 10;
                            lock[i / 2] *= 16;
                        }
                        else if (('0' <= argv[4][i]) && (argv[4][i] <= '9'))
                        {
                            lock[i / 2] = argv[4][i] - '0';
                            lock[i / 2] *= 16;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        if (('A' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'Z'))
                        {
                            lock[i / 2 ] += argv[4][i + 1] - 'A' + 10;
                        }
                        else if (('a' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'z'))
                        {
                            lock[i / 2 ] += argv[4][i + 1] - 'a' + 10;
                        }
                        else if (('0' <= argv[4][i + 1]) && (argv[4][i + 1] <= '9'))
                        {
                            lock[i / 2 ] += argv[4][i + 1] - '0';
                        }
                        else
                        {
                            return 5;
                        }
                    }
                    
                    /* set the lock */
                    res = mifare_ultralight_basic_set_lock(lock);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    mifare_ultralight_interface_debug_print("mifare_ultralight: lock is ");
                    for (i = 0; i < 5; i++)
                    {
                        mifare_ultralight_interface_debug_print("0x%02X ", lock[i]);
                    }
                    mifare_ultralight_interface_debug_print("\n");
                    
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 0;
                }
                else if (strcmp("-mode", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    mifare_ultralight_modulation_mode_t mode;
                    mifare_ultralight_storage_t type;
                    
                    /* init */
                    res = mifare_ultralight_basic_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    /* search */
                    res = mifare_ultralight_basic_search(&type, id, 50);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    /* ouput the type */
                    if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                    }
                    else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                    }
                    else
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                    }
                    mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                    for (i = 0; i < 8; i++)
                    {
                        mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                    }
                    mifare_ultralight_interface_debug_print("\n");
                    
                    /* get the mode */
                    if (strcmp("NORMAL", argv[4]) == 0)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: set the normal mode.\n");
                        mode = MIFARE_ULTRALIGHT_MODULATION_MODE_NORMAL;
                    }
                    else if (strcmp("STRONG", argv[4]) == 0)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: set the strong mode.\n");
                        mode = MIFARE_ULTRALIGHT_MODULATION_MODE_STRONG;
                    }
                    else
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 5;
                    }

                    /* set the static lock */
                    res = mifare_ultralight_basic_set_modulation_mode(mode);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 0;
                }
                else if (strcmp("-protect", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    mifare_ultralight_storage_t type;
                    
                    /* init */
                    res = mifare_ultralight_basic_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    /* search */
                    res = mifare_ultralight_basic_search(&type, id, 50);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    /* ouput the type */
                    if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                    }
                    else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                    }
                    else
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                    }
                    mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                    for (i = 0; i < 8; i++)
                    {
                        mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                    }
                    mifare_ultralight_interface_debug_print("\n");
                    
                    /* set protect start page */
                    res = mifare_ultralight_basic_set_protect_start_page((uint8_t)atoi(argv[4]));
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    mifare_ultralight_interface_debug_print("mifare_ultralight: set protect start page %d.\n", atoi(argv[4]));
                    
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 0;
                }
                else if (strcmp("-limitation", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    mifare_ultralight_storage_t type;
                    
                    /* init */
                    res = mifare_ultralight_basic_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    /* search */
                    res = mifare_ultralight_basic_search(&type, id, 50);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    /* ouput the type */
                    if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                    }
                    else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                    }
                    else
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                    }
                    mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                    for (i = 0; i < 8; i++)
                    {
                        mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                    }
                    mifare_ultralight_interface_debug_print("\n");
                    
                    /* set authenticate limitation */
                    res = mifare_ultralight_basic_set_authenticate_limitation((uint8_t)atoi(argv[4]));
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    mifare_ultralight_interface_debug_print("mifare_ultralight: set authenticate limitation %d.\n", atoi(argv[4]));
                    
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            if (strcmp("authenticate", argv[2]) == 0)
            {
                uint8_t res;
                uint8_t i;
                uint8_t id[8];
                uint8_t pwd[4];
                uint8_t pack[2];
                uint32_t size;
                mifare_ultralight_storage_t type;
                
                /* init */
                res = mifare_ultralight_basic_init();
                if (res != 0)
                {
                    return 1;
                }
                
                /* search */
                res = mifare_ultralight_basic_search(&type, id, 50);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                /* ouput the type */
                if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                }
                else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                }
                else
                {
                    mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                }
                mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                for (i = 0; i < 8; i++)
                {
                    mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                }
                mifare_ultralight_interface_debug_print("\n");
                
                /* get the pwd */
                size = (uint8_t)strlen(argv[3]);
                for (i = 0; i < size; i += 2)
                {
                    if (('A' <= argv[3][i]) && (argv[3][i] <= 'Z'))
                    {
                        pwd[i / 2] = argv[3][i] - 'A' + 10;
                        pwd[i / 2] *= 16;
                    }
                    else if (('a' <= argv[3][i]) && (argv[3][i] <= 'z'))
                    {
                        pwd[i / 2] = argv[3][i] - 'a' + 10;
                        pwd[i / 2] *= 16;
                    }
                    else if (('0' <= argv[3][i]) && (argv[3][i] <= '9'))
                    {
                        pwd[i / 2] = argv[3][i] - '0';
                        pwd[i / 2] *= 16;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (('A' <= argv[3][i + 1]) && (argv[3][i + 1] <= 'Z'))
                    {
                        pwd[i / 2 ] += argv[3][i + 1] - 'A' + 10;
                    }
                    else if (('a' <= argv[3][i + 1]) && (argv[3][i + 1] <= 'z'))
                    {
                        pwd[i / 2 ] += argv[3][i + 1] - 'a' + 10;
                    }
                    else if (('0' <= argv[3][i + 1]) && (argv[3][i + 1] <= '9'))
                    {
                        pwd[i / 2 ] += argv[3][i + 1] - '0';
                    }
                    else
                    {
                        return 5;
                    }
                }
                
                /* get the pack */
                size = (uint8_t)strlen(argv[4]);
                for (i = 0; i < size; i += 2)
                {
                    if (('A' <= argv[4][i]) && (argv[4][i] <= 'Z'))
                    {
                        pack[i / 2] = argv[4][i] - 'A' + 10;
                        pack[i / 2] *= 16;
                    }
                    else if (('a' <= argv[4][i]) && (argv[4][i] <= 'z'))
                    {
                        pack[i / 2] = argv[4][i] - 'a' + 10;
                        pack[i / 2] *= 16;
                    }
                    else if (('0' <= argv[4][i]) && (argv[4][i] <= '9'))
                    {
                        pack[i / 2] = argv[4][i] - '0';
                        pack[i / 2] *= 16;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (('A' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'Z'))
                    {
                        pack[i / 2 ] += argv[4][i + 1] - 'A' + 10;
                    }
                    else if (('a' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'z'))
                    {
                        pack[i / 2 ] += argv[4][i + 1] - 'a' + 10;
                    }
                    else if (('0' <= argv[4][i + 1]) && (argv[4][i + 1] <= '9'))
                    {
                        pack[i / 2 ] += argv[4][i + 1] - '0';
                    }
                    else
                    {
                        return 5;
                    }
                }
                
                /* authenticate */
                res = mifare_ultralight_basic_authenticate(pwd, pack);
                if (res != 0)
                {
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 1;
                }
                
                mifare_ultralight_interface_debug_print("mifare_ultralight: authenticate password 0x%02X 0x%02X 0x%02X 0x%02X ok.\n",
                                              pwd[0], pwd[1], pwd[2], pwd[3]);
                mifare_ultralight_interface_debug_print("mifare_ultralight: authenticate pack 0x%02X 0x%02X ok.\n", pack[0], pack[1]);
                
                (void)mifare_ultralight_basic_deinit();
                
                return 0;
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 6)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("set", argv[2]) == 0)
            {
                if (strcmp("-access", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint8_t id[8];
                    mifare_ultralight_storage_t type;
                    mifare_ultralight_access_t access_param;
                    mifare_ultralight_bool_t enable;
                    
                    /* init */
                    res = mifare_ultralight_basic_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    /* search */
                    res = mifare_ultralight_basic_search(&type, id, 50);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    /* ouput the type */
                    if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                    }
                    else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                    }
                    else
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                    }
                    mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                    for (i = 0; i < 8; i++)
                    {
                        mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                    }
                    mifare_ultralight_interface_debug_print("\n");
                    
                    if (strcmp("READ_PROTECTION", argv[4]) == 0)
                    {
                        access_param = MIFARE_ULTRALIGHT_ACCESS_READ_PROTECTION;
                        mifare_ultralight_interface_debug_print("mifare_ultralight: set access read protection ");
                    }
                    else if (strcmp("USER_CONF_PROTECTION", argv[4]) == 0)
                    {
                        access_param = MIFARE_ULTRALIGHT_ACCESS_USER_CONF_PROTECTION;
                        mifare_ultralight_interface_debug_print("mifare_ultralight: set access user configuration protection ");
                    }
                    else
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 5;
                    }
                    
                    if (strcmp("0", argv[5]) == 0)
                    {
                        enable = MIFARE_ULTRALIGHT_BOOL_FALSE;
                        mifare_ultralight_interface_debug_print("false.\n");
                    }
                    else if (strcmp("1", argv[5]) == 0)
                    {
                        enable = MIFARE_ULTRALIGHT_BOOL_TRUE;
                        mifare_ultralight_interface_debug_print("true.\n");
                    }
                    else
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 5;
                    }
                    
                    /* set the access */
                    res = mifare_ultralight_basic_set_access(access_param, enable);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 0;
                }
                else if (strcmp("-pwd", argv[3]) == 0)
                {
                    uint8_t res;
                    uint8_t i;
                    uint32_t size;
                    uint8_t id[8];
                    uint8_t pwd[4];
                    uint8_t pack[2];
                    mifare_ultralight_storage_t type;
                    
                    /* init */
                    res = mifare_ultralight_basic_init();
                    if (res != 0)
                    {
                        return 1;
                    }
                    
                    /* search */
                    res = mifare_ultralight_basic_search(&type, id, 50);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    /* ouput the type */
                    if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
                    }
                    else if (type == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul21 card.\n");
                    }
                    else
                    {
                        mifare_ultralight_interface_debug_print("mifare_ultralight: invalid type.\n");
                    }
                    mifare_ultralight_interface_debug_print("mifare_ultralight: id is ");
                    for (i = 0; i < 8; i++)
                    {
                        mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
                    }
                    mifare_ultralight_interface_debug_print("\n");
                    
                    /* get the pwd */
                    size = (uint8_t)strlen(argv[4]);
                    for (i = 0; i < size; i += 2)
                    {
                        if (('A' <= argv[4][i]) && (argv[4][i] <= 'Z'))
                        {
                            pwd[i / 2] = argv[4][i] - 'A' + 10;
                            pwd[i / 2] *= 16;
                        }
                        else if (('a' <= argv[4][i]) && (argv[4][i] <= 'z'))
                        {
                            pwd[i / 2] = argv[4][i] - 'a' + 10;
                            pwd[i / 2] *= 16;
                        }
                        else if (('0' <= argv[4][i]) && (argv[4][i] <= '9'))
                        {
                            pwd[i / 2] = argv[4][i] - '0';
                            pwd[i / 2] *= 16;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        if (('A' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'Z'))
                        {
                            pwd[i / 2 ] += argv[4][i + 1] - 'A' + 10;
                        }
                        else if (('a' <= argv[4][i + 1]) && (argv[4][i + 1] <= 'z'))
                        {
                            pwd[i / 2 ] += argv[4][i + 1] - 'a' + 10;
                        }
                        else if (('0' <= argv[4][i + 1]) && (argv[4][i + 1] <= '9'))
                        {
                            pwd[i / 2 ] += argv[4][i + 1] - '0';
                        }
                        else
                        {
                            return 5;
                        }
                    }
                    
                    /* get the pack */
                    size = (uint8_t)strlen(argv[5]);
                    for (i = 0; i < size; i += 2)
                    {
                        if (('A' <= argv[5][i]) && (argv[5][i] <= 'Z'))
                        {
                            pack[i / 2] = argv[5][i] - 'A' + 10;
                            pack[i / 2] *= 16;
                        }
                        else if (('a' <= argv[5][i]) && (argv[5][i] <= 'z'))
                        {
                            pack[i / 2] = argv[5][i] - 'a' + 10;
                            pack[i / 2] *= 16;
                        }
                        else if (('0' <= argv[5][i]) && (argv[5][i] <= '9'))
                        {
                            pack[i / 2] = argv[5][i] - '0';
                            pack[i / 2] *= 16;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        if (('A' <= argv[5][i + 1]) && (argv[5][i + 1] <= 'Z'))
                        {
                            pack[i / 2 ] += argv[5][i + 1] - 'A' + 10;
                        }
                        else if (('a' <= argv[5][i + 1]) && (argv[5][i + 1] <= 'z'))
                        {
                            pack[i / 2 ] += argv[5][i + 1] - 'a' + 10;
                        }
                        else if (('0' <= argv[5][i + 1]) && (argv[5][i + 1] <= '9'))
                        {
                            pack[i / 2 ] += argv[5][i + 1] - '0';
                        }
                        else
                        {
                            return 5;
                        }
                    }
                    
                    /* set the password pack */
                    res = mifare_ultralight_basic_set_password_pack(pwd, pack);
                    if (res != 0)
                    {
                        (void)mifare_ultralight_basic_deinit();
                        
                        return 1;
                    }
                    
                    mifare_ultralight_interface_debug_print("mifare_ultralight: set password 0x%02X 0x%02X 0x%02X 0x%02X ok.\n",
                                                  pwd[0], pwd[1], pwd[2], pwd[3]);
                    mifare_ultralight_interface_debug_print("mifare_ultralight: set pack 0x%02X 0x%02X ok.\n", pack[0], pack[1]);
                    
                    (void)mifare_ultralight_basic_deinit();
                    
                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart1 init */
    uart1_init(115200);
    
    /* shell init && register mifare_ultralight fuction */
    shell_init();
    shell_register("mifare_ultralight", mifare_ultralight);
    uart1_print("mifare_ultralight: welcome to libdriver mifare_ultralight.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart1_read(g_buf, 256);
        if (g_len)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart1_print("mifare_ultralight: run failed.\n");
            }
            else if (res == 2)
            {
                uart1_print("mifare_ultralight: unknow command.\n");
            }
            else if (res == 3)
            {
                uart1_print("mifare_ultralight: length is too long.\n");
            }
            else if (res == 4)
            {
                uart1_print("mifare_ultralight: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart1_print("mifare_ultralight: param is invalid.\n");
            }
            else
            {
                uart1_print("mifare_ultralight: unknow status code.\n");
            }
            uart1_flush();
        }
        delay_ms(100);
    }
}
