### 1. Chip

#### 1.1 Chip Info

chip name : STM32F407ZGT6.

extern oscillator : 8MHz.

uart pin: TX/RX PA9/PA10.

iic pin: SCL/SDA PB8/PB9.

spi pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

gpio pin: INT PB0.

reset pin: RESET PA8.

### 2. Shell

#### 2.1 Shell Parameter

baud rate: 115200.

data bits : 8.

stop bits: 1.

parity: none.

flow control: none.

### 3. MIFARE_ULTRALIGHT

#### 3.1 Command Instruction

​           mifare_ultralight is a basic command which can test all mifare_ultralight driver function:

​           -i          show mifare_ultralight chip and driver information.

​           -h        show mifare_ultralight help.

​           -p        show mifare_ultralight pin connections of the current board.

​           -t card        run mifare_ultralight card test. 

​           -c (halt | wake_up | read <page> | read_pages <startpage> <stoppage> | read4 <startpage> | write <page> | version | check <addr> | otp (-read | -write <data>) | counter <addr> | inc <addr>  <cnt> | signature | serial_number | set (-pwd <pwd> <pack> | -lock <lock> | -mode <mode> | -protect <page> | -limitation <limit> | -access <access> <enable>) | authenticate <pwd> <pack>)

​           -c halt        chip halt.

​           -c wake_up        chip wake up.

​           -c read <page>        chip read page.page is the read page.

​           -c read_pages <startpage> <stoppage>        chip read pages.startpage is the start page,stoppage is the stop page.

​           -c read4 <startpage>        chip read four pages.startpage is the start page.

​           -c write <page>        chip write page.page is the written page.

​           -c version        get the version.

​           -c counter <addr>        get the chip read counter.addr is the read counter address.

​           -c inc <addr>  <cnt>      increment the chip counter.addr is the increment counter address.cnt is the increment counter.

​           -c signature        get the chip signature.

​           -c serial_number        get the chip serial number.

​           -c set -pwd <pwd> <pack>        set the password.pwd is the password,pack is the checked pack.

​           -c set -lock <lock>        set the lock.lock is the set lock.

​           -c set -mode <mode>        set the mode.mode can be "NORMAL" or "STRONG".

​           -c set -protect <page>        set the start protect page.page is the start page.

​           -c set -limitation <limit>        set the authenticate limitation.limit is the set limit and it can be 0 - 7.

​           -c set -access <access> <enable>        set the access.access can be "READ_PROTECTION", "USER_CONF_PROTECTION".enable is the set bool and it can be 0 or 1.

​           -c authenticate <pwd> <pack>        password authenticate .pwd is the password,pack is the checked pack.

​           -c otp -read        read the otp data.

​           -c otp -write <data>        write the otp data.data is the written data.

​           -c check <addr>        check the tearing event.addr is the check address.

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
0x04 0xB0 0xA7 0x9B 0x12 0xFE 0x48 0x80 0x24 0x48 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 
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
mifare_ultralight: read counter 17.
mifare_ultralight: check tearing event flag 0xBD.
mifare_ultralight: finish card test.
```

```shell
mifare_ultralight -c halt

mifare_ultralight: halt.
```

```shell
mifare_ultralight -c wake_up

