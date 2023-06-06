### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

GPIO Pin: INT PB0.

RESET Pin: RESET PA8.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. MIFARE_ULTRALIGHT

#### 3.1 Command Instruction

1. Show mifare_ultralight chip and driver information.

   ```shell
   mifare_ultralight (-i | --information)
   ```

2. Show mifare_ultralight help.

   ```shell
   mifare_ultralight (-h | --help)
   ```

3. Show mifare_ultralight pin connections of the current board.

   ```shell
   mifare_ultralight (-p | --port)
   ```

4. Run mifare_ultralight card test.

   ```shell
   mifare_ultralight (-t card | --test=card)
   ```

5. Run chip halt function.

   ```shell
   mifare_ultralight (-e halt | --example=halt)
   ```

6. Run chip wake up function.

   ```shell
   mifare_ultralight (-e wake-up | --example=wake-up)
   ```

7. Run chip read page function, addr is the read page address.

   ```shell
   mifare_ultralight (-e read | --example=read) [--page=<addr>]
   ```

8. Run chip read pages function, taddr is the start page address, paddr is the stop page address.

   ```shell
   mifare_ultralight (-e read-pages | --example=read-pages) [--start=<taddr>] [--stop=<paddr>]
   ```

9. Run chip read four pages function, addr is the start page address.

   ```shell
   mifare_ultralight (-e read4 | --example=read4) [--page=<addr>]
   ```

10. Run chip write page function, addr is the write page, hex is the write data.

    ```shell
    mifare_ultralight (-e write | --example=write) [--page=<addr>] [--data=<hex>]
    ```

11. Run get version function.

    ```shell
    mifare_ultralight (-e version | --example=version)
    ```

12. Run get chip read counter function.

    ```shell
    mifare_ultralight (-e counter | --example=counter) [--addr=<0 | 1 | 2>]
    ```

13. Run increment chip counter function, data is the increment counter.

    ```shell
    mifare_ultralight (-e counter-inc | --example=counter-inc) [--addr=<0 | 1 | 2>] [--inc=<data>]
    ```

14. Run get chip signature function.

    ```shell
    mifare_ultralight (-e signature | --example=signature)
    ```

15. Run get chip serial number function.

    ```shell
    mifare_ultralight (-e serial | --example=serial)
    ```

16. Run set password function, password is the set password, pak is the checked pack.

    ```shell
    mifare_ultralight (-e set-pwd | --example=set-pwd) [--pwd=<password>] [--pack=<pak>]
    ```

17. Run set lock function, hex is the set lock.

    ```shell
    mifare_ultralight (-e lock | --example=lock) [--lock=<hex>]
    ```

18. Run set mode function.

    ```shell
    mifare_ultralight (-e set-mode | --example=set-mode) [--mode=<NORMAL | STRONG>]
    ```

19. Run set start protect page function, addr is the start page address.

    ```shell
    mifare_ultralight (-e set-protect | --example=set-protect) [--page=<addr>]
    ```

20. Run set authenticate limitation function.

    ```shell
    mifare_ultralight (-e set-limit | --example=set-limit) [--limit=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]
    ```

21. Run set access function.

    ```shell
    mifare_ultralight (-e set-access | --example=set-access) [--access=<READ_PROTECTION | USER_CONF_PROTECTION>] [--enable=<true | false>]
    ```

22. Run password authenticate function, password is the set password, pak is the checked pack.

    ```shell
    mifare_ultralight (-e authenticate | --example=authenticate) [--pwd=<password>] [--pack=<pak>]
    ```

23. Run read otp data function.

    ```shell
    mifare_ultralight (-e otp-read | --example=otp-read)
    ```

24. Run write otp data function, hex is the write data.

    ```shell
    mifare_ultralight (-e otp-write | --example=otp-write) [--data=<hex>]
    ```

25. Run check tearing event function.

    ```shell
    mifare_ultralight (-e check | --example=check) [--addr=<0 | 1 | 2>]
    ```

#### 3.2 Command Example

