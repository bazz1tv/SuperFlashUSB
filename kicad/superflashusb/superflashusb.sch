EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR?
U 1 1 5BE46560
P 3550 6800
F 0 "#PWR?" H 3550 6550 50  0001 C CNN
F 1 "GND" H 3555 6627 50  0000 C CNN
F 2 "" H 3550 6800 50  0001 C CNN
F 3 "" H 3550 6800 50  0001 C CNN
	1    3550 6800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5BE465CB
P 3550 4800
F 0 "#PWR?" H 3550 4650 50  0001 C CNN
F 1 "+5V" H 3565 4973 50  0000 C CNN
F 2 "" H 3550 4800 50  0001 C CNN
F 3 "" H 3550 4800 50  0001 C CNN
	1    3550 4800
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:BC108 Q?
U 1 1 5BE46814
P 6450 7150
F 0 "Q?" H 6641 7196 50  0000 L CNN
F 1 "BC108" H 6641 7105 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-18-3" H 6650 7075 50  0001 L CIN
F 3 "http://www.b-kainka.de/Daten/Transistor/BC108.pdf" H 6450 7150 50  0001 L CNN
	1    6450 7150
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS273 U?
U 1 1 5BE46A91
P 3550 5900
F 0 "U?" H 3550 6878 50  0000 C CNN
F 1 "74LS273" H 3550 6787 50  0000 C CNN
F 2 "" H 3550 5900 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS273" H 3550 5900 50  0001 C CNN
	1    3550 5900
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATmega:ATmega16U2-AU U?
U 1 1 5BE46D20
P 4300 2350
F 0 "U?" H 4300 864 50  0000 C CNN
F 1 "ATmega16U2-AU" H 4300 773 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 4300 2350 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc7799.pdf" H 4300 2350 50  0001 C CNN
	1    4300 2350
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal Y?
U 1 1 5BE471A0
P 1750 2350
F 0 "Y?" H 1800 2550 50  0000 R CNN
F 1 "16 MHz" H 1900 2650 50  0000 R CNN
F 2 "" H 1750 2350 50  0001 C CNN
F 3 "~" H 1750 2350 50  0001 C CNN
	1    1750 2350
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 5BE474D5
P 1450 2500
F 0 "C?" H 1565 2546 50  0000 L CNN
F 1 "C" H 1565 2455 50  0000 L CNN
F 2 "" H 1488 2350 50  0001 C CNN
F 3 "~" H 1450 2500 50  0001 C CNN
	1    1450 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5BE476DF
P 2050 2500
F 0 "C?" H 2165 2546 50  0000 L CNN
F 1 "C" H 2165 2455 50  0000 L CNN
F 2 "" H 2088 2350 50  0001 C CNN
F 3 "~" H 2050 2500 50  0001 C CNN
	1    2050 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 2350 1900 2350
Connection ~ 2050 2350
Wire Wire Line
	1450 2350 1600 2350
Wire Wire Line
	4300 3750 4300 4050
$Comp
L power:GND #PWR?
U 1 1 5BE47D63
P 4300 4050
F 0 "#PWR?" H 4300 3800 50  0001 C CNN
F 1 "GND" H 4305 3877 50  0000 C CNN
F 2 "" H 4300 4050 50  0001 C CNN
F 3 "" H 4300 4050 50  0001 C CNN
	1    4300 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 950  4300 850 
$Comp
L power:+5V #PWR?
U 1 1 5BE47F23
P 4300 750
F 0 "#PWR?" H 4300 600 50  0001 C CNN
F 1 "+5V" H 4315 923 50  0000 C CNN
F 2 "" H 4300 750 50  0001 C CNN
F 3 "" H 4300 750 50  0001 C CNN
	1    4300 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3050 6050 3050
Text GLabel 6050 3050 2    50   Output ~ 0
~CART
Wire Wire Line
	5000 3150 5850 3150
Text GLabel 5850 3150 2    50   Output ~ 0
~WR
Wire Wire Line
	5000 3250 5650 3250
