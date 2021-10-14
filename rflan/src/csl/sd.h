/*
 * sd.h
 *
 *  Created on: Dec 26, 2019
 *      Author: enelson
 */

#ifndef SRC_SD_H_
#define SRC_SD_H_

#include "xil_types.h"
#include "ff.h"


int SD_Init( void );
int SD_Read( const char *filename, char *s, FSIZE_t ofs, int len );
int SD_Write( const char *filename, char *s );
int SD_ReadBinary( const char *filename, u8 *buf, FSIZE_t ofs, int len );

#endif /* SRC_UTIL_H_ */