```shell
mifare_ultralight -i

mifare_ultralight: chip is NXP Ultralight.
mifare_ultralight: manufacturer is NXP.
mifare_ultralight: interface is RF.
mifare_ultralight: driver version is 1.0.
mifare_ultralight: min supply voltage is 3.3V.
mifare_ultralight: max supply voltage is 4.0V.
mifare_ultralight: max current is 30.00mA.
mifare_ultralight: max temperature is 70.0C.
mifare_ultralight: min temperature is -25.0C.
```

```shell
mifare_ultralight -p

mifare_ultralight: SPI interface SCK connected to GPIOA PIN5.
mifare_ultralight: SPI interface MISO connected to GPIOA PIN6.
mifare_ultralight: SPI interface MOSI connected to GPIOA PIN7.
mifare_ultralight: SPI interface CS connected to GPIOA PIN4.
mifare_ultralight: IIC interface SCL connected to GPIOB PIN8.
mifare_ultralight: IIC interface SDA connected to GPIOB PIN9.
mifare_ultralight: UART interface TX connected to GPIOA PIN2.
mifare_ultralight: UART interface RX connected to GPIOA PIN3.
mifare_ultralight: INT connected to GPIOB PIN0.
mifare_ultralight: RESET connected to GPIOA PIN8.
```

```shell
mifare_ultralight -t card

mifare_ultralight: chip is NXP Ultralight.
mifare_ultralight: manufacturer is NXP.
mifare_ultralight: interface is RF.
mifare_ultralight: driver version is 1.0.
mifare_ultralight: min supply voltage is 3.3V.
mifare_ultralight: max supply voltage is 4.0V.
mifare_ultralight: max current is 30.00mA.
mifare_ultralight: max temperature is 70.0C.
mifare_ultralight: min temperature is -25.0C.
mifare_ultralight: start card test.
mifare_ultralight: find ultralight card.
mifare_ultralight: id1 is 0x88 0x04 0xB0 0xA7.
mifare_ultralight: id2 is 0x12 0xFE 0x48 0x80.
mifare_ultralight: serial number is 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: signature is 0xF1 0x37 0xB2 0xC3 0x9B 0x06 0x10 0x64 0x56 0x9C 0x06 0x93 0x5F 0x2F 0x2F 0xB6 0x3F 0x3F 0x3D 0x5E 0x5D 0xFE 0x51 0xA8 0x05 0x36 0x67 0xF4 0x9E 0x8E 0xF3 0x62 
mifare_ultralight: read four pages from page 0.
0x04 0xB0 0xA7 0x9B 0x12 0xFE 0x48 0x80 0x24 0x48 0x00 0x00 0x00 0x00 0x00 0x00 
mifare_ultralight: read page 3.
0x00 0x00 0x00 0x00 
mifare_ultralight: fast read page from page 0 - page 5.
0x04 0xB0 0xA7 0x9B 0x12 0xFE 0x48 0x80 0x24 0x48 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x7B 0x0B 0x00 0x08 0x00 0x00 0x00 0x00 
mifare_ultralight: compatibility write page 16.
mifare_ultralight: check page ok.
mifare_ultralight: write page 17.
mifare_ultralight: check page ok.
mifare_ultralight: fixed_header is 0x00
mifare_ultralight: vendor_id is 0x04
mifare_ultralight: product_type is 0x03
mifare_ultralight: product_subtype is 0x01
mifare_ultralight: major_product_version is 0x01
mifare_ultralight: minor_product_version is 0x00
mifare_ultralight: storage_size is 0x0B
mifare_ultralight: protocol_type is 0x03
mifare_ultralight: authenticate.
mifare_ultralight: set lock 0x00 0x00 0x00 0x00 0x00
mifare_ultralight: set lock 0x00 0x00 0x00 0x00 0x00
mifare_ultralight: set normal mode.
mifare_ultralight: check modulation mode ok.
mifare_ultralight: set strong mode.
mifare_ultralight: check modulation mode ok.
mifare_ultralight: set protect start page 0xFF.
mifare_ultralight: check protect start page ok.
mifare_ultralight: read access protection disable.
mifare_ultralight: check read access protection ok.
mifare_ultralight: user configuration protection disable.
mifare_ultralight: check user configuration protection ok.
mifare_ultralight: set authenticate limitation 7.
mifare_ultralight: check authenticate limitation ok.
mifare_ultralight: set virtual card type identifier 5.
mifare_ultralight: check virtual card type identifier ok.
mifare_ultralight: write otp 0x00 0x00 0x00 0x00.
mifare_ultralight: check opt ok.
mifare_ultralight: increment counter 1.
mifare_ultralight: read counter 26.
mifare_ultralight: check tearing event flag 0xBD.
mifare_ultralight: finish card test.
```

