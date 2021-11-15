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
//  File Name      : zmodem_pri.c
//  Summary        : Z Modem Parser
//
//******************************************************************************

// =============================================================================
// INCLUDE FILES
// =============================================================================

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "zmodem_pub.h"
#include "zmodem_pri.h"
#include "zfile.h"
#include "ztime.h"
#include "zcrc.h"

static char HexL[] = "0123456789abcdef";

//SendZFile and SendZData Variables
ZMODEM_INSTANCE*    InstSendZFile = NULL;
ZMODEM_INSTANCE*    InstSendZData = NULL;

//--------------------------------------------------------------------------
// Function:    LoadPosition (Local Only)                               
//
// Parameters:  ZModem Instance, Position to Load
//
// Return:      None
//
// Description: Loads a 32 bit Position into the Header Bytes
//--------------------------------------------------------------------------
void LoadPosition(ZMODEM_INSTANCE* Instance, ULONG Position)
{
    UBYTE   i;
    
    for (i = 0; i < ZM_HEADER_SIZE; i++)
    {
        Instance->Tx.Header[i] = Position & 0xFF;
        Position >>= 8;
    }
}


//--------------------------------------------------------------------------
// Function:    GetFileInfo (Local Only)                                
//
// Parameters:  ZModem Instance
//
// Return:      Updated File Information
//
// Description: Takes the results of a ZFILE message in Instance.Buffer
//              and stores them in Instance.File
//--------------------------------------------------------------------------
void GetFileInfo(ZMODEM_INSTANCE* Instance)
{
    USHORT  j = 0;
    USHORT  i = 0;
    
    //Start by pulling in Filename (stripping out the directory if any)
    while(Instance->Buffer[j] != '\0')
    {
        switch (Instance->Buffer[j])
        {
            case 0x5C: // '\' - PC Directory Indicator
            case 0x2F: // '/' - Unix Directory Indicator
            case 0x3A: // ':' - Drive Indicator
                i = 0;
            break;
                
            default:
                if (i < (ZMODEM_FILE_NAME_LEN - 1))
                {
                    //if ((Instance->Buffer[j] >= 'a') && (Instance->Buffer[j] <= 'z'))
                    //{
                        //Switch to upper case
                    //    Instance->File.Name[i++] = Instance->Buffer[j] - 'a' + 'A';
                    //}
                    //else
                    //{
                        Instance->File.Name[i++] = Instance->Buffer[j];
                    //}
                }
                else
                {
                    //Return Empty File
                    Instance->File.Name[0] = '\0';
                }
            break;
        }
        j++;    
    }   
    Instance->File.Name[i] = '\0';
    
    //Next pull out file size (Decimal Number)
    j++;
    i = 0;      
    Instance->File.Size = 0;

    while ((Instance->Buffer[j] >= '0') && 
           (Instance->Buffer[j] <= '9') && 
           (i <= 16))
    {
        Instance->File.Size *= 10;
        Instance->File.Size += (Instance->Buffer[j++] - '0');
    }
    
    //Next pull out file date (Octal Number)
    j++;
    i = 0;      
    Instance->File.Date = 0;

    while ((Instance->Buffer[j] >= '0') && 
           (Instance->Buffer[j] <= '7') && 
           (i <= 16))
    {
        Instance->File.Date <<= 3;
        Instance->File.Date += (Instance->Buffer[j++] - '0');
    }   
}

//--------------------------------------------------------------------------
// Function:    PutFileInfo (Local Only)                                
//
// Parameters:  ZModem Instance
//
// Return:      Length of Data, Updated Buffer Information
//
// Description: Takes the contents of Instance.File and turns it into
//              a ZFILE message in Instance.Buffer
//--------------------------------------------------------------------------
USHORT PutFileInfo(ZMODEM_INSTANCE* Instance)
{
    USHORT  k = 0;  
    USHORT  j = 0;
    USHORT  i = 0;
    ULONG   Temp;
    ULONG   Dec = 1000000000;
    ULONG   Result;
    UBYTE   FileBytes[] = " 0 0 1 "; //File Mode: 0, SN: 0, Files: 1
    
    //Store File Name
    //Go until we hit a NULL
    while (Instance->File.Name[i])
    {
        Instance->Buffer[j++] = Instance->File.Name[i++];
    } 
    Instance->Buffer[j++] = '\0';
    
    //Store File Size as Decimal String
    Temp = Instance->File.Size;
    k = j;
    i = FALSE;
    while (Dec)
    {
        Result = Temp / Dec;
        if (Result) i = TRUE;
        if (i)      Instance->Buffer[j++] = '0' + Result;
        
        Temp -= (Result * Dec);
        Dec /= 10;
    }
    Instance->Buffer[j++] = ' ';    
    
    //Store File Date as Octal String   
    Temp = Instance->File.Date;
    Dec = 30;
    i = FALSE;
    while (Dec)
    {
        Result = Temp >> Dec;
        Result &= 0x07;
        if (Result) i = TRUE;
        if (i)      Instance->Buffer[j++] = '0' + Result;

        Dec -= 3;
    }
    Instance->Buffer[j++] = '0' + (Temp & 0x7);
    
    //Do the rest of the stuffing
    i = 0;
    while (FileBytes[i])
    {
        Instance->Buffer[j++] = FileBytes[i++];
    }
    
    //Repeat File Size as Bytes Remaining
    while (Instance->Buffer[k] != ' ')
    {
        Instance->Buffer[j++] = Instance->Buffer[k++];
    }   
    Instance->Buffer[j++] = '\0';
    
    return j;
}

//--------------------------------------------------------------------------
// Function:    SendBinByte (Local Only)                                
//
// Parameters:  ZModem Instance, Byte to Send
//
// Return:      TRUE if Successful, FALSE if Not
//
// Description: Writes a ZDLE escaped byte to the Output Function
//              Escape ZM_XON, ZM_XOFF, and CR following @ (Telenet net escape)
//--------------------------------------------------------------------------
USHORT SendBinByte(ZMODEM_INSTANCE* Instance, UBYTE Byte)
{
    USHORT  RetVal = TRUE;
    
    switch (Byte)
    {
        case ZM_CR:
        case (ZM_CR | 0x80):
            if ((Instance->Tx.LastChar & 0x7F) != '@')
            {
                Instance->Tx.LastChar = Byte;
            }
            else
            {
                RetVal = Instance->Tx.WriteFn(ZM_ZDLE);
                Instance->Tx.LastChar = Byte ^ 0100; //0x40
            }
        break;
        
        case ZM_ZDLE:           //Same as ZM_CAN: 0x18 CAN
        case ZM_DLE:            //0x10  DLE
        case ZM_XON:            //0x11  DC1 (XON)
        case ZM_XOFF:           //0x13  DC3 (XOFF)
        case (ZM_DLE  | 0x80):  //0x10 with MSB Set (0x80)
        case (ZM_XON  | 0x80):  //0x11 with MSB Set (0x80)
        case (ZM_XOFF | 0x80):  //0x13 with MSB Set (0x80)
            RetVal = Instance->Tx.WriteFn(ZM_ZDLE);
            Instance->Tx.LastChar = Byte ^ 0100;    //0x40
        break;
        
        default:
            Instance->Tx.LastChar = Byte;
        break;
    }
    
    if (RetVal)
    {
        RetVal = Instance->Tx.WriteFn(Instance->Tx.LastChar);
    }
    
    return RetVal;
}

