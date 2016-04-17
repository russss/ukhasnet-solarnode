EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:Tag-Connect
LIBS:tinkerforge
LIBS:stm32
LIBS:cn3083
LIBS:powerint
LIBS:mcp1700
LIBS:maxim
LIBS:agg-kicad
LIBS:solarnode-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "UKHASnet Solar Node"
Date "15/04/2016"
Rev "1"
Comp "Russ Garrett"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TC2030-CTX P2
U 1 1 570EA0A9
P 9700 3750
F 0 "P2" H 9650 3550 40  0000 L CNN
F 1 "TC2030-CTX" H 9700 3950 40  0000 C CNN
F 2 "Tag-Connect:TC2030-NL_SMALL" H 9700 3750 60  0001 C CNN
F 3 "" H 9700 3750 60  0000 C CNN
	1    9700 3750
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR01
U 1 1 570EA11A
P 9250 3650
F 0 "#PWR01" H 9250 3500 50  0001 C CNN
F 1 "VCC" H 9250 3800 50  0000 C CNN
F 2 "" H 9250 3650 50  0000 C CNN
F 3 "" H 9250 3650 50  0000 C CNN
	1    9250 3650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 570EA1CF
P 9250 3850
F 0 "#PWR02" H 9250 3600 50  0001 C CNN
F 1 "GND" H 9250 3700 50  0000 C CNN
F 2 "" H 9250 3850 50  0000 C CNN
F 3 "" H 9250 3850 50  0000 C CNN
	1    9250 3850
	1    0    0    -1  
$EndComp
Text GLabel 5000 2950 2    60   BiDi ~ 0
SWDIO
Text GLabel 10150 3650 2    60   BiDi ~ 0
SWDIO
Text GLabel 10150 3750 2    60   Output ~ 0
SWCLK
Text GLabel 5000 3050 2    60   Input ~ 0
SWCLK
$Comp
L RFM69HW U4
U 1 1 570EA289
P 6550 2300
F 0 "U4" H 6200 2850 60  0000 C CNN
F 1 "RFM69HW" H 6850 1800 60  0000 C CNN
F 2 "tinkerforge:RFM69HW" H 6200 2850 60  0001 C CNN
F 3 "" H 6200 2850 60  0000 C CNN
	1    6550 2300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 570EA34F
P 6550 2900
F 0 "#PWR03" H 6550 2650 50  0001 C CNN
F 1 "GND" H 6550 2750 50  0000 C CNN
F 2 "" H 6550 2900 50  0000 C CNN
F 3 "" H 6550 2900 50  0000 C CNN
	1    6550 2900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR04
U 1 1 570EA369
P 6550 1700
F 0 "#PWR04" H 6550 1550 50  0001 C CNN
F 1 "VCC" H 6550 1850 50  0000 C CNN
F 2 "" H 6550 1700 50  0000 C CNN
F 3 "" H 6550 1700 50  0000 C CNN
	1    6550 1700
	1    0    0    -1  
$EndComp
$Comp
L CON-SMA ANT1
U 1 1 570EBF25
P 7800 2950
F 0 "ANT1" H 7700 3500 60  0000 C CNN
F 1 "ANT" V 8050 3150 60  0000 C CNN
F 2 "tinkerforge:CON-SMA-EDGE" H 7800 2950 60  0001 C CNN
F 3 "" H 7800 2950 60  0000 C CNN
	1    7800 2950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 570EBFA2
P 7800 3200
F 0 "#PWR05" H 7800 2950 50  0001 C CNN
F 1 "GND" H 7800 3050 50  0000 C CNN
F 2 "" H 7800 3200 50  0000 C CNN
F 3 "" H 7800 3200 50  0000 C CNN
	1    7800 3200
	1    0    0    -1  
