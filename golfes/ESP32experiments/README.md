# Generics

Some [information on Github](https://github.com/espressif/arduino-esp32/issues/544)

[Getting started](https://docs.micropython.org/en/latest/esp32/tutorial/intro.html) with Micropython on the ESP.

[PinOut reference and general, very useful, info](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)


[DOIT ESP32 DEVKIT V1 – 30 PINES](https://www.esploradores.com/eligiendo_una_placa_esp32/)

*    ENLACES DE COMPRA: https://es.banggood.com   –  https://www.amazon.es  –  https://www.ebay.com   –  https://es.aliexpress.com –  https://tienda.bricogeek.com
*    DOCUMENTACIÓN: Descripción. Mapa de pines. Esquema.
*    MODULO: ESP32-WROOM-32 – USB/UART: CP2102
*    SRAM: 520 KB – FLASH: 4 MB – psRAM: –
*    PULSADORES: EN y BOOT  – LED_BUILTIN: GPIO02-AZUL
*    EXTRAS: –

# Flash Firmware

* Download firmware from [here](https://micropython.org/download#esp32). I have used GENERIC : esp32-idf3-20200407-v1.12-351-gbd5633778.bin

* Install esptool

   pip3 install esptool
   pip3 show esptool

If the [python the executable does not work](https://stackoverflow.com/questions/35898734/pip-installs-packages-successfully-but-executables-not-found-from-command-line/35899029), add this to .bashrc file

`export PATH="$HOME/.local/bin:$PATH"`
    
* Press the flash button and write

Hello

`esptool.py --port /dev/ttyUSB0 erase_flash`
 
 MAC: 4c:11:ae:c8:e1:a8

* Press the flash button and wirte

`esptool.py --chip esp32 --port /dev/ttyUSB0 write_flash -z 0x1000 esp32-idf3-20200407-v1.12-351-gbd5633778.bin`

* Log into the ESP32

`picocom /dev/ttyUSB0 -b115200`

**NOTE: to disconnect picocom use ```<Ctrl-a><Ctrl-x>```**

# First Tests

## LED on and off

```
>>> import machine
>>> pin = machine.Pin(2, machine.Pin.OUT)
>>> pin.on()
>>> pin.off()

```

## Read Digital Input
```
>>> pin_en=machine.Pin(25,machine.Pin.IN)
>>> print(pin_en.value())
0
>>> print(pin_en.value())
1

```
## Read Analog Input
```
>>> pot=machine.ADC(machine.Pin(33))
>>> pot.width(machine.ADC.WIDTH_10BIT)
>>> pot.atten(machine.ADC.ATTN_11DB)
>>> print(pot.read())
532
>>> print(pot.read())
691
>>> print(pot.read())
1023
```


## Connect to network
```
picocom /dev/ttyUSB0 -b115200

>>> import network
>>> sta_if.active(True)
>>> sta_if.connect('...mySSID...','...passwd...')

I (53522837) network: CONNECTED
I (53522877) wifi: AP's beacon interval = 102400 us, DTIM period = 1
I (53523797) event: sta ip: 192.168.1.25, mask: 255.255.255.0, gw: 192.168.1.2
I (53523797) network: GOT_IP
```
## Connect to a server
On the server side
```
netcat -l 9000
```
On the ESP32
```
>>> s=socket.socket()
>>> s.connect(('192.168.1.15',9000))
>>> s.sendall(b'Hello\n')
```
## Run files on the ESP

A good tutorial can be found [here}(https://www.digikey.es/en/maker/projects/micropython-basics-load-files-run-code/fb1fcedaf11e4547943abfdd8ad825ce)

*  Install ampy
   ```
   pip3 install adafruit-ampy
   ```
*  Edit a local file and send it to be executed on the ESP:
   ```
   ampy --port /dev/ttyUSB0 run test.py
   ```
   This sends the output to the local window
*  As an alternative, send the file, execute in on the ESP and **then** log in the ESP with picocom
   ```
   ampy --port /dev/ttyUSB0 run --no-output test.py
   ```
## Configuring AMPY

Edit configuration file
```
nano ~\.ampy
```
And write this
```
AMPY_PORT=/dev/ttyUSB0
AMPY_BAUD=115200
AMPY_DELAY=0.5
```
After this, you can type 
```
ampy ls
```
to list the files on the ESP
