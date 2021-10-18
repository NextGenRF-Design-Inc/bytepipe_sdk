#ifndef SRC_PHY_H_
#define SRC_PHY_H_

#include "parameters.h"
#include "xstatus.h"
#include "adrv9001.h"


/**
 **  PHY Event Data
 */
typedef union
{
} phy_evt_data_t;

/**
 **  PHY Event Type
 */
typedef enum
{
  PhyEvtType_Stream        = 0,
} phy_evt_type_t;

/**
 **  ADRV9001 Event
 */
typedef struct
{
  phy_evt_type_t       Type;
  phy_evt_data_t      *Data;
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
* This function enables continuous streaming of IQ data to or from a file.
*
* To disable the stream for a particular port call this function the filename
* set to NULL.
*
* \param[in]  Port is the port being requested
*
* \param[in]  filename is the name of the file
*
* \return     Status
*
*******************************************************************************/
int32_t Phy_IqStream( adrv9001_port_t Port, const char *filename );

/*******************************************************************************
*
* \details
*
* This function loads a new ADRV9001 profile from the file system
*
* \param[in]  Port is the port being requested
*
* \param[in]  State is the radio state set for the requested port
*
* \return     Status
*
*******************************************************************************/
int32_t Phy_LoadProfile( uint8_t *Buf );

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
