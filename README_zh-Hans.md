[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MIFARE_Ultralight

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mifare_ultralight/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NXP Semiconductors开发的MIFARE Ultralight EV1 MF0ULx1用于非接触式智能票据、智能卡或令牌，并与接近耦合设备（PCD）结合使用。MF0ULx1设计用于符合ISO/IEC 14443 A类标准的环境。目标应用包括公共交通网络中的单程票、限用票、可信卡或活动日通行证。MF0ULx1可替代传统的票务解决方案，如纸质票、磁条票或硬币。它是MIFARE DESFire或MIFARE Plus等非接触式卡系列的票务对应产品。MIFARE Ultralight EV1是MIFARE Ultralight售票IC的后续产品，功能完全向后兼容。其增强的功能和命令集实现了更高效的实现，并在系统设计中提供了更大的灵活性。MIFARE Ultralight EV1的机械和电气规格是为满足嵌入式和纸质票制造商的要求而定制的。

LibDriver MIFARE_Ultralight是LibDriver推出的MIFARE_Ultralight全功能驱动，该驱动提供页读取，页写入等功能并且它符合MISRA标准。

本仓库包含子模块，您需要增加“--recursive”选项完整克隆整个工程。

```shell
git clone https://github.com/libdriver/mifare_ultralight.git --recursive
```

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver MIFARE_Ultralight的源文件。

/interface目录包含了LibDriver MIFARE_Ultralight与平台无关的RF总线模板。

/test目录包含了LibDriver MIFARE_Ultralight驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver MIFARE_Ultralight编程范例。

/doc目录包含了LibDriver MIFARE_Ultralight离线文档。

/datasheet目录包含了MIFARE_Ultralight数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的RF总线模板，完成指定平台的RF总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_mifare_ultralight_basic.h"

uint8_t res;
uint8_t i;
uint8_t id[8];
uint8_t signature[32];
uint8_t number[7];
uint8_t data[16];
uint8_t lock[5];
uint8_t pwd[4];
uint8_t pack[2];
uint32_t cnt;
uint8_t flag;
uint16_t len;
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

...
    
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

...
    
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

...
    
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

...
    
/* read data */
res = mifare_ultralight_basic_read(0, data);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}
mifare_ultralight_interface_debug_print("mifare_ultralight: read page %d: ", 0);
for (i = 0; i < 4; i++)
{
    mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
}
mifare_ultralight_interface_debug_print("\n");

...
    
/* read data */
res = mifare_ultralight_basic_read_four_pages(0, data);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}
mifare_ultralight_interface_debug_print("mifare_ultralight: read page %d: ", 0);
for (i = 0; i < 16; i++)
{
    mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
}
mifare_ultralight_interface_debug_print("\n");

...
    
/* read the counter */
res = mifare_ultralight_basic_read_counter(0, &cnt);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}
mifare_ultralight_interface_debug_print("addr %d read counter %d.\n", 0, cnt);

...
    
/* check the tearing event */
res = mifare_ultralight_basic_check_tearing_event(0, &flag);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}
mifare_ultralight_interface_debug_print("addr %d check the tearing event 0x%02X.\n", 0, flag);

...

/* read otp */
res = mifare_ultralight_basic_read_otp(data);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}
mifare_ultralight_interface_debug_print("read otp 0x%02X 0x%02X 0x%02X 0x%02X.\n", data[0], data[1], data[2], data[3]);

...
    
/* write data */
res = mifare_ultralight_basic_write(0, data);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}
mifare_ultralight_interface_debug_print("mifare_ultralight: write page %d: ", 0);
for (i = 0; i < 4; i++)
{
    mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
}
mifare_ultralight_interface_debug_print("\n");

...
    
/* read pages */
len = 8;
res = mifare_ultralight_basic_read_pages((0, 0, data, &len);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}
mifare_ultralight_interface_debug_print("mifare_ultralight: read pages %d - %d: ", 0, 0);
for (i = 0; i < len; i++)
{
    mifare_ultralight_interface_debug_print("0x%02X ", data[i]);
}
mifare_ultralight_interface_debug_print("\n");

...

/* write otp */
res = mifare_ultralight_basic_write_otp(data);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();
                        
    return 1;
}
mifare_ultralight_interface_debug_print("write otp 0x%02X 0x%02X 0x%02X 0x%02X.\n", data[0], data[1], data[2], data[3]);        

...
                                        
/* increment the counter */
res = mifare_ultralight_basic_increment_counter(0, 1);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();
                    
    return 1;
}
mifare_ultralight_interface_debug_print("addr %d increment counter %d.\n", 0, 1);

...
                                         
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

...
                                         
/* set the static lock */
res = mifare_ultralight_basic_set_modulation_mode(mode);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();
    
    return 1;
}

...
                                         
/* set protect start page */
res = mifare_ultralight_basic_set_protect_start_page(255);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}
mifare_ultralight_interface_debug_print("mifare_ultralight: set protect start page %d.\n", 255);

...
                                         
/* set authenticate limitation */
res = mifare_ultralight_basic_set_authenticate_limitation(7);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}
mifare_ultralight_interface_debug_print("mifare_ultralight: set authenticate limitation %d.\n", 7);

...
                                         
/* authenticate */
res = mifare_ultralight_basic_authenticate(pwd, pack);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}

...
                                         
/* set the access */
res = mifare_ultralight_basic_set_access(MIFARE_ULTRALIGHT_ACCESS_USER_CONF_PROTECTION, MIFARE_ULTRALIGHT_BOOL_FALSE);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}

...
                                         
/* set the password pack */
res = mifare_ultralight_basic_set_password_pack(pwd, pack);
if (res != 0)
{
    (void)mifare_ultralight_basic_deinit();

    return 1;
}

...
                                         
mifare_ultralight_interface_debug_print("mifare_ultralight: set password 0x%02X 0x%02X 0x%02X 0x%02X ok.\n",
                                        pwd[0], pwd[1], pwd[2], pwd[3]);
mifare_ultralight_interface_debug_print("mifare_ultralight: set pack 0x%02X 0x%02X ok.\n", pack[0], pack[1]);
                                         
...
    
(void)mifare_ultralight_basic_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/mifare_ultralight/index.html](https://www.libdriver.com/docs/mifare_ultralight/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。