```shell
mifare_ultralight -e halt

mifare_ultralight: halt.
```

```shell
mifare_ultralight -e wake-up

mifare_ultralight: wake up.
```

```shell
mifare_ultralight -e otp-read

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
read otp 0x00 0x00 0x00 0x00.
```

```shell
mifare_ultralight -e otp-write --data=0x00000000

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
write otp 0x00 0x00 0x00 0x00.
```

```shell
mifare_ultralight -e read --page=0

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: read page 0: 0x04 0xB0 0xA7 0x9B 
```

```shell
mifare_ultralight -e read-pages --start=0 --stop=3

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: read pages 0 - 3: 0x04 0xB0 0xA7 0x9B 0x12 0xFE 0x48 0x80 0x24 0x48 0x00 0x00 0x00 0x00 0x00 0x00 
```

```shell
mifare_ultralight -e read4 --page=0

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: read page 0: 0x04 0xB0 0xA7 0x9B 0x12 0xFE 0x48 0x80 0x24 0x48 0x00 0x00 0x00 0x00 0x00 0x00 
```

```shell
mifare_ultralight -e write --page=10 --data=0x12345678

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: write page 10: 0x12 0x34 0x56 0x78 
```

```shell
mifare_ultralight -e version

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: fixed_header is 0x00
mifare_ultralight: vendor_id is 0x04
mifare_ultralight: product_type is 0x03
mifare_ultralight: product_subtype is 0x01
mifare_ultralight: major_product_version is 0x01
mifare_ultralight: minor_product_version is 0x00
mifare_ultralight: storage_size is 0x0B
mifare_ultralight: protocol_type is 0x03
```

```shell
mifare_ultralight -e counter --addr=0

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
addr 0 read counter 23.
```

```shell
mifare_ultralight -e check --addr=0

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
addr 0 check the tearing event 0xBD.
```

```shell
mifare_ultralight -e counter-inc --addr=0 --inc=1

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
addr 0 increment counter 1.
```

```shell
mifare_ultralight -e signature

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: signature is 0xF1 0x37 0xB2 0xC3 0x9B 0x06 0x10 0x64 0x56 0x9C 0x06 0x93 0x5F 0x2F 0x2F 0xB6 0x3F 0x3F 0x3D 0x5E 0x5D 0xFE 0x51 0xA8 0x05 0x36 0x67 0xF4 0x9E 0x8E 0xF3 0x62 
```

```shell
mifare_ultralight -e serial

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: serial number is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 
```

```shell
mifare_ultralight -e set-pwd --pwd=0xFFFFFFFF --pack=0x0000

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: set password 0xFF 0xFF 0xFF 0xFF ok.
mifare_ultralight: set pack 0x00 0x00 ok.
```

```shell
mifare_ultralight -e lock --lock=0x0000000000

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: lock is 0x00 0x00 0x00 0x00 0x00 
```

```shell
mifare_ultralight -e set-mode --mode=STRONG

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
```

```shell
mifare_ultralight -e set-protect --page=255

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: set protect start page 255.
```

```shell
mifare_ultralight -e set-limit --limit=7

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: set authenticate limitation 7.
```

```shell
mifare_ultralight -e set-access --access=READ_PROTECTION --enable=false

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
```