Text GLabel 5650 3250 2    50   Output ~ 0
~RD
NoConn ~ 5000 3350
NoConn ~ 5000 3450
Text Notes 750  2850 0    50   ~ 0
Couldn't get internal oscillator to work at 16 MHz (on breadboard).\nThis is probably because I left UVCC disconnected!
Wire Wire Line
	4200 3750 4200 4050
Wire Wire Line
	4200 4050 4300 4050
Connection ~ 4300 4050
Wire Wire Line
	4200 950  4200 850 
Wire Wire Line
	4200 850  4300 850 
Connection ~ 4300 850 
Wire Wire Line
	4300 850  4300 750 
Wire Wire Line
	4400 950  4400 850 
Wire Wire Line
	4400 850  4300 850 
$Comp
L Device:C C?
U 1 1 5BE4963C
P 2450 2500
F 0 "C?" H 2335 2454 50  0000 R CNN
F 1 "1 uF" H 2335 2545 50  0000 R CNN
F 2 "" H 2488 2350 50  0001 C CNN
F 3 "~" H 2450 2500 50  0001 C CNN
	1    2450 2500
	-1   0    0    1   
$EndComp
Wire Wire Line
	1450 2650 1450 3150
$Comp
L power:GND #PWR?
U 1 1 5BE478AF
P 1450 3450
F 0 "#PWR?" H 1450 3200 50  0001 C CNN
F 1 "GND" H 1455 3277 50  0000 C CNN
F 2 "" H 1450 3450 50  0001 C CNN
F 3 "" H 1450 3450 50  0001 C CNN
	1    1450 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 3150 2050 3150
Wire Wire Line
	1450 3150 1450 3450
Connection ~ 1450 3150
Wire Wire Line
	3600 1450 1450 1450
Wire Wire Line
	3600 1650 2050 1650
Wire Wire Line
	2050 1650 2050 2350
Wire Wire Line
	2050 2650 2050 3150
Wire Wire Line
	3600 2150 2450 2150
Wire Wire Line
	2450 2150 2450 2350
Wire Wire Line
	2450 2650 2450 3150
Wire Wire Line
	2450 3150 2050 3150
Connection ~ 2050 3150
Wire Wire Line
	3400 1850 3600 1850
Wire Wire Line
	3050 1950 3600 1950
$Comp
L Device:R_US R?
U 1 1 5BE50D30
P 3250 1850
F 0 "R?" V 3045 1850 50  0000 C CNN
F 1 "20" V 3136 1850 50  0000 C CNN
F 2 "" V 3290 1840 50  0001 C CNN
F 3 "~" H 3250 1850 50  0001 C CNN
	1    3250 1850
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R?
U 1 1 5BE50DBE
P 2900 1950
F 0 "R?" V 3105 1950 50  0000 C CNN
F 1 "20" V 3014 1950 50  0000 C CNN
F 2 "" V 2940 1940 50  0001 C CNN
F 3 "~" H 2900 1950 50  0001 C CNN
	1    2900 1950
	0    -1   -1   0   
$EndComp
$Comp
L Connector:USB_B_Mini J?
U 1 1 5BE52835
P 850 1850
F 0 "J?" H 905 2317 50  0000 C CNN
F 1 "USB_B_Mini" H 905 2226 50  0000 C CNN
F 2 "" H 1000 1800 50  0001 C CNN
F 3 "~" H 1000 1800 50  0001 C CNN
	1    850  1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 1850 3100 1850
Wire Wire Line
	1150 1950 2750 1950
Wire Wire Line
	1150 1650 1300 1650
Wire Wire Line
	1300 1650 1300 1500
$Comp
L power:+5V #PWR?
U 1 1 5BE53CA5
P 1300 1500
F 0 "#PWR?" H 1300 1350 50  0001 C CNN
F 1 "+5V" H 1315 1673 50  0000 C CNN
F 2 "" H 1300 1500 50  0001 C CNN
F 3 "" H 1300 1500 50  0001 C CNN
	1    1300 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BE53DE3
