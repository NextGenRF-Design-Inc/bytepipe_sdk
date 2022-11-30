#ifndef __PIB_H__
#define __PIB_H__
/***************************************************************************//**
*  \ingroup   RFLAN
*  \defgroup  PIB Parameter Information Base Library
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       pib.h
*
*  \details
*
*  This file contains definitions for the parameter information base driver.
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
#ifdef __cplusplus
 extern "C" {
#endif

#define PIB_STATUS_OFFSET    (-5000)


#define PIB_NAME_SIZE 64

#define PIB_FLAGS_DEFAULT               (0x00)      ///< Default Flags
#define PIB_FLAG_READ_ONLY              (0x01)      ///< Parameter is read only
#define PIB_FLAG_FATFS_WRITE_ON_SET     (0x02)      ///< Write to file system on set
#define PIB_FLAG_FATFS_READ_ON_GET      (0x04)      ///< read from file system on get

/**
** Status Definition
*/
typedef enum
{
  PibStatus_Success               = (0),                          ///< Success
  PibStatus_MemoryError           = (PIB_STATUS_OFFSET - 1),      ///< Memory Error
  PibStatus_UnknownParameter      = (PIB_STATUS_OFFSET - 2),      ///< Unknown Parameter
  PibStatus_InvalidParameter      = (PIB_STATUS_OFFSET - 3),      ///< Invalid Parameter
} pib_status_t;

/**
** Variable Type Definition
*/
typedef enum {
 	PibTypeChar     = 0, 	///< Single Character
	PibTypeU8      	= 1, 	///< Unsigned Byte
	PibTypeS8      	= 2, 	///< Signed Byte
	PibTypeU16     	= 3, 	///< Unsigned 16bit word
	PibTypeS16     	= 4, 	///< Signed 16bit word
	PibTypeU32     	= 5, 	///< Unsigned 32bit word
	PibTypeS32     	= 6, 	///< Signed 32bit word
	PibTypeU64     	= 7, 	///< Unsigned 64bit word
	PibTypeS64     	= 8, 	///< Signed 64bit word
	PibTypeFloat    = 9, 	///< Single Precision Float
	PibTypeDouble   = 10,	///< Double Precision Float
	PibTypeStr    	= 11,	///< String
}PibType_t;

/**
** PIB Table Definition
*/
typedef struct
{
  char            name[PIB_NAME_SIZE + 1];      ///< Name of Attribute
  uint16_t        offset;                       ///< Offset of Attribute
  uint8_t		  var_type;                     ///< Variable Type
  uint8_t      	  flags;                        ///< Attribute Entry is modifiable
} pib_def_t;

/**
**  Initialization Structure
**
**  This structure is used to initialization the module.  The application can
**  destroy the corresponding parameter after calling initializing the module.
*/
typedef struct
{
  void           *Params;                       ///< Location to store parameters
  void           *Defaults;                     ///< Default Values
  uint16_t        ParamsSize;                   ///< Size in bytes of Parameters
  uint16_t        PibLen;                       ///< Number of parameters in PIB
  pib_def_t      *Def;                          ///< Definition table
} pib_init_t;

/**
**  Instance structure
**
**  This structure holds the variables associated with this module.  This
**  structure must be allocated and maintained by the application.  The application
**  should not access this structure directly.  The application must pass this
**  variable when calling all APIs.
*/
typedef struct
{
  void         *Params;                       ///< Reference To PIB Data
  uint16_t      ParamsSize;                   ///< Size in bytes of Params Variable
  uint16_t      PibLen;                       ///< Number of parameters in PIB
  pib_def_t    *Def;                          ///< Definition table
} pib_t;


/***************************************************************************//**
*
* \details  Initialize Driver
*
* \param    Instance [in]  Driver Instance
* \param    Init     [in]  Initialization structure
*
* \return   status
*
*******************************************************************************/
int32_t Pib_Init( pib_t *Instance, pib_init_t *Init );

int32_t Pib_GetItemId           ( pib_t *Instance, char *name, int32_t *id );
int32_t Pib_GetItemAddress      ( pib_t *Instance, uint32_t id, uint8_t **ref );
int32_t Pib_GetStringByName     ( pib_t *Instance, char *name, char *value );
int32_t Pib_SetStringByName     ( pib_t *Instance, char *name, char *value );
int32_t Pib_GetByName           ( pib_t *Instance, char *name, uint8_t *value );
int32_t Pib_GetById             ( pib_t *Instance, uint32_t id, uint8_t *value );
int32_t Pib_SetById             ( pib_t *Instance, uint32_t id, uint8_t *value );
int32_t Pib_SetByName           ( pib_t *Instance, char *name, uint8_t *value );
int32_t Pib_StrToNum            ( char *p, PibType_t type, void *param );
int32_t Pib_ValueToString       ( pib_t *Instance, int32_t id, uint8_t *value, char *str );
int32_t Pib_SetByNameByString   ( pib_t *Instance, char *name, char *str );
const char *Pib_TypeName        ( PibType_t type );

#ifdef __cplusplus
}
#endif

#endif // __PIB_H_
