#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "rflan_pib.h"
#include "rflan.h"
#include "pib.h"


#define RFLAN_PIB_FLAG_REBOOT        (0x08)
#define RFLAN_PIB_FLAG_VIRTUAL       (0x10)
#define RFLAN_PIB_FLAG_SET_ACTION    (0x20)
#define RFLAN_PIB_FLAG_GET_ACTION    (0x40)

/* PIB Definition */
static pib_def_t RflanPibDef[] =
{
//  Name                        Offset                        Storage Type          Flags
//-----------------------------------------------------------------------------------------------------------------------//
  { "PibSize",        offsetof(rflan_params_t, PibSize),       PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY  },
  { "FwVer",          offsetof(rflan_params_t, FwVer),         PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY  },
  { "HwVer",          offsetof(rflan_params_t, HwVer),         PibTypeU32,       PIB_FLAGS_DEFAULT | PIB_FLAG_READ_ONLY  },

#ifdef RFLAN_LWIP_ENABLE
  { "IpAddr",         offsetof(rflan_params_t, IpAddr),        PibTypeStr,       PIB_FLAGS_DEFAULT  },
  { "IpMask",         offsetof(rflan_params_t, IpMask),        PibTypeStr,       PIB_FLAGS_DEFAULT  },
  { "IpGwAddr",       offsetof(rflan_params_t, IpGwAddr),      PibTypeStr,       PIB_FLAGS_DEFAULT  },
  { "LwipEnable",     offsetof(rflan_params_t, LwipEnable),    PibTypeU8,        PIB_FLAGS_DEFAULT  },
#endif

#ifdef VERSA_CLOCK5_ENABLE
  { "GtrClockFreq0",  0,                                       PibTypeU32,       PIB_FLAGS_DEFAULT | RFLAN_PIB_FLAG_VIRTUAL },
  { "GtrClockFreq1",  0,                                       PibTypeU32,       PIB_FLAGS_DEFAULT | RFLAN_PIB_FLAG_VIRTUAL },
  { "GtrClockFreq2",  0,                                       PibTypeU32,       PIB_FLAGS_DEFAULT | RFLAN_PIB_FLAG_VIRTUAL },
  { "GtrClockFreq3",  0,                                       PibTypeU32,       PIB_FLAGS_DEFAULT | RFLAN_PIB_FLAG_VIRTUAL },
#endif

};

/* Default PIB Parameters */
static rflan_params_t  RflanPibDefaults =
{
  .PibSize        = sizeof(rflan_params_t),

#ifdef RFLAN_LWIP_ENABLE
  .IpAddr         =  "192.168.1.12",
  .IpMask         =  "255.255.255.0",
  .IpGwAddr       =  "192.168.1.1",
  .LwipEnable     = 1,
#endif

};

static int32_t RflanPib_SetVirtualByNameByString( rflan_pib_t *Instance, char *name, char *str )
{
  int32_t status = 0;
  int32_t id;

  /* Get ID */
  if((status = Pib_GetItemId( &Instance->Pib, name, &id )) != 0)
    return status;

  if( (name == NULL) || (str == NULL) )
  {
	  status = RflanStatus_InvalidPib;
  }

#ifdef VERSA_CLOCK5_ENABLE

  else if( strcmp( name, "GtrClockFreq0") == 0 )
  {
    uint32_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    status = VersaClock5_SetClockFreq(Instance->VersaClock5, 0, tmp);
  }

  else if( strcmp( &name[3], "GtrClockFreq1") == 0 )
  {
    uint32_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    status = VersaClock5_SetClockFreq(Instance->VersaClock5, 1, tmp);
  }

  else if( strcmp( &name[3], "GtrClockFreq2") == 0 )
  {
    uint32_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    status = VersaClock5_SetClockFreq(Instance->VersaClock5, 2, tmp);
  }

  else if( strcmp( &name[3], "GtrClockFreq3") == 0 )
  {
    uint32_t tmp;
    Pib_StrToNum(str, Instance->Pib.Def[id].var_type, &tmp);
    status = VersaClock5_SetClockFreq(Instance->VersaClock5, 3, tmp);
  }

#endif

  else
  {
	  status = RflanStatus_InvalidPib;
  }

  return RflanStatus_Success;
}

