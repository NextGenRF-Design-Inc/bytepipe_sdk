#ifndef SRC_PHY_H_
#define SRC_PHY_H_

#include "parameters.h"
#include "xstatus.h"
#include "adrv9001.h"


#define PHY_STATUS_OFFSET           (-2000)
#define PHY_IS_PORT_TX(p)           (( p == Adrv9001Port_Tx1 ) || ( p == Adrv9001Port_Tx2 )) ? true : false
#define PHY_IS_PORT_RX(p)           (( p == Adrv9001Port_Rx1 ) || ( p == Adrv9001Port_Rx2 )) ? true : false
#define PHY_CHANNEL(p)              (( p == Adrv9001Port_Tx1 ) || ( p == Adrv9001Port_Rx1 )) ? 0 : 1

/**
**  ADRV9001 Status
*/
typedef enum
{
  PhyStatus_Success                     = (0),
  PhyStatus_InvalidPort                 = (PHY_STATUS_OFFSET - 1),
  PhyStatus_MemoryError                 = (PHY_STATUS_OFFSET - 2),
  PhyStatus_NotSupported                = (PHY_STATUS_OFFSET - 3),
  PhyStatus_FileError                   = (PHY_STATUS_OFFSET - 4),
  PhyStatus_OsError                     = (PHY_STATUS_OFFSET - 5),
  PhyStatus_InvalidParameter            = (PHY_STATUS_OFFSET - 6),
} phy_status_t;

/**
**  PHY Stream
*/
typedef struct{
  uint32_t       *Buf;
  uint32_t        Length;
  adrv9001_port_t Port;
}phy_stream_t;

/**
 **  PHY Event Data
 */
typedef union
{
  phy_stream_t     *Stream;
} phy_evt_data_t;

/**
 **  PHY Event Type
 */
typedef enum
{
  PhyEvtType_StreamStart      = 0,
  PhyEvtType_StreamStop       = 1,
  PhyEvtType_StreamError      = 2,
} phy_evt_type_t;

/**
 **  ADRV9001 Event
 */
typedef struct
{
  phy_evt_type_t       Type;
  phy_evt_data_t       Data;
} phy_evt_t;

/**
** PHY Callback
*/
typedef void (*phy_callback_t)( phy_evt_t evt, void *param );

/**
**  PHY Layer Configuration
*/
typedef struct
{
  phy_callback_t     Callback;     ///< Callback
  void              *CallbackRef;  ///< Callback reference data
} phy_cfg_t;

/**
**  PHY Queue
*/
typedef struct
{
  phy_evt_t     Evt;
} phy_queue_t;

/*******************************************************************************
*
* \details
*
* This function enables continuous streaming of IQ data
*
* To disable the stream for a particular port call this function the filename
* set to NULL.
*
* \param[in]  Port is the port being requested
*
* \param[in]  Buf is the IQ sample buffer
*
* \param[in]  Length is the IQ length of samples to stream
*
* \return     Status
*
*******************************************************************************/
int32_t Phy_IqStream( adrv9001_port_t Port, adrv9001_iqdata_t *Buf, uint32_t Length );

/*******************************************************************************
*
* \details
*
* This function loads a new ADRV9001 profile from the file system
*
* \param[in]  filename is the name of the file containing the profile
*
* \return     Status
*
*******************************************************************************/
int32_t Phy_LoadProfile( const char *filename );

/*******************************************************************************
*
* \details
* This function initializes the PHY layer which includes the ADRV9002
*
* \param[in]  Cfg contains configuration data for initializing this PHY instance
*
* \return   int32_t - status
*
*******************************************************************************/
int32_t Phy_Initialize( phy_cfg_t *Cfg );



#endif /* SRC_MAC_H_ */