$EndComp
$Comp
L USB_OTG P1
U 1 1 570EC118
P 5700 3650
F 0 "P1" H 6025 3525 50  0000 C CNN
F 1 "USB_OTG" H 5700 3850 50  0000 C CNN
F 2 "Connect:USB_Micro-B_10103594-0001LF" V 5650 3550 50  0001 C CNN
F 3 "" V 5650 3550 50  0000 C CNN
	1    5700 3650
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR06
U 1 1 570EC1DA
P 5300 3750
F 0 "#PWR06" H 5300 3500 50  0001 C CNN
F 1 "GND" H 5300 3600 50  0000 C CNN
F 2 "" H 5300 3750 50  0000 C CNN
F 3 "" H 5300 3750 50  0000 C CNN
	1    5300 3750
	1    0    0    -1  
$EndComp
Text GLabel 3800 2250 0    60   Input ~ 0
NRST
Text GLabel 9250 3750 0    60   Output ~ 0
NRST
$Comp
L VCC #PWR07
U 1 1 570EC3C3
P 3600 1300
F 0 "#PWR07" H 3600 1150 50  0001 C CNN
F 1 "VCC" H 3600 1450 50  0000 C CNN
F 2 "" H 3600 1300 50  0000 C CNN
F 3 "" H 3600 1300 50  0000 C CNN
	1    3600 1300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 570F4A94
P 4300 6200
F 0 "#PWR08" H 4300 5950 50  0001 C CNN
F 1 "GND" H 4300 6050 50  0000 C CNN
F 2 "" H 4300 6200 50  0000 C CNN
F 3 "" H 4300 6200 50  0000 C CNN
	1    4300 6200
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P3
U 1 1 570F6DA2
P 9800 5450
F 0 "P3" H 9800 5600 50  0000 C CNN
F 1 "C_PANEL" V 9900 5450 50  0000 C CNN
F 2 "tinkerforge:JST-PH-2" H 9800 5450 50  0001 C CNN
F 3 "" H 9800 5450 50  0000 C CNN
	1    9800 5450
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P4
U 1 1 570F6C9A
P 10700 5450
F 0 "P4" H 10700 5600 50  0000 C CNN
F 1 "C_BATT" V 10800 5450 50  0000 C CNN
F 2 "tinkerforge:JST-PH-2" H 10700 5450 50  0001 C CNN
F 3 "" H 10700 5450 50  0000 C CNN
	1    10700 5450
	1    0    0    -1  
$EndComp
$Comp
L ZENERsmall D2
U 1 1 570F6F94
P 9400 5400
F 0 "D2" H 9400 5500 50  0000 C CNN
F 1 "6V" H 9400 5300 50  0000 C CNN
F 2 "Diodes_SMD:SOD-123" H 9400 5400 50  0001 C CNN
F 3 "" H 9400 5400 50  0000 C CNN
	1    9400 5400
	0    1    1    0   
$EndComp
$Comp
L GND #PWR09
U 1 1 570F7362
P 9500 5500
F 0 "#PWR09" H 9500 5250 50  0001 C CNN
F 1 "GND" H 9500 5350 50  0000 C CNN
F 2 "" H 9500 5500 50  0000 C CNN
F 3 "" H 9500 5500 50  0000 C CNN
	1    9500 5500
	1    0    0    -1  
$EndComp
Text GLabel 9500 5300 1    60   Output ~ 0
Vsupp
Text GLabel 5900 3350 1    60   Output ~ 0
Vsupp
Text GLabel 4300 4650 1    60   Input ~ 0
Vsupp
$Comp
L GND #PWR010
U 1 1 570F772C
P 10200 5500
F 0 "#PWR010" H 10200 5250 50  0001 C CNN
F 1 "GND" H 10200 5350 50  0000 C CNN
F 2 "" H 10200 5500 50  0000 C CNN
F 3 "" H 10200 5500 50  0000 C CNN
	1    10200 5500
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR011
U 1 1 570F7752
P 10200 5400
F 0 "#PWR011" H 10200 5250 50  0001 C CNN
F 1 "+BATT" H 10200 5540 50  0000 C CNN
F 2 "" H 10200 5400 50  0000 C CNN
F 3 "" H 10200 5400 50  0000 C CNN
	1    10200 5400
	1    0    0    -1  