static int32_t RflanPib_GetVirtualStringByName( rflan_pib_t *Instance, char *name, char *str )
{
  int32_t status = 0;
  int32_t id;

  /* Get ID */
  if((status = Pib_GetItemId( &Instance->Pib, name, &id )) != 0)
    return status;

  if( (name == NULL) || (str == NULL) )
  {
    status = RflanStatus_InvalidPib;
  }

#ifdef VERSA_CLOCK5_ENABLE

  if( strcmp( name, "GtrClockFreq0") == 0 )
  {
    uint32_t tmp = 0;
    if((status = VersaClock5_GetClockFreq( Instance->VersaClock5, 0, &tmp)) != 0)
      return status;

    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

  else if( strcmp( name, "GtrClockFreq1") == 0 )
  {
    uint32_t tmp = 0;
    if((status = VersaClock5_GetClockFreq( Instance->VersaClock5, 1, &tmp)) != 0)
      return status;

    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

  else if( strcmp( name, "GtrClockFreq2") == 0 )
  {
    uint32_t tmp = 0;
    if((status = VersaClock5_GetClockFreq( Instance->VersaClock5, 2, &tmp)) != 0)
      return status;

    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

  else if( strcmp( name, "GtrClockFreq3") == 0 )
  {
    uint32_t tmp = 0;
    if((status = VersaClock5_GetClockFreq( Instance->VersaClock5, 3, &tmp)) != 0)
      return status;

    Pib_ValueToString( &Instance->Pib, id, (uint8_t*)&tmp, str );
  }

#endif

  else
  {
    return RflanStatus_InvalidPib;
  }


  return status;
}

static int32_t RflanPib_SetActionByNameByString( rflan_pib_t *Instance, char *name, char *str )
{

  return 0;
}

int32_t RflanPib_GetStringByName( rflan_pib_t *Instance, char *name, char *str )
{
  int32_t id;
  int32_t status;

  /* Get ID */
  if((status = Pib_GetItemId( &Instance->Pib, name, &id )) != 0)
    return status;

  if( ( Instance->Pib.Def[id].flags & RFLAN_PIB_FLAG_VIRTUAL ) == RFLAN_PIB_FLAG_VIRTUAL )
  {
    status = RflanPib_GetVirtualStringByName( Instance, name, str );
  }
  else
  {
    status = Pib_GetStringByName( &Instance->Pib, name, str );
  }

  return status;
}

int32_t RflanPib_SetbyNameByString( rflan_pib_t *Instance, char *name, char *str )
{
  int32_t id;
  int32_t status;

  /* Get ID */
  if((status = Pib_GetItemId( &Instance->Pib, name, &id )) != 0)
    return status;

  if( ( Instance->Pib.Def[id].flags & RFLAN_PIB_FLAG_VIRTUAL ) == RFLAN_PIB_FLAG_VIRTUAL )
  {
    status = RflanPib_SetVirtualByNameByString( Instance, name, str );
  }
  else
  {
    status = Pib_SetByNameByString( &Instance->Pib, name, str );

    if(( Instance->Pib.Def[id].flags & RFLAN_PIB_FLAG_SET_ACTION ) == RFLAN_PIB_FLAG_SET_ACTION )
      status = RflanPib_SetActionByNameByString( Instance, name, str );
  }

  return status;
}

int32_t RflanPib_Initialize( rflan_pib_t *Instance, rflan_pib_init_t *Init )
{
  int32_t status;

#ifdef VERSA_CLOCK5_ENABLE
  Instance->VersaClock5 = Init->VersaClock5;
#endif

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