//--------------------------------------------------------------------------
// Function:    SendBin16Header (Local Only)                                
//
// Parameters:  ZModem Instance(with valid Tx.Header), Type
//
// Return:      None
//
// Description: Writes a 16 Bit Binary Header out to the Output Function
//--------------------------------------------------------------------------
void SendBin16Header(ZMODEM_INSTANCE* Instance, UBYTE Type)
{
    SHORT   i;
    USHORT  Crc;
    
//  if (Type == ZM_ZDATA)
//  {
//      for (i = 0; --i >= 0;)
//      {
//          SendBinByte(Instance, 0);
//      }
//  }

    Instance->Tx.WriteFn(ZM_ZPAD);
    Instance->Tx.WriteFn(ZM_ZDLE);  
    Instance->Tx.WriteFn(ZM_ZBIN16);
    
    SendBinByte(Instance, Type);    
    
    Crc =  zUpdateCrc16(Type, 0);
    
    for (i = 0; i < ZM_HEADER_SIZE; i++)
    {
        SendBinByte(Instance, Instance->Tx.Header[i]);
        Crc =  zUpdateCrc16(0377 & Instance->Tx.Header[i], Crc);
    }
    
    Crc = zUpdateCrc16(0, Crc);
    Crc = zUpdateCrc16(0, Crc);
    
    SendBinByte(Instance, Crc >> 8);
    SendBinByte(Instance, Crc & 0xFF);
}

//--------------------------------------------------------------------------
// Function:    SendBinDataPacket (Local Only)                              
//
// Parameters:  ZModem Instance(with valid Buffer), Length of Message, FrameEnd Character
//
// Return:      None
//
// Description: Writes a 16 Bit Binary Data Packet out to the Output Function
//--------------------------------------------------------------------------
void SendBinDataPacket(ZMODEM_INSTANCE* Instance, USHORT Length, UBYTE FrameEnd)
{
    USHORT  Crc = 0;
    USHORT  i;
    
    //Send the Data
    for (i = 0; i < Length; i++)
    {
        SendBinByte(Instance, Instance->Buffer[i]);
        Crc =  zUpdateCrc16(0377 & Instance->Buffer[i], Crc);
    }
    
    //Send Frame End
    Instance->Tx.WriteFn(ZM_ZDLE);
    Instance->Tx.WriteFn(FrameEnd);
    Crc = zUpdateCrc16(FrameEnd, Crc);  
    
    //Calculate Crc
    Crc = zUpdateCrc16(0, Crc);
    Crc = zUpdateCrc16(0, Crc);
    
    //Send Crc
    SendBinByte(Instance, Crc >> 8);
    SendBinByte(Instance, Crc & 0xFF);
    
    //Send XON, if needed
    if (FrameEnd == ZM_ZCRCW)
    {
        Instance->Tx.WriteFn(ZM_XON);
    }
}

//--------------------------------------------------------------------------
// Function:    GetHexValue (Local Only)                                
//
// Parameters:  Hex Character
//
// Return:      Hex Value
//
// Description: Converts a Hex Value (char) into a UBYTE value
//--------------------------------------------------------------------------
UBYTE GetHexValue(UBYTE Hex)
{
    UBYTE   Val;
    
    if ((Hex >= '0') && (Hex <= '9'))
    {
        Val = Hex - '0';
    }
    else if ((Hex >= 'a') && (Hex <= 'f'))
    {
        Val = Hex - 'a' + 0xA;
    }
    else
    {
        Val = 0xFF; //Error
    }
    return Val;
}

//--------------------------------------------------------------------------
// Function:    SendHexByte (Local Only)                                
//
// Parameters:  ZModem Instance, Byte to Send
//
// Return:      None
//
// Description: Writes a Hex byte to the Output Function
//--------------------------------------------------------------------------
void SendHexByte(ZMODEM_INSTANCE* Instance, UBYTE Byte)
{
    Instance->Tx.WriteFn(HexL[Byte >> 4]);      //Write upper Nybble
    Instance->Tx.WriteFn(HexL[Byte & 0x0F]);    //Write lower Nybble    
}

//--------------------------------------------------------------------------
// Function:    SendHexHeader (Local Only)                              
//
// Parameters:  ZModem Instance (with valid Tx.Header), Type
//
// Return:      None
//
// Description: Writes a Hexadecimal Header out to the Output Function
//--------------------------------------------------------------------------
void SendHexHeader(ZMODEM_INSTANCE* Instance, UBYTE Type)
{
    SHORT   i;
    USHORT  Crc;

    Instance->Tx.WriteFn(ZM_ZPAD);  
    Instance->Tx.WriteFn(ZM_ZPAD);
    Instance->Tx.WriteFn(ZM_ZDLE);  
    Instance->Tx.WriteFn(ZM_ZHEX);
    
    SendHexByte(Instance, Type);    
    Crc =  zUpdateCrc16(Type, 0);
    
    for (i = 0; i < ZM_HEADER_SIZE; i++)
    {
        SendHexByte(Instance, Instance->Tx.Header[i]);
        Crc =  zUpdateCrc16(0377 & Instance->Tx.Header[i], Crc);
    }

    Crc = zUpdateCrc16(0, Crc);
    Crc = zUpdateCrc16(0, Crc);
    
    SendHexByte(Instance, Crc >> 8);
    SendHexByte(Instance, Crc & 0xFF);
    
    // Make it Printable
    Instance->Tx.WriteFn(ZM_CR);
    Instance->Tx.WriteFn(ZM_LF | 0x80);
    
    // Send XON in case a fake ZM_XOFF has stopped data flow
    if (Type != ZM_ZFIN) Instance->Tx.WriteFn(ZM_XON);
}

//--------------------------------------------------------------------------
// Function:    StoreHeaderData (Local Only)                                
//
// Parameters:  ZModem Instance, Data Byte to Store
//
// Return:      Header Status
//
// Description: Stores a byte into a header, doing CRC updates
//--------------------------------------------------------------------------
UBYTE StoreHeaderData(ZMODEM_INSTANCE* Instance, UBYTE Data)
{
    UBYTE RetVal = 0;
    
    //Update the CRC
    Instance->Rx.CrcVal = zUpdateCrc16(Data, Instance->Rx.CrcVal);
    //Buffer the Byte               
    (&Instance->Rx.Type)[Instance->Rx.MsgPos++] = Data;
    
    //Decrement CRC Count
    if(--Instance->Rx.CrcCount == 0)
    {
        RetVal = SH_CRC_FOUND;  

        if (Instance->Rx.CrcVal)
        {
            RetVal |= SH_CRC_ERROR;
        }
    }

    return RetVal;
}

