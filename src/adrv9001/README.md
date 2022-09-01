![logo](../../docs/BytePipe_Logo.png)

# ADRV9001

The ADRV9001 module provides the hardware interface between the BytePipe programmable logic and ADRV9002.  This interface has been optimized for the BytePipe and production style products.  Included in this driver is the HDL required for the programmable logic along with the software drivers for communication from the CPU to the programmable logic.  

![adrv9001_files_01.png](docs/adrv9001_files_01.png)

# Programmable Logic (HDL)

The adrv9001 programmable logic converts the Synchronous Serial Interface (SSI) connected to the ADRV9002 to independent transmit and receive AXI STREAM interfaces.  This module also provides a AXI4 interface for controlling the ADRV9002 GPIO and TDD timing.  The following diagram shows the programmable logic structure.  The programmable logic can be built by executing a single script (make_hdl.tcl).  The top-level module, axi_adrv9001.v, implements the adrv9001 configurable registers, two adrv9001 transmit interfaces, two adrv9001 receive interfaces, and an optional integrated logic analyzer.  


![adrv9001_files_02.png](docs/adrv9001_files_02.png)

The resulting axi_adrv9001 standalone ip is shown below.  Each transmit and receive interface to the users programmable logic is implemented as a AXI STREAM interface consisting of axis_tdata, axis_tvalid, and axis_tready signals.  Each AXI STREAM interface has a corresponding clock output operating at 2x the sample rate.

![axi_adrv9001.png](docs/axi_adrv9001.png)

# ADRV9001 Transmit Programmable Logic

The ADRV9001 transmit programmable logic is responsible for converting the AXI STREAM data to ADRV9001 SSI data.  This is done by unpacking the AXI STREAM data into serial bits using built-in SERDES.  The AXI STREAM data is clocked in at twice the sample rate using the AXI STREAM axis_tvalid and axis_tready signals.  Each bit is clocked out using the SERDES at 16x the sample rate.  For the highest sample rate of 61.44MSps the resulting clock rate is 491.52MHz with data clocked on both the rising and falling edge of the clock.    

![adrv9001_files_03.png](docs/adrv9001_files_03.png)

The following diagram shows the timing associated with the transmit AXI STREAM and ADRV9001 enable signals.  The transmit enable signal is controlled by either writing to the tx1_tdd_en register or by driving tx1_pl_en from the programmable logic.  When enabling the transmitter, it is assumed data is valid at the input of the adrv9001 module either from a DMA or custom programmable logic module.  Before data is accepted by the transmit programmable logic the transmit enable signal driven high for `tx1_ssi_enable_cnt` samples which corresponds to the software variable `Tx1SsiEnableDly`.  This allows the ADRV9001 transmitter to transition from primed to enabled.  Data is then clocked into the module by driving tx1_tready high every other clock cycle.  The transmit enable signal remains high for `Tx1DataValidDur` samples followed by `Tx1DisableDly` samples to ensure the data is flushed from the ADRV9001 pipeline.    

![tx_axis_02.png](docs/tx_axis_02.png)


# ADRV9001 Receive Programmable Logic

The ADRV9001 receive programmable logic is responsible for converting the ADRV9001 SSI data to AXI STREAM data.  This is done by packing serial SSI data using built-in SERDES to AXI STREAM data.  The AXI STREAM data is clocked out at twice the sample rate using the AXI STREAM axis_tvalid signal.     

![adrv9001_files_04.png](docs/adrv9001_files_04.png)

The following diagram shows the timing associated with the receive AXI STREAM and ADRV9001 enable signals.  The receive enable signal is controlled by either writing to the rx1_tdd_en register or by driving rx1_pl_en from the programmable logic.  When enabling the receiver, it is assumed upstream modules are ready to receive data such as a DMA or other custom module.  Before AXI STREAM data is available the receive enable signal is driven high for `rx1_ssi_enable_cnt` samples which corresponds to the software variable `Rx1SsiEnableDly`.  This allows the ADRV9001 receiver to transition from primed to enabled.  Data is then clocked out using the AXI STREAM interface by driving rx1_tvalid high every other clock cycle.  The AXI STREAM data continues to be clocked out for `Rx1SampleCntDur` samples followed by `Rx1SsiDisableDly` samples to ensure the data is flushed from the ADRV9001 pipeline.    