$EndComp
NoConn ~ 10150 3850
NoConn ~ 7100 2450
NoConn ~ 7100 2250
NoConn ~ 7100 2150
NoConn ~ 7100 2050
NoConn ~ 6000 1950
NoConn ~ 5600 3350
NoConn ~ 9200 8250
Wire Wire Line
	6500 2900 6600 2900
Connection ~ 6600 2900
Connection ~ 6500 2900
Wire Wire Line
	5000 2350 5450 2350
Wire Wire Line
	5450 2350 5450 2450
Wire Wire Line
	5450 2450 6000 2450
Wire Wire Line
	5000 2450 5350 2450
Wire Wire Line
	5350 2450 5350 2400
Wire Wire Line
	5350 2400 5650 2400
Wire Wire Line
	5650 2400 5650 2350
Wire Wire Line
	5650 2350 6000 2350
Wire Wire Line
	6000 2250 5000 2250
Wire Wire Line
	5000 2150 6000 2150
Wire Wire Line
	6000 2650 6000 2550
Wire Wire Line
	6000 2550 5000 2550
Wire Wire Line
	7100 2650 7450 2650
Wire Wire Line
	7650 3200 7950 3200
Connection ~ 7650 3200
Connection ~ 7750 3200
Connection ~ 7850 3200
Connection ~ 7950 3200
Wire Wire Line
	5000 2750 5800 2750
Wire Wire Line
	5800 2750 5800 3350
Wire Wire Line
	5700 3350 5700 2850
Wire Wire Line
	5700 2850 5000 2850
Wire Wire Line
	5300 3750 5300 3350
Wire Wire Line
	5300 3350 5500 3350
Wire Wire Line
	9600 5500 9400 5500
Wire Wire Line
	9600 5400 9600 5300
Wire Wire Line
	9600 5300 9400 5300
Connection ~ 9500 5500
Connection ~ 9500 5300
Connection ~ 7800 3200
$Comp
L R R8
U 1 1 570F7D75
P 9050 5250
F 0 "R8" V 9130 5250 50  0000 C CNN
F 1 "10k" V 9050 5250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8980 5250 50  0001 C CNN
F 3 "" H 9050 5250 50  0000 C CNN
	1    9050 5250
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 570F7DB5
P 9050 5550
F 0 "R9" V 9130 5550 50  0000 C CNN
F 1 "10k" V 9050 5550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8980 5550 50  0001 C CNN
F 3 "" H 9050 5550 50  0000 C CNN
	1    9050 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 570F7DEF
P 9050 5700
F 0 "#PWR012" H 9050 5450 50  0001 C CNN
F 1 "GND" H 9050 5550 50  0000 C CNN
F 2 "" H 9050 5700 50  0000 C CNN
F 3 "" H 9050 5700 50  0000 C CNN
	1    9050 5700
	1    0    0    -1  
$EndComp
Text GLabel 9050 5100 1    60   Input ~ 0
Vsupp
Text GLabel 8950 5400 0    60   Output ~ 0
Vsuppmon
Wire Wire Line
	8950 5400 9050 5400
Text GLabel 5000 1850 2    60   Input ~ 0
Vsuppmon
$Comp
L GND #PWR013
U 1 1 570F8373
P 5450 5850
F 0 "#PWR013" H 5450 5600 50  0001 C CNN
F 1 "GND" H 5450 5700 50  0000 C CNN
F 2 "" H 5450 5850 50  0000 C CNN
F 3 "" H 5450 5850 50  0000 C CNN
	1    5450 5850
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR014
U 1 1 570F839F
P 5450 5200
F 0 "#PWR014" H 5450 5050 50  0001 C CNN
F 1 "+BATT" H 5450 5340 50  0000 C CNN
F 2 "" H 5450 5200 50  0000 C CNN
F 3 "" H 5450 5200 50  0000 C CNN
	1    5450 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 5400 5450 5400