//--------------------------------------------------------------------------
// Function:    StorePacketData (Local Only)                                
//
// Parameters:  ZModem Instance, Data Byte to Store
//
// Return:      Packet Status
//
// Description: Stores a byte into a packet, doing CRC updates
//--------------------------------------------------------------------------
UBYTE StorePacketData(ZMODEM_INSTANCE* Instance, UBYTE Data)
{
    UBYTE RetVal = 0;
    
    //Update the CRC
    Instance->Rx.CrcVal = zUpdateCrc16(Data, Instance->Rx.CrcVal);
    //Buffer the Byte

    if (Instance->Rx.FrameEnd)      
    {
        //If we've received the FrameEnd, 
        //   count down to the CRC check
        if(--Instance->Rx.CrcCount == 0)
        {
            RetVal |= SH_CRC_FOUND;
        
            if (Instance->Rx.CrcVal)
            {
                RetVal |= SH_CRC_ERROR;
            }
        }
    }
    else
    {
        //Buffer data
        if ((Instance->Rx.Type == ZM_ZDATA) && Instance->FilePtr) //We are writing the file
        {
            //We are writing to a valid file
            if (zFileWrite (&Instance->File, Instance->FilePos, Data))
            {
                Instance->FilePos++;
            }
            else
            {
                RetVal = SH_DATA_OVERFLOW;
            }
        } 
        else
        {
            //We are just using our buffer
            if (Instance->Rx.MsgLen < ZMODEM_BUFFER_SIZE)
            {           
                Instance->Buffer[Instance->Rx.MsgLen++] = Data;
            }
            else
            {
                RetVal = SH_DATA_OVERFLOW;
            }
        }
    }

    return RetVal;
}

//--------------------------------------------------------------------------
// Function:    MpSvcParseZPacket                                   
//
// Parameters:  ZModem Instance Variable, Byte to Parse
//
// Return:      Flags that indicate whether the Byte was used and whether
//              the transmit an / or receive channel should be reserved
//
// Description: Feeds the Receive Portion of the ZModem Receiver / Server
//--------------------------------------------------------------------------
USHORT MpSvcParseZPacket(ZMODEM_INSTANCE* Instance, UBYTE Data)
{
    UBYTE   RetVal = ZH_BYTE_USED;
    UBYTE   SaveData;
    UBYTE   HStatus;

    if (zTimeSecSince(Instance->Time) > Instance->Timeout)
    {
        Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;
        RetVal |= ZH_SESSION_CANNED;
    }
    else
    {
        Instance->Time = zTimeSecElapsed();
    
        switch (Instance->Rx.State)                 
        {
            case ZM_FRM_LOOKING_4_BIN_DATA_START:
                Instance->Rx.MsgLen = 0;
                Instance->Rx.CrcVal = 0;
                Instance->Rx.FrameEnd = 0;
                Instance->Rx.State = ZM_FRM_LOOKING_4_BIN_DATA;

            case ZM_FRM_LOOKING_4_BIN_DATA:
                if (Data == ZM_ZDLE)
                {
                    Instance->Rx.CanCount = 0;
                    Instance->Rx.State = ZM_FRM_LOOKING_4_BIN_DATA_ZDLE;
                }
                else
                {
                    //Store the data
                    HStatus = StorePacketData(Instance, Data);
                    if (HStatus)
                    {
                        //We're Done
                        Instance->Rx.State = ZM_FRM_LOOKING_4_BIN_DATA_START;
                    
                        RetVal |= HStatus;
                    }
                }
            break;
        
            case ZM_FRM_LOOKING_4_BIN_DATA_ZDLE:
                SaveData = TRUE;
                switch (Data)
                {
                    case ZM_CAN:
                        if(++Instance->Rx.CanCount >= 5)
                        {
                            //We're CAN'd   
                            RetVal |= ZH_SESSION_CANNED;
                        }
                        SaveData = FALSE;               
                    break;              

                    //ZM_ZDLE Escape sequences
                    case ZM_ZCRCG:  // CRC next, frame continues nonstop 
                    case ZM_ZCRCQ:  // CRC next, frame continues, ZM_ZACK expected 
                    case ZM_ZCRCE:  // CRC next, frame ends, header packet follows 
                    case ZM_ZCRCW:  // CRC next, ZM_ZACK expected, end of frame 
                        Instance->Rx.FrameEnd = Data;
                        Instance->Rx.CrcCount = 3;
                    break;

                    case ZM_ZRUB0:  // Translate to rubout 0177 
                        //Store the data 
                        Data = 0177;
                    break;  
                            
                    case ZM_ZRUB1:  // Translate to rubout 0377 
                        //Store the data 
                        Data = 0377;                    
                    break;                  
            
                    default:
                        if ((Data & 0140) == 0100) 
                        {
                            Data ^= 0100;
                        }
                        else
                        {
                            SaveData = FALSE;
                            Instance->Rx.State = ZM_FRM_LOOKING_4_BIN_DATA_START;
                            RetVal &= ~ZH_BYTE_USED;    //Not a Z Modem Header Byte
                            RetVal |= ZH_INVALID_CHARACTER; 
                        }                   
                    break;
                }
            
                if(SaveData) // Error (Not a valid ZDLE sequence)
                {
                    //Store the data 
                    HStatus = StorePacketData(Instance, Data);
                    if (HStatus)
                    {
                        //We're Done
                        Instance->Rx.State = ZM_FRM_LOOKING_4_BIN_DATA_START;
                    
                        RetVal |= HStatus;
                    }
                    else
                    {
                        Instance->Rx.State = ZM_FRM_LOOKING_4_BIN_DATA;
                    }
                }
            break;  
        } //end switch (Instance.Frame_State)
    }
    
    return RetVal;
}

//--------------------------------------------------------------------------
// Function:    MpSvcParseZHeader                                   
//
// Parameters:  ZModem Instance Variable, Byte to Parse
//
// Return:      Flags that indicate whether the Byte was used and whether
//              the transmit an / or receive channel should be reserved
//
// Description: Feeds the Receive Portion of the ZModem Receiver / Server
//--------------------------------------------------------------------------