![rx_axis_02.png](docs/rx_axis_02.png)

# ADRV9001 Programmable Logic Configuration

The ADRV9001 programmable logic is created as a standalone IP module.  The following diagram shows the configuration options for the IP.  Each transmit and receive interface has the option for enabling a integrated logic analyzer (ILA).  When an ILA is enabled, users can access the corresponding signals through JTAG using the hardware manager in vivado.

In addition to the ILA configuration each SSI differential pair can be swapped for designs such as the BytePipe where it is advantageous for PCB routing to swap the positive and negative signals of a differential pair.  

![axi_adrv9001_configuration.png](docs/axi_adrv9001_configuration.png)

# ADRV9001 Register Programmable Logic

The ADRV9001 module provides several registers for control and configuration.  The following table defines each registers function and address.  

| Address   | Bit Field   | Field               | Description                                                             |
|-----------|-------------|---------------------|-------------------------------------------------------------------------|
| 0x0000    | [31:1]      | unused              |                                                                         |
|           | [0]         | tx1_tdd_en          | Transmit channel 1 tdd enable.                                          |
| 0x0001    | [31:1]      | unused              |                                                                         |
|           | [0]         | tx2_tdd_en          | Transmit channel 2 tdd enable.                                          |
| 0x0002    | [31:1]      | unused              |                                                                         |
|           | [0]         | rx1_tdd_en          | Receive channel 1 tdd enable.                                           |
| 0x0003    | [31:1]      | unused              |                                                                         |
|           | [0]         | rx2_tdd_en          | Receive channel 2 tdd enable.                                           |
| 0x0004    | [31:1]      | unused              |                                                                         |
|           | [0]         | rstn                | ADRV9001 Reset Pin                                                      |
| 0x0005    | [31:1]      | unused              |                                                                         |
|           | [0]         | tx1_data_src        | Transmit data source ( 0 = AXI_STREAM, 1 = Internal constant)           |
| 0x0006    | [31:1]      | unused              |                                                                         |
|           | [0]         | tx2_data_src        | Transmit data source ( 0 = AXI_STREAM, 1 = Internal constant)           |
| 0x0007    | [31:0]      | tx1_data            | Transmit data constant associated with tx1_data_src = 1                 |
| 0x0008    | [31:0]      | tx2_data            | Transmit data constant associated with tx2_data_src = 1                 |
| 0x0009    | [31:12]     | unused              |                                                                         |
|           | [11:0]      | dgpio_t             | ADRV9001 Digital GPIO direction (0 = output, 1 = input)                 |
| 0x000A    | [31:12]     | unused              |                                                                         |
|           | [11:0]      | dgpio_o             | Corresponds to ADRV9001 Digital GPIO output value when writing.         |
|           | [11:0]      | dgpio_i             | Corresponds to ADRV9001 Digital GPIO input value when reading.          |
| 0x000B    | [31:0]      | tx1_disable_cnt     | Transmit TDD disable count.                                             |
| 0x000C    | [31:0]      | tx1_ssi_enable_cnt  | Transmit TDD SSI enable count.                                          |
| 0x000D    | [31:0]      | tx2_disable_cnt     | Transmit TDD disable count.                                             |
| 0x000E    | [31:0]      | tx2_ssi_enable_cnt  | Transmit TDD SSI enable count.                                          |
| 0x000F    | [31:0]      | rx1_disable_cnt     | Receive TDD disable count.                                              |
| 0x0010    | [31:0]      | rx1_ssi_enable_cnt  | Receive TDD SSI enable count.                                           |
| 0x0011    | [31:0]      | rx1_ssi_disable_cnt | Receive TDD SSI disable count.                                          |
| 0x0012    | [31:0]      | rx2_disable_cnt     | Receive TDD disable count.                                              |
| 0x0013    | [31:0]      | rx2_ssi_enable_cnt  | Receive TDD SSI enable count.                                           |
| 0x0014    | [31:0]      | rx2_ssi_disable_cnt | Receive TDD SSI disable count.                                          |

# ADRV9001 Software

The ADRV9001 software modules provide an interface and hardware abstraction layer to the Analog Devices ADRV9001-SDK Evaluation Software found [here](https://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/adrv9001-sdk-eval-software.zip).  

![adrv9001_files_05.png](docs/adrv9001_files_05.png)