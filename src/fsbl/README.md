![logo](../../docs/BytePipe_Logo.png)

# FSBL

This folder contains custom first stage bootloader files.  To apply these changes to a project first build the desired project and then copy the custom FSBL to the projects hardware platform.

```
cp $srcDir/fsbl/sw/xfsbl_main.c $wrkDir/$proj_name/hwp/zynqmp_fsbl
```

The `rflan` Makefile for example can be called to update the rflan FSBL using the following command:

```
make -f ../src/rflan/Makefile fsbl
```


The changes to the FSBL include programming of the versa clock5 clock device on the BytePipe HDK.  The default clock rates programmed using the FSBL for the BytePipe HDK are shown below:

| ZynqMp Port | 5P49V5935 Port | Clock Frequency |
|-------------|----------------|-----------------|
| GTR_REFCLK0 | OUT3           | 100MHz          |
| GTR_REFCLK1 | OUT4           | 100MHz          |
| GTR_REFCLK2 | OUT2           | 27MHz           |
| GTR_REFCLK3 | OUT1           | 300MHz          |