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
//  File Name      : zmodem.h
//  Summary        : Z-Modem Command Handler Header File
//
//******************************************************************************

#ifndef _ZMODEM_H
#define _ZMODEM_H
    
#include "ztypes.h"
 
int ZModem_Parse(UBYTE c); 
int ZModem_Initialize(char *drive);
 
#endif // _ZMODEM_H

/* eof */

