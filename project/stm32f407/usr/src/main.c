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
 * @date      2022-06-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/06/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_mifare_ultralight_basic.h"
#include "driver_mifare_ultralight_card_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "gpio.h"
#include "getopt.h"
#include <math.h>
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];             /**< uart buffer */
volatile uint16_t g_len;        /**< uart buffer length */

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
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"access", required_argument, NULL, 1},
        {"addr", required_argument, NULL, 2},
        {"data", required_argument, NULL, 3},
        {"enable", required_argument, NULL, 4},
        {"inc", required_argument, NULL, 5},
        {"mode", required_argument, NULL, 6},
        {"limit", required_argument, NULL, 7},
        {"pack", required_argument, NULL, 8},
        {"page", required_argument, NULL, 9},
        {"pwd", required_argument, NULL, 10},
        {"start", required_argument, NULL, 11},
        {"stop", required_argument, NULL, 12},
        {"lock", required_argument, NULL, 13},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    mifare_ultralight_access_t access = MIFARE_ULTRALIGHT_ACCESS_READ_PROTECTION;
    mifare_ultralight_modulation_mode_t mode = MIFARE_ULTRALIGHT_MODULATION_MODE_STRONG;
    mifare_ultralight_bool_t enable = MIFARE_ULTRALIGHT_BOOL_FALSE;
    uint8_t addr = 0;
    uint8_t page = 10;
    uint8_t start = 0;
    uint8_t stop = 3;
    uint8_t limit = 7;
    uint32_t dat = 0x00000000;
    uint32_t inc = 0;
    uint8_t pack[2] = {0x00, 0x00};
    uint8_t pwd[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t lock[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* access */
            case 1 :
            {
                /* set the access */
                if (strcmp("READ_PROTECTION", optarg) == 0)
                {
                    access = MIFARE_ULTRALIGHT_ACCESS_READ_PROTECTION;
                }
                else if (strcmp("USER_CONF_PROTECTION", optarg) == 0)
                {
                    access = MIFARE_ULTRALIGHT_ACCESS_USER_CONF_PROTECTION;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* addr */
            case 2 :
            {
                /* set the addr */
                if (strcmp("0", optarg) == 0)
                {
                    addr = 0;
                }
                else if (strcmp("1", optarg) == 0)
                {
                    addr = 1;
                }
                else if (strcmp("2", optarg) == 0)
                {
                    addr = 2;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* data */
            case 3 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }
                dat = hex_data & 0xFFFFFFFF;

                break;
            }

            /* enable */
            case 4 :
            {
                /* set the bool */
                if (strcmp("true", optarg) == 0)
                {
                    enable = MIFARE_ULTRALIGHT_BOOL_TRUE;
                }
                else if (strcmp("false", optarg) == 0)
                {
                    enable = MIFARE_ULTRALIGHT_BOOL_FALSE;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* inc */
            case 5 :
            {
                /* set the increment */
                inc = atol(optarg);

                break;
            }

            /* mode */
            case 6 :
            {
                /* set the mode */
                if (strcmp("NORMAL", optarg) == 0)
                {
                    mode = MIFARE_ULTRALIGHT_MODULATION_MODE_NORMAL;
                }
                else if (strcmp("STRONG", optarg) == 0)
                {
                    mode = MIFARE_ULTRALIGHT_MODULATION_MODE_STRONG;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* limit */
            case 7 :
            {
                /* set the limit */
                limit = atol(optarg);
                if (limit > 7)
                {
                    return 5;
                }

                break;
            }

            /* pack */
            case 8 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }
                pack[1] = (hex_data >> 0) & 0xFF;
                pack[0] = (hex_data >> 8) & 0xFF;

                break;
            }

            /* page */
            case 9 :
            {
                /* set the page */
                page = atol(optarg);

                break;
            }

            /* pwd */
            case 10 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }
                pwd[3] = (hex_data >> 0) & 0xFF;
                pwd[2] = (hex_data >> 8) & 0xFF;
                pwd[1] = (hex_data >> 16) & 0xFF;
                pwd[0] = (hex_data >> 24) & 0xFF;

                break;
            }

            /* start */
            case 11 :
            {
                /* set the start address */
                start = atol(optarg);

                break;
            }

            /* stop */
            case 12 :
            {
                /* set the stop address */
                stop = atol(optarg);

                break;
            }

            /* lock */
            case 13 :
            {
                char *p;
                uint16_t l;
                uint16_t i;
                uint64_t hex_data;

                /* set the data */
                l = strlen(optarg);

                /* check the header */
                if (l >= 2)
                {
                    if (strncmp(optarg, "0x", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else if (strncmp(optarg, "0X", 2) == 0)
                    {
                        p = optarg + 2;
                        l -= 2;
                    }
                    else
                    {
                        p = optarg;
                    }
                }
                else
                {
                    p = optarg;
                }

                /* init 0 */
                hex_data = 0;

                /* loop */
                for (i = 0; i < l; i++)
                {
                    if ((p[i] <= '9') && (p[i] >= '0'))
                    {
                        hex_data += (p[i] - '0') * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'F') && (p[i] >= 'A'))
                    {
                        hex_data += ((p[i] - 'A') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else if ((p[i] <= 'f') && (p[i] >= 'a'))
                    {
                        hex_data += ((p[i] - 'a') + 10) * (uint32_t)pow(16, l - i - 1);
                    }
                    else
                    {
                        return 5;
                    }
                }
                lock[4] = (hex_data >> 0) & 0xFF;
                lock[3] = (hex_data >> 8) & 0xFF;
                lock[2] = (hex_data >> 16) & 0xFF;
                lock[1] = (hex_data >> 24) & 0xFF;
                lock[0] = (hex_data >> 32) & 0xFF;

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_card", type) == 0)
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
    else if (strcmp("e_halt", type) == 0)
    {
        uint8_t res;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* basic halt */
        res = mifare_ultralight_basic_halt();
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: halt.\n");

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_wake-up", type) == 0)
    {
        uint8_t res;

        /* basic init */
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

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: wake up.\n");

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_otp-read", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        uint8_t data[4];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* output */
        mifare_ultralight_interface_debug_print("read otp 0x%02X 0x%02X 0x%02X 0x%02X.\n", data[0], data[1], data[2], data[3]);

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_otp-write", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        uint8_t data[4];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* write otp */
        data[3] = (dat >> 0) & 0xFF;
        data[2] = (dat >> 8) & 0xFF;
        data[1] = (dat >> 16) & 0xFF;
        data[0] = (dat >> 24) & 0xFF;
        res = mifare_ultralight_basic_write_otp(data);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("write otp 0x%02X 0x%02X 0x%02X 0x%02X.\n", data[0], data[1], data[2], data[3]);

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        uint8_t data[4];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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
        res = mifare_ultralight_basic_read(page, data);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: read page %d: ", page);
        for (i = 0; i < 4; i++)
        {
            mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
        }
        mifare_ultralight_interface_debug_print("\n");

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_read-pages", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        uint8_t data[60];
        uint16_t len;
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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
        res = mifare_ultralight_basic_read_pages(start, stop, data, &len);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: read pages %d - %d: ", start, stop);
        for (i = 0; i < len; i++)
        {
            mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
        }
        mifare_ultralight_interface_debug_print("\n");

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_read4", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        uint8_t data[16];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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
        res = mifare_ultralight_basic_read_four_pages(page, data);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: read page %d: ", page);
        for (i = 0; i < 16; i++)
        {
            mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
        }
        mifare_ultralight_interface_debug_print("\n");

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_write", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        uint8_t data[4];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* write data */
        data[3] = (dat >> 0) & 0xFF;
        data[2] = (dat >> 8) & 0xFF;
        data[1] = (dat >> 16) & 0xFF;
        data[0] = (dat >> 24) & 0xFF;
        res = mifare_ultralight_basic_write(page, data);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: write page %d: ", page);
        for (i = 0; i < 4; i++)
        {
            mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
        }
        mifare_ultralight_interface_debug_print("\n");

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_version", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;
        mifare_ultralight_version_t version;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: fixed_header is 0x%02X\n", version.fixed_header);
        mifare_ultralight_interface_debug_print("mifare_ultralight: vendor_id is 0x%02X\n", version.vendor_id);
        mifare_ultralight_interface_debug_print("mifare_ultralight: product_type is 0x%02X\n", version.product_type);
        mifare_ultralight_interface_debug_print("mifare_ultralight: product_subtype is 0x%02X\n", version.product_subtype);
        mifare_ultralight_interface_debug_print("mifare_ultralight: major_product_version is 0x%02X\n", version.major_product_version);
        mifare_ultralight_interface_debug_print("mifare_ultralight: minor_product_version is 0x%02X\n", version.minor_product_version);
        mifare_ultralight_interface_debug_print("mifare_ultralight: storage_size is 0x%02X\n", version.storage_size);
        mifare_ultralight_interface_debug_print("mifare_ultralight: protocol_type is 0x%02X\n", version.protocol_type);

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_counter", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;
        uint32_t cnt;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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
        res = mifare_ultralight_basic_read_counter(addr, &cnt);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("addr %d read counter %d.\n", addr, cnt);

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_check", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;
        uint8_t flag;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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
        res = mifare_ultralight_basic_check_tearing_event(addr, &flag);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("addr %d check the tearing event 0x%02X.\n", addr, flag);

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_counter-inc", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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
        res = mifare_ultralight_basic_increment_counter(addr, inc);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("addr %d increment counter %d.\n", addr, inc);

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_signature", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;
        uint8_t signature[32];

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: signature is ");
        for (i = 0; i < 32; i++)
        {
            mifare_ultralight_interface_debug_print("0x%02X ", signature[i]);
        }
        mifare_ultralight_interface_debug_print("\n");

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_serial", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;
        uint8_t number[7];

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: serial number is ");
        for (i = 0; i < 7; i++)
        {
            mifare_ultralight_interface_debug_print("0x%02X ", id[i]);
        }
        mifare_ultralight_interface_debug_print("\n");

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_set-pwd", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* set the password pack */
        res = mifare_ultralight_basic_set_password_pack(pwd, pack);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: set password 0x%02X 0x%02X 0x%02X 0x%02X ok.\n",
                                                pwd[0], pwd[1], pwd[2], pwd[3]);
        mifare_ultralight_interface_debug_print("mifare_ultralight: set pack 0x%02X 0x%02X ok.\n", pack[0], pack[1]);

        /* basic deint */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_lock", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* set the lock */
        res = mifare_ultralight_basic_set_lock(lock);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: lock is ");
        for (i = 0; i < 5; i++)
        {
            mifare_ultralight_interface_debug_print("0x%02X ", lock[i]);
        }
        mifare_ultralight_interface_debug_print("\n");

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_set-mode", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* set the static lock */
        res = mifare_ultralight_basic_set_modulation_mode(mode);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_set-protect", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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
        res = mifare_ultralight_basic_set_protect_start_page(page);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: set protect start page %d.\n", page);

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_set-limit", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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
        res = mifare_ultralight_basic_set_authenticate_limitation(limit);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: set authenticate limitation %d.\n", limit);

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_set-access", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* set the access */
        res = mifare_ultralight_basic_set_access(access, enable);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("e_authenticate", type) == 0)
    {
        uint8_t res;
        uint8_t i;
        uint8_t id[8];
        mifare_ultralight_storage_t type_s;

        /* basic init */
        res = mifare_ultralight_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* search */
        res = mifare_ultralight_basic_search(&type_s, id, 50);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output type */
        if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL11)
        {
            mifare_ultralight_interface_debug_print("mifare_ultralight: find mf0ul11 card.\n");
        }
        else if (type_s == MIFARE_ULTRALIGHT_STORAGE_MF0UL21)
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

        /* authenticate */
        res = mifare_ultralight_basic_authenticate(pwd, pack);
        if (res != 0)
        {
            (void)mifare_ultralight_basic_deinit();

            return 1;
        }

        /* output */
        mifare_ultralight_interface_debug_print("mifare_ultralight: authenticate password 0x%02X 0x%02X 0x%02X 0x%02X ok.\n",
                                                pwd[0], pwd[1], pwd[2], pwd[3]);
        mifare_ultralight_interface_debug_print("mifare_ultralight: authenticate pack 0x%02X 0x%02X ok.\n", pack[0], pack[1]);

        /* basic deinit */
        (void)mifare_ultralight_basic_deinit();

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        mifare_ultralight_interface_debug_print("Usage:\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-i | --information)\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-h | --help)\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-p | --port)\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-t card | --test=card)\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e halt | --example=halt)\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e wake-up | --example=wake-up)\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e read | --example=read) [--page=<addr>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e read-pages | --example=read-pages) [--start=<taddr>] [--stop=<paddr>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e read4 | --example=read4) [--page=<addr>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e write | --example=write) [--page=<addr>] [--data=<hex>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e version | --example=version)\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e otp-read | --example=otp-read)\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e otp-write | --example=otp-write) [--data=<hex>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e counter | --example=counter) [--addr=<0 | 1 | 2>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e check | --example=check) [--addr=<0 | 1 | 2>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e counter-inc | --example=counter-inc) [--addr=<0 | 1 | 2>] [--inc=<data>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e signature | --example=signature)\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e serial | --example=serial)\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e set-pwd | --example=set-pwd) [--pwd=<password>] [--pack=<pak>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e lock | --example=lock) [--lock=<hex>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e set-mode | --example=set-mode) [--mode=<NORMAL | STRONG>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e set-protect | --example=set-protect) [--page=<addr>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e set-limit | --example=set-limit) [--limit=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e set-access | --example=set-access) [--access=<READ_PROTECTION | USER_CONF_PROTECTION>] [--enable=<true | false>]\n");
        mifare_ultralight_interface_debug_print("  mifare_ultralight (-e authenticate | --example=authenticate) [--pwd=<password>] [--pack=<pak>]\n");
        mifare_ultralight_interface_debug_print("\n");
        mifare_ultralight_interface_debug_print("Options:\n");
        mifare_ultralight_interface_debug_print("      --access=<READ_PROTECTION | USER_CONF_PROTECTION>\n");
        mifare_ultralight_interface_debug_print("                                 Set access mode.([default: READ_PROTECTION])\n");
        mifare_ultralight_interface_debug_print("      --addr=<0 | 1 | 2>         Set counter address.([default: 0])\n");
        mifare_ultralight_interface_debug_print("      --data=<hex>               Set opt write data and it is hexadecimal.([default: 0x00000000])\n");
        mifare_ultralight_interface_debug_print("  -e <halt | wake-up | read | read-pages | read4 | write | version | otp-read\n");
        mifare_ultralight_interface_debug_print("     | otp-write | counter | check | counter-inc | signature | set-pwd\n");
        mifare_ultralight_interface_debug_print("     | lock | set-mode | set-protect | set-limit | set-access | authenticate>, --example=<halt\n");
        mifare_ultralight_interface_debug_print("     | wake-up | read | read-pages | read4 | write | version | otp-read\n");
        mifare_ultralight_interface_debug_print("     | otp-write | counter | check | counter-inc | signature | set-pwd\n");
        mifare_ultralight_interface_debug_print("     | lock | set-mode | set-protect | set-limit | set-access | authenticate>\n");
        mifare_ultralight_interface_debug_print("                                 Run the driver example.\n");
        mifare_ultralight_interface_debug_print("      --enable=<true | false>    Set access bool.([default: false])\n");
        mifare_ultralight_interface_debug_print("  -h, --help                     Show the help.\n");
        mifare_ultralight_interface_debug_print("  -i, --information              Show the chip information.\n");
        mifare_ultralight_interface_debug_print("      --inc=<data>               Set counter increment.([default: 0])\n");
        mifare_ultralight_interface_debug_print("      --mode=<NORMAL | STRONG>   Set chip mode.([default: STRONG])\n");
        mifare_ultralight_interface_debug_print("      --limit=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>\n");
        mifare_ultralight_interface_debug_print("                                 Set the limit times.([default: 7])\n");
        mifare_ultralight_interface_debug_print("      --lock=<hex>               Set the lock data.([default: 0x0000000000])\n");
        mifare_ultralight_interface_debug_print("  -p, --port                     Display the pin connections of the current board.\n");
        mifare_ultralight_interface_debug_print("      --pack=<pak>               Set the pack authentication and it is hexadecimal.([default: 0x0000])\n");
        mifare_ultralight_interface_debug_print("      --page=<addr>              Set read or write page address.([default: 10])\n");
        mifare_ultralight_interface_debug_print("      --pwd=<password>           Set the password authentication and it is hexadecimal.([default: 0xFFFFFFFF])\n");
        mifare_ultralight_interface_debug_print("      --start=<taddr>            Set read pages start address.([default: 0])\n");
        mifare_ultralight_interface_debug_print("      --stop=<paddr>             Set read pages stop address.([default: 3])\n");
        mifare_ultralight_interface_debug_print("  -t <card>, --test=<card>       Run the driver test.\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
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
    else if (strcmp("p", type) == 0)
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

    /* uart init */
    uart_init(115200);

    /* shell init && register mifare_ultralight function */
    shell_init();
    shell_register("mifare_ultralight", mifare_ultralight);
    uart_print("mifare_ultralight: welcome to libdriver mifare_ultralight.\n");

    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("mifare_ultralight: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("mifare_ultralight: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("mifare_ultralight: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("mifare_ultralight: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("mifare_ultralight: param is invalid.\n");
            }
            else
            {
                uart_print("mifare_ultralight: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
