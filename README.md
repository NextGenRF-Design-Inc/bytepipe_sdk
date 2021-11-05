![logo](docs/BytePipe_Logo.png)

---

# BytePipe Software Development Kit

The BytePipe_x9002 SOM is a Software Defined Radio (SDR) System on Module (SOM) based on Analog Devices Inc.’s ADRV9002 Agile Transceiver™ with the Xilinx ZYNC®- UltraScale+ SoC.  The SOM can be used as an evaluation tool, prototyping platform, or integrated into a full-scale product.  The ADRV9002 2x2 MIMO transceiver with integrated DPD engine operates from 30MHz to 6GHz and supports both narrowband and wideband channel bandwidths from 12.5KHz to 40MHz.  The narrow band support makes it suitable for applications including, Land Mobile Radio, APCO P25-PII while supporting higher bandwidth applications like satellite communications, IoT, cellular,  LTE, or Wi-Fi.  NextGen RF Design, through our design services lab, can also support customized versions optimized for application specific requirements.

The BytePipe Software Development Kit (SDK) provides a framework for quickly evaluating the BytePipe hardware platform.  It also serves as a starting point for users to create custom applications.  Below is a high level block diagram of the software.  The software includes Linux Desktop running on one of the Cortex A53 application processors and the RFLAN FreeRTOS application running on one of the Cortex R5 DSP processors.  The programmable logic can be used to support a wide variety of RF communication protocols and includes MathWorks examples.  

![sdk_01](docs/Architecture/sdk_01.png)

---

# Evaluation Quick Start

To get started with evaluation of the BytePipe and the software development kit users can load the latest pre-compiled release and immediately start interacting with the BytePipe hardware.  Detailed instructions can be found in the [Quick Start Guide](docs/QuickStart/QuickStart.md).


# Development Quick Start

To begin development using the BytePipe SDK, start by reviewing the [BytePipe SDK Software Architecture](docs/Architecture/Architecture.md).  This document details the overall software architecture along with the Linux, RFLAN, and programmable logic components.

The environment and design tools required for building the BytePipe SDK components are detailed in [Build Environment](docs/BuildEnv/BuildEnv.md).

Build instructions for each component are documented in [Building RFLAN](docs/RflanBuild/RflanBuild.md), [Building HDL](docs/HdlBuild/HdlBuild.md), and [Building Linux](docs/LinuxBuild/LinuxBuild.md).  The HDL must be built before the RFLAN and Linux components can be built.  However, if the user is only interested in the RFLAN or Linux component, the latest HDL release can be downloaded and used for building these components.  

# User Interface

The BytePipe SDK supports several different user interface tools.  For interfacing to the RFLAN directly refer to [RFLAN CLI](docs/RflanCli/RflanCli.md) and [BytePipe Terminal](docs/BytePipeTerminal/BytePipeTerminal.md).  For interfacing to the Linux framework refer to [BytePipe Terminal](docs/BytePipeTerminal/BytePipeTerminal.md), [Industrial I/O](docs/IIO/IIO.md), and [Ubuntu Desktop](docs/Ubuntu/Ubuntu.md).

The [Industrial I/O](docs/IIO/IIO.md) Linux kernel driver supports several off the shelf programs including [MATLAB](docs/Matlab/Matlab.md), [GNU Radio](docs/GnuRadio/GnuRadio.md), [IIO-Oscilloscope](docs/IIO-Oscilloscope/IIO-Oscilloscope.md), and many other tools that support the [Industrial I/O Driver](https://www.kernel.org/doc/html/v5.0/driver-api/iio/index.html).


# Hardware Overview

The following hardware platforms are supported by the BytePipe_SDK.

|  Document                                                 | Description                           |                                                      |
|-----------------------------------------------------------|---------------------------------------|------------------------------------------------------|
| [BytePipe_x900x](docs/BytePipe_x900x/BytePipe_x900x.md)   | BytePipe ZynqMP / ADRV900x SOM        | ![pic](docs/BytePipe_x900x/bytepipe_x900x.png)       |
| [BytePipe HDK](docs/hdk/hdk.md)                           | BytePipe Hardware Development Kit     | ![pic](docs/hdk/platform.png)                        |
| [Radio Carbon Frontend](docs/RadioCarbon/RadioCarbon.md)  | Richardson RFPD Radio Carbon Frontend | ![pic](docs/RadioCarbon/radio_carbon.png)            |


# DISCLAIMER

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Copyright 2021 (c) NextGen RF Design, Inc. All rights reserved.
