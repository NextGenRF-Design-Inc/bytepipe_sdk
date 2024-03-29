/***************************************************************************//**
*  \addtogroup PIB
*   @{
*******************************************************************************/
/***************************************************************************//**
*  \file       pib.c
*
*  \details    This file contains the PIB library.
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
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pib.h"

static int32_t pib_typeSize( PibType_t type )
{
	int32_t len = 0;

	switch(type)
	{
	case PibTypeChar: 	len = sizeof(char); 		break;
	case PibTypeU8: 		len = sizeof(uint8_t); 	break;
	case PibTypeS8: 		len = sizeof(int8_t); 	break;
	case PibTypeU16: 		len = sizeof(uint16_t);	break;
	case PibTypeS16: 		len = sizeof(int16_t); 	break;
	case PibTypeU32: 		len = sizeof(uint32_t);	break;
	case PibTypeS32: 		len = sizeof(int32_t);  break;
	case PibTypeU64: 		len = sizeof(uint64_t); break;
	case PibTypeS64: 		len = sizeof(int64_t); 	break;
	case PibTypeFloat: 	len = sizeof(float); 		break;
	case PibTypeDouble: len = sizeof(double); 	break;
	case PibTypeStr: 		len = PIB_NAME_SIZE; 		break;
	default:						len = 0;								break;
	}

	return len;
}

const char *Pib_TypeName( PibType_t type )
{
  const char *str = NULL;

	switch(type)
	{
	case PibTypeChar:       str = "char"; 	break;
	case PibTypeU8: 		str = "uint8"; 	break;
	case PibTypeS8: 		str = "int8"; 	break;
	case PibTypeU16: 		str = "uint16";	break;
	case PibTypeS16: 		str = "int16"; 	break;
	case PibTypeU32: 		str = "uint32";	break;
	case PibTypeS32: 		str = "int32";  break;
	case PibTypeU64: 		str = "uint64"; break;
	case PibTypeS64: 		str = "int64"; 	break;
	case PibTypeFloat: 	    str = "float"; 	break;
	case PibTypeDouble:     str = "double"; break;
	case PibTypeStr: 		str = "string";	break;
	default:				str = NULL;     break;
	}

	return str;
}

int32_t Pib_StrToNum( char *p, PibType_t type, void *param )
{
	short       tmpResult = -1;
	int64_t 		*temIPtr;
	uint64_t 		*temUPtr;
	double 			doubleFloat;

	if( (p == NULL) || (param == NULL))
	  return PibStatus_InvalidParameter;

	switch( type )
	{
	case PibTypeU8:
		if( (p[1] == 'x') || (p[1] == 'X') )
		{
			uint16_t utemp;							// this is a work around for scanf
			tmpResult = sscanf( p, "%hx", &utemp );
			*(uint8_t*)param = (uint8_t)utemp;
		}
		else
			tmpResult = sscanf( p, "%hhu", (uint8_t*)param );
		break;

	case PibTypeU16:
		if( (p[1] == 'x') || (p[1] == 'X') )
			tmpResult = sscanf( p, "%hx", (uint16_t*)param );
		else
			tmpResult = sscanf( p, "%hu", (uint16_t*)param );
		break;

	case PibTypeU32:
		if( (p[1] == 'x') || (p[1] == 'X') )
			tmpResult = sscanf( p, "%lx", (uint32_t*)param );
		else
			tmpResult = sscanf( p, "%lu", (uint32_t*)param );
		break;

	case PibTypeU64:	// This is a work around for the Cortex M4
		temUPtr = (uint64_t*)param;
		tmpResult = sscanf( p, "%lf", (double*)&doubleFloat );
		*temUPtr = (uint64_t)doubleFloat;
		break;

	case PibTypeS8:
		if( (p[1] == 'x') || (p[1] == 'X') )
		{
			int16_t itemp;							// this is a work around for scanf
			tmpResult = sscanf( p, "%hx", &itemp );
			*(int8_t*)param = (int8_t)itemp;
		}
		else
			tmpResult = sscanf( p, "%hhi", (char*)param );
		break;

	case PibTypeS16:
		if( (p[1] == 'x') || (p[1] == 'X') )
			tmpResult = sscanf( p, "%hx", (short*)param );
		else
			tmpResult = sscanf( p, "%hi", (short*)param );
		break;

	case PibTypeS32:
		if( (p[1] == 'x') || (p[1] == 'X') )
			tmpResult = sscanf( p, "%lx", (long*)param );
		else
			tmpResult = sscanf( p, "%li", (long*)param );
		break;

	case PibTypeS64:	// This is a work around for the Cortex M4
		temIPtr = (int64_t*)param;
		tmpResult = sscanf( p, "%lf", (double*)&doubleFloat );
		*temIPtr = (int64_t)doubleFloat;
		break;

	case PibTypeFloat:
		tmpResult = sscanf( p, "%f", (float *)param );
		break;

	case PibTypeDouble:
		tmpResult = sscanf( p, "%lf", (double*)param );
		break;

	case PibTypeStr:
		tmpResult = sscanf( p, "%s", (uint8_t *)param );
		break;

	default:
		tmpResult = 0;
		break;
	}

	if( tmpResult == 0 )
	  return PibStatus_InvalidParameter;

	return PibStatus_Success;
}

int32_t Pib_Init( pib_t *Instance, pib_init_t *Init )
{
  if((Instance == NULL) || (Init == NULL))
		return PibStatus_InvalidParameter;

  if((Init->Params == NULL) || (Init->ParamsSize == 0))
    return PibStatus_InvalidParameter;

  Instance->Def         = Init->Def;
  Instance->Params      = Init->Params;
  Instance->ParamsSize  = Init->ParamsSize;
  Instance->PibLen      = Init->PibLen;

  /* Copy Defaults */
  if( Init->Defaults != NULL )
    memcpy(((uint8_t*)Instance->Params), ((uint8_t*)Init->Defaults), Init->ParamsSize);

  return PibStatus_Success;
}