USHORT MpSvcParseZHeader(ZMODEM_INSTANCE* Instance, UBYTE Data)
{
    UBYTE   RetVal = ZH_BYTE_USED;
    USHORT  Value;
    UBYTE   SaveData;
    UBYTE   Data7;
    UBYTE   HStatus;
    
    Data7 = Data & 0x7F;    //Strip Parity
    
    //Check to see if connection timed out
    if (Instance->Rx.State != ZM_FRM_LOOKING_4_ZPAD)
    {
        if (zTimeSecSince(Instance->Time) > Instance->Timeout)
        {
            Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;
            RetVal |= ZH_SESSION_CANNED;
        }
    }
    Instance->Time = zTimeSecElapsed();
    
    switch (Instance->Rx.State)                 
    {
        case ZM_FRM_LOOKING_4_ZPAD: 
            if (Data7 == ZM_ZPAD)
            {
                Instance->Rx.State = ZM_FRM_LOOKING_4_ZDLE;
            }
            else
            {
                RetVal &= ~ZH_BYTE_USED;    //Not a Z Modem Header Byte
            }
        break;
        
        case ZM_FRM_LOOKING_4_ZDLE: 
            switch (Data7)
            {
                case ZM_XON:    //Ignore XON
                case ZM_XOFF:   //Ignore XOFF
                break;
                
                case ZM_ZPAD:
                break;
                
                case ZM_ZDLE:
                    Instance->Rx.State = ZM_FRM_LOOKING_4_HEADER;
                break;

                default:
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;
                    RetVal &= ~ZH_BYTE_USED;    //Not a Z Modem Header Byte
                break;
            }
        break;
    
        case ZM_FRM_LOOKING_4_HEADER: 
            Instance->Rx.CrcCount = 7;
            Instance->Rx.MsgPos = 0;
            Instance->Rx.CrcVal = 0;                
        
            switch (Data7)
            {
                case ZM_XON:    //Ignore XON
                case ZM_XOFF:   //Ignore XOFF
                break;
                
                case ZM_ZPAD:
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZDLE;
                break;

                case ZM_ZBIN16: // Binary Frame 16-Bit
                    Instance->Rx.State = ZM_FRM_LOOKING_4_MSG_BIN;
                    Instance->Rx.PacketType = Data7;
                break;

                case ZM_ZHEX:   // Hex Frame
                    Instance->Rx.State = ZM_FRM_LOOKING_4_MSG_HEX1;
                    Instance->Rx.PacketType = Data7;
                break;
                
                case ZM_ZBIN32: // Binary Frame 32-Bit
                                // Don't Know how to deal with this (fall thru)

                default:    //Type Error - Start Over
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;
                    RetVal &= ~ZH_BYTE_USED;    //Not a Z Modem Header Byte
                    RetVal |= ZH_INVALID_CHARACTER;
                break;
            }
        break;
            
        case ZM_FRM_LOOKING_4_MSG_BIN: // 16-Bit Binary
            if (Data == ZM_ZDLE)
            {
                Instance->Rx.CanCount = 0;
                Instance->Rx.State = ZM_FRM_LOOKING_4_MSG_BIN_ZDLE;
            }
            else
            {
                //Store the data 
                HStatus = StoreHeaderData(Instance, Data);
                if (HStatus)
                {
                    //We're Done
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;                 
                    
                    RetVal |= HStatus;
                }
            }
        break;
        
        case ZM_FRM_LOOKING_4_MSG_BIN_ZDLE: // 16-Bit Binary
            SaveData = TRUE;
            switch (Data)
            {
                case ZM_CAN:
                    if(++Instance->Rx.CanCount >= 5)
                    {
                        //We're CAN'd   
                        RetVal |= ZH_SESSION_CANNED;
                    }
                    SaveData = FALSE;               
                break;              

                //ZM_ZDLE Escape sequences
                case ZM_ZCRCE:  // CRC next, frame ends, header packet follows 
                case ZM_ZCRCG:  // CRC next, frame continues nonstop 
                case ZM_ZCRCQ:  // CRC next, frame continues, ZM_ZACK expected 
                case ZM_ZCRCW:  // CRC next, ZM_ZACK expected, end of frame                 
                break;

                case ZM_ZRUB0:  // Translate to rubout 0177 
                    //Store the data 
                    Data = 0177;
                break;  
                            
                case ZM_ZRUB1:  // Translate to rubout 0377 
                    //Store the data 
                    Data = 0377;                    
                break;                  
            
                default:
                    if ((Data & 0140) == 0100) 
                    {
                        Data ^= 0100;
                    }
                    else
                    {
                        SaveData = FALSE;
                        Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;
                        RetVal &= ~ZH_BYTE_USED;    //Not a Z Modem Header Byte
                        RetVal |= ZH_INVALID_CHARACTER; 
                    }                   
                break;
            }
            
            if(SaveData) // Error (Not a valid ZDLE sequence)
            {
                //Store the data 
                HStatus = StoreHeaderData(Instance, Data);
                if (HStatus)
                {
                    //We're Done
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;                 
                    
                    RetVal |= HStatus;
                }
                else
                {
                    Instance->Rx.State = ZM_FRM_LOOKING_4_MSG_BIN;
                }
            }
        break;

        case ZM_FRM_LOOKING_4_MSG_HEX1: 
            if ((Data7 != ZM_XON) && (Data7 != ZM_XOFF))
            {
                Value = GetHexValue(Data7);
                if (Value <= 0xF)
                {
                    Instance->Rx.Nybble = Value << 4; //Store Upper Nybble
                    Instance->Rx.State = ZM_FRM_LOOKING_4_MSG_HEX2;
                }
                else    //Error: Invalid Hex Character
                {
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;
                    RetVal &= ~ZH_BYTE_USED;    //Not a Z Modem Header Byte
                    RetVal |= ZH_INVALID_CHARACTER;
                }               
            }
        break;
        
        case ZM_FRM_LOOKING_4_MSG_HEX2: 
            if((Instance->Rx.CrcCount == 1) && (Instance->Rx.Type == 8))
            {
                Data = Data;        
            }
            if ((Data7 != ZM_XON) && (Data7 != ZM_XOFF))
            {
                Value = GetHexValue(Data7);
                if (Value <= 0xF)
                {
                    //Combine with Upper Nybble
                    Value |= Instance->Rx.Nybble;

                    //Store the data 
                    HStatus = StoreHeaderData(Instance, Value);
                    if (HStatus)
                    {
                        //We're Done
                        Instance->Rx.State = ZM_FRM_LOOKING_4_CR;
                        //Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;                   
                    
                        RetVal |= HStatus;
                    }
                    else
                    {
                        Instance->Rx.State = ZM_FRM_LOOKING_4_MSG_HEX1;
                    }
                }
                else    //Error: Invalid Hex Character
                {
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;
                    RetVal &= ~ZH_BYTE_USED;    //Not a Z Modem Header Byte
                    RetVal |= ZH_INVALID_CHARACTER;
                }               
            }
        break;          

        case ZM_FRM_LOOKING_4_CR: 
            switch (Data7)
            {
                case ZM_XOFF:   //Ignore XOFF
                break;
                
                case ZM_ZPAD:
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZDLE;
                break;

                case ZM_CR:
                    Instance->Rx.State = ZM_FRM_LOOKING_4_LF;
                break;
                
                case ZM_LF:
                case ZM_XON:                
                default:    
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;
                break;
            }
        break;
        
        case ZM_FRM_LOOKING_4_LF: 
            switch (Data7)
            {
                case ZM_XOFF:   //Ignore XOFF
                break;
                
                case ZM_ZPAD:
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZDLE;
                break;
                
                case ZM_LF:
                case ZM_XON:                
                default:
                    Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;
                break;
            }   
        break;  
    } //end switch (Instance.Frame_State)


    return RetVal;
    
}