Connection ~ 5450 5400
$Comp
L C C4
U 1 1 570F84D5
P 4450 4800
F 0 "C4" H 4475 4900 50  0000 L CNN
F 1 "22u" H 4475 4700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 4488 4650 50  0001 C CNN
F 3 "" H 4450 4800 50  0000 C CNN
	1    4450 4800
	0    1    1    0   
$EndComp
Wire Wire Line
	4300 4650 4300 4950
Connection ~ 4300 4800
$Comp
L GND #PWR015
U 1 1 570F85D9
P 4700 4800
F 0 "#PWR015" H 4700 4550 50  0001 C CNN
F 1 "GND" H 4700 4650 50  0000 C CNN
F 2 "" H 4700 4800 50  0000 C CNN
F 3 "" H 4700 4800 50  0000 C CNN
	1    4700 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 4800 4700 4800
$Comp
L R R3
U 1 1 570F89C7
P 4950 6050
F 0 "R3" V 5030 6050 50  0000 C CNN
F 1 "3k6" V 4950 6050 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 4880 6050 50  0001 C CNN
F 3 "" H 4950 6050 50  0000 C CNN
	1    4950 6050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 570F8A12
P 4950 6200
F 0 "#PWR016" H 4950 5950 50  0001 C CNN
F 1 "GND" H 4950 6050 50  0000 C CNN
F 2 "" H 4950 6200 50  0000 C CNN
F 3 "" H 4950 6200 50  0000 C CNN
	1    4950 6200
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 570F8B8E
P 5200 6050
F 0 "R5" V 5280 6050 50  0000 C CNN
F 1 "10k" V 5200 6050 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5130 6050 50  0001 C CNN
F 3 "" H 5200 6050 50  0000 C CNN
	1    5200 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 5900 5200 5900
Connection ~ 4950 5900
$Comp
L C C5
U 1 1 570F8C7A
P 5200 6350
F 0 "C5" H 5225 6450 50  0000 L CNN
F 1 "C" H 5225 6250 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5238 6200 50  0001 C CNN
F 3 "" H 5200 6350 50  0000 C CNN
	1    5200 6350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 570F8CC5
P 5200 6500
F 0 "#PWR017" H 5200 6250 50  0001 C CNN
F 1 "GND" H 5200 6350 50  0000 C CNN
F 2 "" H 5200 6500 50  0000 C CNN
F 3 "" H 5200 6500 50  0000 C CNN
	1    5200 6500
	1    0    0    -1  
$EndComp
Text GLabel 5350 6200 2    60   Output ~ 0
Icharge
Wire Wire Line
	5350 6200 5200 6200
Connection ~ 5200 6200
Text GLabel 5000 1950 2    60   Input ~ 0
Icharge
$Comp
L R R4
U 1 1 570F8F80
P 5200 5250
F 0 "R4" V 5280 5250 50  0000 C CNN
F 1 "0" V 5200 5250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 5130 5250 50  0001 C CNN
F 3 "" H 5200 5250 50  0000 C CNN
	1    5200 5250
	0    1    1    0   
$EndComp
Wire Wire Line
	5450 5400 5450 5200
Wire Wire Line
	5350 5250 5450 5250
Connection ~ 5450 5250
Wire Wire Line
	5050 5250 4950 5250
$Comp
L MCP1700 U3
U 1 1 570F970D
P 6600 5400
F 0 "U3" H 6400 5650 60  0000 C CNN
F 1 "MCP1700" H 6850 5100 60  0000 C CNN
F 2 "agg:SOT-23" H 6850 5100 60  0001 C CNN
F 3 "" H 6850 5100 60  0000 C CNN
	1    6600 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 570F97E4
