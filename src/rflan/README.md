![logo](../../docs/BytePipe_Logo.png)

# RFLAN

The rflan application is an embedded C real-time application running on the Cortex R5 real-time processing unit (RPU).  The programmable logic includes the [adrv9001](../adrv9001/README.md) synchrounous serial interface along with DMAs for streaming IQ data or modem data to and from shared memory.  The RFLAN firmware configures and controls the ADRV9001.  It also directs DMA data to and from shared memory to the command line interface (CLI) using a serial port or Ethernet.  The following diagram shows the main components of the RFLAN including firmware running on the RPU and programmable logic.  The RFLAN can operate as a standalone application or integrated with a Linux application running on the Cortex A53 using interproccessor communication (IPC).  

To evaluate the rflan application the latest executables can be downloaded from the latest release folder found [here](https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/releases) or built from source files.

![software_stack](docs/software_stack_01.png)

# Programmable Logic

The RFLAN programmable logic consists of the ADRV9002 IP, ZynqUltrascale+ CPU, and transmit and receive DMAs as shown below.  The programmable logic allows for streaming of IQ data to and from the processor for evaluation of the BytePipe_x9002.  It also serves as a starting point for developing custom applications by inserting custom programmable logic between the ADRV9001 and DMAs which can be used for data transfer or debugging.  This is shown in the [rflan_qpsk example](../rflan_qpsk/README.md).  Additional information regarding the ADRV9002 can be found [here](../adrv9001/README.md).

To build the programmable logic see `Building Source` found below.

[![hdl_01](docs/hdl_01.png)]()

# Real-Time Processor (RPU) Software

The RFLAN software is designed to execute on a single Cortex-R5 real-time processor (RPU).  The RFLAN software provides all of the basic configuration and control of the ADRV9002 and DMAs.  Users can interact with the RFLAN through a command line interface (CLI) connected via the serial port or ethernet port.  The RFLAN application allows for evalution of the BytePipe_x9002 and serves as a starting point for custom production designs.  The RFLAN software is capable of interfacing to a Petalinux application running on the application processing unit (APU).  

The following diagram shows the main software components including several applications running in parallel along with a collection of libries including general libraries (lib) the chip support library (csl), and adrv9001 library.  

![rflan_files_01](docs/rflan_files_01.png)

The following diagram provides an overview of each library component.  The ADRV9001 library primarily uses the [adrv9001 software library](../adrv9001/README.md).  The RFLAN ADRV9001 library does include its own default profile generated from Analog Devices Transceiver Evaluation Software.  The default profile can be easilly updated to support a variety of applications with directions found in the following sections.    

![rflan_files_02](docs/rflan_files_02.png)

# CLI Interface

The command line interface (CLI) provides an external user inteface accessible through a serial port or ethernet connection.  Both interfaces are operational simultaneously allowing users to use one or both interfaces.  The serial port is accessible through the JTAG/USB interface using a USB micro cable connected to J26 on the [BytePipe Hardware Development Kit](../../docs/hardware/hdk/hdk.md).  [Tera Term](https://download.cnet.com/Tera-Term/3000-2094_4-75766675.html) is the preferred serial terminal operating at 115200 baud.  

![cli_01](docs/cli_01.png)

To access the CLI using ethernet connect an ethernet cable to the RFJ45 connector on the [BytePipe Hardware Development Kit](../../docs/hardware/hdk/hdk.md) directly to a dedicated NIC on a PC.  The RFLAN ethernet stack [(lwip)](https://docs.xilinx.com/v/u/en-US/xapp1026) is configured to use a static IP address.  To connect to the BytePipe using a Windows PC the dedicated NIC IP address should be configured to use the same subnet of the BytePipe as shwon below.  

![network_settings](docs/network_settings.png)

[Putty](https://www.putty.org/) is the preffered tool for interfacing to the CLI via etherent.  The following diagram shows the Putty configuration.  

![putty_01](docs/putty_01.png)

Once connected CLI commands can be executed by typing in the Putty terminal.  The following shows the results of the `help` command which provides a list of supported CLI commands.  Users can easily implment their own custom commands supporting a range of custom applications.  For additional information on the CLI interface watch the following video found [here]() or by clicking the following figure.

[![putty_02](docs/putty_02.png)]()

# Building Source

Building the source files is straightforward using the supplied build scripts and video tutorials.  The first step in building the rflan source files is to build the HDL.  An alternative to building the HDL from source is to download the latest released version.  To build the HDL from source see the [Building HDL](#building-hdl) section below.  To use the latest pre-built HDL release follow instructions for [Building RPU Software](#building-rpu-software).  The source files can be built anywhere on the host machine.  It is good to build them in a seperate folder from the source files so they can be deleted without effecting the source.  The following example assumes the bytepipe_sdk repository is cloned or downloaded to the users `C:` drive.  

```
cd c:
git clone https://github.com/NextGenRF-Design-Inc/bytepipe_sdk.git
cd bytepipe_sdk/
mkdir workspace
cd workspace/
```

# Building Programmable Logic

The following describes the process for building the HDL from source files.  The HDL is built using [Vivado 2021.1](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vivado-design-tools/2021-1.html). For additional information on setting up the build environment refer [here](../../docs/build_environment/BuildEnv.md). 

Start by launching cygwin on your windows machine and execute the following commands.  If Vivado is not installed at `C:/Xilinx/` update the path accordingly.  The build script will download the necessary source files and build the HDL.  When the build is finished the following output will be generated: `workspace/rflan/rflan_xzcuxxx.xsa`.  Once the build is completed the project can be opended using Vivado.  

```
export PATH=/cygdrive/c/Xilinx/Vivado/2021.1/bin/:$PATH
make -f ../src/rflan/Makefile hdl
```

Please review the video found below for additional information.

[![building_hdl](docs/building_hdl.png)]()

# Building RPU Software

The software is built using the Xilinx Vitis IDE.  The build script will automatically download the latest HDL outputs if not previously generated.  Start by launching cygwin on your windows machine and execute the following commands.  If Vitis is not installed at `C:/Xilinx/` update the path accordingly.  The script will generate the hardware platform, board support package, and rflan application.  Once the script is finished the project can be openend using Vitis with the workspace set to `workspace/rflan`. 

```
export PATH=/cygdrive/c/Xilinx/Vitis/2021.1/bin/:$PATH
make -f ../src/rflan/Makefile sw
```

Please review the video found below for additional information.

[![building_rpu](docs/building_rpu.png)]()

# Building ADRV9002 Profile

The ADRV9002 configuration is generated from Analog Devices Tranceiver Evaluation Software (TES).  The RFLAN includes a default configuration found in `src/rflan/sw/adrv9001/profile'.  To update the profile new settings need to be exported from TES and then compiled into the RFLAN. 

```
export PATH=/cygdrive/c/Xilinx/Vitis/2021.1/bin/:$PATH
make -f ../src/rflan/Makefile profile
```

Please review the video found below for additional information.

[![generating_rflan_profile](docs/generating_rflan_profile.png)](https://youtu.be/SMOLgKAgsfg)


# Programming BytePipe RFLAN

To program the BytePipe with the RFLAN application execute the following command once the code has been built.  This will create a folder called `sd_card` with the contents to be copied to the sd card.  

```
make -f ../src/rflan/Makefile sd_card
```

Please review the video found below for additional information.

[![programming_sd_card](docs/programming_sd_card.png)](https://youtu.be/JeQZI49h6uE)

# MATLAB bpToolbox

The RFLAN application can be controlled from MATLAB through the CLI interface accessible using a serial port or ethernet port.  For additional information see [bpToolbox](../bpToolbox/README.md).