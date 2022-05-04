
#include <stdio.h>
#include <string.h>

#include "adrv9001.h"
#include "initialize.h"
#include "calibrate.h"
#include "prime.h"
#include "configure.h"


int32_t Adrv9001_Initialize( adi_adrv9001_Device_t *adrv9001Device_0 )
{
  int32_t status = 0;

  if((status = initialize(adrv9001Device_0 )) != 0)
    return status;

  if((status = calibrate(adrv9001Device_0)) != 0)
    return status;

  if((status = configure(adrv9001Device_0)) != 0)
    return status;

  if((status = prime(adrv9001Device_0)) != 0)
    return status;

  return status;
}