P 6600 5850
F 0 "#PWR018" H 6600 5600 50  0001 C CNN
F 1 "GND" H 6600 5700 50  0000 C CNN
F 2 "" H 6600 5850 50  0000 C CNN
F 3 "" H 6600 5850 50  0000 C CNN
	1    6600 5850
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR019
U 1 1 570F981C
P 6600 5000
F 0 "#PWR019" H 6600 4850 50  0001 C CNN
F 1 "+BATT" H 6600 5140 50  0000 C CNN
F 2 "" H 6600 5000 50  0000 C CNN
F 3 "" H 6600 5000 50  0000 C CNN
	1    6600 5000
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR020
U 1 1 570F9854
P 7300 5400
F 0 "#PWR020" H 7300 5250 50  0001 C CNN
F 1 "VCC" H 7300 5550 50  0000 C CNN
F 2 "" H 7300 5400 50  0000 C CNN
F 3 "" H 7300 5400 50  0000 C CNN
	1    7300 5400
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 570F99AD
P 7300 5550
F 0 "C7" H 7325 5650 50  0000 L CNN
F 1 "1u" H 7325 5450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 7338 5400 50  0001 C CNN
F 3 "" H 7300 5550 50  0000 C CNN
	1    7300 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR021
U 1 1 570F99FE
P 7300 5700
F 0 "#PWR021" H 7300 5450 50  0001 C CNN
F 1 "GND" H 7300 5550 50  0000 C CNN
F 2 "" H 7300 5700 50  0000 C CNN
F 3 "" H 7300 5700 50  0000 C CNN
	1    7300 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 5400 7300 5400
Connection ~ 7300 5400
$Comp
L R R6
U 1 1 570FA163
P 8300 5250
F 0 "R6" V 8380 5250 50  0000 C CNN
F 1 "4k7" V 8300 5250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8230 5250 50  0001 C CNN
F 3 "" H 8300 5250 50  0000 C CNN
	1    8300 5250
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 570FA1C5
P 8300 5550
F 0 "R7" V 8380 5550 50  0000 C CNN
F 1 "10k" V 8300 5550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 8230 5550 50  0001 C CNN
F 3 "" H 8300 5550 50  0000 C CNN
	1    8300 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR022
U 1 1 570FA276
P 8300 5700
F 0 "#PWR022" H 8300 5450 50  0001 C CNN
F 1 "GND" H 8300 5550 50  0000 C CNN
F 2 "" H 8300 5700 50  0000 C CNN
F 3 "" H 8300 5700 50  0000 C CNN
	1    8300 5700
	1    0    0    -1  
$EndComp
$Comp
L +BATT #PWR023
U 1 1 570FA2B4
P 8300 5100
F 0 "#PWR023" H 8300 4950 50  0001 C CNN
F 1 "+BATT" H 8300 5240 50  0000 C CNN
F 2 "" H 8300 5100 50  0000 C CNN
F 3 "" H 8300 5100 50  0000 C CNN
	1    8300 5100
	1    0    0    -1  
$EndComp
Text GLabel 8200 5400 0    60   Output ~ 0
Vbattmon
Wire Wire Line
	8200 5400 8300 5400
Connection ~ 8300 5400
Connection ~ 9050 5400
Text GLabel 3800 2450 0    60   Input ~ 0
Vbattmon
Text GLabel 5000 2050 2    60   BiDi ~ 0
ONEWIRE
$Comp
L R R10
U 1 1 5710D76C
P 9500 1550
F 0 "R10" V 9580 1550 50  0000 C CNN
F 1 "4k7" V 9500 1550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 9430 1550 50  0001 C CNN
F 3 "" H 9500 1550 50  0000 C CNN
	1    9500 1550
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR024
U 1 1 5710D7C5
P 9500 1400
F 0 "#PWR024" H 9500 1250 50  0001 C CNN
F 1 "VCC" H 9500 1550 50  0000 C CNN
F 2 "" H 9500 1400 50  0000 C CNN
F 3 "" H 9500 1400 50  0000 C CNN
	1    9500 1400
	1    0    0    -1  
