EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "WemosTrafficLight"
Date "2021-01-30"
Rev "1.0"
Comp "Avans University of Applied Sciences"
Comment1 "Author: Maurice Snoeren"
Comment2 "Course: Intelligent Wireless Sensor Networks"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:WeMos_D1_mini U?
U 1 1 601562AD
P 5050 3950
F 0 "U?" H 4750 4750 50  0000 C CNN
F 1 "WeMos_D1_mini" H 4750 4850 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 5050 2800 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H 3200 2800 50  0001 C CNN
	1    5050 3950
	1    0    0    -1  
$EndComp
Text Notes 4300 2900 0    50   ~ 0
Use USB for power and programming
$Comp
L Device:LED D?
U 1 1 60157628
P 6600 3500
F 0 "D?" H 6450 3550 50  0000 C CNN
F 1 "RED" H 6593 3625 50  0000 C CNN
F 2 "" H 6600 3500 50  0001 C CNN
F 3 "~" H 6600 3500 50  0001 C CNN
	1    6600 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 601587F5
P 6600 3750
F 0 "D?" H 6450 3800 50  0000 C CNN
F 1 "YELLOW" H 6593 3875 50  0000 C CNN
F 2 "" H 6600 3750 50  0001 C CNN
F 3 "~" H 6600 3750 50  0001 C CNN
	1    6600 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 60158BAE
P 6600 4000
F 0 "D?" H 6450 4050 50  0000 C CNN
F 1 "GREEN" H 6593 4125 50  0000 C CNN
F 2 "" H 6600 4000 50  0001 C CNN
F 3 "~" H 6600 4000 50  0001 C CNN
	1    6600 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 3650 6050 3650
Wire Wire Line
	6050 3650 6050 3500
Wire Wire Line
	6050 3500 6450 3500
Wire Wire Line
	5450 3750 6450 3750
Wire Wire Line
	5450 3850 6050 3850
Wire Wire Line
	6050 3850 6050 4000
Wire Wire Line
	6050 4000 6450 4000
$Comp
L Device:R R?
U 1 1 6015F301
P 7000 3500
F 0 "R?" V 6950 3350 50  0000 C CNN
F 1 "220" V 6884 3500 50  0000 C CNN
F 2 "" V 6930 3500 50  0001 C CNN
F 3 "~" H 7000 3500 50  0001 C CNN
	1    7000 3500
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 6015FAD4
P 7000 3750
F 0 "R?" V 6950 3600 50  0000 C CNN
F 1 "220" V 6884 3750 50  0000 C CNN
F 2 "" V 6930 3750 50  0001 C CNN
F 3 "~" H 7000 3750 50  0001 C CNN
	1    7000 3750
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 6015FD72
P 7000 4000
F 0 "R?" V 6950 3850 50  0000 C CNN
F 1 "220" V 6884 4000 50  0000 C CNN
F 2 "" V 6930 4000 50  0001 C CNN
F 3 "~" H 7000 4000 50  0001 C CNN
	1    7000 4000
	0    1    1    0   
$EndComp
Wire Wire Line
	6750 3500 6850 3500
Wire Wire Line
	6850 3750 6750 3750
Wire Wire Line
	6750 4000 6850 4000
Wire Wire Line
	5150 3150 5150 2950
Wire Wire Line
	5150 2950 7400 2950
Wire Wire Line
	7400 2950 7400 3500
Wire Wire Line
	7400 4000 7150 4000
Wire Wire Line
	7150 3750 7400 3750
Connection ~ 7400 3750
Wire Wire Line
	7400 3750 7400 4000
Wire Wire Line
	7150 3500 7400 3500
Connection ~ 7400 3500
Wire Wire Line
	7400 3500 7400 3750
Text Notes 5600 3250 0    50   ~ 0
Output port as sink can take more current.
$Comp
L power:+3.3V #PWR?
U 1 1 60161571
P 7400 2900
F 0 "#PWR?" H 7400 2750 50  0001 C CNN
F 1 "+3.3V" H 7415 3073 50  0000 C CNN
F 2 "" H 7400 2900 50  0001 C CNN
F 3 "" H 7400 2900 50  0001 C CNN
	1    7400 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 2900 7400 2950
Connection ~ 7400 2950
$Comp
L power:GND #PWR?
U 1 1 60161F83
P 5050 4800
F 0 "#PWR?" H 5050 4550 50  0001 C CNN
F 1 "GND" H 5055 4627 50  0000 C CNN
F 2 "" H 5050 4800 50  0001 C CNN
F 3 "" H 5050 4800 50  0001 C CNN
	1    5050 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 4800 5050 4750
$Comp
L Switch:SW_Push SW?
U 1 1 6017E775
P 6600 4250
F 0 "SW?" H 6400 4350 50  0000 C CNN
F 1 "SW_Push" H 6850 4350 50  0000 C CNN
F 2 "" H 6600 4450 50  0001 C CNN
F 3 "~" H 6600 4450 50  0001 C CNN
	1    6600 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4250 6400 4250
Wire Wire Line
	6800 4250 6800 4800
Wire Wire Line
	6800 4800 5050 4800
Connection ~ 5050 4800
Text Notes 6850 4300 0    50   ~ 0
Enable Pull-Up on D7
$EndSCHEMATC
