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
//  File Name      : zmodem_pri.h
//  Summary        : Z-Modem Command Handler Header File
//
//******************************************************************************

#ifndef _ZMODEM_PRI_H
#define _ZMODEM_PRI_H
    
#include "ztypes.h"

//--------------------------------------------------------------------------
// Defines                              
//--------------------------------------------------------------------------
#define ZMODEM_TIMEOUT_DEFAULT  15 //seconds

//Return Flags From StoreHeaderData and StorePacketData
#define SH_CRC_FOUND            bit0
#define SH_CRC_ERROR            bit1
#define SH_DATA_OVERFLOW        bit2

//Return Flags From MpSvcParseZHeader and MpSvcParseZPacket
#define ZH_DATA_FOUND           SH_CRC_FOUND
#define ZH_BAD_CRC              SH_CRC_ERROR
#define ZH_INVALID_CHARACTER    SH_DATA_OVERFLOW

#define ZH_BYTE_USED            bit4
#define ZH_SESSION_CANNED       bit5

//SendZDataCallBack Constants
#define ZM_ZDATA_MAX_BYTES  32      //Send Max of 32 Bytes
#define ZM_ZDATA_INTERVAL   8       //Every 8 ms (7 ms is minimum Error free at 57600)
#define ZM_ZDATA_MAX_FRAME  1024    //1024 Bytes Max per Frame

//ZMODEM PARSER STATES
enum ZMODEM_STATES
{
    STATE_ZM_INITIALIZE = 0,
    STATE_ZM_FIND_HEADER,
    STATE_ZM_FIND_PACKET,   
    STATE_ZM_CLEANUP_START,
    STATE_ZM_CLEANUP
};

//ZMODEM OPERATION
enum ZMODEM_OPERATIONS
{
    ZM_OPER_LISTEN      = 0,
    ZM_OPER_UPLOAD,
    ZM_OPER_DOWNLOAD
};

//ZMODEM HEADER PARSER STATES   
enum ZMODEM_HEADER_STATE
{
    ZM_FRM_LOOKING_4_ZPAD   = 0,
    ZM_FRM_LOOKING_4_ZDLE,
    ZM_FRM_LOOKING_4_HEADER,        
    ZM_FRM_LOOKING_4_MSG_BIN,
    ZM_FRM_LOOKING_4_MSG_BIN_ZDLE,
    ZM_FRM_LOOKING_4_MSG_HEX1,  
    ZM_FRM_LOOKING_4_MSG_HEX2,
    ZM_FRM_LOOKING_4_BYTE4, 
    ZM_FRM_LOOKING_4_CR,
    ZM_FRM_LOOKING_4_LF,    
    ZM_FRM_LOOKING_4_XON,                                                           
};

//ZMODEM PACKET PARSER STATES   
enum ZMODEM_PACKET_STATE
{
    ZM_FRM_LOOKING_4_BIN_DATA_START = ZM_FRM_LOOKING_4_ZPAD,
    ZM_FRM_LOOKING_4_BIN_DATA,
    ZM_FRM_LOOKING_4_BIN_DATA_ZDLE                                                          
};

//-----------------------------------------------------------------
// Z Modem Constants
//----------------------------------------------------------------- 

// -----------------------------------------------------------------------------
// ORIGINAL ZMODEM DEFINES, prefixed with ZM_ to protect namespace
// ----------------------------------------------------------------------------- 
#define ZM_ZPAD '*'             // 0x2A - 052 Padding character begins frames 
#define ZM_ZDLE (030)           // 0x18 - Ctrl-X Zmodem escape - `ala BISYNC DLE 030 - octal
#define ZM_ZDLEE (ZM_ZDLE^0100) // 0x58 - Escaped ZM_ZDLE as transmitted 
#define ZM_ZBIN16   'A'         // Binary frame indicator 
#define ZM_ZHEX     'B'         // HEX frame indicator
#define ZM_ZBIN32   'C'         // Binary frame indicator   

// Frame types
#define ZM_ZRQINIT     (0)  // Request receive init 
#define ZM_ZRINIT      (1)  // Receive init 
#define ZM_ZSINIT      (2)  // Send init sequence (optional) 
#define ZM_ZACK        (3)  // ACK to above 
#define ZM_ZFILE       (4)  // File name from sender 
#define ZM_ZSKIP       (5)  // To sender: skip this file 
#define ZM_ZNAK        (6)  // Last packet was garbled 
#define ZM_ZABORT      (7)  // Abort batch transfers 
#define ZM_ZFIN        (8)  // Finish session 
#define ZM_ZRPOS       (9)  // Resume data trans at this position 
#define ZM_ZDATA      (10)  // Data packet(s) follow 
#define ZM_ZEOF       (11)  // End of file 
#define ZM_ZFERR      (12)  // Fatal Read or Write error Detected 
#define ZM_ZCRC       (13)  // Request for file CRC and response 
#define ZM_ZCHALLENGE (14)  // Receiver's Challenge 
#define ZM_ZCOMPL     (15)  // Request is complete 
#define ZM_ZCAN       (16)  // Other end canned session with CAN*5 
#define ZM_ZFREECNT   (17)  // Request for free bytes on filesystem 
#define ZM_ZCOMMAND   (18)  // Command from sending program 
#define ZM_ZSTDERR    (19)  // Output to standard error, data follows 
#define ZM_ZRQINITB   (20)  // ZM_ZRQINIT and set broadcast mode, responder in ZM_ZF0 