//--------------------------------------------------------------------------
// Function:    ClearHeader (Local Only)                                
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Clears (zeroes) the Header Bytes
//--------------------------------------------------------------------------
void ClearHeader(ZMODEM_INSTANCE* Instance)
{
    UBYTE   i;
    
    for (i = 0; i < ZM_HEADER_SIZE; i++)
    {
        Instance->Tx.Header[i] = 0;
    }
}

//--------------------------------------------------------------------------
// Function:    GetFilePos (Local Only)                             
//
// Parameters:  ZModem Instance
//
// Return:      File Position from Instance->Rx.Header
//
// Description: Recovers the File Position from the Receive Header
//--------------------------------------------------------------------------
ULONG GetFilePos(ZMODEM_INSTANCE* Instance)
{
    ULONG   FilePos = 0;
    SHORT   i;
    
    for(i = 3; i >= 0; i--)
    {
        FilePos <<= 8;
        FilePos |= Instance->Rx.Header[i];
    }
    
    return FilePos;
}

//--------------------------------------------------------------------------
// Function:    PutFilePos (Local Only)                             
//
// Parameters:  ZModem Instance, Position
//
// Return:      None
//
// Description: Loads the File Position into the Transmit Header
//--------------------------------------------------------------------------
void PutFilePos(ZMODEM_INSTANCE* Instance, ULONG Position)
{
    UBYTE   i;
    
    for (i = 0; i < ZM_HEADER_SIZE; i++)
    {
        Instance->Tx.Header[i] = Position & 0xFF;
        Position >>= 8;
    }
}

//--------------------------------------------------------------------------
// Function:    SendZrQInit (Local Only)                                
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Sends a ZRINIT Header
//--------------------------------------------------------------------------
void SendZrQInit(ZMODEM_INSTANCE* Instance)
{
    ClearHeader(Instance);      
    SendHexHeader(Instance, ZM_ZRQINIT);
}

//--------------------------------------------------------------------------
// Function:    SendZrInit (Local Only)                             
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Sends a ZRINIT Header
//--------------------------------------------------------------------------
void SendZrInit(ZMODEM_INSTANCE* Instance)
{
    Instance->Tx.Header[ZM_ZF0] = ZM_CANOVIO;
    Instance->Tx.Header[ZM_ZF1] = 0;
    //HexTerminal doesn't like non-default lengths
    Instance->Tx.Header[ZM_ZP0] = 0;//ZMODEM_BUFFER_LEN & 0xFF;
    Instance->Tx.Header[ZM_ZP1] = 0;//ZMODEM_BUFFER_LEN >> 8;       
    SendHexHeader(Instance, ZM_ZRINIT);
}

//--------------------------------------------------------------------------
// Function:    SendZFerr (Local Only)                              
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Sends a ZFERR Header
//--------------------------------------------------------------------------
void SendZFerr(ZMODEM_INSTANCE* Instance)
{
    ClearHeader(Instance);  
    SendHexHeader(Instance, ZM_ZFERR);
}

//--------------------------------------------------------------------------
// Function:    SendZAbort (Local Only)                             
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Sends a ZABORT Header
//--------------------------------------------------------------------------
void SendZAbort(ZMODEM_INSTANCE* Instance)
{
    ClearHeader(Instance);  
    SendHexHeader(Instance, ZM_ZABORT);
}

//--------------------------------------------------------------------------
// Function:    SendZSkip (Local Only)                              
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Sends a ZSKIP Header
//--------------------------------------------------------------------------
void SendZSkip(ZMODEM_INSTANCE* Instance)
{
    Instance->Tx.Header[ZM_ZF0] = ZM_CANOVIO;
    Instance->Tx.Header[ZM_ZF1] = 0;
    //HexTerminal doesn't like non-default lengths
    Instance->Tx.Header[ZM_ZP0] = 0;//ZMODEM_BUFFER_LEN & 0xFF;
    Instance->Tx.Header[ZM_ZP1] = 0;//ZMODEM_BUFFER_LEN >> 8;
    SendHexHeader(Instance, ZM_ZSKIP);
}

//--------------------------------------------------------------------------
// Function:    SendZrPos (Local Only)                              
//
// Parameters:  ZModem Instance, Position
//
// Return:      None
//
// Description: Sends a ZPOS Header
//--------------------------------------------------------------------------
void SendZrPos(ZMODEM_INSTANCE* Instance, ULONG Position)
{
    PutFilePos(Instance, Position);
    
    SendHexHeader(Instance, ZM_ZRPOS);
}

//--------------------------------------------------------------------------
// Function:    SendZAck (Local Only)                               
//
// Parameters:  ZModem Instance, Acknowledgement
//
// Return:      None
//
// Description: Sends a ZACK Header
//--------------------------------------------------------------------------
void SendZAck(ZMODEM_INSTANCE* Instance, ULONG Acknowledge)
{
    PutFilePos(Instance, Acknowledge);  

    SendHexHeader(Instance, ZM_ZACK);
}

//--------------------------------------------------------------------------
// Function:    SendZFin (Local Only)                               
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Sends a ZFIN Header
//--------------------------------------------------------------------------
void SendZFin(ZMODEM_INSTANCE* Instance)
{
    ClearHeader(Instance);
    SendHexHeader(Instance, ZM_ZFIN);
}

//--------------------------------------------------------------------------
// Function:    SendZCan (Local Only)                               
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Sends a CAN characters to end transmission
//--------------------------------------------------------------------------
void SendZCan(ZMODEM_INSTANCE* Instance)
{
    UBYTE   i;
    
    for (i = 0; i < 8; i++)
    {
        Instance->Tx.WriteFn(ZM_CAN);
    }
}

//--------------------------------------------------------------------------
// Function:    SendZFileCallBack (Local Only)                              
//
// Parameters:  None (InstSendZFile and StateSendZFile) are pre-defined
//
// Return:      None
//
// Description: Do not call this function directly, call SendZFile instead
//              This function finsihes the work SendZFile starts
//--------------------------------------------------------------------------
void SendZFileCallBack(void)
{
    static USHORT Length = 0;
    static USHORT State = 0;
    
    if (InstSendZFile)  //Must have a valid instance
    {
        switch (State++)
        {
            case 0:
                //Pack the File Data Packet
                Length = PutFileInfo(InstSendZFile);
                //IoSvcRequestTimer(2, SendZFileCallBack); (TBD)
            break;
            
            case 1:
                SendBinDataPacket(InstSendZFile, Length, ZM_ZCRCW);
            default:
                State = 0;
                InstSendZFile = NULL;
                //Reset time out
                InstSendZFile->Timeout = ZMODEM_TIMEOUT_DEFAULT;
            break;      
        }
    }   
}