mifare_ultralight: wake up.
```

```shell
mifare_ultralight -c otp -read

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
read otp 0x00 0x00 0x00 0x00.
```

```shell
mifare_ultralight -c otp -write 00000000

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
write otp 0x00 0x00 0x00 0x00.
```

```shell
mifare_ultralight -c read 0

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: read page 0: 0x04 0xB0 0xA7 0x9B 
```

```shell
mifare_ultralight -c read_pages 0 3

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: read pages 0 - 3: 0x04 0xB0 0xA7 0x9B 0x12 0xFE 0x48 0x80 0x24 0x48 0x00 0x00 0x00 0x00 0x00 0x00 
```

```shell
mifare_ultralight -c read4 0

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: read page 0: 0x04 0xB0 0xA7 0x9B 0x12 0xFE 0x48 0x80 0x24 0x48 0x00 0x00 0x00 0x00 0x00 0x00 
```

```shell
mifare_ultralight -c write 10 12345678 

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: write page 10: 0x12 0x34 0x56 0x78 
```

```shell
mifare_ultralight -c version

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
mifare_ultralight -c counter 0

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
addr 0 read counter 17.
```

```shell
mifare_ultralight -c check 0

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
addr 0 check the tearing event 0xBD.
```

```shell
mifare_ultralight -c inc 0 1

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
addr 0 increment counter 1.
```

```shell
mifare_ultralight -c signature

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: signature is 0xF1 0x37 0xB2 0xC3 0x9B 0x06 0x10 0x64 0x56 0x9C 0x06 0x93 0x5F 0x2F 0x2F 0xB6 0x3F 0x3F 0x3D 0x5E 0x5D 0xFE 0x51 0xA8 0x05 0x36 0x67 0xF4 0x9E 0x8E 0xF3 0x62 
```

```shell
mifare_ultralight -c serial_number

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: serial number is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 
```

```shell
mifare_ultralight -c set -pwd FFFFFFFF 0000

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: set password 0xFF 0xFF 0xFF 0xFF ok.
mifare_ultralight: set pack 0x00 0x00 ok.
```

```shell
mifare_ultralight -c set -lock 0000000000

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: lock is 0x00 0x00 0x00 0x00 0x00 
```

```shell
mifare_ultralight -c set -mode STRONG

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: set the strong mode.
```

```shell
mifare_ultralight -c set -protect 255

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: set protect start page 255.
```

```shell
mifare_ultralight -c set -limitation 7

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: set authenticate limitation 7.
```

```shell
mifare_ultralight -c set -access READ_PROTECTION 0

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: set access read protection false.
```

```shell
mifare_ultralight -c authenticate FFFFFFFF 0000

mifare_ultralight: find mf0ul11 card.
mifare_ultralight: id is 0x88 0x04 0xB0 0xA7 0x12 0xFE 0x48 0x80 
mifare_ultralight: authenticate password 0xFF 0xFF 0xFF 0xFF ok.
mifare_ultralight: authenticate pack 0x00 0x00 ok.
```


```shell
mifare_ultralight -h

mifare_ultralight -i
	show mifare_ultralight chip and driver information.
mifare_ultralight -h
	show mifare_ultralight help.
mifare_ultralight -p
	show mifare_ultralight pin connections of the current board.
mifare_ultralight -t card
	run mifare_ultralight card test.
mifare_ultralight -c halt
	chip halt.
mifare_ultralight -c wake_up
	chip wake up.
mifare_ultralight -c read <page>
	chip read page.page is the read page.
mifare_ultralight -c read_pages <startpage> <stoppage>
	chip read pages.startpage is the start page,stoppage is the stop page.
mifare_ultralight -c read4 <startpage>
	chip read four pages.startpage is the start page.
mifare_ultralight -c write <page>
	chip write page.page is the written page.
mifare_ultralight -c version
	get the version.
mifare_ultralight -c counter <addr>
	get the chip read counter.addr is the read counter address.
mifare_ultralight -c inc <addr> <cnt>
	increment the chip counter.addr is the increment counter address.cnt is the increment counter.
mifare_ultralight -c otp -read
	read the otp data.
mifare_ultralight -c otp -write <data>
	write the otp data.data is the written data.
mifare_ultralight -c check <addr>
	check the tearing event.addr is the check address.
mifare_ultralight -c signature
	get the chip signature.
mifare_ultralight -c serial_number
	get the chip serial number.
mifare_ultralight -c set -pwd <pwd> <pack>
	set the password.pwd is the password,pack is the checked pack.
mifare_ultralight -c set -lock <lock>
	set the lock.lock is the set lock.
mifare_ultralight -c set -mode <mode>
	set the mode.mode can be "NORMAL" or "STRONG".
mifare_ultralight -c set -protect <page>
	set the start protect page.page is the start page.
mifare_ultralight -c set -limitation <limit>
	set the authenticate limitation.limit is the set limit and it can be 0 - 7.
mifare_ultralight -c set -access <access> <enable>
	set the access.access can be "READ_PROTECTION", "USER_CONF_PROTECTION".enable is the set bool and it can be 0 or 1.
mifare_ultralight -c authenticate <pwd> <pack>
	password authenticate.pwd is the password,pack is the checked pack.
```