// ZM_ZDLE sequences
#define ZM_ZCRCE 'h'        // CRC next, frame ends, header packet follows 
#define ZM_ZCRCG 'i'        // CRC next, frame continues nonstop 
#define ZM_ZCRCQ 'j'        // CRC next, frame continues, ZM_ZACK expected 
#define ZM_ZCRCW 'k'        // CRC next, ZM_ZACK expected, end of frame 
#define ZM_ZRUB0 'l'        // Translate to rubout 0177 
#define ZM_ZRUB1 'm'        // Translate to rubout 0377 

// Using These
#define ZM_CR       (0x0D)  // '\r'     (CR)
#define ZM_LF       (0x0A)  // '\n'     (LF)
#define ZM_DLE      (0x10)  // 'p'&037  (DLE)
#define ZM_XON      (0x11)  // 'q'&037  (DC1)   
#define ZM_XOFF     (0x13)  // 's'&037  (DC3)
#define ZM_CAN      (0x18)  // 'X'&037  (CAN)   

    // zdlread return values (internal)
    // -1 is general error, -2 is timeout
//  #define ZM_GOTOR   (0400)
//  #define ZM_GOTCRCE (ZM_ZCRCE|ZM_GOTOR) // ZM_ZDLE-ZM_ZCRCE received 
//  #define ZM_GOTCRCG (ZM_ZCRCG|ZM_GOTOR) // ZM_ZDLE-ZM_ZCRCG received 
//  #define ZM_GOTCRCQ (ZM_ZCRCQ|ZM_GOTOR) // ZM_ZDLE-ZM_ZCRCQ received 
//  #define ZM_GOTCRCW (ZM_ZCRCW|ZM_GOTOR) // ZM_ZDLE-ZM_ZCRCW received 
//  #define ZM_GOTCAN  (ZM_GOTOR|030)      // CAN*5 seen 

// Byte positions within header array
#define ZM_ZF0     (3)       // First flags byte 
#define ZM_ZF1     (2)
#define ZM_ZF2     (1)
#define ZM_ZF3     (0)
#define ZM_ZP0     (0)       // Low order 8 bits of position 
#define ZM_ZP1     (1)
#define ZM_ZP2     (2)
#define ZM_ZP3     (3)       // High order 8 bits of file position 

// Bit Masks for ZM_ZRINIT flags byte ZM_ZF0
#define ZM_CANFDX  (01)      // Rx can send and receive true FDX 
#define ZM_CANOVIO (02)      // Rx can receive data during disk I/O 
#define ZM_CANBRK  (04)      // Rx can send a break signal 
#define ZM_CANCRY  (010)     // Receiver can decrypt 
#define ZM_CANLZW  (020)     // Receiver can uncompress 

// Parameters for ZM_ZSINIT frame
#define ZM_ZATTNLEN (32)     // Max length of attention string 

// Parameters for ZM_ZFILE frame
// Conversion options one of these in ZM_ZF0
#define ZM_ZCBIN   (1)       // Binary transfer - inhibit conversion 
#define ZM_ZCNL    (2)       // Convert NL to local end of line convention 
#define ZM_ZCRESUM (3)       // Resume interrupted file transfer 

// Management options, one of these in ZM_ZF1
#define ZM_ZMNEW   (1)       // Transfer if source newer or longer 
#define ZM_ZMCRC   (2)       // Transfer if different file CRC or length 
#define ZM_ZMAPND  (3)       // Append contents to existing file (if any) 
#define ZM_ZMCLOB  (4)       // Replace existing file 
#define ZM_ZMSPARS (5)       // Encoding for sparse file 
#define ZM_ZMDIFF  (6)       // Transfer if dates or lengths different 
#define ZM_ZMPROT  (7)       // Protect destination file 

// Transport options, one of these in ZM_ZF2
#define ZM_ZTLZW   (1)       // Lempel-Ziv compression 
#define ZM_ZTCRYPT (2)       // Encryption 
#define ZM_ZTRLE   (3)       // Run Length encoding 

// Parameters for ZM_ZCOMMAND frame ZM_ZF0 (otherwise 0)
#define ZM_ZCACK1  (1)       // Acknowledge, then do command 

#define ZM_OK      (0)
#define ZM_ERROR   (-1)

// difference in seconds between Jan 1st 1980 & Jan 1st 1970  
#define ZM_JAN1ST70  (315532800L)

#endif // _ZMODEM_PRI_H

/* eof */