//--------------------------------------------------------------------------
// Function:    SendZFile (Local Only)                              
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Sends a ZFILE Header and Data Sub Packet
//              This function uses a call back SendZFileCallBack to
//              spread the work out over time.
//--------------------------------------------------------------------------
void SendZFile(ZMODEM_INSTANCE* Instance)
{
    //Save Instance for call back
    InstSendZFile = Instance;
    
    //Pack and send the Header
    Instance->Tx.Header[ZM_ZF0] = ZM_ZCRESUM;
    Instance->Tx.Header[ZM_ZF1] = 0;
    Instance->Tx.Header[ZM_ZF2] = 0;
    Instance->Tx.Header[ZM_ZF3] = 0;
    SendBin16Header(Instance, ZM_ZFILE);
    
    //IoSvcRequestTimer(2, SendZFileCallBack); (TBD)
    Instance->Timeout = ZMODEM_TIMEOUT_DEFAULT;
}

//--------------------------------------------------------------------------
// Function:    SendZEof (Local Only)                               
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Sends a ZEOF Header
//--------------------------------------------------------------------------
void SendZEof(ZMODEM_INSTANCE* Instance)
{
    PutFilePos(Instance, Instance->FilePos);
    
    SendBin16Header(Instance, ZM_ZEOF);
    
    //We have sent all of the Data, Close the file
    zFileCancel (Instance);
    Instance->FilePtr = NULL;
}

//--------------------------------------------------------------------------
// Function:    SendZDataCallBack (Local Only)                              
//
// Parameters:  None (InstSendZData and StateSendZData) are pre-defined
//
// Return:      None
//
// Description: Do not call this function directly, call SendZData instead
//              This function finsihes the work SendZData starts
//--------------------------------------------------------------------------
void SendZDataCallBack(void)
{
    static USHORT FrameCount = 0;
    static USHORT Crc = 0;
    USHORT Count = 0;   
    USHORT  Data;
    UBYTE   FrameEnd;

    if (InstSendZData)  //Must have a valid instance
    {   
        //We are sending files in small bursts (Count) at a time,
        //but we need to send a frame end by the time we reach the
        //Max Frame size (FrameCount).  In addition, we need to quit
        //sending data at the end of file (InstSendZData->FilePos).
        //We use three counters to keep track of it all.
        while((Count < ZM_ZDATA_MAX_BYTES) &&
              (FrameCount < ZM_ZDATA_MAX_FRAME) &&
              (InstSendZData->FilePos < InstSendZData->File.Size))
        {
            Data = zFileRead (InstSendZData, InstSendZData->FilePos);
            if (Data & 0xFF00)
            {
                Data &= 0xFF;
                
                if (SendBinByte(InstSendZData, Data))
                {
                    Crc =  zUpdateCrc16(Data, Crc);
                    InstSendZData->FilePos++;
                    FrameCount++;
                    Count++;
                }
                else
                {
                    //Pretend we're at the end of the count
                    Count = ZM_ZDATA_MAX_BYTES;
                }
            }
            else
            {
                //FAILURE (Pretend we're at the end of file)
                InstSendZData->FilePos = InstSendZData->File.Size;
            }
        }

        //Add a frame end as needed, either end of Max Frame, or end
        //of file.
        if((InstSendZData->FilePos >= InstSendZData->File.Size) ||
           (FrameCount >= ZM_ZDATA_MAX_FRAME))
        {
            //We're starting a new frame
            FrameCount = 0;
            
            //Check for type of frame end
            if(InstSendZData->FilePos >= InstSendZData->File.Size)
            {
                //End of file
                FrameEnd = ZM_ZCRCE;    // CRC next, frame ends, header packet follows
            }
            else
            {
                //End of frame
                if (InstSendZData->AckRequired)
                {
                    FrameEnd = ZM_ZCRCW;    // CRC next, ZM_ZACK expected, end of frame
                }
                else
                {
                    FrameEnd = ZM_ZCRCG;    // CRC next, frame continues nonstop
                }
            }
            
            //Send Frame End
            InstSendZData->Tx.WriteFn(ZM_ZDLE);
            InstSendZData->Tx.WriteFn(FrameEnd);
            Crc = zUpdateCrc16(FrameEnd, Crc);  
    
            //Calculate Crc
            Crc = zUpdateCrc16(0, Crc);
            Crc = zUpdateCrc16(0, Crc);
    
            //Send Crc
            SendBinByte(InstSendZData, Crc >> 8);
            SendBinByte(InstSendZData, Crc & 0xFF);
            Crc = 0;
            
            //Send XON, if needed
            if (FrameEnd == ZM_ZCRCW)
            {
                InstSendZData->Tx.WriteFn(ZM_XON);
            }
        }
        
        //Check to see if we're done, or need to continue
        if (InstSendZData->FilePos < InstSendZData->File.Size) //Still work to do?
        {
            if ((FrameCount) || (!InstSendZData->AckRequired))
            {
                //IoSvcRequestTimer(ZM_ZDATA_INTERVAL, SendZDataCallBack); (TBD)
            }
        }
        else    
        {
            //Reset time out
            InstSendZData->Timeout = ZMODEM_TIMEOUT_DEFAULT;
            //Send End Of File Header
            SendZEof(InstSendZData);            
            //Clean up for next time
            InstSendZData = NULL;
        }       
    }
}

//--------------------------------------------------------------------------
// Function:    SendZData (Local Only)                              
//
// Parameters:  ZModem Instance
//
// Return:      None
//
// Description: Sends a ZDATA Header and one or more Data Sub Packet(s)
//              This function uses a call back SendZDataCallBack to
//              spread the work out over time.
//--------------------------------------------------------------------------
void SendZData(ZMODEM_INSTANCE* Instance)
{
    //Save Instance for call back
    InstSendZData = Instance;
    
    //Pack and send the Header
    PutFilePos(Instance, Instance->FilePos);

    SendBin16Header(Instance, ZM_ZDATA);
    
    //IoSvcRequestTimer(2, SendZDataCallBack); (TBD)
    
    Instance->Timeout = 600; //Ten minutes
}

//--------------------------------------------------------------------------
// Function:    zModemParse                                    
//
// Parameters:  ZModem Instance Variable
//              Buffer to Parse
//
// Return:      Flags that indicate whether the Byte was used and whether
//              the transmit an / or receive channel should be reserved
//
// Description: Feeds the Receive Portion of the ZModem Receiver / Server
//--------------------------------------------------------------------------

