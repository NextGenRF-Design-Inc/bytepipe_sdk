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
//  File Name      : zmodem_pub.h
//  Summary        : Z-Modem Command Handler Header File
//
//******************************************************************************

#ifndef _ZMODEM_PUB_H
#define _ZMODEM_PUB_H
    
#include "ztypes.h"
#include "ff.h"
 
//============================================================
// ZMODEM INSTANCE STRUCTURE
//============================================================  
//ZMODEM PARSER RETURN VALUES       
#define ZM_BYTE_USED            0x10
#define ZM_RESERVE_CHANNEL      0x20    
#define ZM_UNRESERVE_CHANNEL    0x40

// -----------------------------------------------------------------------------
// Function that writes to a datalink stream
//
// A variable of this type is a pointer to a function that has the
// following signature:
//    int_16 zmDlWrite(void *buffer_p, uint_16 length)
//       buffer_p  Pointer to the buffer to send.
//       length    Length of the buffer in <buffer_p>, in bytes.
//
//    Return value :
//       0  if operation successfull.
//      -1  otherwise.
// -----------------------------------------------------------------------------
typedef SHORT (*FN_ZMODEM_WRITE)(UBYTE); //Should return TRUE if successful, FALSE if not

#define ZMODEM_BUFFER_LEN       1024                        //This is the length that we report
#define ZMODEM_BUFFER_SIZE      (ZMODEM_BUFFER_LEN + 4)     //The last 4 control bytes aren't included in the count
#define ZM_HEADER_SIZE          4

#define ZMODEM_FILE_NAME_LEN    32
#define ZMODEM_DRIVE_NAME_LEN   16

typedef struct
{
    ULONG       FileSize;                                   //Size of the last file
    ULONG       FileRead;                                   //Amount of file read
    ULONG       FileWrite;                                  //Amount of file write
    char        FileName[ZMODEM_FILE_NAME_LEN];             //Name of the last file
} ZMODEM_FILE_STATS;

typedef struct
{
    ULONG       Size;                                       //Size of the file
    ULONG       Date;                                       //Date of the file (seconds from Jan 1, 1970 UTC)
    UBYTE       Name[ZMODEM_FILE_NAME_LEN];                 //Name of the file
} ZMODEM_FILE_HEADER;

typedef struct
{   
    USHORT                  CrcVal;                         //Current Crc Value
    USHORT                  MsgPos;                         //Position of Parser
    USHORT                  MsgLen;                         //Length of Current Message
    UBYTE                   CrcCount;                       //#Bytes until CRC is expected
    UBYTE                   CanCount;                       //# of ZM_CAN Characters received
    
    UBYTE                   State;                          //State for current Header
    UBYTE                   PacketType;                     //Header: A-Bin16, B-Hex, C-Bin32
    UBYTE                   Nybble;                         //Value of First Nybble (Hex Frames)
    UBYTE                   FrameEnd;                       //Binary Data Packet Frame End
    
    //These three need to stay together
    UBYTE                   Type;                           //Type of Header: ZRQINIT, ZRINIT, etc.
    UBYTE                   Header[ZM_HEADER_SIZE];         //Header Bytes
    UBYTE                   Crc[2];                         //Crc Bytes
    //These three need to stay together
} ZMODEM_RX_HEADER;

typedef struct
{   
    FN_ZMODEM_WRITE         WriteFn;                        //Function to Write Data Out 
    UBYTE                   Header[ZM_HEADER_SIZE];         //Four Header Bytes (Flags or Position)
    UBYTE                   LastChar;                       //Last Character Sent
} ZMODEM_TX_HEADER; 

typedef struct      
{
    ZMODEM_FILE_HEADER      File;                           //Storage for the File Information
    ZMODEM_TX_HEADER        Tx;                             //Storage for the latest Tx header
    ZMODEM_RX_HEADER        Rx;                             //Storage for the latest Rx header
    ZMODEM_FILE_STATS       Stats;                          //Storage statistics
    ULONG                   Time;                           //Storage for last byte time
    ULONG                   Timeout;                        //Storage for time out in seconds
    ULONG                   FilePos;                        //Storage for the File Position
    void                    *FilePtr;                       //Storage for the file control block ptr
    FIL                     fil;                            //Storage for the actual file control block.
    UBYTE                   State;                          //State of ZModem State Machine
    UBYTE                   Operation;                      //Upload, Download, Listen
    UBYTE                   ChannelReq;                     //
    UBYTE                   AckRequired;                    //
    UBYTE                   Buffer[ZMODEM_BUFFER_SIZE];     //ZModem Data Buffer
    char                    Drive[ZMODEM_DRIVE_NAME_LEN];   //The drive to use
} ZMODEM_INSTANCE;
 
UBYTE zModemParse(ZMODEM_INSTANCE* Instance, UBYTE Data); 
void zModemInit(ZMODEM_INSTANCE* Instance, FN_ZMODEM_WRITE WriteFn, char *drive); 
 
#endif // _ZMODEM_PUB_H

/* eof */

