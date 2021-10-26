
![logo](../BytePipe_Logo.png)

# Debugging Source

This document describes debugging the RFLAN application.  Once the application is built, navigate to `Debug As` and select the debugger as shown below.  Make sure the `rflan` project is selected in the project explorer before doing this.

![debug_01](debug_01.png)

Once the debugger is launched, right click and disconnect it.  Navigate to Run->Debug Configurations and check `Reset entire system`.  This should automatically check `Program FPGA` as shown below.

![debug_02](debug_02.png)

Select `Apply` then `Debug`.  The ZynqMP processor does not allow for the debugger to be restarted.  To restart, the debug session must be terminated and re-launched.

# Disclaimer
----------------------
NextGen RF Design makes no warranty for the use of this code or design. This code is provided  "As Is". NextGen RF Design assumes no responsibility for
any errors, which may appear in this code, nor does it make a commitment to update the information contained herein. NextGen RF Design specifically
disclaims any implied warranties of fitness for a particular purpose.
Copyright(c) NextGen RF Design
All rights reserved.

