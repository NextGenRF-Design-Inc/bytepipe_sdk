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
//  File Name      : zfile.h
//  Summary        : file system interface for zmodem
//
//******************************************************************************

#ifndef _ZFILE_H
#define _ZFILE_H

#include "ztypes.h"

extern void *zFileOpen(void *Inst, BOOLEAN FileWrite);
extern USHORT zFileWrite(void *Inst, ULONG Pos, UBYTE Data);
extern USHORT zFileRead(void *Inst, ULONG Pos);
extern USHORT zFileClose(void *Inst);
extern void zFileCancel(void *Inst);

#endif  // _ZFILE_H

/* eof */


