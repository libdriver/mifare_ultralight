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
 * @file      driver_mifare_ultralight.c
 * @brief     driver mifare_ultralight source file
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

#include "driver_mifare_ultralight.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "NXP Ultralight"        /**< chip name */
#define MANUFACTURER_NAME         "NXP"                   /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.3f                    /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        4.0f                    /**< chip max supply voltage */
#define MAX_CURRENT               30.0f                   /**< chip max current */
#define TEMPERATURE_MIN           -25.0f                  /**< chip min operating temperature */
#define TEMPERATURE_MAX           70.0f                   /**< chip max operating temperature */
#define DRIVER_VERSION            1000                    /**< driver version */

/**
 * @brief chip command definition
 */
#define MIFARE_ULTRALIGHT_COMMAND_REQUEST                  0x26           /**< request command */
#define MIFARE_ULTRALIGHT_COMMAND_WAKE_UP                  0x52           /**< wake up command */
#define MIFARE_ULTRALIGHT_COMMAND_ANTICOLLISION_CL1        0x9320U        /**< anti collision cl1 command */
#define MIFARE_ULTRALIGHT_COMMAND_SELECT_CL1               0x9370U        /**< select cl1 command */
#define MIFARE_ULTRALIGHT_COMMAND_ANTICOLLISION_CL2        0x9520U        /**< anti collision cl2 command */
#define MIFARE_ULTRALIGHT_COMMAND_SELECT_CL2               0x9570U        /**< select cl2 command */
#define MIFARE_ULTRALIGHT_COMMAND_HALT                     0x5000U        /**< halt command */
#define MIFARE_ULTRALIGHT_COMMAND_GET_VERSION              0x60           /**< get version command */
#define MIFARE_ULTRALIGHT_COMMAND_READ                     0x30           /**< read command */
#define MIFARE_ULTRALIGHT_COMMAND_FAST_READ                0x3A           /**< fast read command */
#define MIFARE_ULTRALIGHT_COMMAND_WRITE                    0xA2           /**< write command */
#define MIFARE_ULTRALIGHT_COMMAND_COMP_WRITE               0xA0           /**< comp write command */
#define MIFARE_ULTRALIGHT_COMMAND_READ_CNT                 0x39           /**< read cnt command */
#define MIFARE_ULTRALIGHT_COMMAND_INCR_CNT                 0xA5           /**< increment cnt command */
#define MIFARE_ULTRALIGHT_COMMAND_PWD_AUTH                 0x1B           /**< pwd auth command */
#define MIFARE_ULTRALIGHT_COMMAND_READ_SIG                 0x3C           /**< read sig command */
#define MIFARE_ULTRALIGHT_COMMAND_CHECK_TEARING_EVENT      0x3E           /**< check tearing event command */
#define MIFARE_ULTRALIGHT_COMMAND_VCSL                     0x4B           /**< vcsl command */

/**
 * @brief     crc calculation
 * @param[in] *p points to a data buffer
 * @param[in] len is the data length
 * @note      none
 */
static void a_mifare_ultralight_iso14443a_crc(uint8_t *p, uint8_t len, uint8_t output[2])
{
    uint32_t w_crc = 0x6363;
    
    do 
    {
        uint8_t  bt;
        
        bt = *p++;                                                                                        /* get one byte */
        bt = (bt ^ (uint8_t)(w_crc & 0x00FF));                                                            /* xor */
        bt = (bt ^ (bt << 4));                                                                            /* xor */
        w_crc = (w_crc >> 8) ^ ((uint32_t) bt << 8) ^ ((uint32_t) bt << 3) ^ ((uint32_t) bt >> 4);        /* get the crc */
    } while (--len);                                                                                      /* len-- */

    output[0] = (uint8_t)(w_crc & 0xFF);                                                                  /* lsb */
    output[1] = (uint8_t)((w_crc >> 8) & 0xFF);                                                           /* msb */
}

/**
 * @brief      mifare_ultralight read conf
 * @param[in]  *handle points to a mifare_ultralight handle structure
 * @param[in]  page is the page of read
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read conf failed
 * @note       none
 */
static uint8_t a_mifare_ultralight_conf_read(mifare_ultralight_handle_t *handle, uint8_t page, uint8_t data[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[5];
    uint8_t output_len;
    uint8_t output_buf[6];
    uint8_t crc_buf[2];
    
    input_len = 5;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_FAST_READ;                                          /* set the command */
    input_buf[1] = page;                                                                         /* set the start page */
    input_buf[2] = page;                                                                         /* set the stop page */
    a_mifare_ultralight_iso14443a_crc(input_buf , 3, input_buf + 3);                             /* get the crc */
    output_len = 6;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 6)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 1;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 4, crc_buf);                                   /* get the crc */
    if ((output_buf[4] == crc_buf[0]) && (output_buf[5] == crc_buf[1]))                          /* check the crc */
    {
        memcpy(data, output_buf, 4);                                                             /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 1;                                                                                /* return error */
    }
}

