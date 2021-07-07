
## hyphop ##

==gpios test examples & utils 

http://linux-sunxi.org/GPIO

E.g for PH18 this would be ( 8 - 1) * 32 + 18 = 224 + 18 = 242 (since 'h' is the 8th letter). 

==gpio calculate num by name

abcdefghijkl
123456789abc
0123456789ab

11*32 + 03 = 355	POWER BTN
11*32 + 10 = 362	GREN POWER LED

PA17	0+17    = 17	RED LED

PG06	6*32+06 = 198
PG07	6*32+07 = 199
PG12	6*32+12 = 204
PL02	32*11+2 = 354

==fex/bin

check fex for used gpios and make its free // _used = 0

    [spi1]
    spi_used = 0
    spi_cs_bitmap = 1
    spi_cs0 = port:PA13<2><1><default><default>
    spi_sclk = port:PA14<2><default><default><default>
    spi_mosi = port:PA15<2><default><default><default>
    spi_miso = port:PA16<2><default><default><default>

next section look like not needed

    [gpio_para]
    gpio_used = 0
    gpio_num = 0
    ;gpio_pin_1 = port:PA07<1><default><default><0>
    ;gpio_pin_2 = port:PA10<1><default><default><0>
    ;gpio_pin_3 = port:PA06<1><default><default><0>
    ;gpio_pin_4 = port:PA03<1><default><default><0>
    ;gpio_pin_5 = port:PA00<1><default><default><0>
    ;gpio_pin_6 = port:PA01<1><default><default><0>

> gpio_exported 
grep: /sys/class/gpio/gpio*/edge: No such file or directory
no any exported gpios

> gpio_irq_show 0
watch IRQ gpios handles 0 sec
 43:          0          0          0          0       GIC  PA
 49:          0          0          0          0       GIC  PG
 77:          0          0          0          0       GIC  PL

> egpios_test 
Using GPIO 14 16 198 199
wait
have 4 events
1111
wait
have 1 events
1101
wait
have 1 events
1111
wait
have 1 events
1110
wait
have 1 events
1111
wait
^C

## make enxport for used gpios before use it in epool again

> gpio_irq_show 0
watch IRQ gpios handles 0 sec
  6:          2          0          0          0  sunxi_gpio_irq_chip  gpiolib
  7:          2          0          0          0  sunxi_gpio_irq_chip  gpiolib
 14:          0          0          0          0  sunxi_gpio_irq_chip  gpiolib
 43:          0          0          0          0       GIC  PA
 49:          4          0          0          0       GIC  PG
 77:          0          0          0          0       GIC  PL
160:          0          0          0          0  sunxi_gpio_irq_chip  gpiolib


> gpio_exported 
/sys/class/gpio/gpio14/edge:both
/sys/class/gpio/gpio16/edge:both
/sys/class/gpio/gpio198/edge:both
/sys/class/gpio/gpio199/edge:both
/sys/class/gpio/gpio14/direction:in
/sys/class/gpio/gpio16/direction:in
/sys/class/gpio/gpio198/direction:in
/sys/class/gpio/gpio199/direction:in
/sys/class/gpio/gpio14/value:1
/sys/class/gpio/gpio16/value:1
/sys/class/gpio/gpio198/value:1
/sys/class/gpio/gpio199/value:1

> egpios_test 
unexport gpio 14
unexport gpio 16
unexport gpio 198
unexport gpio 199
Using GPIO 14 16 198 199
wait
have 4 events
1111
wait
have 1 events
0111

....

have 2 events
1111
wait
have 2 events
1100

....

> gpio_irq_show 0
watch IRQ gpios handles 0 sec
  6:        435          0          0          0  sunxi_gpio_irq_chip  gpiolib
  7:        633          0          0          0  sunxi_gpio_irq_chip  gpiolib
 14:        331          0          0          0  sunxi_gpio_irq_chip  gpiolib
 43:        730          0          0          0       GIC  PA
 49:        897          0          0          0       GIC  PG
 77:          0          0          0          0       GIC  PL
160:        589          0          0          0  sunxi_gpio_irq_chip  gpiolib

## make enxport for used gpios before use it in epool again

> egpios_clean 
unexport gpio 14
unexport gpio 16
unexport gpio 198
unexport gpio 199


