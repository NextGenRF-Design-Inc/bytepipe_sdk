#ifndef IQ_FILE_H_
#define IQ_FILE_H_


#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>


int32_t IqFile_Read( const char* filename, uint32_t **Buf, uint32_t *Length );

#endif /* PHY_CLI_H_ */