$EndComp
Text GLabel 9350 1700 0    60   BiDi ~ 0
ONEWIRE
Wire Wire Line
	9350 1700 9800 1700
$Comp
L C_Small C8
U 1 1 5710DE1F
P 6750 1700
F 0 "C8" H 6760 1770 50  0000 L CNN
F 1 "100n" H 6760 1620 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 6750 1700 50  0001 C CNN
F 3 "" H 6750 1700 50  0000 C CNN
	1    6750 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6650 1700 6550 1700
Connection ~ 6550 1700
$Comp
L GND #PWR025
U 1 1 5710DF88
P 6850 1700
F 0 "#PWR025" H 6850 1450 50  0001 C CNN
F 1 "GND" H 6850 1550 50  0000 C CNN
F 2 "" H 6850 1700 50  0000 C CNN
F 3 "" H 6850 1700 50  0000 C CNN
	1    6850 1700
	0    -1   -1   0   
$EndComp
$Comp
L C_Small C2
U 1 1 5710E301
P 3900 1300
F 0 "C2" H 3910 1370 50  0000 L CNN
F 1 "100n" V 3800 1100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3900 1300 50  0001 C CNN
F 3 "" H 3900 1300 50  0000 C CNN
	1    3900 1300
	0    -1   -1   0   
$EndComp
$Comp
L C_Small C3
U 1 1 5710E38A
P 3900 1500
F 0 "C3" H 3910 1570 50  0000 L CNN
F 1 "100n" V 3800 1300 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3900 1500 50  0001 C CNN
F 3 "" H 3900 1500 50  0000 C CNN
	1    3900 1500
	0    -1   -1   0   
$EndComp
$Comp
L C_Small C1
U 1 1 5710E429
P 3900 1100
F 0 "C1" H 3910 1170 50  0000 L CNN
F 1 "4.7u" V 3850 900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 3900 1100 50  0001 C CNN
F 3 "" H 3900 1100 50  0000 C CNN
	1    3900 1100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4000 1100 4150 1100
Wire Wire Line
	4150 1100 4150 1500
Wire Wire Line
	4150 1500 4000 1500
Wire Wire Line
	4000 1300 4300 1300
Connection ~ 4150 1300
$Comp
L GND #PWR026
U 1 1 5710EA2F
P 4300 1300
F 0 "#PWR026" H 4300 1050 50  0001 C CNN
F 1 "GND" H 4300 1150 50  0000 C CNN
F 2 "" H 4300 1300 50  0000 C CNN
F 3 "" H 4300 1300 50  0000 C CNN
	1    4300 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 1100 3750 1100
Wire Wire Line
	3750 1100 3750 1500
Wire Wire Line
	3750 1500 3800 1500
Wire Wire Line
	3600 1300 3800 1300
Connection ~ 3750 1300
Wire Wire Line
	3600 1300 3600 1850
Text GLabel 3800 2750 0    60   Input ~ 0
BATT_CHARGE
Text GLabel 3800 2850 0    60   Input ~ 0
BATT_OK
$Comp
L R R1
U 1 1 5710EFC9
P 3650 5600
F 0 "R1" V 3730 5600 50  0000 C CNN
F 1 "10k" V 3650 5600 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3580 5600 50  0001 C CNN
F 3 "" H 3650 5600 50  0000 C CNN
	1    3650 5600
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5710F032
P 3650 6050
F 0 "R2" V 3730 6050 50  0000 C CNN
F 1 "10k" V 3650 6050 50  0000 C CNN
F 2 "Resistors_SMD:R_0805" V 3580 6050 50  0001 C CNN
F 3 "" H 3650 6050 50  0000 C CNN
	1    3650 6050
	1    0    0    -1  