/**
 * @brief     mifare_ultralight write conf
 * @param[in] *handle points to a mifare_ultralight handle structure
 * @param[in] page is the page of write
 * @param[in] *data points to a data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write conf failed
 * @note      none
 */
static uint8_t a_mifare_ultralight_conf_write(mifare_ultralight_handle_t *handle, uint8_t page, uint8_t data[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_WRITE;                                              /* set the command */
    input_buf[1] = page;                                                                         /* set the setting page */
    input_buf[2] = data[0];                                                                      /* set data0 */
    input_buf[3] = data[1];                                                                      /* set data1 */
    input_buf[4] = data[2];                                                                      /* set data2 */
    input_buf[5] = data[3];                                                                      /* set data3 */
    a_mifare_ultralight_iso14443a_crc(input_buf, 6, input_buf + 6);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 1;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: ack error.\n");                                  /* ack error */
        
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_set_storage(mifare_ultralight_handle_t *handle, mifare_ultralight_storage_t storage)
{
    if (handle == NULL)                         /* check handle */
    {
        return 2;                               /* return error */
    }
    if (handle->inited != 1)                    /* check handle initialization */
    {
        return 3;                               /* return error */
    }
    
    handle->end_page = (uint8_t)storage;        /* set the storage */
    
    return 0;                                   /* success return 0 */
}

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
uint8_t mifare_ultralight_get_storage(mifare_ultralight_handle_t *handle, mifare_ultralight_storage_t *storage)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    *storage = (mifare_ultralight_storage_t)(handle->end_page);        /* get the storage */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t mifare_ultralight_init(mifare_ultralight_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                                         /* check debug_print */
    {
        return 3;                                                                            /* return error */
    }
    if (handle->contactless_init == NULL)                                                    /* check contactless_init */
    {
        handle->debug_print("mifare_ultralight: contactless_init is null.\n");               /* contactless_init is null */
        
        return 3;                                                                            /* return error */
    }
    if (handle->contactless_deinit == NULL)                                                  /* check contactless_deinit */
    {
        handle->debug_print("mifare_ultralight: contactless_deinit is null.\n");             /* contactless_deinit is null */
        
        return 3;                                                                            /* return error */
    }
    if (handle->contactless_transceiver == NULL)                                             /* check contactless_transceiver */
    {
        handle->debug_print("mifare_ultralight: contactless_transceiver is null.\n");        /* contactless_transceiver is null */
        
        return 3;                                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                                            /* check delay_ms */
    {
        handle->debug_print("mifare_ultralight: delay_ms is null.\n");                       /* delay_ms is null */
        
        return 3;                                                                            /* return error */
    }
    
    res = handle->contactless_init();                                                        /* contactless init */
    if (res != 0)                                                                            /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless init failed.\n");                /* contactless init failed */
        
        return 1;                                                                            /* return error */
    }
    handle->type = (uint8_t)MIFARE_ULTRALIGHT_TYPE_INVALID;                                  /* set the invalid type */
    handle->end_page = 0xFF;                                                                 /* set the end page */
    handle->inited = 1;                                                                      /* flag inited */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t mifare_ultralight_deinit(mifare_ultralight_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = handle->contactless_deinit();                                                /* contactless deinit */
    if (res != 0)                                                                      /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless deinit failed.\n");        /* contactless deinit failed */
        
        return 1;                                                                      /* return error */
    }
    handle->inited = 0;                                                                /* flag closed */
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t mifare_ultralight_request(mifare_ultralight_handle_t *handle, mifare_ultralight_type_t *type)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[1];
    uint8_t output_len;
    uint8_t output_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 1;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_REQUEST;                                            /* set the command */
    output_len = 2;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 2)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if ((output_buf[0] == 0x44) && (output_buf[1] == 0x00))                                      /* check classic type */
    {
        *type = MIFARE_ULTRALIGHT_TYPE_ULTRALIGHT;                                               /* ultralight */
        handle->type = *type;                                                                    /* save the type */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        *type = MIFARE_ULTRALIGHT_TYPE_INVALID;                                                  /* invalid */
        handle->type = *type;                                                                    /* save the type */
        handle->debug_print("mifare_ultralight: type is invalid.\n");                            /* type is invalid */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_wake_up(mifare_ultralight_handle_t *handle, mifare_ultralight_type_t *type)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[1];
    uint8_t output_len;
    uint8_t output_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    handle->delay_ms(1);                                                                         /* delay 1ms */
    input_len = 1;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_WAKE_UP;                                            /* set the command */
    output_len = 2;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 2)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if ((output_buf[0] == 0x44) && (output_buf[1] == 0x00))                                      /* check classic type */
    {
        *type = MIFARE_ULTRALIGHT_TYPE_ULTRALIGHT;                                               /* ultralight */
        handle->type = *type;                                                                    /* save the type */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        *type = MIFARE_ULTRALIGHT_TYPE_INVALID;                                                  /* invalid */
        handle->type = *type;                                                                    /* save the type */
        handle->debug_print("mifare_ultralight: type is invalid.\n");                            /* type is invalid */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_halt(mifare_ultralight_handle_t *handle)
{
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = (MIFARE_ULTRALIGHT_COMMAND_HALT >> 8) & 0xFF;                                 /* set the command */
    input_buf[1] = (MIFARE_ULTRALIGHT_COMMAND_HALT >> 0) & 0xFF;                                 /* set the command */
    a_mifare_ultralight_iso14443a_crc(input_buf, 2, input_buf + 2);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    (void)handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_anticollision_cl1(mifare_ultralight_handle_t *handle, uint8_t id[4])
{
    uint8_t res;
    uint8_t i;
    uint8_t check;
    uint8_t input_len;
    uint8_t input_buf[2];
    uint8_t output_len;
    uint8_t output_buf[5];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 2;                                                                               /* set the input length */
    input_buf[0] = (MIFARE_ULTRALIGHT_COMMAND_ANTICOLLISION_CL1 >> 8) & 0xFF;                    /* set the command */
    input_buf[1] = (MIFARE_ULTRALIGHT_COMMAND_ANTICOLLISION_CL1 >> 0) & 0xFF;                    /* set the command */
    output_len = 5;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 5)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    check = 0;                                                                                   /* init 0 */
    for (i = 0; i < 4; i++)                                                                      /* run 4 times */
    {
        id[i] = output_buf[i];                                                                   /* get one id */
        check ^= output_buf[i];                                                                  /* xor */
    }
    if (check != output_buf[4])                                                                  /* check the result */
    {
        handle->debug_print("mifare_ultralight: check error.\n");                                /* check error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_anticollision_cl2(mifare_ultralight_handle_t *handle, uint8_t id[4])
{
    uint8_t res;
    uint8_t i;
    uint8_t check;
    uint8_t input_len;
    uint8_t input_buf[2];
    uint8_t output_len;
    uint8_t output_buf[5];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 2;                                                                               /* set the input length */
    input_buf[0] = (MIFARE_ULTRALIGHT_COMMAND_ANTICOLLISION_CL2 >> 8) & 0xFF;                    /* set the command */
    input_buf[1] = (MIFARE_ULTRALIGHT_COMMAND_ANTICOLLISION_CL2 >> 0) & 0xFF;                    /* set the command */
    output_len = 5;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 5)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    check = 0;                                                                                   /* init 0 */
    for (i = 0; i < 4; i++)                                                                      /* run 4 times */
    {
        id[i] = output_buf[i];                                                                   /* get one id */
        check ^= output_buf[i];                                                                  /* xor */
    }
    if (check != output_buf[4])                                                                  /* check the result */
    {
        handle->debug_print("mifare_ultralight: check error.\n");                                /* check error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_select_cl1(mifare_ultralight_handle_t *handle, uint8_t id[4])
{
    uint8_t res;
    uint8_t i;
    uint8_t input_len;
    uint8_t input_buf[9];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 9;                                                                               /* set the input length */
    input_buf[0] = (MIFARE_ULTRALIGHT_COMMAND_SELECT_CL1 >> 8) & 0xFF;                           /* set the command */
    input_buf[1] = (MIFARE_ULTRALIGHT_COMMAND_SELECT_CL1 >> 0) & 0xFF;                           /* set the command */
    input_buf[6] = 0;                                                                            /* init 0 */
    for (i = 0; i < 4; i++)                                                                      /* run 4 times */
    {
        input_buf[2 + i] = id[i];                                                                /* get one id */
        input_buf[6] ^= id[i];                                                                   /* xor */
    }
    a_mifare_ultralight_iso14443a_crc(input_buf, 7, input_buf + 7);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] == 0x04)                                                                   /* check the sak */
    {
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: sak error.\n");                                  /* sak error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_select_cl2(mifare_ultralight_handle_t *handle, uint8_t id[4])
{
    uint8_t res;
    uint8_t i;
    uint8_t input_len;
    uint8_t input_buf[9];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 9;                                                                               /* set the input length */
    input_buf[0] = (MIFARE_ULTRALIGHT_COMMAND_SELECT_CL2 >> 8) & 0xFF;                           /* set the command */
    input_buf[1] = (MIFARE_ULTRALIGHT_COMMAND_SELECT_CL2 >> 0) & 0xFF;                           /* set the command */
    input_buf[6] = 0;                                                                            /* init 0 */
    for (i = 0; i < 4; i++)                                                                      /* run 4 times */
    {
        input_buf[2 + i] = id[i];                                                                /* get one id */
        input_buf[6] ^= id[i];                                                                   /* xor */
    }
    a_mifare_ultralight_iso14443a_crc(input_buf, 7, input_buf + 7);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] == 0x00)                                                                   /* check the sak */
    {
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: sak error.\n");                                  /* sak error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_get_version(mifare_ultralight_handle_t *handle, mifare_ultralight_version_t *version)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[3];
    uint8_t output_len;
    uint8_t output_buf[10];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 3;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_GET_VERSION;                                        /* set the command */
    a_mifare_ultralight_iso14443a_crc(input_buf, 1, input_buf + 1);                              /* get the crc */
    output_len = 10;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 10)                                                                        /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 8, crc_buf);                                   /* get the crc */
    if ((output_buf[8] == crc_buf[0]) && (output_buf[9] == crc_buf[1]))                          /* check the crc */
    {
        version->fixed_header = output_buf[0];                                                   /* fixed header */
        version->vendor_id = output_buf[1];                                                      /* vendor id */
        version->product_type = output_buf[2];                                                   /* product type */
        version->product_subtype = output_buf[3];                                                /* product subtype */
        version->major_product_version = output_buf[4];                                          /* major product version */
        version->minor_product_version = output_buf[5];                                          /* minor product version */
        version->storage_size = output_buf[6];                                                   /* storage size */
        if (version->storage_size == 0x0B)                                                       /* if 20 pages */
        {
            handle->end_page = MIFARE_ULTRALIGHT_STORAGE_MF0UL11;                                /* set the end page */
        }
        else if (version->storage_size == 0x0E)                                                  /* if 41 pages */
        {
            handle->end_page = MIFARE_ULTRALIGHT_STORAGE_MF0UL21;                                /* set the end page */
        }
        else
        {
                                                                                                 /* do nothing */
        }
        version->protocol_type = output_buf[7];                                                  /* protocol type */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_read_counter(mifare_ultralight_handle_t *handle, uint8_t addr, uint32_t *cnt)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[5];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (addr > 0x2)                                                                              /* check the addr */
    {
        handle->debug_print("mifare_ultralight: addr > 0x2.\n");                                 /* addr > 0x2 */
        
        return 6;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_READ_CNT;                                           /* set the command */
    input_buf[1] = addr;                                                                         /* set the address */
    a_mifare_ultralight_iso14443a_crc(input_buf, 2, input_buf + 2);                              /* get the crc */
    output_len = 5;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 5)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 3, crc_buf);                                   /* get the crc */
    if ((output_buf[3] == crc_buf[0]) && (output_buf[4] == crc_buf[1]))                          /* check the result */
    {
        *cnt = ((uint32_t)output_buf[2] << 16) | ((uint32_t)output_buf[1] << 8) |
               ((uint32_t)output_buf[0] << 0);                                                   /* set the counter */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_increment_counter(mifare_ultralight_handle_t *handle, uint8_t addr, uint32_t cnt)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (addr > 0x2)                                                                              /* check the addr */
    {
        handle->debug_print("mifare_ultralight: addr > 0x2.\n");                                 /* addr > 0x2 */
        
        return 6;                                                                                /* return error */
    }

    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_INCR_CNT;                                           /* set the command */
    input_buf[1] = addr;                                                                         /* set the address */
    input_buf[2] = (cnt >> 0) & 0xFF;                                                            /* set cnt */
    input_buf[3] = (cnt >> 8) & 0xFF;                                                            /* set cnt */
    input_buf[4] = (cnt >> 16) & 0xFF;                                                           /* set cnt */
    input_buf[5] = 0x00;
    a_mifare_ultralight_iso14443a_crc(input_buf, 6, input_buf + 6);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: ack error.\n");                                  /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_check_tearing_event(mifare_ultralight_handle_t *handle, uint8_t addr, uint8_t *flag)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[3];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (addr > 0x2)                                                                              /* check the addr */
    {
        handle->debug_print("mifare_ultralight: addr > 0x2.\n");                                 /* addr > 0x2 */
        
        return 6;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_CHECK_TEARING_EVENT;                                /* set the command */
    input_buf[1] = addr;                                                                         /* set the address */
    a_mifare_ultralight_iso14443a_crc(input_buf, 2, input_buf + 2);                              /* get the crc */
    output_len = 3;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 3)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 1, crc_buf);                                   /* get the crc */
    if ((output_buf[1] == crc_buf[0]) && (output_buf[2] == crc_buf[1]))                          /* check the result */
    {
        *flag = output_buf[0];                                                                   /* set the output buffer */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
                               uint8_t pcd_capabilities[4], uint8_t *identifier)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[23];
    uint8_t output_len;
    uint8_t output_buf[3];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 23;                                                                              /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_CHECK_TEARING_EVENT;                                /* set the command */
    memcpy(input_buf + 1, installation_identifier, 16);
    memcpy(input_buf + 17, pcd_capabilities, 4);
    a_mifare_ultralight_iso14443a_crc(input_buf, 21, input_buf + 21);                            /* get the crc */
    output_len = 3;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 3)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 1, crc_buf);                                   /* get the crc */
    if ((output_buf[1] == crc_buf[0]) && (output_buf[2] == crc_buf[1]))                          /* check the result */
    {
        *identifier = output_buf[0];                                                             /* set the output buffer */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_read_signature(mifare_ultralight_handle_t *handle, uint8_t signature[32])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[34];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_READ_SIG;                                           /* set the command */
    input_buf[1] = 0x00;                                                                         /* set the address */
    a_mifare_ultralight_iso14443a_crc(input_buf, 2, input_buf + 2);                              /* get the crc */
    output_len = 34;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 34)                                                                        /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 32, crc_buf);                                  /* get the crc */
    if ((output_buf[32] == crc_buf[0]) && (output_buf[33] == crc_buf[1]))                        /* check the result */
    {
        memcpy(signature, output_buf, 32);                                                       /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_get_serial_number(mifare_ultralight_handle_t *handle, uint8_t number[7])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[18];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_READ;                                               /* set the command */
    input_buf[1] = 0x00;                                                                         /* set the read page */
    a_mifare_ultralight_iso14443a_crc(input_buf , 2, input_buf + 2);                             /* get the crc */
    output_len = 18;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 18)                                                                        /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 16, crc_buf);                                  /* get the crc */
    if ((output_buf[16] == crc_buf[0]) && (output_buf[17] == crc_buf[1]))                        /* check the crc */
    {
        number[0] = output_buf[0];                                                               /* set the number 0 */
        number[1] = output_buf[1];                                                               /* set the number 1 */
        number[2] = output_buf[2];                                                               /* set the number 2 */
        number[3] = output_buf[4];                                                               /* set the number 3 */
        number[4] = output_buf[5];                                                               /* set the number 4 */
        number[5] = output_buf[6];                                                               /* set the number 5 */
        number[6] = output_buf[7];                                                               /* set the number 6 */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_read_four_pages(mifare_ultralight_handle_t *handle, uint8_t start_page, uint8_t data[16])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[18];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_READ;                                               /* set the command */
    input_buf[1] = start_page;                                                                   /* set the page */
    a_mifare_ultralight_iso14443a_crc(input_buf , 2, input_buf + 2);                             /* get the crc */
    output_len = 18;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 18)                                                                        /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 16, crc_buf);                                  /* get the crc */
    if ((output_buf[16] == crc_buf[0]) && (output_buf[17] == crc_buf[1]))                        /* check the crc */
    {
        memcpy(data, output_buf, 16);                                                            /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_read_page(mifare_ultralight_handle_t *handle, uint8_t page, uint8_t data[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[18];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_READ;                                               /* set the command */
    input_buf[1] = page;                                                                         /* set the page */
    a_mifare_ultralight_iso14443a_crc(input_buf , 2, input_buf + 2);                             /* get the crc */
    output_len = 18;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 18)                                                                        /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 16, crc_buf);                                  /* get the crc */
    if ((output_buf[16] == crc_buf[0]) && (output_buf[17] == crc_buf[1]))                        /* check the crc */
    {
        memcpy(data, output_buf, 4);                                                             /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_fast_read_page(mifare_ultralight_handle_t *handle, uint8_t start_page, uint8_t stop_page, uint8_t *data, uint16_t *len)
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[5];
    uint8_t output_len;
    uint8_t cal_len;
    uint8_t output_buf[64];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                                 /* check handle */
    {
        return 2;                                                                                       /* return error */
    }
    if (handle->inited != 1)                                                                            /* check handle initialization */
    {
        return 3;                                                                                       /* return error */
    }
    if (stop_page < start_page)                                                                         /* check start and stop page */
    {
        handle->debug_print("mifare_ultralight: stop_page < start_page.\n");                            /* stop_page < start_page */
        
        return 4;                                                                                       /* return error */
    }
    if (stop_page - start_page + 1 > 15)                                                                /* check start and stop page */
    {
        handle->debug_print("mifare_ultralight: stop_page - start_page + 1 is over 15.\n");             /* stop_page - start_page + 1 is over 15 */
        
        return 5;                                                                                       /* return error */
    }
    if ((*len) < (4 * (stop_page - start_page + 1)))                                                    /* check the length */
    {
        handle->debug_print("mifare_ultralight: len < %d.\n", 4 * (stop_page - start_page + 1));        /* len is invalid */
        
        return 6;                                                                                       /* return error */
    }
    
    input_len = 5;                                                                                      /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_FAST_READ;                                                 /* set the command */
    input_buf[1] = start_page;                                                                          /* set the start page */
    input_buf[2] = stop_page;                                                                           /* set the stop page */
    a_mifare_ultralight_iso14443a_crc(input_buf , 3, input_buf + 3);                                    /* get the crc */
    cal_len = 4 * (stop_page - start_page + 1);                                                         /* set the cal length */
    output_len = (uint8_t)(cal_len + 2);                                                                /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);               /* transceiver */
    if (res != 0)                                                                                       /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");                    /* contactless transceiver failed */
        
        return 1;                                                                                       /* return error */
    }
    if (output_len != (cal_len + 2))                                                                    /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                             /* output_len is invalid */
        
        return 7;                                                                                       /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, (uint8_t)cal_len, crc_buf);                           /* get the crc */
    if ((output_buf[cal_len] == crc_buf[0]) && (output_buf[cal_len + 1] == crc_buf[1]))                 /* check the crc */
    {
        memcpy(data, output_buf, cal_len);                                                              /* copy the data */
        *len = cal_len;                                                                                 /* set the length */
        
        return 0;                                                                                       /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                         /* crc error */
        
        return 8;                                                                                       /* return error */
    }
}

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
uint8_t mifare_ultralight_compatibility_write_page(mifare_ultralight_handle_t *handle, uint8_t page, uint8_t data[4])
{
    uint8_t res;
    uint8_t i;
    uint8_t input_len;
    uint8_t input_buf[18];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_COMP_WRITE;                                         /* set the command */
    input_buf[1] = page;                                                                         /* set the page */
    a_mifare_ultralight_iso14443a_crc(input_buf, 2, input_buf + 2);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: ack error.\n");                                  /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    for (i = 0; i < 4; i ++)                                                                     /* 4 times */
    {
        input_buf[i] = data[i];                                                                  /* copy data */
    }
    for (i = 0; i < 12; i ++)                                                                    /* 12 times */
    {
        input_buf[4 + i] = 0x00;                                                                 /* copy data */
    }
    a_mifare_ultralight_iso14443a_crc(input_buf, 16, input_buf + 16);                            /* get the crc */
    input_len = 18;                                                                              /* set the input length */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: ack error.\n");                                  /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_write_page(mifare_ultralight_handle_t *handle, uint8_t page, uint8_t data[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_WRITE;                                              /* set the command */
    input_buf[1] = page;                                                                         /* set the page */
    input_buf[2] = data[0];                                                                      /* set data0 */
    input_buf[3] = data[1];                                                                      /* set data1 */
    input_buf[4] = data[2];                                                                      /* set data2 */
    input_buf[5] = data[3];                                                                      /* set data3 */
    a_mifare_ultralight_iso14443a_crc(input_buf, 6, input_buf + 6);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: ack error.\n");                                  /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_authenticate(mifare_ultralight_handle_t *handle, uint8_t pwd[4], uint8_t pack[2])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[7];
    uint8_t output_len;
    uint8_t output_buf[4];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 7;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_PWD_AUTH;                                           /* set the command */
    input_buf[1] = pwd[0];                                                                       /* set pwd0 */
    input_buf[2] = pwd[1];                                                                       /* set pwd1 */
    input_buf[3] = pwd[2];                                                                       /* set pwd2 */
    input_buf[4] = pwd[3];                                                                       /* set pwd3 */
    a_mifare_ultralight_iso14443a_crc(input_buf, 5, input_buf + 5);                              /* get the crc */
    output_len = 4;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 4)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 2, crc_buf);                                   /* get the crc */
    if ((output_buf[2] == crc_buf[0]) && (output_buf[3] == crc_buf[1]))                          /* check the crc */
    {
        if ((output_buf[0] != pack[0]) || (output_buf[1] != pack[1]))                            /* check the pack */
        {
            handle->debug_print("mifare_ultralight: pack check failed.\n");                      /* pack check failed. */
            
            return 6;                                                                            /* return error */
        }
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_set_password(mifare_ultralight_handle_t *handle, uint8_t pwd[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_WRITE;                                              /* set the command */
    input_buf[1] = handle->end_page - 1;                                                         /* set the last page */
    input_buf[2] = pwd[0];                                                                       /* set pwd0 */
    input_buf[3] = pwd[1];                                                                       /* set pwd1 */
    input_buf[4] = pwd[2];                                                                       /* set pwd2 */
    input_buf[5] = pwd[3];                                                                       /* set pwd3 */
    a_mifare_ultralight_iso14443a_crc(input_buf, 6, input_buf + 6);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: ack error.\n");                                  /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_set_pack(mifare_ultralight_handle_t *handle, uint8_t pack[2])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_WRITE;                                              /* set the command */
    input_buf[1] = handle->end_page;                                                             /* set the last page */
    input_buf[2] = pack[0];                                                                      /* set pack0 */
    input_buf[3] = pack[1];                                                                      /* set pack1 */
    input_buf[4] = 0x00;                                                                         /* set 0x00 */
    input_buf[5] = 0x00;                                                                         /* set 0x00 */
    a_mifare_ultralight_iso14443a_crc(input_buf, 6, input_buf + 6);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: ack error.\n");                                  /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_set_modulation_mode(mifare_ultralight_handle_t *handle, mifare_ultralight_modulation_mode_t mode)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                            /* clear the conf */
    res = a_mifare_ultralight_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                                    /* return error */
    }
    conf[0] &= ~(1 << 2);                                                            /* clear the settings */
    conf[0] |= mode << 2;                                                            /* set the mode */
    res = a_mifare_ultralight_conf_write(handle, handle->end_page - 3, conf);        /* write conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t mifare_ultralight_get_modulation_mode(mifare_ultralight_handle_t *handle, mifare_ultralight_modulation_mode_t *mode)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                            /* clear the conf */
    res = a_mifare_ultralight_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                                    /* return error */
    }
    *mode = (mifare_ultralight_modulation_mode_t)((conf[0] >> 2) & 0x1);             /* get the conf */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t mifare_ultralight_set_protect_start_page(mifare_ultralight_handle_t *handle, uint8_t page)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                            /* clear the conf */
    res = a_mifare_ultralight_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                                    /* return error */
    }
    conf[3] = page;                                                                  /* set the page */
    res = a_mifare_ultralight_conf_write(handle, handle->end_page - 3, conf);        /* write conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t mifare_ultralight_get_protect_start_page(mifare_ultralight_handle_t *handle, uint8_t *page)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                            /* clear the conf */
    res = a_mifare_ultralight_conf_read(handle, handle->end_page - 3, conf);         /* read conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                                    /* return error */
    }
    *page = conf[3];                                                                 /* get the page */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t mifare_ultralight_set_access(mifare_ultralight_handle_t *handle, mifare_ultralight_access_t access, mifare_ultralight_bool_t enable)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                            /* clear the conf */
    res = a_mifare_ultralight_conf_read(handle, handle->end_page - 2, conf);         /* read conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                                    /* return error */
    }
    conf[0] &= ~(1 << access);                                                       /* clear the settings */
    conf[0] |= enable << access;                                                     /* set the access */
    res = a_mifare_ultralight_conf_write(handle, handle->end_page - 2, conf);        /* write conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t mifare_ultralight_get_access(mifare_ultralight_handle_t *handle, mifare_ultralight_access_t access, mifare_ultralight_bool_t *enable)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                            /* clear the conf */
    res = a_mifare_ultralight_conf_read(handle, handle->end_page - 2, conf);         /* read conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                                    /* return error */
    }
    *enable = (mifare_ultralight_bool_t)((conf[0] >> access) & 0x1);                 /* get the bool */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t mifare_ultralight_set_authenticate_limitation(mifare_ultralight_handle_t *handle, uint8_t limit)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    if (limit > 7)                                                                   /* check the limit */
    {
        handle->debug_print("mifare_ultralight: limit > 7.\n");                      /* limit > 7 */
        
        return 4;                                                                    /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                            /* clear the conf */
    res = a_mifare_ultralight_conf_read(handle, handle->end_page - 2, conf);         /* read conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                                    /* return error */
    }
    conf[0] &= ~(7 << 0);                                                            /* clear the settings */
    conf[0] |= limit << 0;                                                           /* set the limit */
    res = a_mifare_ultralight_conf_write(handle, handle->end_page - 2, conf);        /* write conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t mifare_ultralight_get_authenticate_limitation(mifare_ultralight_handle_t *handle, uint8_t *limit)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                            /* clear the conf */
    res = a_mifare_ultralight_conf_read(handle, handle->end_page - 2, conf);         /* read conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                                    /* return error */
    }
    *limit = conf[0] & 0x7;                                                          /* set the limit */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t mifare_ultralight_set_virtual_card_type_identifier(mifare_ultralight_handle_t *handle, uint8_t identifier)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                            /* clear the conf */
    res = a_mifare_ultralight_conf_read(handle, handle->end_page - 2, conf);         /* read conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                                    /* return error */
    }
    conf[1] = identifier;                                                            /* set the identifier */
    res = a_mifare_ultralight_conf_write(handle, handle->end_page - 2, conf);        /* write conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf write failed.\n");              /* conf write failed */
        
        return 1;                                                                    /* return error */
    }
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t mifare_ultralight_get_virtual_card_type_identifier(mifare_ultralight_handle_t *handle, uint8_t *identifier)
{
    uint8_t res;
    uint8_t conf[4];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    memset(conf, 0, sizeof(uint8_t) * 4);                                            /* clear the conf */
    res = a_mifare_ultralight_conf_read(handle, handle->end_page - 2, conf);         /* read conf */
    if (res != 0)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: conf read failed.\n");               /* conf read failed */
        
        return 1;                                                                    /* return error */
    }
    *identifier = conf[1];                                                           /* get the identifier */
    
    return 0;                                                                        /* success return 0 */
}

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
uint8_t mifare_ultralight_set_lock(mifare_ultralight_handle_t *handle, uint8_t lock[5])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_WRITE;                                              /* set the command */
    input_buf[1] = 0x02;                                                                         /* set the setting page */
    input_buf[2] = 0x00;                                                                         /* set 0x00 */
    input_buf[3] = 0x00;                                                                         /* set 0x00 */
    input_buf[4] = lock[0];                                                                      /* set lock0 */
    input_buf[5] = lock[1];                                                                      /* set lock1 */
    a_mifare_ultralight_iso14443a_crc(input_buf, 6, input_buf + 6);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: ack error.\n");                                  /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_WRITE;                                              /* set the command */
    input_buf[1] = handle->end_page - 4;                                                         /* set the setting page */
    input_buf[2] = lock[2];                                                                      /* set lock2 */
    input_buf[3] = lock[3];                                                                      /* set lock3 */
    input_buf[4] = lock[4];                                                                      /* set lock4 */
    input_buf[5] = 0x00;                                                                         /* set 0x00 */
    a_mifare_ultralight_iso14443a_crc(input_buf, 6, input_buf + 6);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: ack error.\n");                                  /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_get_lock(mifare_ultralight_handle_t *handle, uint8_t lock[5])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[5];
    uint8_t output_len;
    uint8_t output_buf[6];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 5;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_FAST_READ;                                          /* set the command */
    input_buf[1] = 2;                                                                            /* set the start page */
    input_buf[2] = 2;                                                                            /* set the stop page */
    a_mifare_ultralight_iso14443a_crc(input_buf , 3, input_buf + 3);                             /* get the crc */
    output_len = 6;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 6)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 4, crc_buf);                                   /* get the crc */
    if ((output_buf[4] == crc_buf[0]) && (output_buf[5] == crc_buf[1]))                          /* check the crc */
    {
        memcpy(lock, output_buf + 2, 2);                                                         /* copy the data */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
    
    input_len = 5;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_FAST_READ;                                          /* set the command */
    input_buf[1] = handle->end_page - 4;                                                         /* set the start page */
    input_buf[2] = handle->end_page - 4;                                                         /* set the stop page */
    a_mifare_ultralight_iso14443a_crc(input_buf , 3, input_buf + 3);                             /* get the crc */
    output_len = 6;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 6)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 4, crc_buf);                                   /* get the crc */
    if ((output_buf[4] == crc_buf[0]) && (output_buf[5] == crc_buf[1]))                          /* check the crc */
    {
        memcpy(lock + 2, output_buf, 3);                                                         /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_read_otp(mifare_ultralight_handle_t *handle, uint8_t data[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[4];
    uint8_t output_len;
    uint8_t output_buf[18];
    uint8_t crc_buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 4;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_READ;                                               /* set the command */
    input_buf[1] = 0x03;                                                                         /* set the page */
    a_mifare_ultralight_iso14443a_crc(input_buf , 2, input_buf + 2);                             /* get the crc */
    output_len = 18;                                                                             /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 18)                                                                        /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    a_mifare_ultralight_iso14443a_crc(output_buf, 16, crc_buf);                                  /* get the crc */
    if ((output_buf[16] == crc_buf[0]) && (output_buf[17] == crc_buf[1]))                        /* check the crc */
    {
        memcpy(data, output_buf, 4);                                                             /* copy the data */
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("mifare_ultralight: crc error.\n");                                  /* crc error */
        
        return 5;                                                                                /* return error */
    }
}

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
uint8_t mifare_ultralight_write_otp(mifare_ultralight_handle_t *handle, uint8_t data[4])
{
    uint8_t res;
    uint8_t input_len;
    uint8_t input_buf[8];
    uint8_t output_len;
    uint8_t output_buf[1];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    input_len = 8;                                                                               /* set the input length */
    input_buf[0] = MIFARE_ULTRALIGHT_COMMAND_WRITE;                                              /* set the command */
    input_buf[1] = 0x03;                                                                         /* set the page */
    input_buf[2] = data[0];                                                                      /* set data0 */
    input_buf[3] = data[1];                                                                      /* set data1 */
    input_buf[4] = data[2];                                                                      /* set data2 */
    input_buf[5] = data[3];                                                                      /* set data3 */
    a_mifare_ultralight_iso14443a_crc(input_buf, 6, input_buf + 6);                              /* get the crc */
    output_len = 1;                                                                              /* set the output length */
    res = handle->contactless_transceiver(input_buf, input_len, output_buf, &output_len);        /* transceiver */
    if (res != 0)                                                                                /* check the result */
    {
        handle->debug_print("mifare_ultralight: contactless transceiver failed.\n");             /* contactless transceiver failed */
        
        return 1;                                                                                /* return error */
    }
    if (output_len != 1)                                                                         /* check the output_len */
    {
        handle->debug_print("mifare_ultralight: output_len is invalid.\n");                      /* output_len is invalid */
        
        return 4;                                                                                /* return error */
    }
    if (output_buf[0] != 0xA)                                                                    /* check the result */
    {
        handle->debug_print("mifare_ultralight: ack error.\n");                                  /* ack error */
        
        return 5;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t mifare_ultralight_transceiver(mifare_ultralight_handle_t *handle, uint8_t *in_buf, uint8_t in_len, uint8_t *out_buf, uint8_t *out_len)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    if (handle->contactless_transceiver(in_buf, in_len, 
                                        out_buf, out_len) != 0)        /* transceiver data */
    {
        return 1;                                                      /* return error */
    }
    else
    {
        return 0;                                                      /* success return 0 */
    }
}

/**
 * @brief      get chip information
 * @param[out] *info points to a mifare_ultralight info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t mifare_ultralight_info(mifare_ultralight_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(mifare_ultralight_info_t));              /* initialize mifare_ultralight info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "RF", 8);                              /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
