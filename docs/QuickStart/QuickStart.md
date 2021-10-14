![logo](../BytePipe_Logo.png)

---

# BytePipe SDK Quick Start Guide

The following instructions guide users through setting up and exercising the BytePipe hardware with the latest BytePipe SDK release.

# HDK Unboxing

The following video shows unboxing of the BytePipe hardware development kit.

[![BytePipe Unboxing](docs/../../unboxing_01.png)](https://youtu.be/jkKvJihfD4M)

# Hardware Setup

The following diagram shows the basic hardware setup.  At a minimum a 12V 24W power supply (supplied with HDK), a SD card flashed with the latest software release, and either a serial port connected to a PC or an ethernet network connection is required.  Additional items such as a display connected to the, keyboard and mouse, PMODs, and SSD are optional. 

The RF receive and transmit ports can be connected to the HDK with the supplied SMPM blind adapters or connected through the U.FL connectors on the top side of the SOM.  When plugging the SOM into the HDK take care to properly align the board to board connections including the SMPM adapters.

If it is desired to connect a keyboard and mouse a [OTG USB Adapter](https://www.amazon.com/UGREEN-Adapter-Samsung-Controller-Smartphone/dp/B00LN3LQKQ/ref=sr_1_5?dchild=1&keywords=usb+otg+adapter&qid=1624389445&sr=8-5) is required.  If using a display it should be connected using a mini display port cable. The display must natively support display port and no display port adapters should be used.

![hdk](../../hardware/hdk/Overview/overview.png)


# Flashing SD Card

The BytePipe software boots from JTAG, eMMC, or a SD card.  The quickest way to load the software is to flash the SD card with the latest released image found [here](https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/releases).  Instructions for formatting and copying the appropriate build outputs to the SD card are described in subsequent sections and are often needed when building the source.  However, the most efficient way to load an existing release is to erase and flash the SD card with the lastest SD card image.  Flashing the SD card ensures it is formatted all necessary files are copied correctly.  

To flash the SD card download and install [dotNet Disk Imager](https://sourceforge.net/projects/dotnetdiskimager/).  

![flashing_sd_01](flashing_sd_01.png)

# User Interface

The BytePipe SDK supports several different user interfaces tools.  For interfacing to the RFLAN directly refer to [RFLAN CLI](../RflanCli/RflanCli.md) and [BytePipe Terminal](../BytePipeTerminal/BytePipeTerminal.md).  For interfacing to the Linux framework refer to [BytePipe Terminal](../BytePipeTerminal/BytePipeTerminal.md), [Industrial I/O](../IIO/IIO.md), and [Ubuntu Desktop](../Ubuntu/Ubuntu.md).

The [Industrial I/O](../IIO/IIO.md) Linux kernel driver supports several off the shelf programs include [Matlab](../Matlab/Matlab.md), [GNU Radio](../GnuRadio/GnuRadio.md), [IIO-Oscilloscope](../IIO-Oscilloscope/IIO-Oscilloscope.md), and many other tools that support the [Industrail I/O Driver](https://www.kernel.org/doc/html/v5.0/driver-api/iio/index.html).





