
/**
 * \file
 * \brief Type definitions for the ADRV9001 profile
 * \copyright Analog Devices Inc. 2021. All rights reserved.
 * Released under the ADRV9001 API license, for more information see "LICENSE.txt" in the SDK
 */

#ifndef _ADI_ADRV9001_PROFILE_TYPES_H_
#define _ADI_ADRV9001_PROFILE_TYPES_H_

#ifndef __KERNEL__
#include <stdint.h>
#endif

#include "adi_adrv9001_clockSettings_types.h"
#include "adi_adrv9001_rxSettings_types.h"
#include "adi_adrv9001_txSettings_types.h"
#include "adi_adrv9001_deviceSysConfig_types.h"
#include "adi_adrv9001_pfirBuffer_types.h"
#include "adi_adrv9001_dynamicProfile_types.h"


/**
* \brief Data structure to hold Stream Binary
*/
typedef struct
{
  uint32_t                      calMask0;
  uint32_t                      calMask1;
  uint32_t                      deviceClockDivisor;
  uint64_t                      carrier0;
  uint64_t                      carrier1;
  uint32_t                      txAtten0;
  uint32_t                      txAtten1;
  uint32_t                      rxGain0;
  uint32_t                      rxGain1;
} adi_adrv9001_C99Config_t;

/**
 * \brief Data structure to hold ADRV9001 device instance initialization settings
 *
 * \implements IPmagTarget
 */
typedef struct adi_adrv9001_Init
{
    adi_adrv9001_ClockSettings_t clocks;      /*!< Holds settings for CLKPLL and reference clock */
    adi_adrv9001_RxSettings_t rx;             /*!< Rx settings data structure */
    adi_adrv9001_TxSettings_t tx;             /*!< Tx settings data structure */
    adi_adrv9001_DeviceSysConfig_t sysConfig; /*!< Device system config struct */
    adi_adrv9001_PfirBuffer_t pfirBuffer;     /*!< Holds the Data Interface CSSI/LSSI data link settings */
    adi_adrv9001_C99Config_t c99Config;
} adi_adrv9001_Init_t;

#endif /* _ADI_ADRV9001_PROFILE_TYPES_H_ */