P 850 2400
F 0 "#PWR?" H 850 2150 50  0001 C CNN
F 1 "GND" H 855 2227 50  0000 C CNN
F 2 "" H 850 2400 50  0001 C CNN
F 3 "" H 850 2400 50  0001 C CNN
	1    850  2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  2250 850  2400
Wire Wire Line
	750  2250 850  2250
Connection ~ 850  2250
Wire Wire Line
	1450 1450 1450 2350
Connection ~ 1450 2350
Wire Wire Line
	5000 2150 5750 2150
Wire Wire Line
	5000 2250 6000 2250
Wire Wire Line
	5000 2350 6250 2350
Wire Wire Line
	5000 2450 6400 2450
NoConn ~ 5000 2550
NoConn ~ 5000 2650
NoConn ~ 5000 2750
NoConn ~ 5000 2850
Wire Wire Line
	3550 5100 3550 4800
Wire Wire Line
	3550 6700 3550 6800
Text Notes 2150 4650 0    50   ~ 0
actually a CD74HCT377E
Wire Wire Line
	5000 1250 5200 1250
Wire Wire Line
	5000 1350 5200 1350
Wire Wire Line
	5000 1450 5200 1450
Wire Wire Line
	5000 1550 5200 1550
Wire Wire Line
	5000 1650 5200 1650
Wire Wire Line
	5000 1750 5200 1750
Wire Wire Line
	5000 1850 5200 1850
Wire Wire Line
	5000 1950 5200 1950
Text Label 5200 1250 0    50   ~ 0
d0
Text Label 5200 1350 0    50   ~ 0
d1
Text Label 5200 1450 0    50   ~ 0
d2
Text Label 5200 1550 0    50   ~ 0
d3
Text Label 5200 1650 0    50   ~ 0
d4
Text Label 5200 1750 0    50   ~ 0
d5
Text Label 5200 1850 0    50   ~ 0
d6
Text Label 5200 1950 0    50   ~ 0
d7
Wire Wire Line
	3050 5400 2900 5400
Wire Wire Line
	3050 5500 2900 5500
Wire Wire Line
	3050 5600 2900 5600
Wire Wire Line
	3050 5700 2900 5700
Wire Wire Line
	3050 5800 2900 5800
Wire Wire Line
	3050 5900 2900 5900
Wire Wire Line
	3050 6000 2900 6000
Wire Wire Line
	3050 6100 2900 6100
Text Label 2900 5400 0    50   ~ 0
d0
Text Label 2900 5500 0    50   ~ 0
d1
Text Label 2900 5600 0    50   ~ 0
d2
Text Label 2900 5700 0    50   ~ 0
d3
Text Label 2900 5800 0    50   ~ 0
d4
Text Label 2900 5900 0    50   ~ 0
d5
Text Label 2900 6000 0    50   ~ 0
d6
Text Label 2900 6100 0    50   ~ 0
d7
$Comp
L power:GND #PWR?
U 1 1 5BE771D0
P 5550 6700
F 0 "#PWR?" H 5550 6450 50  0001 C CNN
F 1 "GND" H 5555 6527 50  0000 C CNN
F 2 "" H 5550 6700 50  0001 C CNN
F 3 "" H 5550 6700 50  0001 C CNN
	1    5550 6700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5BE771D6
P 5550 4700
F 0 "#PWR?" H 5550 4550 50  0001 C CNN
F 1 "+5V" H 5565 4873 50  0000 C CNN
F 2 "" H 5550 4700 50  0001 C CNN
F 3 "" H 5550 4700 50  0001 C CNN
	1    5550 4700
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS273 U?
U 1 1 5BE771DC
P 5550 5800
F 0 "U?" H 5550 6778 50  0000 C CNN
F 1 "74LS273" H 5550 6687 50  0000 C CNN
F 2 "" H 5550 5800 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS273" H 5550 5800 50  0001 C CNN
	1    5550 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 5000 5550 4700
