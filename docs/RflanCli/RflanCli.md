![logo](../BytePipe_Logo.png)

# RFLAN CLI

The RFLAN application supports a custom command line interface (CLI) allowing users to interact with the RFLAN through a serial port and terminal program.  For information on adding custom CLI commands refer to [RFLAN CLI Architecture](../Architecture/Architecture.md#rflan-cli).

# Hardware Connection

To connect to the RFLAN serial port, plug a USB Micro-B cable into the JTAG & Serial Port connector on the BytePipe HDK.

![hdk](../../hardware/hdk/Overview/hdk.png)

# Software Connection

To access the RFLAN CLI a serial terminal such as [Tera Term](https://osdn.net/projects/ttssh2/releases/) or [Putty](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) should be installed.  The serial port on the BytePipe HDK will enumerate as a Virtual COM port using the [FTDI USB Driver](https://ftdichip.com/drivers/). Open the terminal program on the PC and connect to the appropriate COM port with the settings shown below.

![rflan_cli_02](rflan_cli_02.png)

# Bootup

When the RFLAN is initially booted it will display welcome text followed by the results of the ADRV900x initialization.  The welcome text includes the RFLAN version and instructions for displaying a list of CLI commands.

![rflan_cli_01](rflan_cli_01.png)

# Help

Typing `help` displays all the CLI commands available and instructions for using them.  Typing `help` followed by text will display CLI commands that contain the provided text.

![rflan_cli_03](rflan_cli_03.png)

# File System

To list the files BOOT partition type `ls`.  Files can be read issuing the `fread` command.  

![rflan_cli_04](rflan_cli_04.png)


# Waveform Streaming

The RFLAN application supports streaming of IQ data to and from the file system to the radio.  

![rflan_cli_05](rflan_cli_05.png)

# Disclaimer
----------------------
NextGen RF Design makes no warranty for the use of this code or design. This code is provided  "As Is". NextGen RF Design assumes no responsibility for
any errors, which may appear in this code, nor does it make a commitment to update the information contained herein. NextGen RF Design specifically
disclaims any implied warranties of fitness for a particular purpose.
Copyright(c) NextGen RF Design
All rights reserved.

