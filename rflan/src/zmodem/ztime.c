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
//  File Name      : ztime.c
//  Summary        : Time Routines
//
//******************************************************************************

#include "FreeRTOS.h"
#include "task.h"
#include "ztime.h"

//--------------------------------------------------------------------------
// Function:    zTimeSecElapsed
//                                                                          
// Parameters:  None                                                
//                                                                          
// Return:      Number of seconds since program started 
//              (rolls over after 136.1 years)                              
//                                                                          
// Description: Returns the number of seconds since program started
//--------------------------------------------------------------------------
ULONG zTimeSecElapsed(void)
{
  ULONG elapsed;  
  TickType_t nowTicks = xTaskGetTickCount();
  elapsed = (ULONG)(nowTicks/configTICK_RATE_HZ);                                           
  return (elapsed);
}

//--------------------------------------------------------------------------
// Function:    zTimeSecSince
//                                                                          
// Parameters:  LastTime Variable                                               
//                                                                          
// Return:      Difference between current second count and LastTime variable 
//                                                                          
// Description: Returns the number of seconds between Now and LastTime variable 
//              Note: this count rolls over every 136.1 years, so we won't worry
//              about it rolling over.
//--------------------------------------------------------------------------
ULONG zTimeSecSince(ULONG lastTime)
{
  ULONG nowTime;  
  TickType_t nowTicks = xTaskGetTickCount();
  nowTime = (ULONG)(nowTicks/configTICK_RATE_HZ);   
  return (nowTime - lastTime);
}

/* eof */




