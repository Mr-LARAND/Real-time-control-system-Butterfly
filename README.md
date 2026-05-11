
# Motor Control System

![MCU](https://img.shields.io/badge/MCU-STM32F767ZI-blue)
![Protocol](https://img.shields.io/badge/Protocol-CANopen-green)
![Network](https://img.shields.io/badge/Ethernet-UDP-orange)
![IDE](https://img.shields.io/badge/IDE-STM32CubeIDE-lightgrey)


Cистема управления для драйверов двигателей EPOS2 с использованием STM32F767ZI, CANopen и Ethernet-UDP.

---

## Обзор

В этом проекте реализована система управления двигателем maxon, основанная на:

- STM32F767ZI
- CANopen protocol
- EPOS2 motor controller
- Ethernet UDP communication
- lwIP network stack

Система позволяет осуществлять дистанционное управление двигателем и передавать телеметрические данные по Ethernet, взаимодействуя с приводом через объекты CANopen PDO/SDO.

---

## Функции

- CANopen NMT state control
- SDO configuration of EPOS2
- PDO real-time telemetry
- SYNC-based communication
- UDP telemetry server
- Velocity control mode
- Position and status monitoring
- STM32 HAL + lwIP integration

---

## Hardware

- Микроконтроллер STM32F767ZI Nucleo
- Драйвер мотора EPOS2 50/5
- Мотор Maxon motor EC 45 flat 397172
- CAN Transceiver
- Отладочный инструмент CANable pro v1.0 

---

## Software Stack

- STM32CubeIDE
- STM32 HAL
- lwIP
- CANopen stack
- UDP server