UBYTE zModemParse(ZMODEM_INSTANCE* Instance, UBYTE Data)
{
    static  char Cleanup[] = "\r\nOO";
    USHORT  RetVal = 0;
    USHORT  HStatus;
    // SHORT    i; (TBD)

    if (Instance->State == STATE_ZM_INITIALIZE)
    {
        Instance->State = STATE_ZM_FIND_HEADER;
        Instance->Operation = ZM_OPER_LISTEN;
        Instance->Rx.State = ZM_FRM_LOOKING_4_ZPAD;
        Instance->Time = 0;
        Instance->Timeout = ZMODEM_TIMEOUT_DEFAULT;

        //Clear File Info
        Instance->File.Name[0] = '\0';
        Instance->File.Size = 0;

        //Set Tx Parameters
        Instance->Tx.LastChar = 0;      
    }
    
    switch (Instance->State)
    {
        case STATE_ZM_CLEANUP_START:
            Instance->Rx.Type = 0;
            Instance->State = STATE_ZM_CLEANUP;
            
        case STATE_ZM_CLEANUP:
            if ((Instance->Rx.Type < 3) && 
                ((Data & 0x7F) == Cleanup[Instance->Rx.Type++]))
            {
                RetVal  |= ZM_BYTE_USED;
                break;
            }
            else
            {
                Instance->State = STATE_ZM_FIND_HEADER;
            }

        case STATE_ZM_FIND_HEADER:
            HStatus = MpSvcParseZHeader(Instance, Data);
            if (HStatus & ZH_BYTE_USED)
            {
                RetVal |= ZM_BYTE_USED;
                //Block unwanted Sync/Esc responses
                //MpEchoIndications = FALSE;  (TBD)
            }
            
            if (HStatus & (ZH_INVALID_CHARACTER | ZH_BAD_CRC | ZH_SESSION_CANNED))
            {
                if (!(HStatus & ZH_SESSION_CANNED))
                {
                    SendZCan(Instance);
                }
                Instance->Operation = ZM_OPER_LISTEN;
                RetVal |= ZM_UNRESERVE_CHANNEL;
                zFileCancel(Instance);
                Instance->FilePtr = NULL;
            }               
            else if (HStatus & ZH_DATA_FOUND)
            {
                switch(Instance->Rx.Type)
                {
                    case ZM_ZRQINIT:    // Request receive init 
                        //Other End Is Requesting Download
                        Instance->Operation = ZM_OPER_DOWNLOAD;
                        
                        //Need to send ZRINIT
                        RetVal |= ZM_RESERVE_CHANNEL;
                        SendZrInit(Instance);
                    break;
                    
                    case ZM_ZRINIT:     // Receive init 
                        if (Instance->Operation == ZM_OPER_LISTEN)
                        {
                            //Other End Is Requesting Upload
                            RetVal |= ZM_RESERVE_CHANNEL;
                            Instance->Operation = ZM_OPER_UPLOAD;
                            //Need to send ZRQINIT
                            Instance->File.Name[0] = '\0';
                            SendZrQInit(Instance);
                        }                   
                        else if (Instance->Operation == ZM_OPER_UPLOAD)
                        {
                            if(Instance->File.Name[0] == '\0')
                            {
                                //Other End Is Responding to ZRQINIT
                                if (Instance->Rx.Header[ZM_ZF0] & ZM_CANOVIO)
                                {
                                    Instance->AckRequired = FALSE;  
                                    //Normally Ack isn't required, but
                                    //we're going to send one anyway
                                    //Instance->AckRequired = TRUE;
                                }
                                else
                                {
                                    Instance->AckRequired = TRUE;                                   
                                }
                                
                                //Get Down Load File Information
                                // IoSvcGetDownLoadFileInfo (&Instance->File); (TBD)
                                //Try to open the file
                                Instance->FilePtr = zFileOpen(Instance, FALSE); //Read Request
                                if (Instance->FilePtr)
                                {
                                    //Need to send ZFILE
                                    SendZFile(Instance);
                                }
                                else
                                {
                                    //We can't send the file
                                    //Send ZFIN
                                    SendZFin(Instance);                                     
                                }
                            }
                            else
                            {
                                //Other End Is Responding to ZEOF
                                //Send ZFIN
                                SendZFin(Instance);             
                            }
                        }
                    break;
                    
                    case ZM_ZFILE:      // File name from sender 
                        if (Instance->Operation == ZM_OPER_DOWNLOAD)
                        {
                            Instance->State = STATE_ZM_FIND_PACKET;
                        }
                    break;

                    case ZM_ZFIN:       // Finish session 
                        if (Instance->Operation == ZM_OPER_DOWNLOAD)
                        {   
                            // Other end Sent ZFIN
                            // We need to send ZFIN
                            SendZFin(Instance);
                            
                            Instance->State = STATE_ZM_CLEANUP_START;
                            Instance->Operation = ZM_OPER_LISTEN;
                            RetVal |= ZM_UNRESERVE_CHANNEL;
                        }
                        else if(Instance->Operation == ZM_OPER_UPLOAD)
                        {
                            // Other end responded to our ZFIN
                            // We need to send "OO" (Over and Out)
                            Instance->Tx.WriteFn('O');
                            Instance->Tx.WriteFn('O');                          
                            
                            Instance->Operation = ZM_OPER_LISTEN;
                            RetVal |= ZM_UNRESERVE_CHANNEL; 
                        }
                    break;

                    case ZM_ZDATA:      // Data packet(s) follow
                        if (Instance->Operation == ZM_OPER_DOWNLOAD)
                        {
                            Instance->State = STATE_ZM_FIND_PACKET;
                            
                            if (GetFilePos(Instance) != Instance->FilePos)
                            {
                                //Send ZRPOS to update position
                                SendZrPos(Instance, Instance->FilePos);
                            }
                        }
                    break;
                    
                    case ZM_ZEOF:       // End of file
                        if (Instance->Operation == ZM_OPER_DOWNLOAD)
                        {   
//                          if (GetFilePos(Instance) == Instance->FilePos)
//                          {
                                //We have all of the Data, Close the file
                                if (zFileClose(Instance))
                                {
                                    // We need to send ZRINIT
                                    SendZrInit(Instance);
                                }
                                else
                                {
                                    // We cannot save the file
                                    //SendZFerr(Instance);
                                    SendZAbort(Instance);
                                    //Instance->State = STATE_ZM_CLEANUP_START;
                                    Instance->State = STATE_ZM_FIND_HEADER;
                                    Instance->Operation = ZM_OPER_LISTEN;
                                    RetVal |= ZM_UNRESERVE_CHANNEL;
                                }
                                Instance->FilePtr = NULL;
//                          }
                        }
                    break;
                    
                    case ZM_ZRPOS:      // Resume data trans at this position 
                        if (Instance->Operation == ZM_OPER_UPLOAD)
                        {
                            Instance->FilePos = GetFilePos(Instance);
                            //Send ZData Packet(s)
                            SendZData(Instance);
                        }
                    break;          
                            
                    case ZM_ZSKIP:      // To sender: skip this file
                        if (Instance->Operation == ZM_OPER_UPLOAD)
                        {
                            //No More Files, Send ZFIN
                            SendZFin(Instance);
                        }                   
                    break;     
                    
                    case ZM_ZCHALLENGE: // Receiver's Challenge
                        if (Instance->Operation == ZM_OPER_UPLOAD)
                        {   
                            SendZAck(Instance, GetFilePos(Instance));
                        }                                           
                    break;                  
                                 
                    case ZM_ZNAK:       // Last packet was garbled
                        if (Instance->Operation == ZM_OPER_UPLOAD)
                        {                       
                            Data = Data;
                        }
                    break;
                    
                    case ZM_ZACK:       // ACK to above 
                        if (Instance->Operation == ZM_OPER_UPLOAD)
                        {                       
                            SendZDataCallBack();
                        }
                    break;      
        
                    case ZM_ZABORT:     // Abort batch transfers 
                    case ZM_ZSINIT:     // Send init sequence (optional) 
                    case ZM_ZFERR:      // Fatal Read or Write error Detected 
                    case ZM_ZCRC:       // Request for file CRC and response 
                    case ZM_ZCOMPL:     // Request is complete 
                    case ZM_ZCAN:       // Other end canned session with CAN*5 
                    case ZM_ZFREECNT:   // Request for free bytes on filesystem 
                    case ZM_ZCOMMAND:   // Command from sending program 
                    case ZM_ZSTDERR:    // Output to standard error, data follows 
                    case ZM_ZRQINITB:   // ZM_ZRQINIT and set broadcast mode, responder in ZM_ZF0 
                    default:
                        if (Instance->Operation == ZM_OPER_UPLOAD)
                        {                       
                            Data = Data;
                        }                   
                    break;
                }
            }   

        break;
        
        case STATE_ZM_FIND_PACKET:
            HStatus = MpSvcParseZPacket(Instance, Data);
            if (HStatus & ZH_BYTE_USED)
            {
                RetVal |= ZM_BYTE_USED;
            }
            
            if (HStatus & (ZH_INVALID_CHARACTER | ZH_BAD_CRC | ZH_SESSION_CANNED))
            {
                if (!(HStatus & ZH_SESSION_CANNED))
                {
                    SendZCan(Instance);
                }
                Instance->State = STATE_ZM_FIND_HEADER;
                Instance->Operation = ZM_OPER_LISTEN;
                RetVal |= ZM_UNRESERVE_CHANNEL;
                zFileCancel(Instance);
                Instance->FilePtr = NULL;
            }
            else if (HStatus & ZH_DATA_FOUND)
            {
                switch(Instance->Rx.Type)
                {
                    case ZM_ZFILE:      // File name from sender 
                        if (Instance->Operation == ZM_OPER_DOWNLOAD)
                        {
                            // We now have the ZFILE info
                            GetFileInfo(Instance);
                            Data = Data;
                            //Now we need to check to see if it is a file we care about
                            Instance->FilePtr = zFileOpen(Instance, TRUE); //Write Request, don't ignore read-only attribute
                            Instance->FilePos = 0;              

                            // We either send ZRPOS to start download
                            // or ZCRC if we want a 32-Bit CRC of the file (not equipped to do this)
                            // or ZSKIP if we don't want the file           
                            Instance->State = STATE_ZM_FIND_HEADER; 
                            if(Instance->FilePtr)
                            {
                                //Start at File Beginning
                                SendZrPos(Instance, Instance->FilePos);
                            }
                            else
                            {
                                SendZSkip(Instance);
                            }
                        }
                    break;

                    case ZM_ZDATA:      // Data packet(s) follow
                        if (Instance->Operation == ZM_OPER_DOWNLOAD)
                        {
                            switch(Instance->Rx.FrameEnd)
                            {
                                case ZM_ZCRCQ: // CRC next, frame continues, ZM_ZACK expected
                                    SendZAck(Instance, Instance->FilePos);
                                case ZM_ZCRCG: // CRC next, frame continues nonstop
                                    //Get More Data
                                break;
                            
                                case ZM_ZCRCW: // CRC next, ZM_ZACK expected, end of frame
                                    SendZAck(Instance, Instance->FilePos);
                                case ZM_ZCRCE: // CRC next, frame ends, header packet follows 
                                    // Get New Header
                                    Instance->State = STATE_ZM_FIND_HEADER;
                                break;              
                            }
                        }
                    break;
                    
                    case ZM_ZRQINIT:    // Request receive init 
                    case ZM_ZRINIT:     // Receive init 
                    case ZM_ZSINIT:     // Send init sequence (optional) 
                    case ZM_ZACK:       // ACK to above 
                    case ZM_ZSKIP:      // To sender: skip this file 
                    case ZM_ZNAK:       // Last packet was garbled 
                    case ZM_ZABORT:     // Abort batch transfers 
                    case ZM_ZFIN:       // Finish session 
                    case ZM_ZRPOS:      // Resume data trans at this position 
                    case ZM_ZEOF:       // End of file 
                    case ZM_ZFERR:      // Fatal Read or Write error Detected 
                    case ZM_ZCRC:       // Request for file CRC and response 
                    case ZM_ZCHALLENGE: // Receiver's Challenge 
                    case ZM_ZCOMPL:     // Request is complete 
                    case ZM_ZCAN:       // Other end canned session with CAN*5 
                    case ZM_ZFREECNT:   // Request for free bytes on filesystem 
                    case ZM_ZCOMMAND:   // Command from sending program 
                    case ZM_ZSTDERR:    // Output to standard error, data follows 
                    case ZM_ZRQINITB:   // ZM_ZRQINIT and set broadcast mode, responder in ZM_ZF0 

                    default:
                    break;
                }
            }   
        
        break;
        
        default: 
        break;

    } //end switch (Instance.State)

    return RetVal;
    
}

//--------------------------------------------------------------------------
// Function:    zModemInit                                 
//
// Parameters:  ZModem Instance Variable
//
// Return:      None
//
// Description: Initializes an instance of a ZModem Parser
//--------------------------------------------------------------------------

void zModemInit(ZMODEM_INSTANCE* Instance, FN_ZMODEM_WRITE WriteFn, char *drive)
{
    Instance->Tx.WriteFn = WriteFn;
    Instance->State = STATE_ZM_INITIALIZE;

    if (drive != NULL)
    {
        strncpy(Instance->Drive, drive, ZMODEM_DRIVE_NAME_LEN);
        Instance->Drive[ZMODEM_DRIVE_NAME_LEN-1] = '\0';
    }
}

//EOF
