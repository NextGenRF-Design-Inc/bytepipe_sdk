#ifndef IQ_FILE_H_
#define IQ_FILE_H_
/***************************************************************************//**
*  \ingroup    LIB
*  \defgroup   IQ_FILE IQ File
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       iq_file.h
*
*  \details
*
*  This file contains the definitions for reading and writing IQ data to a fat
*  file system.
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
extern "C"
{
#endif

#include <stdint.h>
#include "ff.h"

/*******************************************************************************
*
* \details
*
* This function writes IQ data to a file in csv format.  Existing files with
* the same filename will be deleted before creating a new file and writing its
* contents.
*
* \param[in]  filename is the name of the file created.
*
* \param[in]  Buf is a buffer containing 32bit IQ samples
*
* \param[in]  Length represents the number of samples in Buf
*
* \return     Status
*
*******************************************************************************/
int32_t IqFile_Write( const char* filename, uint32_t *Buf, uint32_t Length );

/*******************************************************************************
*
* \details
*
* This function reads IQ data from a csv formatted file.
*
* \param[in]  filename is the name of the file created.
*
* \param[in]  Buf is a buffer containing 32bit IQ samples
*
* \param[in]  Length represents the number of samples in Buf
*
* \return     Status
*
*******************************************************************************/
int32_t IqFile_Read( const char* filename, uint32_t **Buf, uint32_t *Length );

/*******************************************************************************
*
* \details
*
* This function returns the number of IQ samples from a csv formatted file.
*
* \param[in]  filename is the name of the file created.
*
* \param[in]  SampleCnt is the number of iq samples contained within the file.
*
* \return     Status
*
*******************************************************************************/
int32_t IqFile_GetSampleCnt( FIL *fil, uint32_t *SampleCnt );

#endif /* IQ_FILE_H_ */
