#ifndef SRC_ADRV9001_H_
#define SRC_ADRV9001_H_


#include <stdint.h>
#include <stdbool.h>



#define ADRV9001_STATUS_OFFSET      (-1000)
#define ADRV9001_PROFILE_SIZE       (0x80000)

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
  Adrv9001Status_NotSupported                 = (ADRV9001_STATUS_OFFSET - 12),
  Adrv9001Status_DriverError                  = (ADRV9001_STATUS_OFFSET - 13),

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
 **  ADRV9001 IQ Data Sample
 */
typedef struct
{
  uint16_t    idata;
  uint16_t    qdata;
} adrv9001_iqdata_t;

/**
 **  ADRV9001 Stream Event
 */
typedef struct
{
  adrv9001_port_t     Port;
} adrv9001_evt_stream_t;

/**
 **  ADRV9001 Event Data
 */
typedef union
{
  adrv9001_evt_stream_t     Stream;
} adrv9001_evt_data_t;

/**
 **  ADRV9001 Event Type
 */
typedef enum
{
  Adrv9001EvtType_Stream        = 0,
} adrv9001_evt_type_t;

/**
 **  ADRV9001 Event
 */
typedef struct
{
  adrv9001_evt_type_t       Type;
  adrv9001_evt_data_t      *Data;
} adrv9001_evt_t;

/**
** ADRV9001 Callback
*/
typedef void (*adrv9001_callback_t)( adrv9001_evt_t evt, void *param );

/**
 **  ADRV9001 Configuration
 */
typedef struct
{
  adrv9001_callback_t    Callback;     ///< Callback
  void                  *CallbackRef;  ///< Callback reference data
} adrv9001_cfg_t;

/*******************************************************************************
*
* \details
*
* This function sets the transmit port power boost setting
*
* \param[in]  Port is the port being requested
*
* \param[in]  Enable indicates if power boost is enabled
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_SetTxBoost( adrv9001_port_t Port, bool Enable );

/*******************************************************************************
*
* \details
*
* This function gets the transmit port power boost setting
*
* \param[in]  Port is the port being requested
*
* \param[in]  Enable indicates if power boost is enabled
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_GetTxBoost( adrv9001_port_t Port, bool *Enable );

/*******************************************************************************
*
* \details
*
* This function sets the transmit port attenuation
*
* \param[in]  Port is the port being requested
*
* \param[in]  Attn_mdB is the attenuation in milli dB, ie 10000 = 10.000dB
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_SetTxAttenuation( adrv9001_port_t Port, uint16_t Attn_mdB );

/*******************************************************************************
*
* \details
*
* This function gets the transmit port attenuation
*
* \param[in]  Port is the port being requested
*
* \param[in]  Attn_mdB is the attenuation in milli dB, ie 10000 = 10.000dB
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_GetTxAttenuation( adrv9001_port_t Port, uint16_t *Attn_mdB );

/*******************************************************************************
*
* \details
*
* This function gets the sample rate for the requested port.
*
* \param[in]  Port is the port being requested
*
* \param[in]  FreqHz is the sample frequency in Hz
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_GetSampleRate( adrv9001_port_t Port, uint32_t *FreqHz );

/*******************************************************************************
*
* \details
*
* This function gets the carrier frequency for the requested port.
*
* \param[in]  Port is the port being requested
*
* \param[in]  FreqHz is the carrier frequency in Hz
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_GetCarrierFrequency( adrv9001_port_t Port, uint64_t *FreqHz );

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
* This function will transition the specified port to the PRIMED state from any
* state where it is valid to do so.
*
* \param[in]  Port is the port being requested
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_ToRfPrimed( adrv9001_port_t Port );

/*******************************************************************************
*
* \details
*
* This function will transition the specified port to the Enabled state from any
* state where it is valid to do so.
*
* \param[in]  Port is the port being requested
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_ToRfEnabled( adrv9001_port_t Port );

/*******************************************************************************
*
* \details
*
* This function will transition the specified port to the Calibrated state from
* any state where it is valid to do so.
*
* \param[in]  Port is the port being requested
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_ToRfCalibrated( adrv9001_port_t Port );

/*******************************************************************************
*
* \details
*
* This function enables continuous streaming of IQ data.  This function enables
* the appropriate radio state and sets up the appropriate DMA to cyclically
* transfer IQ samples from memory to the SSI or vice versa.
*
* When selecting a transmit port the IQ data is copied to an internal buffer
* and continuously streamed to the SSI looping back to the first IQ sample
* after a SampleCnt number of samples are reached.  Once this function has been
* called the user can reuse or free the memory location of Buf.
*
* When selecting a receive port the DMA continuously streams IQ data to an
* internal buffer.  Once a SampleCnt number of samples have been received the
* memory is copied to the user provided buffer and a callback is generated
* indicating to the user the buffer is full.  The DMA will continue to stream
* samples to the internal 1M sample buffer and generate callbacks as a
* SampleCnt number of samples are received.  This allows for some throttling
* before samples are dropped.  If the user callback is not executed immediately
* samples will not be dropped until the 1M buffer overflows.  The user must
* provide Buf with a size of SampleCnt as long as the stream is enabled.
*
* To disable the stream for a particular port call this function with SampleCnt
* set to zero.
*
* \param[in]  Port is the port being requested
*
* \param[in]  Buf is a buffer of 32bit IQ data
*
* \param[in]  SampleCnt is the number of samples and the length of Buf
*
* \return     Status
*
*******************************************************************************/
adrv9001_status_t Adrv9001_IQStream( adrv9001_port_t Port, adrv9001_iqdata_t *Buf, uint32_t SampleCnt );

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
adrv9001_status_t Adrv9001_Initialize( adrv9001_cfg_t *Cfg );

#endif
