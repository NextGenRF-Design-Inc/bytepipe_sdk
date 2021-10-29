#ifndef SRC_PHY_H_
#define SRC_PHY_H_

#include "parameters.h"
#include "xstatus.h"
#include "adrv9001.h"


#define PHY_STATUS_OFFSET           (-2000)


/**
**  ADRV9001 Status
*/
typedef enum
{
  PhyStatus_Success             = (0),
  PhyStatus_InvalidPort         = (PHY_STATUS_OFFSET - 1),
  PhyStatus_MemoryError         = (PHY_STATUS_OFFSET - 2),
  PhyStatus_NotSupported        = (PHY_STATUS_OFFSET - 3),
  PhyStatus_OsError             = (PHY_STATUS_OFFSET - 4),
  PhyStatus_Busy                = (PHY_STATUS_OFFSET - 5),
  PhyStatus_InvalidParameter    = (PHY_STATUS_OFFSET - 6),
  PhyStatus_Adrv9001Error       = (PHY_STATUS_OFFSET - 7),
} phy_status_t;

/**
 **  PHY Event Type
 */
typedef enum
{
  PhyEvtType_StreamStart      = 0,      ///< Indicates Stream has started. EvtData = phy_stream_t
  PhyEvtType_StreamStop       = 1,      ///< Indicates Stream has started. EvtData = phy_stream_t
  PhyEvtType_StreamError      = 2,      ///< Indicates Stream has an error and has aborted. EvtData = phy_stream_t
  PhyEvtType_StreamData       = 3,      ///< Indicates stream is ready for more data or has more data to deliver. EvtData = phy_stream_t
  PhyEvtType_ProfileUpdated   = 4,      ///< Indicates profile has been updated. No Evt data
} phy_evt_type_t;

/**
** PHY Callback
*/
typedef void (*phy_callback_t)( phy_evt_type_t EvtType, void *EvtData, void *param );

/**
**  PHY Stream
*/
typedef struct{
  uint32_t         *SampleBuf;        ///< Sample Buffer
  uint32_t          SampleCnt;        ///< Length in samples
  adrv9001_port_t   Port;             ///< Port
  phy_callback_t    Callback;         ///< Callback
  void             *CallbackRef;      ///< User Data
  bool              Cyclic;           ///< Flag indicates the stream will continue Indefinitely
}phy_stream_t;

/**
** PHY Profile
*/
typedef uint8_t profile_t[ADRV9001_PROFILE_SIZE];

/*******************************************************************************
*
* \details
*
* This function enables continuous streaming of IQ data.
*
* \param[in]  Stream is the stream configuration.  This parameter is provided
*             by the caller and can be released once the function is returned.
*             However some of the parameters making up this variable must be
*             maintained in memory for parts or the duration the stream.
*
*             All streams are non-blocking.  The caller will receive event
*             callbacks indicating when the stream starts, stops, there is an
*             error, or it is ready for more data.  The callback function is
*             unique to each stream.  The caller also has the option of
*             providing a CallbackRef which points to user specific data.
*
*            -Transmit Stream
*             When streaming to a transmit port the SampleBuf must be maintained
*             until the Stream Start event is triggered and the caller receives
*             a callback with PhyEvtType_StreamStart.  At this point the caller
*             can free or re-purpose memory associated with SampleBuf.  This
*             applies to all modes including cyclic mode.  If the cyclic flag
*             is set the SampleBuf will be streamed continuously until the caller
*             executes Phy_IqStreamDisable.  If the cyclic flag is not set the
*             stream will stop once a number of SampleCnt samples have been
*             streamed.  Once the stream is done a callback of
*             PhyEvtType_StreamStop will be executed informing the caller.
*
*            -Receive Stream
*             When streaming to a receive port the caller will receive a
*             callback of PhyEvtType_StreamStart once streaming has started.
*             Once SampleCnt number of samples have been streamed to memory
*             the user will receive a callback .
*
*
* \return     Status
*
*******************************************************************************/
phy_status_t Phy_IqStreamEnable( phy_stream_t *Stream );

/*******************************************************************************
*
* \details
*
* This function enables continuous streaming of IQ data.
*
* \param[in]  Stream is the stream configuration
*
* \return     Status
*
*******************************************************************************/
phy_status_t Phy_IqStreamDisable( adrv9001_port_t Port );

/*******************************************************************************
*
* \details
*
* This function loads a new ADRV9001 profile from the file system
*
* \param[in]  Buf is a buffer containing the profile
*
* \param[in]  Length is the length of the buffer in bytes
*
* \return     Status
*
*******************************************************************************/
phy_status_t Phy_LoadProfile( profile_t *Profile );

/*******************************************************************************
*
* \details
* This function initializes the PHY layer which includes the ADRV9002
*
* \return   int32_t - status
*
*******************************************************************************/
int32_t Phy_Initialize( void );



#endif /* SRC_MAC_H_ */
