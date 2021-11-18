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
//  File Name      : ztypes.h
//  Summary        : Global Type Definition Header File
//
//******************************************************************************

#ifndef _ZTYPES_H
#define _ZTYPES_H
    
    //NOTE: Blackfin Default Storage Types and Sizes (since these are always machine dependant)
    //==========================================================================================
    // Type                 | Bit Size      | Number Representation     | sizeof returns
    //------------------------------------------------------------------------------------------
    // char                 | 8 - signed    | 8-bit two's complement    | 1
    // unsigned char        | 8 - unsigned  | 8-bit unsigned magnitude  | 1
    // short                | 16 - signed   | 16-bit two's complement   | 2
    // unsigned short       | 16 - unsigned | 16-bit unsigned magnitude | 2
    // int                  | 32 - signed   | 32-bit two's complement   | 4
    // unsigned int         | 32 - unsigned | 32-bit unsigned magnitude | 4
    // long                 | 32 - signed   | 32-bit two's complement   | 4
    // unsigned long        | 32 - unsigned | 32-bit unsigned magnitude | 4
    // long long            | 64 - signed   | 64-bit two's complement   | 8 *
    // unsigned long long   | 64 - unsigned | 64-bit unsigned magnitude | 8 *
    // pointer              | 32 bits       | 32-bit two's complement   | 4
    // function pointer     | 32 bits       | 32-bit two's complement   | 4
    // float                | 32 bits       | 32-bit IEEE single-prec.  | 4 *
    // double               | 64 bits       | 64-bit IEEE double-prec.  | 8 *
    // long double          | 64 bits       | 64-bit IEEE double-prec.  | 8 *
    // fract16              | typedef-short | 1.15 fractional           | 2 #
    // fract32              | typedef-long  | 1.31 fractional           | 4 #
    //------------------------------------------------------------------------------------------
    // * - Floating Point and 64 bit data types are implemented using software emulation, and are
    //      expected to run more slowly than hardware supported native data types.
    // # - The fract16 and fract32 are not actually intrinsic data types - they are typedefs to
    //      to short and long, respectively.  In C, you need to use built-in functions to do basic 
    //      arithmetic; you cannot do fract16 * fract16 and get the right result.
    

    /* Key stroke saving typedef's */
//  #ifndef BYTE
//  typedef signed char         BYTE;
//  #endif

    #ifndef UBYTE
    typedef unsigned char       UBYTE;
    #endif

//  #ifndef WORD
//  typedef signed short        WORD;
//  #endif

//  #ifndef UWORD
//  typedef unsigned short      UWORD;  
//  #endif

    #ifndef LONG
    typedef signed long         LONG;
    #endif

    #ifndef ULONG
    typedef unsigned long       ULONG;
    #endif

    #ifndef SHORT
    typedef signed short        SHORT;
    #endif

    #ifndef USHORT
    typedef unsigned short      USHORT;
    #endif

    #ifndef BOOLEAN
    typedef unsigned short int  BOOLEAN;
    #endif
        
    /*
    **  Function Macro  Definitions
    */
    #define MIN(A,B)    ((A) < (B) ? (A) : (B))     
    #define MAX(A,B)    ((A) > (B) ? (A) : (B)) 
    
    /*
    **  Common Definitions
    */      
    #ifdef FALSE
    #undef FALSE
    #endif
    #define FALSE               (0)

    #ifdef TRUE
    #undef TRUE
    #endif
    #define TRUE                (1)

    #define FAILURE             FALSE
    #define SUCCESS             TRUE

//  #define OFF                 FALSE
//  #define ON                  TRUE
//  #define NO                  FALSE
//  #define YES                 TRUE
//  #define LOW                 FALSE
//  #define HIGH                TRUE

    /*
    **  Bit Field Definitions
    */
    #define bit0                (USHORT)0x0001
    #define bit1                (USHORT)0x0002
    #define bit2                (USHORT)0x0004
    #define bit3                (USHORT)0x0008
    #define bit4                (USHORT)0x0010
    #define bit5                (USHORT)0x0020
    #define bit6                (USHORT)0x0040
    #define bit7                (USHORT)0x0080
    #define bit8                (USHORT)0x0100
    #define bit9                (USHORT)0x0200
    #define bit10               (USHORT)0x0400
    #define bit11               (USHORT)0x0800
    #define bit12               (USHORT)0x1000
    #define bit13               (USHORT)0x2000
    #define bit14               (USHORT)0x4000
    #define bit15               (USHORT)0x8000
    #define bit16               (ULONG)0x00010000
    #define bit17               (ULONG)0x00020000
    #define bit18               (ULONG)0x00040000
    #define bit19               (ULONG)0x00080000
    #define bit20               (ULONG)0x00100000
    #define bit21               (ULONG)0x00200000
    #define bit22               (ULONG)0x00400000
    #define bit23               (ULONG)0x00800000
    #define bit24               (ULONG)0x01000000
    #define bit25               (ULONG)0x02000000
    #define bit26               (ULONG)0x04000000
    #define bit27               (ULONG)0x08000000
    #define bit28               (ULONG)0x10000000
    #define bit29               (ULONG)0x20000000
    #define bit30               (ULONG)0x40000000
    #define bit31               (ULONG)0x80000000

#endif // _ZTYPES_H

/* eof */


