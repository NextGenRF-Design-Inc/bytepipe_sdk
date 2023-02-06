[![logo](../BytePipe_Logo.png)](../../README.md)

# Overview
In order to boot the BytePipe SOM from a micro-SD card on a carrier board such as the [HDK](../main/docs/hardware/hdk/hdk.md) or [RadioCarbon](../main/docs/hardware/RadioCarbon/RadioCarbon.md), the card needs to be properly formatted and contain the appropriate files.

If you purchased the HDK Kit with SOM, a pre-formatted micro-SD card was included which contained a release of the RFLAN application.  To boot using a different version of RFLAN simply insert the micro-SD card into a laptop or PC, delete the existing files on the card, and copy the new files to the root folder.

**Note:** It is ok to store different versions of files, including the boot.bin, in sub-folders on the micro-SD card.  Only the files in the root folder will used during the boot sequence.

## Supported micro-SD Cards
NextGen RF Design does not qualify specific brands of micro-SD cards for use with the BytePipe SOM as most commonly available cards should be compatible, however, there are some basic requirements in order for the card to be recognized and properly boot.

### Size and Formatting
The primary partition of the micro-SD card must be formatted as FAT or FAT32, so a 1GB to 32GB card is recommended as these will typically be pre-formatted as these types.  If the primary partition is formatted using any other file allocation table format, such as exFAT, NTFS, or any Linux-based format, the SOM will not boot.

In order to use a micro-SD card larger than 32GB, a primary partion of 32GB or smaller will need to be created.  This can be done on MS Windows or Linux machines with built-in or third-party tools.  The detailed instructions on this process are beyond the scope of this documentation, but are readily available on the internet.

**NOTE:** Extreme caution should always be used when partitioning and formatting devices as unintended dataloss can occur if settings are not properly chosen. 

# Additional Details

## Boot Files
At a minimum, the card must contain a boot.bin (case insensitive) file in the root folder in order for the SOM to boot.  RFLAN-based applications will automatically run commands contained in plain text files named adrv9001_setup.txt and rflan_setup.txt if they exist in the root folder.  These can be used to automate any command that can be entered via the CLI.  Comment lines can be added by starting with `//`.

## Linux Builds
The micro-SD card used for Linux builds may vary, but typically include a small (1GB) partition for the MBR formatted as FAT or FAT32, and a larger partion for the Linux File System formatted as ext4

# DISCLAIMER
THIS SOFTWARE IS COVERED BY A DISCLAIMER FOUND [HERE](https://github.com/NextGenRF-Design-Inc/bytepipe_sdk/blob/main/DISCLAIMER.md).
