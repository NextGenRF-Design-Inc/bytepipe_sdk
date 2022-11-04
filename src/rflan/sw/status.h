#ifndef SRC_RFLAN_STATUS_H_
#define SRC_RFLAN_STATUS_H_
/***************************************************************************//**
*  \ingroup    RFLAN
*  \defgroup   APP RFLAN Application
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       app.h
*
*  \details    This file contains the RFLAN application definitions.
*
*  \copyright
*
*  Copyright 2021(c) NextGen RF Design, Inc.
*
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*   - Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   - Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
*   - The use of this software may or may not infringe the patent rights of one
*     or more patent holders.  This license does not release you from the
*     requirement that you obtain separate licenses from these patent holders
*     to use this software.
*   - Use of the software either in source or binary form, must be run on or
*     directly connected to a NextGen RF Design, Inc. product.
*
*  THIS SOFTWARE IS PROVIDED BY NEXTGEN RF DESIGN "AS IS" AND ANY EXPRESS OR
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
*  EVENT SHALL NEXTGEN RF DESIGN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
*  INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/
#include "cli.h"
#include "ff.h"
#include "adrv9001.h"
#include "rflan.h"

#ifdef VERSA_CLOCK5_ENABLE
#include "versa_clock5.h"
#endif

static const char *StatusString( int32_t status )
{
  switch(status)
  {
    case 0:
      return "Success";
      
#ifdef VERSA_CLOCK5_ENABLE

    case VersaClock5Status_InvalidParameter:
    	return "VersaClock5 Invalid Parameter";

    case VersaClock5Status_IicError:
    	return "VersaClock5 I2c Error";      

#endif

    case Adrv9001Status_ProfileInitErr:
      return "Profile Initialization Error";

    case Adrv9001Status_ProfileCalErr:
      return "Profile Calibration Error";

    case Adrv9001Status_ProfileCfgErr:
      return "Adrv9001 Profile Configuration Error";

    case Adrv9001Status_ToPrimedErr:
      return "Adrv9001 To Primed Error";

    case Adrv9001Status_ToCalErr:
      return "Adrv9001 To Calibrated Error";

    case Adrv9001Status_ToEnabledErr:
      return "Adrv9001 To Enabled Error";

    case Adrv9001Status_NotSupported:
      return "Adrv9001 Not Supported";

    case Adrv9001Status_DacErr:
      return "Adrv9001 DAC Error";

    case Adrv9001Status_TxAttnErr:
      return "Adrv9001 Tx Attenuation Error";

    case Adrv9001Status_TxBoostErr:
      return "Adrv9001 Tx Boost Error";

    case Adrv9001Status_GpioErr:
      return "Adrv9001 Gpio Error";

    case Adrv9001Status_LogErr:
      return "Adrv9001 Log Error";

    case Adrv9001Status_Busy:
      return "Adrv9001 Busy";

    case Adrv9001Status_InvalidPib:
      return "Adrv9001 Invalid PIB";

    case Adrv9001Status_SpiErr:
      return "Adrv9001 SPI Error";

    case Adrv9001Status_SsiSweepErr:
      return "Adrv9001 SSI Sweep Error";

    case Adrv9001Status_SsiTestModeErr:
      return "Adrv9001 SSI Test Mode Error";

    case Adrv9001Status_SsiSetErr:
      return "Adrv9001 SSI Set Error";

    case Adrv9001Status_InvalidState:
      return "Adrv9001 Invalid State";

    case Adrv9001Status_CarrierFreqErr:
      return "Adrv9001 Carrier Frequency Error";

    case Adrv9001Status_ReadErr:
      return "Adrv9001 Read Error";

    case Adrv9001Status_EnableModeErr:
      return "Adrv9001 Enable Mode Error";
    
    case Adrv9001Status_InvalidEnableMode:
      return "Adrv9001 Invalid Enable Mode";

    case Adrv9001Status_Tx1SsiCalErr:
    	return "Adrv9001 Tx1 SSI Calibration Error";

    case Adrv9001Status_Tx2SsiCalErr:
    	return "Adrv9001 Tx2 SSI Calibration Error";

    case Adrv9001Status_Rx1SsiCalErr:
    	return "Adrv9001 Rx1 SSI Calibration Error";

    case Adrv9001Status_Rx2SsiCalErr:
    	return "Adrv9001 Rx2 SSI Calibration Error";

//    case Adrv9001Status_SsiSetErr:
//      return "Adrv9001";
//
//    case Adrv9001Status_SsiSetErr:
//      return "Adrv9001";

    case CliStatus_MemoryError:
      return "Memory Error";

    case CliStatus_NotSupported:
      return "Not Supported";

    case CliStatus_OsError:
      return "OS Error";

    case CliStatus_Busy:
      return "Busy";

    case CliStatus_InvalidParameter:
      return "Invalid Parameter";

    case CliStatus_FileError:
      return "File Error";

    case FR_DISK_ERR:
      return "Disk Error";

    case FR_INT_ERR:
      return "Disk Assert";

    case FR_NOT_READY:
      return "Disk Not Ready";

    case FR_NO_FILE:
      return "Could not find file";

    case FR_NO_PATH:
      return "Could not find path";

    case FR_INVALID_NAME:
      return "Invalid path";

    case FR_DENIED:
    case FR_EXIST:
      return "Access denied";

    case FR_INVALID_OBJECT:
      return "Invalid file or path";

    case FR_WRITE_PROTECTED:
      return "Write Protected";

    case FR_INVALID_DRIVE:
      return "Invalid Drive";

    case FR_NOT_ENABLED:
      return "Drive not enabled";

    case FR_NO_FILESYSTEM:
      return "No FAT file system";

    case FR_MKFS_ABORTED:
      return "mkfs Error";

    case FR_TIMEOUT:
      return "Disk Timeout";

    case FR_LOCKED:
      return "Disk Locked";

    case FR_NOT_ENOUGH_CORE:
      return "LFN buffer not available";

    case FR_TOO_MANY_OPEN_FILES:
      return "Too many open files";

    case FR_INVALID_PARAMETER:
      return "Invalid Parameter";

    case PibStatus_UnknownParameter:
    	return "Unknown PIB Parameter";

    case PibStatus_InvalidParameter:
      return "PIB Invalid Parameter";

    case RflanStatus_InvalidPib:
      return "Rflan Invalid Pib";

    default:
      return "Unknown Status";
  }
}

#endif /* SRC_RFLAN_H_ */