Wire Wire Line
	5550 6600 5550 6700
Wire Wire Line
	5050 5300 4900 5300
Wire Wire Line
	5050 5400 4900 5400
Wire Wire Line
	5050 5500 4900 5500
Wire Wire Line
	5050 5600 4900 5600
Wire Wire Line
	5050 5700 4900 5700
Wire Wire Line
	5050 5800 4900 5800
Wire Wire Line
	5050 5900 4900 5900
Wire Wire Line
	5050 6000 4900 6000
Text Label 4900 5300 0    50   ~ 0
d0
Text Label 4900 5400 0    50   ~ 0
d1
Text Label 4900 5500 0    50   ~ 0
d2
Text Label 4900 5600 0    50   ~ 0
d3
Text Label 4900 5700 0    50   ~ 0
d4
Text Label 4900 5800 0    50   ~ 0
d5
Text Label 4900 5900 0    50   ~ 0
d6
Text Label 4900 6000 0    50   ~ 0
d7
$Comp
L power:GND #PWR?
U 1 1 5BE78B3F
P 9050 4650
F 0 "#PWR?" H 9050 4400 50  0001 C CNN
F 1 "GND" H 9055 4477 50  0000 C CNN
F 2 "" H 9050 4650 50  0001 C CNN
F 3 "" H 9050 4650 50  0001 C CNN
	1    9050 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5BE78B45
P 9050 2650
F 0 "#PWR?" H 9050 2500 50  0001 C CNN
F 1 "+5V" H 9065 2823 50  0000 C CNN
F 2 "" H 9050 2650 50  0001 C CNN
F 3 "" H 9050 2650 50  0001 C CNN
	1    9050 2650
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS273 U?
U 1 1 5BE78B4B
P 9050 3750
F 0 "U?" H 9050 4728 50  0000 C CNN
F 1 "74LS273" H 9050 4637 50  0000 C CNN
F 2 "" H 9050 3750 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS273" H 9050 3750 50  0001 C CNN
	1    9050 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 2950 9050 2650
Wire Wire Line
	9050 4550 9050 4650
Wire Wire Line
	8550 3250 8400 3250
Wire Wire Line
	8550 3350 8400 3350
Wire Wire Line
	8550 3450 8400 3450
Wire Wire Line
	8550 3550 8400 3550
Wire Wire Line
	8550 3650 8400 3650
Wire Wire Line
	8550 3750 8400 3750
Wire Wire Line
	8550 3850 8400 3850
Wire Wire Line
	8550 3950 8400 3950
Text Label 8400 3250 0    50   ~ 0
d0
Text Label 8400 3350 0    50   ~ 0
d1
Text Label 8400 3450 0    50   ~ 0
d2
Text Label 8400 3550 0    50   ~ 0
d3
Text Label 8400 3650 0    50   ~ 0
d4
Text Label 8400 3750 0    50   ~ 0
d5
Text Label 8400 3850 0    50   ~ 0
d6
Text Label 8400 3950 0    50   ~ 0
d7
$Comp
L power:GND #PWR?
U 1 1 5BE7AD3F
P 1550 6850
F 0 "#PWR?" H 1550 6600 50  0001 C CNN
F 1 "GND" H 1555 6677 50  0000 C CNN
F 2 "" H 1550 6850 50  0001 C CNN
F 3 "" H 1550 6850 50  0001 C CNN
	1    1550 6850
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5BE7AD45
P 1550 4850
F 0 "#PWR?" H 1550 4700 50  0001 C CNN
F 1 "+5V" H 1565 5023 50  0000 C CNN
F 2 "" H 1550 4850 50  0001 C CNN
F 3 "" H 1550 4850 50  0001 C CNN
	1    1550 4850
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS273 U?
U 1 1 5BE7AD4B
P 1550 5950
F 0 "U?" H 1550 6928 50  0000 C CNN
F 1 "74LS273" H 1550 6837 50  0000 C CNN
F 2 "" H 1550 5950 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS273" H 1550 5950 50  0001 C CNN
	1    1550 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 5150 1550 4850
