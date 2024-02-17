[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MIFARE_Ultralight

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mifare_ultralight/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NXP Semiconductors는 비접촉식 스마트 티켓, 스마트 카드 또는 토큰과 함께 PCD(근접 결합 장치)에 사용하기 위해 MIFARE Ultralight EV1 MF0ULx1을 개발했습니다. MF0ULx1은 ISO/IEC 14443 Type-A 호환 환경에서 작동하도록 설계되었습니다. 대상 응용 프로그램에는 단일 여행 또는 대중 교통 네트워크의 제한된 사용 티켓, 로열티 카드 또는 이벤트 당일 패스가 포함됩니다. MF0ULx1은 종이 티켓, 마그네틱 스트라이프 티켓 또는 동전과 같은 기존의 티켓팅 솔루션을 대체합니다. MIFARE DESFire 또는 MIFARE Plus와 같은 비접촉식 카드 제품군의 티켓팅 대응물 역할을 합니다. MIFARE Ultralight EV1은 MIFARE Ultralight 티켓팅 IC의 뒤를 이으며 완벽하게 이전 버전과 호환됩니다. 향상된 기능과 명령 세트는 보다 효율적인 구현을 가능하게 하고 시스템 설계에 더 많은 유연성을 제공합니다. MIFARE Ultralight EV1의 기계적 및 전기적 사양은 인레이 및 종이 티켓 제조업체의 요구 사항을 충족하도록 맞춤화되었습니다.

LibDriver MIFARE_Ultralight는 LibDriver에서 출시한 MIFARE_Ultralight의 전체 기능 드라이버입니다. 페이지 읽기, 페이지 쓰기 등의 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

이 리포지토리에는 하위 모듈이 포함되어 있으며 전체 프로젝트를 복제하려면 "--recursive"를 추가해야 합니다.

```shell
git clone https://github.com/libdriver/mifare_ultralight.git --recursive
```

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver MIFARE_Ultralight의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver MIFARE_Ultralight용 플랫폼 독립적인 RF 버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver MIFARE_Ultralight드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver MIFARE_Ultralight프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver MIFARE_Ultralight오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 MIFARE_Ultralight데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 RF 버스 템플릿을 참조하여 지정된 플랫폼에 대한 RF 버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/mifare_ultralight/index.html](https://www.libdriver.com/docs/mifare_ultralight/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.