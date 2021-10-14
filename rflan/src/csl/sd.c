/*
 * sd.c
 *
 *  Created on: Dec 26, 2019
 *      Author: enelson
 */


#include "xparameters.h"
#include "xsdps.h"
#include "xil_printf.h"
#include "sd.h"

static FATFS sdfs;

int SD_Init( void )
{
	/* Logical Drive 0 */
	TCHAR *Path = "0:/";

	/* Register volume and initialize device */
	if(f_mount(&sdfs, Path, 0) != FR_OK) return XST_FAILURE;

	return XST_SUCCESS;
}

int SD_Open( char *filename,  FIL *fil )
{
	if(f_open(fil, filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ) != FR_OK) return XST_FAILURE;

	return XST_SUCCESS;
}

int SD_Close( FIL *fil )
{
	if(f_close(fil) != FR_OK) return XST_FAILURE;

	return XST_SUCCESS;
}

int SD_Write( const char *filename, char *s )
{
	FIL fil;
	u16 len = 0;

	/* Open File */
	if(f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ) != FR_OK)
		return XST_FAILURE;

	/* Pointer to beginning of file */
	if(f_lseek(&fil, 0) != FR_OK) return XST_FAILURE;

	/* Write data to file */
	if(f_write(&fil, (const void*)s, strlen(s), (UINT*)&len) != FR_OK) return XST_FAILURE;

	if(strlen(s) != len) return XST_FAILURE;

	return XST_SUCCESS;
}

int SD_ReadBinary( const char *filename, u8 *buf, FSIZE_t ofs, int len )
{
	FIL fil;

	/* Open File */
	if(f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ) != FR_OK)
		return XST_FAILURE;

	u32 size = f_size(&fil);

	if(len == -1) len = size;

	size = size - ofs;

	if(size < len)
		len = size;

	/* Pointer to beginning of file */
	if(f_lseek(&fil, ofs) != FR_OK)
		return XST_FAILURE;

	/* Write data to file */
	if(f_read(&fil, buf, len, (UINT*)&len) != FR_OK)
		return XST_FAILURE;


	return XST_SUCCESS;
}

int SD_Read( const char *filename, char *s, FSIZE_t ofs, int len )
{
	FIL fil;

	/* Open File */
	if(f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ) != FR_OK)
		return XST_FAILURE;

	u32 size = f_size(&fil);

	if(len == -1) len = size;

	size = size - ofs;

	if(size < len) len = size;

	/* Pointer to beginning of file */
	if(f_lseek(&fil, ofs) != FR_OK) return XST_FAILURE;

	/* Write data to file */
	if(f_read(&fil, s, len, (UINT*)&len) != FR_OK) return XST_FAILURE;

	if(strlen(s) != len) return XST_FAILURE;

	return XST_SUCCESS;
}

