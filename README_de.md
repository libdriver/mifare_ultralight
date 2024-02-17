[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MIFARE_Ultralight

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/mifare_ultralight/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

NXP Semiconductors hat das MIFARE Ultralight EV1 MF0ULx1 für den Einsatz in einem kontaktlosen Smart Ticket, einer Smart Card oder einem Token in Kombination mit einem Proximity Coupling Device (PCD) entwickelt. Der MF0ULx1 wurde für den Betrieb in einer ISO/IEC 14443 Typ-A-konformen Umgebung entwickelt. Zu den Zielanwendungen zählen Einzelfahrt- oder Zeitfahrkarten in öffentlichen Verkehrsnetzen, Kundenkarten oder Tageskarten für Veranstaltungen. Der MF0ULx1 dient als Ersatz für herkömmliche Ticketing-Lösungen wie Papiertickets, Magnetstreifentickets oder Münzen. Es dient als Ticketing-Gegenstück zu kontaktlosen Kartenfamilien wie MIFARE DESFire oder MIFARE Plus. Das MIFARE Ultralight EV1 tritt die Nachfolge des MIFARE Ultralight Ticketing IC an und ist voll funktionsfähig abwärtskompatibel. Sein erweiterter Funktions- und Befehlssatz ermöglicht effizientere Implementierungen und bietet mehr Flexibilität bei Systemdesigns. Die mechanischen und elektrischen Spezifikationen von MIFARE Ultralight EV1 sind auf die Anforderungen von Inlay- und Papierticketherstellern zugeschnitten.

LibDriver MIFARE_Ultralight ist der voll funktionsfähige Treiber von MIFARE_Ultralight, der von LibDriver gestartet wurde. Er bietet die Funktion zum Lesen von Seiten, Schreiben von Seiten usw. LibDriver ist MISRA-konform.

Dieses Repository enthält ein Submodul und Sie sollten "--recursive" hinzufügen, um das gesamte Projekt zu klonen.

```shell
git clone https://github.com/libdriver/mifare_ultralight.git --recursive
```

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver MIFARE_Ultralight-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver MIFARE_Ultralight RF.

/test enthält den Testcode des LibDriver MIFARE_Ultralight-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver MIFARE_Ultralight-Beispielcode.

/doc enthält das LibDriver MIFARE_Ultralight-Offlinedokument.

/Datenblatt enthält MIFARE_Ultralight-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige RF-Schnittstellenvorlage und stellen Sie Ihren Plattform-RF-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/mifare_ultralight/index.html](https://www.libdriver.com/docs/mifare_ultralight/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.