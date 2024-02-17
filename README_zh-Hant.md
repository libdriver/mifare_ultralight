[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MIFARE_Ultralight

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mifare_ultralight/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NXP Semiconductors開發的MIFARE Ultralight EV1 MF0ULx1用於非接觸式智慧票據、智慧卡或權杖，並與接近耦合設備（PCD）結合使用。 MF0ULx1設計用於符合ISO/IEC 14443 A類標準的環境。 目標應用包括公共交通網絡中的單程票、限用票、可信卡或活動日通行證。 MF0ULx1可替代傳統的票務解決方案，如紙質票、磁條票或硬幣。 它是MIFARE DESFire或MIFARE Plus等非接觸式卡系列的票務對應產品。 MIFARE Ultralight EV1是MIFARE Ultralight售票IC的後續產品，功能完全向後相容。 其增强的功能和命令集實現了更高效的實現，並在系統設計中提供了更大的靈活性。 MIFARE Ultralight EV1的機械和電力規格是為滿足嵌入式和紙質票製造商的要求而定制的。

LibDriver MIFARE_ Ultralight是LibDriver推出的MIFARE_ Ultralight全功能驅動，該驅動提供頁讀取，頁寫入等功能並且它符合MISRA標準。

本倉庫包含子模塊，您需要增加“--recursive”選項完整克隆整個工程。

```shell
git clone https://github.com/libdriver/mifare_ultralight.git --recursive
```

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver MIFARE_Ultralight的源文件。

/interface目錄包含了LibDriver MIFARE_Ultralight與平台無關的RF總線模板。

/test目錄包含了LibDriver MIFARE_Ultralight驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver MIFARE_Ultralight編程範例。

/doc目錄包含了LibDriver MIFARE_Ultralight離線文檔。

/datasheet目錄包含了MIFARE_Ultralight數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的RF總線模板，完成指定平台的RF總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/mifare_ultralight/index.html](https://www.libdriver.com/docs/mifare_ultralight/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
