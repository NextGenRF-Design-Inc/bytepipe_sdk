//******************************************************************************
//
//  NEXTGEN RF DESIGN RESTRICTED MATERIAL
//  Copyright 2010-2021 NextGen RF Design Inc.
//  All rights reserved worldwide
//  Unauthorized use, distribution or duplication is
//  strictly prohibited without written authorization.
//
//******************************************************************************

//******************************************************************************
//
//  File Name      : zfile.c
//  Summary        : IO Device Flash Initialization and Routines
//
//******************************************************************************

#include <stdio.h>
#include <string.h>

#include "ff.h"
#include "zfile.h"
#include "zmodem_pub.h"

// #define SIMULATE_FILESYSTEM (1)

//--------------------------------------------------------------------------
// Function:    zFileOpen                                   
//
// Parameters:  File Structure with Name, Size, and Date,
//              FileWrite is TRUE if you want to write the file
//              FileWrite is FALSE if you want to read the file
//
// Return:      NULL in case of error, otherwilse the address of the ctrl block.
//
// Description: This requests file access priviledges for the specified file
//--------------------------------------------------------------------------
void *zFileOpen (void *Inst, BOOLEAN FileWrite)
{
  ZMODEM_INSTANCE *Instance = (ZMODEM_INSTANCE *)Inst;

  strncpy(Instance->Stats.FileName, (char *)Instance->File.Name, ZMODEM_FILE_NAME_LEN); 
  Instance->Stats.FileName[ZMODEM_FILE_NAME_LEN-1] = '\0';
  Instance->Stats.FileRead  = 0;
  Instance->Stats.FileWrite = 0;
  Instance->Stats.FileSize  = Instance->File.Size;

#ifdef SIMULATE_FILESYSTEM
  return ((void *)&Instance->fil);
#else

  char Filename[ZMODEM_DRIVE_NAME_LEN + ZMODEM_FILE_NAME_LEN];
  snprintf(Filename, ZMODEM_DRIVE_NAME_LEN + ZMODEM_FILE_NAME_LEN, "%s%s", Instance->Drive, Instance->File.Name);
  Filename[ZMODEM_DRIVE_NAME_LEN + ZMODEM_FILE_NAME_LEN - 1] = '\0';

  if (FileWrite == FALSE)
  {
    if (f_open(&Instance->fil, Filename, FA_OPEN_EXISTING | FA_READ) == FR_OK)
      return ((void *)&Instance->fil);
  }
  else
  {
    if (f_open(&Instance->fil, Filename, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
      return ((void *)&Instance->fil);
  }
   
  return NULL;
#endif
}

//--------------------------------------------------------------------------
// Function:    zFileWrite                                  
//
// Parameters:  File Number (from IoSvcRequestFileOpen), 
//              Position in file, Size of write,
//              Pointer to Data
//
// Return:      SUCCESS if Ok, FAILURE if Not
//
// Description: This writes all or part of a file
// Note:        The file must have been opened first
//--------------------------------------------------------------------------

USHORT zFileWrite (void *Inst, ULONG Pos, UBYTE Data)
{
  ZMODEM_INSTANCE *Instance = (ZMODEM_INSTANCE *)Inst;

#ifdef SIMULATE_FILESYSTEM
  Instance->Stats.FileWrite++;
  return (1);
#else
  FRESULT fres;
  FSIZE_t ofs;
  UINT bw = 0;  // bytes written
  UINT btw = 1; // bytes to writte

  Instance->Stats.FileWrite++;

  ofs = (FSIZE_t)Pos;

  fres = f_lseek(&Instance->fil, ofs);
  if (fres == FR_OK)
  {
    fres = f_write(&Instance->fil, (const void*) &Data, btw, &bw);
    if ((fres == FR_OK) && (bw == 1))
      return (1);
  }

  return (0);
#endif
}

//--------------------------------------------------------------------------
// Function:    zFileRead                                   
//
// Parameters:  File Number (from IoSvcRequestFileOpen), 
//              Position in file, Size of write,
//              Pointer to Data
//
// Return:      Lower Byte: Data Byte
//              Upper Byte: SUCCESS if Ok, FAILURE if Not
//
// Description: This reads all or part of a file
// Note:        The file must have been opened first
//--------------------------------------------------------------------------
USHORT zFileRead (void *Inst, ULONG Pos)
{
  ZMODEM_INSTANCE *Instance = (ZMODEM_INSTANCE *)Inst;

#ifdef SIMULATE_FILESYSTEM
  USHORT RetVal = SUCCESS << 8;
  Instance->Stats.FileRead++;
  RetVal |= 0x39;
  return (RetVal);
#else
  USHORT RetVal = FAILURE << 8;
  FRESULT fres;
  FSIZE_t ofs;
  UINT br = 0;  // bytes read
  UINT btr = 1; // bytes to read
  UBYTE Data;

  Instance->Stats.FileRead++;

  ofs = (FSIZE_t)Pos;

  fres = f_lseek(&Instance->fil, ofs);
  if (fres == FR_OK)
  {
    fres = f_read(&Instance->fil, (void* ) &Data, btr, &br);
    if ((fres == FR_OK) && (br == 1))
    {
      RetVal = SUCCESS << 8;
      RetVal |= Data;
    }
  }
 
  return (RetVal);
#endif
}

//--------------------------------------------------------------------------
// Function:    zFileClose                                  
//
// Parameters:  File Number (from IoSvcRequestFileOpen)
//
// Return:      SUCCESS if Ok, FAILURE if Not
//
// Description: This closes file access priviledges for the specified file
//              If File Write, the file will be written to either Flash or Eeprom
//              FILE CHANGES ARE SAVED
//--------------------------------------------------------------------------
USHORT zFileClose (void *Inst)
{
#ifdef SIMULATE_FILESYSTEM
  return (SUCCESS);
#else
  ZMODEM_INSTANCE *Instance = (ZMODEM_INSTANCE *)Inst;
  FRESULT fres;

  fres = f_close(&Instance->fil);
  if (fres == FR_OK)
  {
    return (SUCCESS);
  }

  return (FAILURE);
#endif
}

//--------------------------------------------------------------------------
// Function:    zFileCancel                                 
//
// Parameters:  File Number (from IoSvcRequestFileOpen)
//
// Return:      None
//
// Description: This closes file access priviledges for the specified file
//              If File Write, the file will be overwritten by the Flash or Eeprom copy
//              Same as IoSvcRequestFileClose except FILE CHANGES ARE IGNORED
//--------------------------------------------------------------------------
void zFileCancel (void *Inst)
{
#ifndef SIMULATE_FILESYSTEM
  ZMODEM_INSTANCE *Instance = (ZMODEM_INSTANCE *)Inst;
  f_close(&Instance->fil);
#endif
}
