# Monitor

Es tracta de tenir un sistema per monitoritzar diversos respiradors.

## Material demanat


[Motor NEMA 17 / 3.5Kg con conector y cable](https://tienda.bricogeek.com/motores-paso-a-paso/1360-motor-nema-17-35kg-con-conector-y-cable.html)

[Controlador de alta corriente DRV8825]( https://tienda.bricogeek.com/impresion-3d/853-controlador-de-alta-corriente-drv8825.html)

[Arduino CNC Shield v3](https://tienda.bricogeek.com/shields-arduino/837-arduino-cnc-shield-v3.html )

[Arduino Starter Kit (Español)](https://tienda.bricogeek.com/kits-electronica-para-montar/541-arduino-starter-kit.html )

[ESP32 Wroom WIFI + Bluetooth](https://tienda.bricogeek.com/arduino-compatibles/1274-esp32-wroom-wifi-bluetooth.html )


## MOTOR NEMA 17

 *   Modelo: 42BYGH40-1.8-22A
 *   Ángulo : 1.8 grados
 *   2 fases
 *   Voltaje nominal: 3.4V
 *   Corriente: **1.7A/fase**
 *   Holding Torque : 3.5kg*cm (48.6 oz/in)
 *   Detent Torque: 180g*cm
 *   Diámetro de eje: 5mm
 *   Resistance del bobinado: 2±10% Ω/fase
 *   Inductancia del bobinado: 2±20% mH/fase
 *   Flux máximo: 1.8 Vs
 *   Maximum Detent Torque: 0.016 N.M
 *   Total inertia (kg.m.m): 3.5 Kg.m.m
 *   Total friction (kg.m/s): 4 Kg.m/s
 *   Step Angle Accuracy : ±5% (full step, no load) 
 *   Temperatura de funcionamiento:  80°C   Max
 *   Cable: 50cm con conector tipo JST
 *   Cable: DFRobot FIT0278
 *   Peso: 270 gramos
 *   Dimensiones: 42x42x40mm
 *   Life: 6000 hours = 250 dies! [segons DFRobot](https://www.dfrobot.com/product-785.html)

![Motor](motor.png)


## Driver

 * Basat en DRV8825 de Texas Instruments 
 * Compatible amb A4988 de Pololu

![Driver](driver.png)

 * S'ha de limitar el corrent del driver amb el tornavís al trimmer.
   Per limitar a 1A, cal posar Vref=0.5 (Ilim(A)=2xVref(V).
   El [Video!](https://www.youtube.com/watch?feature=player_embedded&v=89BHS9hfSUk) ho explica bé

 * Al Datasheet del DRV8825 (Texas) hi ha la taula 2 que diu el corrent en funció
   del tipus de step que es fa.

 * Si es fa microstepping el corrent que dóna és realment el 100% que està programat
 * Si només es fa full stepping només s'arriba al 70%. Per tant, es podria programar corrent més alt.

 * Encara que la tensió de la font és 12V, la tensió a les bobines és menor (2V al video)


## Com usar el driver

 * Ajustament de microsteps, en funció dels pins M0 M1 i M2.
   Si es deixen oberts, hi ha pull-downs, i queda en mode full-step: 200 passos/360º
   Estan

![Imatge de com es connecten](stepper motor with arduino cnc shield)
 
