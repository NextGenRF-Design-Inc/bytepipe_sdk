[![logo](../../BytePipe_Logo.png)](../../README.md)

---

# BytePipe_x900x

The BytePipe_x9002 is a high performance RF System On Module (SOM).  The BytePipe_x9002 integrates a high performance Xilinx Zynq UltraScale+ baseband processor (BBP) with the Analog Devices ADRV9002 RFIC.  

![som](som_02.png)

# Hardware Architecture

The following shows a block diagram of the BytePipe_x900x Hardware Architecture.

![blockdiagram](blockdiagram.png)


# J600

J600 is a board to board connector which provides access to all of the digital signals on the SOM.  Some of these signals provide hardcoded functionality while others can be configured by the BBP.  The mating connector used on the host board is JAE Electronics WR-120S-VHF30-N1.  For additional information see the datasheet on [NextGen RF Design's website.](https://www.nextgenrf.com/products/rf-system-on-a-module/) 


|Pin|Signal|Description|FPGA Pin Name|FPGA Pin|
|-----|----------------|-------------------------------|----------------|-----|
|1|GTR_TX3_P|Gigabit transceiver data|PS_MGTRTXP3_505|A19|
|2|GTR_RX3_P|Gigabit transceiver data|PS_MGTRRXP3_505|B21|
|3|GTR_TX3_N|Gigabit transceiver data|PS_MGTRTXN3_505|A20|
|4|GTR_RX3_N|Gigabit transceiver data|PS_MGTRRXN3_505|B22|
|5|GND|Ground|||
|6|GND|Ground|||
|7|GTR_TX2_P|Gigabit transceiver data|PS_MGTRTXP2_505|C19|
|8|GTR_RX2_P|Gigabit transceiver data|PS_MGTRRXP2_505|D21|
|9|GTR_TX2_N|Gigabit transceiver data|PS_MGTRTXN2_505|C20|
|10|GTR_RX2_N|Gigabit transceiver data|PS_MGTRRXN2_505|D22|
|11|GND|Ground|||
|12|GND|Ground|||
|13|GTR_TX1_P|Gigabit transceiver data|PS_MGTRTXP1_505|F21|
|14|GTR_RX1_P|Gigabit transceiver data|PS_MGTRRXP1_505|H21|
|15|GTR_TX1_N|Gigabit transceiver data|PS_MGTRTXN1_505|F22|
|16|GTR_RX1_N|Gigabit transceiver data|PS_MGTRRXN1_505|H22|
|17|GND|Ground|||
|18|GND|Ground|||
|19|GTR_TX0_P|Gigabit transceiver data|PS_MGTRTXP0_505|K21|
|20|GTR_RX0_P|Gigabit transceiver data|PS_MGTRRXP0_505|M21|
|21|GTR_TX0_N|Gigabit transceiver data|PS_MGTRTXN0_505|K22|
|22|GTR_RX0_N|Gigabit transceiver data|PS_MGTRRXN0_505|M22|
|23|GND|Ground|||
|24|GND|Ground|||
|25|GTR_REFCLK3_N|Gigabit transceiver clock|PS_MGTREFCLK3N_505|E20|
|26|GTR_REFCLK2_P|Gigabit transceiver clock|PS_MGTREFCLK2P_505|G19|
|27|GTR_REFCLK3_P|Gigabit transceiver clock|PS_MGTREFCLK3P_505|E19|
|28|GTR_REFCLK2_N|Gigabit transceiver clock|PS_MGTREFCLK2N_505|G20|
|29|GND|Ground|||
|30|GND|Ground|||
|31|GTR_REFCLK1_N|Gigabit transceiver clock|PS_MGTREFCLK1N_505|J20|
|32|GTR_REFCLK0_P|Gigabit transceiver clock|PS_MGTREFCLK0P_505|L19|
|33|GTR_REFCLK1_P|Gigabit transceiver clock|PS_MGTREFCLK1P_505|J19|
|34|GTR_REFCLK0_N|Gigabit transceiver clock|PS_MGTREFCLK0N_505|L20|
|35|GND|Ground|||
|36|GND|Ground|||
|37|ETH_MD3_N|Ethernet Pair|N/A|N/A|
|38|USB_N|USB2.0|N/A|N/A|
|39|ETH_MD3_P|Ethernet Pair|N/A|N/A|
|40|USB_P|USB2.0|N/A|N/A|
|41|GND|Ground|||
|42|GND|Ground|||
|43|ETH_MD2_N|Ethernet Pair|N/A|N/A|
|44|ETH_MD4_N|Ethernet Pair|N/A|N/A|
|45|ETH_MD2_P|Ethernet Pair|N/A|N/A|
|46|ETH_MD4_P|Ethernet Pair|N/A|N/A|
|47|GND|Ground|||
|48|GND|Ground|||
|49|IO_L3_P|BPP PL I/O|IO_L3P_T0L_N4_AD15P_65|U2|
|50|ETH_MD1_N|Ethernet Pair|N/A|N/A|
|51|IO_L3_N|BPP PL I/O|IO_L3N_T0L_N5_AD15N_65|U1|
|52|ETH_MD1_P|Ethernet Pair|N/A|N/A|
|53|GND|Ground|||
|54|GND|Ground|||
|55|IO_L2_P|BPP PL I/O|IO_L2P_T0L_N2_65|P3|
|56|IO_L12_N|BPP PL I/O|IO_L12N_T1U_N11_GC_65|L3|
|57|IO_L2_N|BPP PL I/O|IO_L2N_T0L_N3_65|R3|
|58|IO_L12_P|BPP PL I/O|IO_L12P_T1U_N10_GC_65|L4|
|59|GND|Ground|||
|60|GND|Ground|||
|61|IO_L1_N|BPP PL I/O|IO_L1N_T0L_N1_DBC_65|T2|
|62|IO_L11_P|BPP PL I/O|IO_L11P_T1U_N8_GC_65|L2|
|63|IO_L1_P|BPP PL I/O|IO_L1P_T0L_N0_DBC_65|T3|
|64|IO_L11_N|BPP PL I/O|IO_L11N_T1U_N9_GC_65|L1|
|65|GND|Ground|||
|66|GND|Ground|||
|67|IO_L4_P|BPP PL I/O|IO_L4P_T0U_N6_DBC_AD7P_SMBALERT_65|R4|
|68|IO_L10_P|BPP PL I/O|IO_L10P_T1U_N6_QBC_AD4P_65|M5|
|69|IO_L4_N|BPP PL I/O|IO_L4N_T0U_N7_DBC_AD7N_65|T4|
|70|IO_L10_N|BPP PL I/O|IO_L10N_T1U_N7_QBC_AD4N_65|M4|
|71|GND|Ground|||
|72|GND|Ground|||
|73|IO_L5_P|BPP PL I/O|IO_L5P_T0U_N8_AD14P_65|R1|
|74|USB_VBUS|USB Bus Voltage|N/A|N/A|
|75|IO_L5_N|BPP PL I/O|IO_L5N_T0U_N9_AD14N_65|T1|
|76|USB_CPEN|USB Output Enable|N/A|N/A|
|77|GND|Ground|||
|78|USB_ID|USB ID|N/A|N/A|
|79|ETH_PHY_LED1|Ethernet Activity Indicator|N/A|N/A|
|80|UART0_TX|UART0 Output|N/A|N/A|
|81|ETH_PHY_LED0|Ethernet Speed Indicator|N/A|N/A|
|82|UART1_TX|UART1 Output|N/A|N/A|
|83|I2C1_SDA|I2C data (VCCO_PSIO_501)|N/A|N/A|
|84|UART0_RX|UART0 Input|N/A|N/A|
|85|I2C1_SCL|I2C clock (VCCO_PSIO_501)|N/A|N/A|
|86|UART1_RX|UART1 Input|N/A|N/A|
|87|MIO37|Multiplexed I/O|PS_MIO37|E11|
|88|MIO38|Multiplexed I/O|PS_MIO38|C9|
|89|MIO46|Multiplexed I/O|PS_MIO46|C12|
|90|MIO41|Multiplexed I/O|PS_MIO41|B10|
|91|MIO28|Multiplexed I/O|PS_MIO28|G12|
|92|MIO39|Multiplexed I/O|PS_MIO39|C10|
|93|MIO33|Multiplexed I/O|PS_MIO33|E9|
|94|MIO45|Multiplexed I/O|PS_MIO45|A11|
|95|MIO26|Multiplexed I/O|PS_MIO26|G9|
|96|MIO44|Multiplexed I/O|PS_MIO44|B11|
|97|MIO29|Multiplexed I/O|PS_MIO29|F9|
|98|MIO40|Multiplexed I/O|PS_MIO40|D11|
|99|MIO30|Multiplexed I/O|PS_MIO30|G10|
|100|MIO42|Multiplexed I/O|PS_MIO42|D12|
|101|MIO35|Multiplexed I/O|PS_MIO35|E10|
|102|MIO34|Multiplexed I/O|PS_MIO34|F13|
|103|MIO36|Multiplexed I/O|PS_MIO36|D10|
|104|MIO47|Multiplexed I/O|PS_MIO47|B12|
|105|MIO27|Multiplexed I/O|PS_MIO27|G11|
|106|MIO48|Multiplexed I/O|PS_MIO48|A12|
|107|MIO31|Multiplexed I/O|PS_MIO31|F11|
|108|MIO43|Multiplexed I/O|PS_MIO43|E13|
|109|MIO49|Multiplexed I/O|PS_MIO49|D13|
|110|MIO50|Multiplexed I/O|PS_MIO50|A13|
|111|MIO32|Multiplexed I/O|PS_MIO32|F12|
|112|MIO51|Multiplexed I/O|PS_MIO51|C13|
|113|PS_DONE|Done Booting|PS_DONE|L12|
|114|PS_JTAG_TCK|JTAG|PS_JTAG_TCK|H13|
|115|PS_SRST_N|BBP Reset|PS_SRST_B|K13|
|116|PS_JTAG_TDI|JTAG|PS_JTAG_TDI|H12|
|117|PWR_GOOD|Power Good|N/A|N/A|
|118|PS_JTAG_TDO|JTAG|PS_JTAG_TDO|J13|
|119|PS_JTAG_PWR|1.8V JTAG power reference.|VCCO_PSIO0_500|AA5|
|120|PS_JTAG_TMS|JTAG|PS_JTAG_TMS|J12|

# J601

J601 is a board to board connector which provides access to power and RF control signals.  The mating connector used on the host board is JAE Electronics WR-40SB-VHF30-N1.  The following table defines the signal connections including connections associated with the ADRV9002.  The SDK function represents the software functionality included in the Software Development Kit. 

|   SDK Function   |       Description             |        Signal         | Pin | Pin |        Signal         |       Description             |   SDK Function   |
|------------------|-------------------------------|-----------------------|-----|-----|-----------------------|-------------------------------|------------------|                 
|                  | Ground                        | GND                   | 1   | 2   | GND                   | Ground                        |                  |
|                  | RF External Clock             | RF_CLK_N              | 3   | 4   | MCS_P                 | ADRV9002 MCS+ (D7)            |                  |
|                  | RF External Clock             | RF_CLK_P              | 5   | 6   | MCS_N                 | ADRV9002 MCS- (D8)            |                  |
|                  | Ground                        | GND                   | 7   | 8   | GND                   | Ground                        |                  |
|                  | ADRV9002 AGPIO_2 (E3)         | AGPIO_2               | 9   | 10  | AUXADC_0              | ADRV9002 AUXADC_0 (H11)       |                  |
|                  | ADRV9002 AGPIO_4 (F4)         | AGPIO_4               | 11  | 12  | AUXADC_1              | ADRV9002 AUXADC_1 (B8)        |                  |
| RX1 Enable       | ADRV9002 AGPIO_6 (G6)         | AGPIO_6               | 13  | 14  | GND                   | Ground                        |                  |
|                  | Ground                        | GND                   | 15  | 16  | AGPIO_1               | ADRV9002 AGPIO_1 (F10)        |                  |
|                  | Ground                        | GND                   | 17  | 18  | AGPIO_3               | ADRV9002 AGPIO_3 (F5)         |                  |
|                  | Ground                        | GND                   | 19  | 20  | AGPIO_5               | ADRV9002 AGPIO_5 (G4)         | RX2 Enable       |
|                  | MIO Reference Voltage         | VCCIO                 | 21  | 22  | GND                   | Ground                        |                  |
|                  | Ground                        | GND                   | 23  | 24  | GND                   | Ground                        |                  |
|                  | Ground                        | GND                   | 25  | 26  | GND                   | Ground                        |                  |
|                  | Ground                        | GND                   | 27  | 28  | GND                   | Ground                        |                  |
|                  | 5V                            | VCC5V0                | 29  | 30  | VCC5V0                | 5V                            |                  |
|                  | 5V                            | VCC5V0                | 31  | 32  | VCC5V0                | 5V                            |                  |
|                  | 5V                            | VCC5V0                | 33  | 34  | VCC5V0                | 5V                            |                  |
|                  | 5V                            | VCC5V0                | 35  | 36  | VCC5V0                | 5V                            |                  |
|                  | 5V                            | VCC5V0                | 37  | 38  | VCC5V0                | 5V                            |                  |
|                  | 5V                            | VCC5V0                | 39  | 40  | VCC5V0                | 5V                            |                  |

# Boot Switch

The boot switch allows the user to select the location of boot code for the baseband processor.  When SD is selected the BBP will boot from the external SD card.  When EMMC is selected the BBP will boot from the 8GB on-board eMMC flash.    

![boot_switch](boot_switch.png)

# ADRV900x Interface

Additional information can be found on [NextGen RF Design's website.](https://www.nextgenrf.com/products/rf-system-on-a-module/)

# DAC

The BytePipe_x9002 provides three 12-bit digital to analog converters (DAC) for controlling external analog peripherals.

![dac](dac.png)

# ADC

The BytePipe_x9002 supports several different analog to digital converter (ADC) channels accessible by the host.  Two ADRV9002 auxiliary ADCs are useful for measuring DC and near-DC signals.  The auxiliary ADCs operate from 0.05V to 0.95V with 12-bits of resolution.  In addition to the auxiliary ADCs, the BBP supports up to 4 differential ADC channels through J600.  The BBP ADCs support up to 200kSps with 10-bits of resolution.

![adc](adc.png)

# MCS

The BytePipe_x9002 allows for multiple SOMs to be synchronized by connecting the multi-chip synchronization (MCS) signal between devices.  When multi-chip synchronization is used, the RF clock must also be shared across devices.

![mcs](mcs.png)

# SPI

Up to two SPI peripherals can be mapped to the PS MIO pins.   Each SPI peripheral supports the following functionality: 
*	128 byte read and write FIFO
*	Master or slave modes
*	Up to three chip select lines
*	Multi-master support
*	Poll or interrupt modes

# UART

The BytePipe_x9002 has two dedicated UART channels, UART0 and UART1.   Both channels operate at the host defined VCCIO voltage.  They are connected to PS bank 500 and routed to the two internal PS UART channels.  The following table shows the BBP connections. Each UART can be configured for any standard baud rate from 110 baud to 921600 baud.  If additional UART channels or non-standard baud rates are required, they can be implemented using the programmable logic and connected to a combination of IO_L[x]_N/P pins.

|  Signal             | BBP Pin     | Description                  |
|---------------------|-------------|------------------------------|
| UART0_TX            | MIO11       | UART0 transmit output        |
| UART0_RX            | MIO10       | UART0 receive input          |
| UART1_TX            | MIO8        | UART1 transmit output        |
| UART1_RX            | MIO9        | UART1 receive input          |

# I2C

The BytePipe_x9002 has one dedicated I2C channel (I2C1) operating at the host defined VCCIO voltage.  This interface is connected to PS bank 500 and routed to the PS I2C1 module internal to the BBP.  

|  Signal             | BBP Pin     | Description                  |
|---------------------|-------------|------------------------------|
| I2C1_SCL            | MIO24       | I2C1 bi-directional clock    |
| I2C1_SDA            | MIO25       | I2C1 bi-directional data     |

# CAN

Up to two CAN peripherals can be mapped to the PS MIO pins.  Each CAN peripheral supports the following functionality:
*	Conforms to the ISO 11898-1, CAN2.0A, and CAN 2.0B standards
*	Both standard (11-bit identifier) and extended (29-bit identifier) frames
*	Bit rates up to 1Mb/s
*	64 byte transmit and receive FIFOs
*	Interrupt support
*	Automatic re-transmission on errors or arbitration loss in normal mode
*	Sleep Mode with automatic wake-up
*	Snoop Mode
*	16-bit timestamping for receive messages

# GPIO

The PS multiplexed I/O (MIO) are connected directly to bank 501 of the BBP.   The table below shows the supported interfaces for each pin.  The voltage for bank 501 is supplied by the host and can be 1.8V, 2.5V, or 3.3V.

![ps_mio](ps_mio.png)

# GigaBit Transceivers (GTR)

Four gigabit transceivers (GTR) are connected from bank 505 to J600.  Each of the four GTR channels can be connected to one of five high-speed serial I/O peripherals.  The serial I/O peripherals include PCIe, SATA, Display Port, USB3.0, and GMII.  

# RF Ports

The BytePipe_x9002 RF interface allows for a wide range of applications.  The ADRV9002’s two transmit and four receive ports are matched to a single-ended 50ohm impedance and accessible through U.FL top side connectors or SMPM bottom side connectors.  All 6 ports support 30MHz to 6GHz.  The top side U.FL connectors allow for cabled connections to be made to any of the ports.  The SMPM bottom side connectors allow for either a cabled connector or board to board connection using a SMPM blind adapter.  The connectors are not switched.  Instead both the U.FL and SMPM connections are connected in parallel and directly above or below one another.  A single via connects directly to both connectors from the 50ohm RF stripline.  This connection is not ideal, but it is sufficient and allows users to connect using top side U.FL cables or bottom side SMPM board to board connectors with minimal complexity.  Most applications will require an external RF frontend due to the modest output power and relatively high receiver noise figure of the ADRV9002.  The minimal losses associated with the connectors will be easily made up for on the transmit path with an external PA and on the receive path where an external LNA is required to reduce the noise figure.

![rf_ports](rf_ports.png)

# RF Clock

The RF clock provides the ADRV9002 a reference for its internal PLLs and clocks.  It can be supplied using an on-board 38.4MHz VC-TCXO or supplied externally by the host through J601.  A mux allows the ADRV9002 to control which reference clock to use.  By default, the VC-TCXO is enabled.  The VC-TCXO can be fine-tuned using the AGPIO0 DAC from the ADRV9002.  For applications requiring specific clocking requirements or for multi-chip synchronization a clock can be supplied externally through J601.  

![rf_clk](rf_clk.png)


# Gigabit Ethernet

The Gigabit Ethernet signals are generated by the DP83867IRRGZR Ethernet transceiver connected to the BBP via RGMII interface through PS bank 502. The BBP includes a gigabit Ethernet MAC (GEM) controller which implements a 10/100/1000 Mb/s Ethernet MAC that is compatible with the IEEE Standard for Ethernet (IEEE Std 802.3-2008) and capable of operating in either half or full-duplex mode in 10/100 mode and full-duplex in 1000 mode. The following table shows the BBP connections to the Ethernet transceiver. 

|  Signal             | BBP Pin    | Description                  |
|---------------------|------------|------------------------------|
| TX_CLK              | MIO64      | Transmit Clock               |
| TX_DATA[0]          | MIO65      | Transmit Data                |
| TX_DATA[1]          | MIO66      | Transmit Data                |
| TX_DATA[2]          | MIO67      | Transmit Data                |
| TX_DATA[3]          | MIO68      | Transmit Data                |
| TX_CTL              | MIO69      | Transmit Control             |
| RX_CLK              | MIO70      | Receive Clock                |
| RX_DATA[0]          | MIO71      | Receive Data                 |
| RX_DATA[1]          | MIO72      | Receive Data                 |
| RX_DATA[2]          | MIO73      | Receive Data                 |
| RX_DATA[3]          | MIO74      | Receive Data                 |
| RX_CTL              | MIO75      | Receive Control              |
| MDC                 | MIO76      | MDC Clock                    |
| MDCIO               | MIO77      | MDC Data                     |

The BBP supports up to four GEMs. GEM3 is used for the dedicated BytePipe_x9002 gigabit Ethernet interface. GEM0 or GEM1 can be configured to implement additional gigabit Ethernet interfaces on the host PCB. This is supported by configuring PS bank 501 MIO pins as reduced gigabit media independent interface (RGMII) 2.0. The host will have to implement an Ethernet transceiver like the DP83867IRRGZR.

* IEEE Standard 802.3-2008 compatible
* 10/100/1000 Mb/s transfer rates
* RGMII with external PHY attached to MIO pins
* GMII/MII interface to PL (TBI, RGMII v2.0).
* SGMII to PS GTR transceivers (1000 Mb/s rate, only)
* 1000BASE-SX and 1000BASE-LX to PS GTR transceivers
* IPv4 and IPv6 transmit and receive IP, TCP, and UDP checksum offload
* MDIO interface for physical layer management
* 64-bit AXI DMA master
* Interrupt generation
* Automatic CRC
* Automatic Frame filtering
* Full duplex flow control
* Address checking
* Ethernet loopback mode
* IEEE Precision Time Protocol (PTP)
* Statistics counter registers for RMON/MIB
* Jumbo frames up to 10,240 bytes.

The BytePipe_x9002 SOM comes pre-programmed with a globally unique EUI-64TM which can be used for the MAC address. This is implemented using the Microchip 24AA025E64T-I/OT EEPROM device.

# USB PHY

USB2.0 and USB3.0 are supported when using a combination of the USB2.0 PHY and GTR channels. USB on-the-go (OTG), host mode, or device mode is supported. The USB2.0 interface uses the Microchip USB3320 USB2.0 ULPI PHY. The USB2.0 PHY is connected PS bank 502 of the BBP.

|  Signal         | BBP Pin    | Description              |
|-----------------|------------|--------------------------|
| CLK             | MIO52      | ULPI PHY Clock           |
| DIR             | MIO53      | Data Bus Direction       |
| DATA[0]         | MIO56      | Data Bus                 |
| DATA[1]         | MIO57      | Data Bus                 |
| DATA[2]         | MIO54      | Data Bus                 |
| DATA[3]         | MIO59      | Data Bus                 |
| DATA[4]         | MIO60      | Data Bus                 |
| DATA[5]         | MIO61      | Data Bus                 |
| DATA[6]         | MIO62      | Data Bus                 |
| DATA[7]         | MIO63      | Data Bus                 |
| NXT             | MIO55      | Data Throttle            |
| STP             | MIO58      | Data Stop                |

# Cover

The gold anodized aluminum cover provides RF shielding along with a heat sink interface.  

# ADRV900x Version
Early engineering samples of the BytePipe_x9002 had a mix of ADRV9002 silicon.  These versions used the ADRV9002 RevB0 silicon while later versions used the ADRV9002 RevC0 silicon.  Currently all released versions of the BytePipe_x9002 have the ADRV9002 RevC0 silicon. 

# DISCLAIMER

THIS SOFTWARE IS COVERED BY A DISCLAIMER FOUND [HERE](../../DISCLAIMER.md).
