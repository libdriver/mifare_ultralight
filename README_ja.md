[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MIFARE_Ultralight

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mifare_ultralight/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NXPセミコンダクターズは、非接触型スマートチケット、スマートカード、または近接結合デバイス（PCD）と組み合わせたトークンで使用するためのMIFARE UltralightEV1MF0ULx1を開発しました。 MF0ULx1は、ISO / IEC14443Type-A準拠の環境で動作するように設計されています。対象となるアプリケーションには、公共交通機関ネットワークでの1回の旅行または限定使用のチケット、ポイントカード、またはイベントの1日パスが含まれます。 MF0ULx1は、紙の切符、磁気ストライプの切符、硬貨などの従来の発券ソリューションの代替品として機能します。 MIFAREDESFireやMIFAREPlusなどの非接触型カードファミリの発券カウンターパートとして機能します。MIFAREUltralightEV1は、MIFARE Ultralight発券ICの後継であり、完全に機能する下位互換性があります。その強化された機能とコマンドセットにより、より効率的な実装が可能になり、システム設計の柔軟性が高まります。MIFAREUltralight EV1の機械的および電気的仕様は、インレイおよび紙のチケットメーカーの要件を満たすように調整されています。

LibDriver MIFARE_Ultralightは、LibDriverによって起動されるMIFARE_Ultralightの全機能ドライバーであり、ページの読み取り、ページの書き込みなどの機能を提供します。 LibDriverはMISRAに準拠しています。

このリポジトリにはサブモジュールが含まれており、「--recursive」を追加してプロジェクト全体を複製する必要があります。

```shell
git clone https://github.com/libdriver/mifare_ultralight.git --recursive
```

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver MIFARE_Ultralightのソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver MIFARE_Ultralight用のプラットフォームに依存しないRFバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver MIFARE_Ultralightドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver MIFARE_Ultralightプログラミング例が含まれています。

/ docディレクトリには、LibDriver MIFARE_Ultralightオフラインドキュメントが含まれています。

/ datasheetディレクトリには、MIFARE_Ultralightデータシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないRFバステンプレートを参照して、指定したプラットフォームのRFバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/mifare_ultralight/index.html](https://www.libdriver.com/docs/mifare_ultralight/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。