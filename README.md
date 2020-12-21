# UPC Manresa ventilator unit

This is an experimental project to design a low cost mechanical
ventilation unit based on an Ambu hand ventilator and an Arduino.  The
project includes:

* The mechanical design of a system that uses an Ambu unit as a pump.

* An specific control circuit that includes an Arduino as its main
  asset. It is responsible for driving the system motor, and the user
  interface.

* A software component that controls all the system.


## Project structure

* libvent: The base software library of the project. It's written in C
  and assumes no underlaying software other than AVR libc and
  protothreads.

* vcontrol: The control software of the ventilator unit. Builds on top
  of libvent and the specific hardware of the ventilator. It is in
  charge of the user interface of the system and the control of the
  motor.

* golfes: A directory that contains old materials of historical value
  for the project.