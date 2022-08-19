#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "rflan_pib.h"
#include "rflan.h"
#include "pib.h"

/* PIB Definition */
static pib_def_t RflanPibDef[] =
{
//  Name                        Offset                        Storage Type          Flags
//-----------------------------------------------------------------------------------------------------------------------//
  { "PibSize",        offsetof(rflan_params_t, PibSize),       PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY  },
  { "FwVer",          offsetof(rflan_params_t, FwVer),         PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY  },
  { "HwVer",          offsetof(rflan_params_t, HwVer),         PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY  },
  { "IpAddr",         offsetof(rflan_params_t, IpAddr),        PibTypeStr,       PIB_FLAGS_DEFAULT  },
  { "IpMask",         offsetof(rflan_params_t, IpMask),        PibTypeStr,       PIB_FLAGS_DEFAULT  },
  { "IpGwAddr",       offsetof(rflan_params_t, IpGwAddr),      PibTypeStr,       PIB_FLAGS_DEFAULT  },
  { "LwipEnable",     offsetof(rflan_params_t, LwipEnable),    PibTypeU8,        PIB_FLAGS_DEFAULT  },
};

/* Default PIB Parameters */
static rflan_params_t  RflanPibDefaults =
{
  .PibSize        = sizeof(rflan_params_t),
  .IpAddr         =  "192.168.1.12",
  .IpMask         =  "255.255.255.0",
  .IpGwAddr       =  "192.168.1.1",
  .LwipEnable     = 1,

};

int32_t RflanPib_GetByName( rflan_pib_t *Instance, char *name, uint8_t *value )
{
  int32_t status;

  /* Get Parameter By Name */
  if((status = Pib_GetByName( &Instance->Pib, name, value)) != 0)
    return status;

  return status;
}

int32_t RflanPib_SetbyName( rflan_pib_t *Instance, char *name, char *value )
{
  int32_t status;

  /* Set Parameter By Name */
  if((status = Pib_SetByNameByString( &Instance->Pib, name, value)) != 0)
    return status;

  return status;
}

int32_t RflanPib_Initialize( rflan_pib_t *Instance, rflan_pib_init_t *Init )
{
  int32_t status;

  /* Create PIB Config */
  pib_init_t PibInit = {
    .Params         = &Instance->Params,
    .Defaults       = &RflanPibDefaults,
    .PibLen         = sizeof(RflanPibDef) / sizeof(pib_def_t),
    .ParamsSize     = sizeof(rflan_params_t),
    .Def            = RflanPibDef,
  };

  /* Initialize PIB Instance */
  if((status = Pib_Init( &Instance->Pib, &PibInit )) != 0)
    return status;

  /* Update Version */
  Instance->Params.FwVer = RFLAN_FW_VER;
  Instance->Params.HwVer = Init->HwVer;

  return status;
}


/** @} */
