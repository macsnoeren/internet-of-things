# Workshop Finite State Machine
Welcome at the github project that supports the workshop Finite State Machines (FSM). During this workshop we zoom into the embedded software development. Based on an Internet-Of-Thing (IoT) platform, the FSM design pattern is explained. This embedded platform will be made available during the workshop itself.

# Assignment: Traffic Light One
The project we are going to implement is de Traffic Light IoT. We are going to create one traffic light with the Wemos D1 Mini embedded system and one switch. This embedded device connects with a Wi-Fi access point and therefore communicate with some REST backends or MQTT. So, you will not only learn how to make robust embedded software! You will also learn to program a full IoT software stack!

[tlo]()

## Hardware
* ESP8266 D1 Mini
* 2x led green
* 2x led yellow
* 2x led red
* 2x tactical switch
* 1x bread board
* A bunch of wires

Please see the [schematic](https://github.com/macsnoeren/internet-of-things/blob/development/workshop-fsm/Schematic%20Traffic%20Light%20Project.pdf) to make the correct wiring with the switches and leds.

## Finite State Machine
The finite state machine can be found [here](https://github.com/macsnoeren/internet-of-things/blob/development/workshop-fsm/FSM%20Traffic%20Light%20One.pdf). In this FSM the following states have been defined:
1. START
2. WIFI_CONNECT
3. MQTT_CONNECT
4. RED
5. GREEN
6. ORANGE
7. OUT_OF_ORDER

To transit from one state to another, the following events have been defined:
1. READY
2. BUTTON_1
3. TIMER
4. OUT_OF_ORDER
5. START
6. ERROR

# Assignment: Impl