$EndComp
Text GLabel 3450 5750 0    60   Output ~ 0
BATT_CHARGE
Text GLabel 3450 5900 0    60   Output ~ 0
BATT_OK
Wire Wire Line
	3450 5750 3650 5750
Wire Wire Line
	3650 5900 3450 5900
$Comp
L GND #PWR027
U 1 1 5710F1DE
P 3650 6200
F 0 "#PWR027" H 3650 5950 50  0001 C CNN
F 1 "GND" H 3650 6050 50  0000 C CNN
F 2 "" H 3650 6200 50  0000 C CNN
F 3 "" H 3650 6200 50  0000 C CNN
	1    3650 6200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR028
U 1 1 5710F246
P 3650 5450
F 0 "#PWR028" H 3650 5200 50  0001 C CNN
F 1 "GND" H 3650 5300 50  0000 C CNN
F 2 "" H 3650 5450 50  0000 C CNN
F 3 "" H 3650 5450 50  0000 C CNN
	1    3650 5450
	-1   0    0    1   
$EndComp
$Comp
L Jumper_NO_Small JP1
U 1 1 5710F5F9
P 2900 2800
F 0 "JP1" H 2900 2880 50  0000 C CNN
F 1 "Boot" H 2910 2740 50  0001 C CNN
F 2 "Resistors_SMD:R_0805" H 2900 2800 50  0001 C CNN
F 3 "" H 2900 2800 50  0000 C CNN
	1    2900 2800
	1    0    0    -1  
$EndComp
$Comp
L Led_Small D1
U 1 1 5710F743
P 2900 2550
F 0 "D1" H 2850 2675 50  0000 L CNN
F 1 "Status" H 2725 2450 50  0000 L CNN
F 2 "agg:0805-LED" V 2900 2550 50  0001 C CNN
F 3 "" V 2900 2550 50  0000 C CNN
	1    2900 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR029
U 1 1 5710F92A
P 2600 2550
F 0 "#PWR029" H 2600 2300 50  0001 C CNN
F 1 "GND" H 2600 2400 50  0000 C CNN
F 2 "" H 2600 2550 50  0000 C CNN
F 3 "" H 2600 2550 50  0000 C CNN
	1    2600 2550
	1    0    0    -1  
$EndComp
$Comp
L DS18B20 U5
U 1 1 57110037
P 10100 1700
F 0 "U5" H 9950 1950 50  0000 C CNN
F 1 "DS18B20" H 10100 1450 50  0000 C CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 9950 1950 50  0001 C CNN
F 3 "" H 9950 1950 50  0000 C CNN
	1    10100 1700
	1    0    0    -1  
$EndComp
$Comp
L DS18B20 U6
U 1 1 571100A4
P 10100 2300
F 0 "U6" H 9950 2550 50  0000 C CNN
F 1 "DS18B20" H 10100 2050 50  0000 C CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow" H 9950 2550 50  0001 C CNN
F 3 "" H 9950 2550 50  0000 C CNN
	1    10100 2300
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR030
U 1 1 57110112
P 9800 1600
F 0 "#PWR030" H 9800 1450 50  0001 C CNN
F 1 "VCC" H 9800 1750 50  0000 C CNN
F 2 "" H 9800 1600 50  0000 C CNN
F 3 "" H 9800 1600 50  0000 C CNN
	1    9800 1600
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR031
U 1 1 57110166
P 9800 2200
F 0 "#PWR031" H 9800 2050 50  0001 C CNN
F 1 "VCC" H 9800 2350 50  0000 C CNN
F 2 "" H 9800 2200 50  0000 C CNN
F 3 "" H 9800 2200 50  0000 C CNN
	1    9800 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR032