Wire Wire Line
	1550 6750 1550 6850
Wire Wire Line
	1050 5450 900  5450
Wire Wire Line
	1050 5550 900  5550
Wire Wire Line
	1050 5650 900  5650
Wire Wire Line
	1050 5750 900  5750
Wire Wire Line
	1050 5850 900  5850
Wire Wire Line
	1050 5950 900  5950
Wire Wire Line
	1050 6050 900  6050
Wire Wire Line
	1050 6150 900  6150
Text Label 900  5450 0    50   ~ 0
d0
Text Label 900  5550 0    50   ~ 0
d1
Text Label 900  5650 0    50   ~ 0
d2
Text Label 900  5750 0    50   ~ 0
d3
Text Label 900  5850 0    50   ~ 0
d4
Text Label 900  5950 0    50   ~ 0
d5
Text Label 900  6050 0    50   ~ 0
d6
Text Label 900  6150 0    50   ~ 0
d7
Text Label 2050 5450 0    50   ~ 0
SNES_A0
Text Label 2050 5550 0    50   ~ 0
SNES_A1
Text Label 2050 5650 0    50   ~ 0
SNES_A2
Text Label 2050 5750 0    50   ~ 0
SNES_A3
Text Label 2050 5850 0    50   ~ 0
SNES_A4
Text Label 2050 5950 0    50   ~ 0
SNES_A5
Text Label 2050 6050 0    50   ~ 0
SNES_A6
Text Label 2050 6150 0    50   ~ 0
SNES_A7
Text Label 4050 5400 0    50   ~ 0
SNES_A8
Text Label 4050 5500 0    50   ~ 0
SNES_A9
Text Label 4050 5600 0    50   ~ 0
SNES_A10
Text Label 4050 5700 0    50   ~ 0
SNES_A11
Text Label 4050 5800 0    50   ~ 0
SNES_A12
Text Label 4050 5900 0    50   ~ 0
SNES_A13
Text Label 4050 6000 0    50   ~ 0
SNES_A14
Text Label 4050 6100 0    50   ~ 0
SNES_A15
Text Label 6050 5300 0    50   ~ 0
SNES_A16
Text Label 6050 5400 0    50   ~ 0
SNES_A17
Text Label 6050 5500 0    50   ~ 0
SNES_A18
Text Label 6050 5600 0    50   ~ 0
SNES_A19
Text Label 6050 5700 0    50   ~ 0
SNES_A20
Text Label 6050 5800 0    50   ~ 0
SNES_A21
Text Label 6050 5900 0    50   ~ 0
SNES_A22
Text Label 6050 6000 0    50   ~ 0
SNES_A23
Wire Wire Line
	1050 6450 1050 6750
Wire Wire Line
	1050 6750 1550 6750
Connection ~ 1550 6750
Wire Wire Line
	3050 6400 3050 6700
Wire Wire Line
	3050 6700 3550 6700
Connection ~ 3550 6700
Wire Wire Line
	5050 6300 5050 6600
Wire Wire Line
	5050 6600 5550 6600
Connection ~ 5550 6600
Text Label 5750 2150 0    50   ~ 0
FF_ADDR_L
Text Label 1050 6350 2    50   ~ 0
FF_ADDR_L
Text Label 3050 6300 2    50   ~ 0
FF_ADDR_H
Text Label 5050 6200 2    50   ~ 0
FF_ADDR_B
Text Label 6000 2250 0    50   ~ 0
FF_ADDR_H
Text Label 6250 2350 0    50   ~ 0
FF_ADDR_B
Text Label 6400 2450 0    50   ~ 0
FF_STATUS_LED
Text Label 8450 4150 2    50   ~ 0
FF_STATUS_LED
Wire Wire Line
	8450 4150 8550 4150
Wire Wire Line
	8550 4250 8550 4550
Wire Wire Line
	8550 4550 9050 4550
Connection ~ 9050 4550
$EndSCHEMATC
