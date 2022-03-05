#ifndef SRC_PHY_PROFILE_H_
#define SRC_PHY_PROFILE_H_
/***************************************************************************//**
*  \ingroup    APP
*  \defgroup   PHY RFLAN PHY Layer
*  @{
*******************************************************************************/
/***************************************************************************//**
*  \file       phy_profile.h
*
*  \details
*
*  This file contains the RFLAN PHY layer profile definitions.
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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "phy_types.h"
#include "jsmn.h"
#include "adi_adrv9001_profileutil.h"
#include "adrv9001_Init_t_parser.h"

#define PHY_ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

#define PHY_IF_JSON_EQ(jsonBuffer, jsmnTok, token) \
if (jsmnTok.type == JSMN_STRING && (int)strlen(token) == jsmnTok.end - jsmnTok.start && \
    strncmp(jsonBuffer + jsmnTok.start, token, jsmnTok.end - jsmnTok.start) == 0)

#define PHY_COPY_TOKEN_TO_BUFFER(tokenArray, tokenIndex, jsonBuffer, copyBuffer) \
snprintf(copyBuffer, \
    PHY_ARRAY_LEN(copyBuffer), \
    "%.*s", tokenArray[tokenIndex + 1].end - tokenArray[tokenIndex + 1].start, \
    jsonBuffer + tokenArray[tokenIndex + 1].start);

#define PHY_STORE_CHAR(tokenArray, tokenIndex, jsonBuffer, charParsingBuffer, charDestination) \
PHY_COPY_TOKEN_TO_BUFFER(tokenArray, tokenIndex, jsonBuffer, charParsingBuffer) \
charDestination = (uint8_t)atoi(charParsingBuffer);

#define PHY_STORE_INT(tokenArray, tokenIndex, jsonBuffer, intParsingBuffer, intDestination) \
PHY_COPY_TOKEN_TO_BUFFER(tokenArray, tokenIndex, jsonBuffer, intParsingBuffer) \
intDestination = atoi(intParsingBuffer);

#define PHY_STORE_LONG(tokenArray, tokenIndex, jsonBuffer, longParsingBuffer, longDestination) \
PHY_COPY_TOKEN_TO_BUFFER(tokenArray, tokenIndex, jsonBuffer, longParsingBuffer) \
longDestination = atoll(longParsingBuffer);

#define PHY_STORE_BOOL(tokenArray, tokenIndex, jsonBuffer, boolParsingBuffer, boolDestination) \
PHY_COPY_TOKEN_TO_BUFFER(tokenArray, tokenIndex, jsonBuffer, boolParsingBuffer) \
boolDestination = ('0' != boolParsingBuffer[0]) && ('f' != boolParsingBuffer[0]) && ('F' != boolParsingBuffer[0]);

#define PHY_STORE_STR(tokenArray, tokenIndex, jsonBuffer, dummyBuffer, strDestination) \
PHY_COPY_TOKEN_TO_BUFFER(tokenArray, tokenIndex, jsonBuffer, strDestination);

#define PHY_PROCESS_X(X, tokenArray, tokenIndex, jsonBuffer, parsingBuffer, varDestination, varName) \
PHY_IF_JSON_EQ(jsonBuffer, tokenArray[tokenIndex], varName) { \
    X(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, varDestination); \
    tokenIndex++; \
    continue; \
}

#define PHY_PROCESS_INT(tokenArray, tokenIndex, jsonBuffer, intParsingBuffer, intDestination, intName) \
PHY_PROCESS_X(PHY_STORE_INT, tokenArray, tokenIndex, jsonBuffer, intParsingBuffer, intDestination, intName)

#define PHY_PROCESS_LONG(tokenArray, tokenIndex, jsonBuffer, longParsingBuffer, longDestination, longName) \
PHY_PROCESS_X(PHY_STORE_LONG, tokenArray, tokenIndex, jsonBuffer, longParsingBuffer, longDestination, longName)

#define PHY_PROCESS_BOOL(tokenArray, tokenIndex, jsonBuffer, boolParsingBuffer, boolDestination, boolName) \
PHY_PROCESS_X(PHY_STORE_BOOL, tokenArray, tokenIndex, jsonBuffer, boolParsingBuffer, boolDestination, boolName)

#define PHY_PROCESS_STR(tokenArray, tokenIndex, jsonBuffer, dummyBuffer, strDestination, strName) \
PHY_PROCESS_X(PHY_STORE_STR, tokenArray, tokenIndex, jsonBuffer, dummyBuffer, strDestination, strName)

#define PHY_PROCESS_ARRAY_X(X, tokenArray, tokenIndex, jsonBuffer, parsingBuffer, arrayName) \
PHY_IF_JSON_EQ(jsonBuffer, tokenArray[tokenIndex], arrayName) { \
    int16_t size = tokenArray[++tokenIndex].size; \
    int16_t k = 0; \
    for (k = 0; k < size; k++) { \
        X; \
        tokenIndex++; \
    } \
    continue; \
}

#define ARRAY_STRUCT_INDEXER(structName) structName##_k


#define PHY_PROCESS_ARRAY_INT(tokenArray, tokenIndex, jsonBuffer, intParsingBuffer, intArrayDestination, intArrayName) \
PHY_PROCESS_ARRAY_X(PHY_STORE_INT(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, intArrayDestination[k]), tokenArray, tokenIndex, jsonBuffer, intParsingBuffer, intArrayName)

#define PHY_PROCESS_ARRAY_LONG(tokenArray, tokenIndex, jsonBuffer, longParsingBuffer, longArrayDestination, longArrayName) \
PHY_PROCESS_ARRAY_X(PHY_STORE_LONG(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, longArrayDestination[k]), tokenArray, tokenIndex, jsonBuffer, longParsingBuffer, longArrayName)

#define PHY_PROCESS_STRUCT_X(X, tokenArray, tokenIndex, jsonBuffer, parsingBuffer, structName) \
PHY_IF_JSON_EQ(jsonBuffer, tokenArray[tokenIndex], structName) { \
    int16_t size = tokenArray[++tokenIndex].size; \
    int16_t end = tokenArray[tokenIndex].end; \
    { \
        int16_t j = 0; \
        for (j = 0; j < size; j++) { \
            tokenIndex++; \
            \
            X; \
        } \
    } \
    while (tokenArray[tokenIndex + 1].start <= end) { \
        tokenIndex++; \
    } \
    continue; \
}

#define PHY_PROCESS_ARRAY_STRUCT_X(X, tokenArray, tokenIndex, jsonBuffer, parsingBuffer, structName, indexName) \
PHY_IF_JSON_EQ(jsonBuffer, tokenArray[tokenIndex], structName) { \
    int16_t size = tokenArray[++tokenIndex].size; \
    int16_t indexName = 0; \
    for (indexName = 0; indexName < size; indexName++) { \
        int16_t end, size = tokenArray[++tokenIndex].size; \
        end = tokenArray[tokenIndex].end; \
        { \
            int16_t j = 0; \
            for (j = 0; j < size; j++) { \
                tokenIndex++; \
                X; \
            } \
        } \
        while (tokenArray[tokenIndex + 1].start <= end) { \
            tokenIndex++; \
        } \
    } \
    continue; \
}

/* ---- PHY_RX_PARAMS_T ---- */
#define PHY_RX_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_rx_params_tInstance) \
PHY_PROCESS_LONG     (tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_rx_params_tInstance.Gain,                                                  "Gain"); \

