#ifndef SRC_ADRV9001_H_
#define SRC_ADRV9001_H_


#include <stdint.h>
#include <stdbool.h>


#define ADRV9001_STATUS_OFFSET      (-1000)

/**
**  ADRV9001 Status
*/
typedef enum
{
  Adrv9001Status_Success                      = (0),
  Adrv9001Status_InvalidPort                  = (ADRV9001_STATUS_OFFSET - 1),
  Adrv9001Status_InvalidChannel               = (ADRV9001_STATUS_OFFSET - 2),
  Adrv9001Status_Initialized                  = (ADRV9001_STATUS_OFFSET - 3),
  Adrv9001Status_MemoryError                  = (ADRV9001_STATUS_OFFSET - 3),
  Adrv9001Status_InvalidSSI                   = (ADRV9001_STATUS_OFFSET - 4),
  Adrv9001Status_InvalidGpio                  = (ADRV9001_STATUS_OFFSET - 5),
  Adrv9001Status_InvalidRadioState            = (ADRV9001_STATUS_OFFSET - 6),
  Adrv9001Status_CommError                    = (ADRV9001_STATUS_OFFSET - 7),
  Adrv9001Status_ProfileInitError             = (ADRV9001_STATUS_OFFSET - 8),
  Adrv9001Status_ProfileCfgError              = (ADRV9001_STATUS_OFFSET - 9),
  Adrv9001Status_ProfileCalError              = (ADRV9001_STATUS_OFFSET - 10),
  Adrv9001Status_ProfilePrimeError            = (ADRV9001_STATUS_OFFSET - 11),
} adrv9001_status_t;

/**
 **  ADRV9001 Port
 */
typedef enum
{
  Adrv9001Port_Rx1    = (0),
  Adrv9001Port_Rx2    = (1),
  Adrv9001Port_Tx1    = (2),
  Adrv9001Port_Tx2    = (3)
} adrv9001_port_t;

/**
 **  ADRV9001 Radio State
 */
typedef enum
{
  Adrv9001RadioState_Standby          = 0,
  Adrv9001RadioState_Calibrated       = 1,
  Adrv9001RadioState_Primed           = 2,
  Adrv9001RadioState_Enabled          = 3,
} adrv9001_radio_state_t;

/**
 **  ADRV9001 Configuration
 */
typedef struct
{


} adrv9001_Cfg_t;

/*******************************************************************************
*
* \details
*
* This function starts receiving based on the TES profile.
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_BeginReceiving( void );

/*******************************************************************************
*
* \details
*
* This function starts transmitting based on the TES profile.
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_BeginTransmitting( void );


/*******************************************************************************
*
* \details
*
* This function enables or disables the internal loop back within the ADRV9001
* based on the requested port.  The requested port results in the following loop
* back path within the ADRV9001.
*
*             |       Port        | Loop back Path |
*             |-------------------|----------------|
*             | Adrv9001Port_Rx1  |   RX1 -> TX1   |
*             | Adrv9001Port_Rx2  |   RX2 -> TX2   |
*             | Adrv9001Port_Tx1  |   TX1 -> RX1   |
*             | Adrv9001Port_Tx2  |   TX2 -> RX2   |
*
* \param[in]  Port is the port being requested
*
* \param[in]  Enabled indicates whether the loop back should be enabled or
*             disabled.
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_SetInternalLoopBack( adrv9001_port_t Port, bool Enabled );

/*******************************************************************************
*
* \details
*
* This function sets the state of the radio for the requested port.
*
* \param[in]  Port is the port being requested
*
* \param[in]  State is the radio state set for the requested port
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_SetRadioState( adrv9001_port_t Port, adrv9001_radio_state_t State );

/*******************************************************************************
*
* \details
*
* This function returns the state of the radio for the requested port.
*
* \param[in]  Port is the port being requested
*
* \param[out] State is the returned State for the requested port
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_GetRadioState( adrv9001_port_t Port, adrv9001_radio_state_t *State );

/*******************************************************************************
*
* \details
*
* This function clears any errors within the adi_adrv9001 device driver.
*
* \return     none
*
*******************************************************************************/
adrv9001_status_t Adrv9001_ClearError( void );

/*******************************************************************************
*
* \details
*
* This function initializes the ADRV9001 with the provided profile..
*
* \param[in]  Profile contains a reference to the profile to be loaded
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_LoadProfile( void );

/*******************************************************************************
*
* \details
*
* This function performs a hard reset of the ADRV9001.
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_HwReset( void );

/*******************************************************************************
*
* \details
*
* This function initializes the ADRV9001 using the adi_adrv9001 device driver
* along with initializing the HDL associated with the ADRV9001.
*
* \param[in]  Cfg contains configuration data for initializing the ADRV9001
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_Initialize( adrv9001_Cfg_t *Cfg );

#endif
