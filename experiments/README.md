# Monitor

Es tracta de tenir un sistema per monitoritzar diversos respiradors.

## Material demanat


[Motor NEMA 17 / 3.5Kg con conector y cable!](https://tienda.bricogeek.com/motores-paso-a-paso/1360-motor-nema-17-35kg-con-conector-y-cable.html)

[Controlador de alta corriente DRV8825 !]( https://tienda.bricogeek.com/impresion-3d/853-controlador-de-alta-corriente-drv8825.html)
[Arduino CNC Shield v3 !](https://tienda.bricogeek.com/shields-arduino/837-arduino-cnc-shield-v3.html )
[Arduino Starter Kit (Español) !](https://tienda.bricogeek.com/kits-electronica-para-montar/541-arduino-starter-kit.html )
[ESP32 Wroom WIFI + Bluetooth !](https://tienda.bricogeek.com/arduino-compatibles/1274-esp32-wroom-wifi-bluetooth.html )


## MOTOR



### ESP 32

Programat des de Arduino IDE en C++

Llegeix regularment dades analógiques. En fa un processat intern i envia dades de cada
inspiració: 

* Durada cicle (ms)
* Pressió màxima del cicle (cm H2O)
* Pressió mínima del cicle (cm H2O)
* Pressió Plateau durant inspiració (cm H2O)
* Ratio inspiració/espiració (1:x) típicament entre 1:1 1:2
* Pressió al final espiració (cm H2O)

El periode de mostreig hauria de ser 20ms (màxim 60 ms)

Fa un paquet amb identificador del dispositiu i  ABC,T,max,plateau,ratio,peep i l'envia per UDP



### Raspberry Pi

Qualsevol model serveix. 


# Elements a desenvolupar

## Codi ESP

Funcionalitat recollida de dades 

Processat per calcular

Configuració: Abans de posar-lo en servei, cada trasto s'ha de connectar momentàniament 
a un PC per programar-lo amb identificador ABC que vulguin. Això s'ha 
de poder fer "en el camp".


## Core Raspberry

Produir Acces Point

Donar adreces DHCP

*Establir entorn de desenvolupament*: que s'hi puguin connectar ESP's, que s'hi pugi accedir des del 
PC de desenvolupament, que s'hi puguin instal·lar coses


## Aplicació Raspberry

Rebre paquets UDP

Guardar-los. Un fitxer CSV per cada hora i per cada respirador

Presentació d'informació en temps real. Mode text

Presentació de gràfiques en temps real. Mode gràfic

Si apapreix un nou dispositiu, genera gràfica nova

Alarmes configurables. Interfície d'usuari simple

Timer de canvi de filtre. Alarma. Interfície d'usuari



## Accessoris Raspberry

Sistema per generar alarma auditiva. Producció Audio. Altaveu
