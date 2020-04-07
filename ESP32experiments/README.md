# Generics

Some [information on Github](https://github.com/espressif/arduino-esp32/issues/544)

[Getting started](https://docs.micropython.org/en/latest/esp32/tutorial/intro.html) with Micropython on the ESP.




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

    export PATH="$HOME/.local/bin:$PATH"
    
* Press the flash button and write

    esptool.py --port /dev/ttyUSB0 erase_flash
 
 
 MAC: 4c:11:ae:c8:e1:a8

* Press the flash button and wirte

    esptool.py --chip esp32 --port /dev/ttyUSB0 write_flash -z 0x1000 esp32-idf3-20200407-v1.12-351-gbd5633778.bin

* Log into the ESP32

    picocom /dev/ttyUSB0 -b115200
