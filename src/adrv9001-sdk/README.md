![logo](../../docs/BytePipe_Logo.png)

# adrv9001-sdk

This folder includes scripts for bulding the Analog Devices [ADRV9001-SDK Evaluation Software](https://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/adrv9001-sdk-eval-software.zip) for the BytePipe_x9002 hardware.


# Building HDL

The HDL is built using Vivado 2019.2 on a Windows machine using cygwin.  See [Build Environment](../../docs/build_environment/BuildEnv.md) for additional information.


After launching cygwin on your windows machine execute the following commands:

```
cd D:
git clone https://github.com/NextGenRF-Design-Inc/bytepipe_sdk.git
export PATH=/cygdrive/d/Xilinx/Vivado/2019.2/bin/:$PATH
cd bytepipe_sdk/
mkdir workspace
cd workspace/
make -f ../src/adrv9001-sdk/Makefile hdl
```

This will download the necessary source files and build the HDL.

# Building Software




# Programming

Follow instructions for programming the sd card provided in the [ADRV9001-SDK Evaluation Software](https://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/adrv9001-sdk-eval-software.zip).  Once the SD card has been created execute the following commands from a linux bash terminal.

```
make -f ../src/adrv9001-sdk/Makefile sd_card
```




# Transceiver Evaluation Software (TES)