#define PHY_PROCESS_STRUCT_RX_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_rx_params_tInstance, phy_rx_params_tName) \
PHY_PROCESS_STRUCT_X(PHY_RX_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_rx_params_tInstance), tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_rx_params_tName);

#define PHY_PROCESS_ARRAY_RX_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_rx_params_tInstance, phy_rx_params_tName) \
PHY_PROCESS_ARRAY_STRUCT_X(PHY_RX_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_rx_params_tInstance[ARRAY_STRUCT_INDEXER(phy_rx_params_t)]), \
                           tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_rx_params_tName, ARRAY_STRUCT_INDEXER(phy_rx_params_t));


/* ---- PHY_TX_PARAMS_T ---- */
#define PHY_TX_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_tx_params_tInstance) \
PHY_PROCESS_INT      (tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_tx_params_tInstance.Attn,                                                  "Attn"); \
PHY_PROCESS_INT      (tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_tx_params_tInstance.Boost,                                                 "Boost"); \

#define PHY_PROCESS_STRUCT_TX_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_tx_params_tInstance, phy_tx_params_tName) \
PHY_PROCESS_STRUCT_X(PHY_TX_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_tx_params_tInstance), tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_tx_params_tName);

#define PHY_PROCESS_ARRAY_TX_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_tx_params_tInstance, phy_tx_params_tName) \
PHY_PROCESS_ARRAY_STRUCT_X(PHY_TX_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_tx_params_tInstance[ARRAY_STRUCT_INDEXER(phy_tx_params_t)]), \
                           tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_tx_params_tName, ARRAY_STRUCT_INDEXER(phy_tx_params_t));

/* ---- PHY_LO_PARAMS_T ---- */
#define PHY_LO_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_lo_params_tInstance) \
PHY_PROCESS_LONG     (tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_lo_params_tInstance.Frequency,                                             "Frequency"); \

#define PHY_PROCESS_STRUCT_LO_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_lo_params_tInstance, phy_lo_params_tName) \
PHY_PROCESS_STRUCT_X(PHY_LO_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_lo_params_tInstance), tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_lo_params_tName);

#define PHY_PROCESS_ARRAY_LO_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_lo_params_tInstance, phy_lo_params_tName) \
PHY_PROCESS_ARRAY_STRUCT_X(PHY_LO_PARAMS_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_lo_params_tInstance[ARRAY_STRUCT_INDEXER(phy_lo_params_t)]), \
                           tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_lo_params_tName, ARRAY_STRUCT_INDEXER(phy_lo_params_t));


/* ---- PHY_PROFILE_T ---- */
#define PHY_PROFILE_T(tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_profile_tInstance) \
PHY_PROCESS_ARRAY_LO_PARAMS_T                         (tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_profile_tInstance->Lo,                      "Lo"); \
PHY_PROCESS_ARRAY_TX_PARAMS_T                         (tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_profile_tInstance->Tx,                      "Tx"); \
PHY_PROCESS_ARRAY_RX_PARAMS_T                         (tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_profile_tInstance->Rx,                      "Rx"); \
ADI_PROCESS_STRUCT_ADRV9001_INIT_T                    (tokenArray, tokenIndex, jsonBuffer, parsingBuffer, phy_profile_tInstance->AdrvInit,                "AdrvInit"); \


phy_status_t PhyProfile_Initialize( phy_t *Instance, phy_profile_t *Profile );
phy_status_t PhyProfile_Load      ( phy_t *Instance, uint32_t Index );

#endif /* SRC_PHY_PROFILE_H_ */