U 1 1 571101BA
P 9800 1800
F 0 "#PWR032" H 9800 1550 50  0001 C CNN
F 1 "GND" H 9800 1650 50  0000 C CNN
F 2 "" H 9800 1800 50  0000 C CNN
F 3 "" H 9800 1800 50  0000 C CNN
	1    9800 1800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 57110282
P 9800 2400
F 0 "#PWR033" H 9800 2150 50  0001 C CNN
F 1 "GND" H 9800 2250 50  0000 C CNN
F 2 "" H 9800 2400 50  0000 C CNN
F 3 "" H 9800 2400 50  0000 C CNN
	1    9800 2400
	1    0    0    -1  
$EndComp
Connection ~ 9500 1700
Wire Wire Line
	9500 1700 9500 2300
Wire Wire Line
	9500 2300 9800 2300
$Comp
L PWR_FLAG #FLG034
U 1 1 57110FA7
P 10500 5500
F 0 "#FLG034" H 10500 5595 50  0001 C CNN
F 1 "PWR_FLAG" H 10500 5680 50  0000 C CNN
F 2 "" H 10500 5500 50  0000 C CNN
F 3 "" H 10500 5500 50  0000 C CNN
	1    10500 5500
	-1   0    0    1   
$EndComp
Wire Wire Line
	10500 5500 10200 5500
Wire Wire Line
	10200 5400 10500 5400
$Comp
L CN3083 U2
U 1 1 57110BE8
P 4300 5650
F 0 "U2" H 3800 6200 60  0000 C CNN
F 1 "CN3083" H 4300 5750 60  0000 C CNN
F 2 "agg:SOIC-8-EP-CN" H 4300 5750 60  0001 C CNN
F 3 "" H 4300 5750 60  0000 C CNN
	1    4300 5650
	1    0    0    -1  
$EndComp
Connection ~ 3650 5750
Connection ~ 3650 5900
Wire Wire Line
	3800 2550 3000 2550
Wire Wire Line
	3000 2550 3000 2800
Wire Wire Line
	2800 2800 2800 2550
Wire Wire Line
	2800 2550 2600 2550
Wire Wire Line
	3800 1950 3800 1850
Wire Wire Line
	3800 1850 3600 1850
$Comp
L GND #PWR035
U 1 1 57112AC9
P 3600 2050
F 0 "#PWR035" H 3600 1800 50  0001 C CNN
F 1 "GND" H 3600 1900 50  0000 C CNN
F 2 "" H 3600 2050 50  0000 C CNN
F 3 "" H 3600 2050 50  0000 C CNN
	1    3600 2050
	0    1    1    0   
$EndComp
Wire Wire Line
	3800 2050 3600 2050
NoConn ~ 7100 2350
NoConn ~ 7100 1950
$Comp
L C C6
U 1 1 570F82DD
P 5450 5550
F 0 "C6" H 5475 5650 50  0000 L CNN
F 1 "4.7u" H 5475 5450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5488 5400 50  0001 C CNN
F 3 "" H 5450 5550 50  0000 C CNN
	1    5450 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 5750 5450 5750
Wire Wire Line
	5450 5700 5450 5850
Connection ~ 5450 5750
Connection ~ 3000 2550
Connection ~ 2800 2550
Connection ~ 3800 1850
$Comp
L STM32F0xxFxPx IC1
U 1 1 57115539
P 4400 2450
F 0 "IC1" H 3900 3150 50  0000 L CNN
F 1 "STM32F0xxFxPx" H 3900 1750 50  0000 L CNN
F 2 "tinkerforge:TSSOP20" H 3900 1650 50  0001 L CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00105814.pdf" H 3900 1550 50  0001 L CNN
F 4 "2469549" H 3900 1450 50  0001 L CNN "Farnell"
	1    4400 2450
	1    0    0    -1  
$EndComp
Connection ~ 6550 2900
$EndSCHEMATC