int32_t Pib_GetItemId( pib_t *Instance, char *name, int32_t *id )
{
	if((name == NULL) || (strlen(name) > PIB_NAME_SIZE))
    return PibStatus_InvalidParameter;

	*id = -1;

	/* Locate Parameter Name*/
	for(int32_t i = 0; i < Instance->PibLen; i++)
	{
	  if(strcmp(Instance->Def[i].name, name) == 0)
		{
			*id = i;
			return PibStatus_Success;
		}
	}

  return PibStatus_UnknownParameter;
}

int32_t Pib_GetItemAddress( pib_t *Instance, uint32_t id, uint8_t **ref )
{
	if((id < 0) || (id >= Instance->PibLen ))
    return PibStatus_UnknownParameter;

	/* Location of Parameter */
	*ref = (((uint8_t*)Instance->Params) + Instance->Def[id].offset);

  return PibStatus_Success;
}

int32_t Pib_GetByName( pib_t *Instance, char *name, uint8_t *value )
{
  if((Instance == NULL) || (name == NULL ))
    return PibStatus_InvalidParameter;

	int32_t id;
	int32_t status;

	/* Get ID */
  if((status = Pib_GetItemId( Instance, name, &id )) != 0)
    return status;

	/* Location of Parameter */
	uint8_t *loc = ((uint8_t*)Instance->Params) + Instance->Def[id].offset;

	/* Copy Value */
	memcpy(value, loc, pib_typeSize(Instance->Def[id].var_type));

  return PibStatus_Success;
}