```shell
mifare_ultralight -e authenticate --pwd=0xFFFFFFFF --pack=0x0000

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: authenticate password 0xFF 0xFF 0xFF 0xFF ok.
mifare_ultralight: authenticate pack 0x00 0x00 ok.
```


```shell
mifare_ultralight -h

Usage:
  mifare_ultralight (-i | --information)
  mifare_ultralight (-h | --help)
  mifare_ultralight (-p | --port)
  mifare_ultralight (-t card | --test=card)
  mifare_ultralight (-e halt | --example=halt)
  mifare_ultralight (-e wake-up | --example=wake-up)
  mifare_ultralight (-e read | --example=read) [--page=<addr>]
  mifare_ultralight (-e read-pages | --example=read-pages) [--start=<taddr>] [--stop=<paddr>]
  mifare_ultralight (-e read4 | --example=read4) [--page=<addr>]
  mifare_ultralight (-e write | --example=write) [--page=<addr>] [--data=<hex>]
  mifare_ultralight (-e version | --example=version)
  mifare_ultralight (-e otp-read | --example=otp-read)
  mifare_ultralight (-e otp-write | --example=otp-write) [--data=<hex>]
  mifare_ultralight (-e counter | --example=counter) [--addr=<0 | 1 | 2>]
  mifare_ultralight (-e check | --example=check) [--addr=<0 | 1 | 2>]
  mifare_ultralight (-e counter-inc | --example=counter-inc) [--addr=<0 | 1 | 2>] [--inc=<data>]
  mifare_ultralight (-e signature | --example=signature)
  mifare_ultralight (-e serial | --example=serial)
  mifare_ultralight (-e set-pwd | --example=set-pwd) [--pwd=<password>] [--pack=<pak>]
  mifare_ultralight (-e lock | --example=lock) [--lock=<hex>]
  mifare_ultralight (-e set-mode | --example=set-mode) [--mode=<NORMAL | STRONG>]
  mifare_ultralight (-e set-protect | --example=set-protect) [--page=<addr>]
  mifare_ultralight (-e set-limit | --example=set-limit) [--limit=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>]
  mifare_ultralight (-e set-access | --example=set-access) [--access=<READ_PROTECTION | USER_CONF_PROTECTION>] [--enable=<true | false>]
  mifare_ultralight (-e authenticate | --example=authenticate) [--pwd=<password>] [--pack=<pak>]

Options:
      --access=<READ_PROTECTION | USER_CONF_PROTECTION>
                                 Set access mode.([default: READ_PROTECTION])
      --addr=<0 | 1 | 2>         Set counter address.([default: 0])
      --data=<hex>               Set opt write data and it is hexadecimal.([default: 0x00000000])
  -e <halt | wake-up | read | read-pages | read4 | write | version | otp-read
     | otp-write | counter | check | counter-inc | signature | set-pwd
     | lock | set-mode | set-protect | set-limit | set-access | authenticate>, --example=<halt
     | wake-up | read | read-pages | read4 | write | version | otp-read
     | otp-write | counter | check | counter-inc | signature | set-pwd
     | lock | set-mode | set-protect | set-limit | set-access | authenticate>
                                 Run the driver example.
      --enable=<true | false>    Set access bool.([default: false])
  -h, --help                     Show the help.
  -i, --information              Show the chip information.
      --inc=<data>               Set counter increment.([default: 0])
      --mode=<NORMAL | STRONG>   Set chip mode.([default: STRONG])
      --limit=<0 | 1 | 2 | 3 | 4 | 5 | 6 | 7>
                                 Set the limit times.([default: 7])
      --lock=<hex>               Set the lock data.([default: 0x0000000000])
  -p, --port                     Display the pin connections of the current board.
      --pack=<pak>               Set the pack authentication and it is hexadecimal.([default: 0x0000])
      --page=<addr>              Set read or write page address.([default: 10])
      --pwd=<password>           Set the password authentication and it is hexadecimal.([default: 0xFFFFFFFF])
      --start=<taddr>            Set read pages start address.([default: 0])
      --stop=<paddr>             Set read pages stop address.([default: 3])
  -t <card>, --test=<card>       Run the driver test.
```