int32_t Pib_ValueToString( pib_t *Instance, int32_t id, uint8_t *value, char *str )
{
  switch(Instance->Def[id].var_type)
  {
  case PibTypeChar:   sprintf(str, "%c", *(char *)value); break;
  case PibTypeU8:     sprintf(str, "%hhu", *(uint8_t *)value); break;
  case PibTypeS8:     sprintf(str, "%hhi", *(int8_t *)value); break;
  case PibTypeU16:    sprintf(str, "%hu", *(uint16_t *)value); break;
  case PibTypeS16:    sprintf(str, "%hi", *(int16_t *)value); break;
  case PibTypeU32:    sprintf(str, "%lu", *(uint32_t *)value); break;
  case PibTypeS32:    sprintf(str, "%li", *(int32_t *)value); break;
  case PibTypeU64:    sprintf(str, "%llu", *(uint64_t *)value); break;
  case PibTypeS64:    sprintf(str, "%lli", *(int64_t *)value); break;
  case PibTypeFloat:  sprintf(str, "%.3f", *(float *)value); break;
  case PibTypeDouble: sprintf(str, "%.6f", *(double *)value); break;
  case PibTypeStr:    sprintf(str, "%s", (char *)value); break;
  default:            sprintf(str, "%s", "Invalid"); break;
  }

  return PibStatus_Success;
}

int32_t Pib_GetStringByName( pib_t *Instance, char *name, char *str )
{
	if((Instance == NULL) || (name == NULL ))
    return PibStatus_InvalidParameter;

	int32_t id;
  int32_t status;

	/* Get ID */
	if((status = Pib_GetItemId( Instance, name, &id )) != 0)
	  return status;

	/* Location of Parameter */
	uint8_t *value = ((uint8_t*)Instance->Params) + Instance->Def[id].offset;

	status = Pib_ValueToString( Instance, id, value, str );

  return status;
}

int32_t Pib_SetByNameByString( pib_t *Instance, char *name, char *str )
{
  if((Instance == NULL) || (name == NULL ) || (str == NULL ))
    return PibStatus_InvalidParameter;

  int32_t status;
  int32_t id;

  uint8_t *value = calloc(1, strlen(str));

  do
  {
    if( value == NULL )
      break;

    /* Get ID */
    if((status = Pib_GetItemId( Instance, name, &id )) != 0)
      break;

    /* Get Parameter Value */
    if((status = Pib_StrToNum(str, Instance->Def[id].var_type, value )) != 0)
      return 0;

    /* Set Value of Parameter */
    if((status = Pib_SetById( Instance, id, value )) != 0)
      break;

  }while(0);

  free(value);

  return status;
}

int32_t Pib_GetById( pib_t *Instance, uint32_t id, uint8_t *value )
{
	if((id < 0) || (id >= Instance->PibLen ))
    return PibStatus_InvalidParameter;

	/* Location of Parameter */
	uint8_t *loc = ((uint8_t*)Instance->Params) + Instance->Def[id].offset;

	/* Copy Value */
	memcpy(value, loc, pib_typeSize(Instance->Def[id].var_type));

  return PibStatus_Success;
}

int32_t Pib_SetById( pib_t *Instance, uint32_t id, uint8_t *value )
{
	if((id < 0) || (id >= Instance->PibLen ) || (Instance->Def[id].flags & PIB_FLAG_READ_ONLY))
		return PibStatus_InvalidParameter;

	/* Location of Parameter */
	uint8_t *loc = ((uint8_t*)Instance->Params) + Instance->Def[id].offset;

	/* Copy Value */
	memcpy(loc, value, pib_typeSize(Instance->Def[id].var_type));

	return PibStatus_Success;
}

int32_t Pib_SetByName( pib_t *Instance, char *name, uint8_t *value )
{
  if((Instance == NULL) || (name == NULL ) || (value == NULL ))
    return PibStatus_InvalidParameter;

  int32_t status;
	int32_t id;

	/* Get ID */
	if((status = Pib_GetItemId( Instance, name, &id )) != 0)
	  return status;

	/* Set Value of Parameter */
	if((status = Pib_SetById( Instance, id, value )) != 0)
	  return status;

  return PibStatus_Success;
}



/** @} */
