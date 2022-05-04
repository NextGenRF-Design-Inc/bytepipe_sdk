
#include <stdio.h>
#include <string.h>
#include "phy_adrv9001.h"
#include "phy.h"
#include "parameters.h"
#include "axi_io.h"
#include "ff.h"
#include "sleep.h"
#include "phy_stream.h"

#include "adrv9001.h"
#include "adi_adrv9001_radio.h"
#include "adi_adrv9001_profileutil.h"
#include "adi_adrv9001_gpio.h"
#include "adi_adrv9001.h"
#include "adi_adrv9001_tx.h"
#include "adi_adrv9001_rx.h"
#include "adi_adrv9001_arm.h"
#include "adi_adrv9001_auxdac.h"
#include "adi_adrv9001_ssi.h"

#define ADI_CHANNEL_RX1_INIT_MASK               (0x40)
#define ADI_CHANNEL_RX2_INIT_MASK               (0x80)
#define ADI_CHANNEL_TX1_INIT_MASK               (0x04)
#define ADI_CHANNEL_TX2_INIT_MASK               (0x08)



static adi_common_Port_e PhyAdrv9001_GetAdiPort( phy_port_t port )
{
  if((port == PhyPort_Rx1) || (port == PhyPort_Rx2))
    return ADI_RX;
  else
    return ADI_TX;
}

static adi_common_ChannelNumber_e PhyAdrv9001_GetAdiChannel( phy_port_t port )
{
  if((port == PhyPort_Rx1) || (port == PhyPort_Tx1))
    return ADI_CHANNEL_1;
  else
    return ADI_CHANNEL_2;
}

static phy_status_t PhyAdrv9001_IsPortEnabled( phy_t *Instance, phy_port_t Port )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  adi_common_Port_e AdiPort = PhyAdrv9001_GetAdiPort(Port);
  adi_common_ChannelNumber_e AdiChannel = PhyAdrv9001_GetAdiChannel(Port);

  if( AdiPort == ADI_RX )
  {
    if( (AdiChannel == ADI_CHANNEL_1 ) && ((Instance->Profile->AdrvInit.rx.rxInitChannelMask & ADI_CHANNEL_RX1_INIT_MASK) > 0))
    {
      return PhyStatus_Success;
    }
    else if( (AdiChannel == ADI_CHANNEL_2 ) && ((Instance->Profile->AdrvInit.rx.rxInitChannelMask & ADI_CHANNEL_RX2_INIT_MASK) > 0))
    {
      return PhyStatus_Success;
    }
  }
  else if( AdiPort == ADI_TX )
  {
    if( (AdiChannel == ADI_CHANNEL_1 ) && ((Instance->Profile->AdrvInit.tx.txInitChannelMask & ADI_CHANNEL_TX1_INIT_MASK) > 0))
    {
      return PhyStatus_Success;
    }
    else if( (AdiChannel == ADI_CHANNEL_2 ) && ((Instance->Profile->AdrvInit.tx.txInitChannelMask & ADI_CHANNEL_TX2_INIT_MASK) > 0))
    {
      return PhyStatus_Success;
    }
  }

  return PhyStatus_InvalidPort;
}

phy_status_t PhyAdrv9001_GetSsiDelay( phy_t *Instance, phy_port_t port, uint8_t *clkDelay, uint8_t *dataDelay )
{
  adi_adrv9001_SsiType_e SsiType = ADI_ADRV9001_SSI_TYPE_LVDS;
  adi_adrv9001_SsiCalibrationCfg_t cal;

  int32_t status = adi_adrv9001_Ssi_Delay_Inspect(Instance->Adrv9001, SsiType, &cal);

  if(port == PhyPort_Rx1)
  {
    *clkDelay   = cal.rxClkDelay[0];
    *dataDelay  = cal.rxIDataDelay[0];
  }
  else if( port == PhyPort_Rx2 )
  {
    *clkDelay   = cal.rxClkDelay[1];
    *dataDelay  = cal.rxIDataDelay[1];
  }
  else if( port == PhyPort_Tx1 )
  {
    *clkDelay   = cal.txClkDelay[0];
    *dataDelay  = cal.txIDataDelay[0];
  }
  else if( port == PhyPort_Tx2)
  {
    *clkDelay   = cal.txClkDelay[1];
    *dataDelay  = cal.txIDataDelay[1];
  }
  else
  {
    status = PhyStatus_InvalidPort;
  }

  return status;
}

phy_status_t PhyAdrv9001_SetSsiDelay( phy_t *Instance, phy_port_t port, uint8_t clkDelay, uint8_t dataDelay )
{
  adi_adrv9001_SsiType_e SsiType = ADI_ADRV9001_SSI_TYPE_LVDS;
  adi_adrv9001_SsiCalibrationCfg_t cal;

  int32_t status;

  if((status = adi_adrv9001_Ssi_Delay_Inspect(Instance->Adrv9001, SsiType, &cal)) == 0)
  {
    if(port == PhyPort_Rx1)
    {
      cal.rxClkDelay[0]     = clkDelay;
      cal.rxStrobeDelay[0]  = dataDelay;
      cal.rxIDataDelay[0]   = dataDelay;
      cal.rxQDataDelay[0]   = dataDelay;
    }
    else if(port == PhyPort_Rx2)
    {
      cal.rxClkDelay[1]     = clkDelay;
      cal.rxStrobeDelay[1]  = dataDelay;
      cal.rxIDataDelay[1]   = dataDelay;
      cal.rxQDataDelay[1]   = dataDelay;
    }
    else if(port == PhyPort_Tx1)
    {
      cal.txClkDelay[0]     = clkDelay;
      cal.txStrobeDelay[0]  = dataDelay;
      cal.txIDataDelay[0]   = dataDelay;
      cal.txQDataDelay[0]   = dataDelay;
    }
    else if(port == PhyPort_Tx2)
    {
      cal.txClkDelay[1]     = clkDelay;
      cal.txStrobeDelay[1]  = dataDelay;
      cal.txIDataDelay[1]   = dataDelay;
      cal.txQDataDelay[1]   = dataDelay;
    }
    else
    {
      return PhyStatus_InvalidPort;
    }

    status = adi_adrv9001_Ssi_Delay_Configure(Instance->Adrv9001, SsiType, &cal);
  }

  return status;
}

phy_status_t PhyAdrv9001_CalibrateSsiDelay( phy_t *Instance, phy_port_t Port )
{
  int32_t status;
  uint8_t results[8][8];
  uint8_t max = 0;
  uint8_t pmax = 0;
  uint8_t clkMax = 0;
  uint8_t dataMax = 0;

  if((status = PhyAdrv9001_PerformSsiSweep( Instance, Port, results )) != 0)
    return status;

  for(uint8_t clkDelay = 0; clkDelay < 8; clkDelay++)
  {
    for(uint8_t dataDelay = 0; dataDelay < 8; dataDelay++)
    {
      max = 0;

      if( results[clkDelay][dataDelay] == 1)
      {
        max += 1;

        if( clkDelay > 0 )
        {
          max += results[clkDelay - 1][dataDelay];

          if( dataDelay > 0 )
          {
            max += results[clkDelay - 1][dataDelay - 1];
          }

          if( dataDelay < 7 )
          {
            max += results[clkDelay - 1][dataDelay + 1];
          }
        }

        if( clkDelay < 7 )
        {
          max += results[clkDelay + 1][dataDelay];

          if( dataDelay > 0 )
          {
            max += results[clkDelay + 1][dataDelay - 1];
          }

          if( dataDelay < 7 )
          {
            max += results[clkDelay + 1][dataDelay + 1];
          }
        }

        if( dataDelay > 0 )
        {
          max += results[clkDelay][dataDelay - 1];
        }

        if( dataDelay < 7 )
        {
          max += results[clkDelay][dataDelay + 1];
        }
      }

      if( max > pmax )
      {
        pmax = max;
        clkMax = clkDelay;
        dataMax = dataDelay;
      }
    }
  }

  if( pmax == 0 )
    return PhyStatus_SsiFailure;

  return PhyAdrv9001_SetSsiDelay( Instance, Port, clkMax, dataMax );
}

phy_status_t PhyAdrv9001_InspectTestMode( phy_t *Instance, phy_port_t Port )
{
  int32_t status;
  uint32_t Buf[1] = {ADRV9001_TEST_MODE_PATTERN};

  /* Get ADI Channel Number */
  adi_common_ChannelNumber_e AdiChannel = PhyAdrv9001_GetAdiChannel( Port );

  /* Enable Continuous DMA Transfer */
  phy_stream_req_t StreamReq = {
      .Callback = NULL,
      .CallbackRef = NULL,
      .Cyclic = true,
      .Blocking = false,
      .Port = Port,
      .SampleBuf = Buf,
      .SampleCnt = 1
  };

  if(PHY_IS_PORT_TX( Port ))
  {
    PhyStream_Enable( Instance, &StreamReq );

    /* Wait */
    usleep( 100 );

    /* Read Test Mode Status */
    adi_adrv9001_TxSsiTestModeCfg_t Cfg =
    {
        .fixedDataPatternToCheck = ADRV9001_TEST_MODE_PATTERN,
        .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_FIXED_PATTERN
    };

    adi_adrv9001_TxSsiTestModeStatus_t s;
    status = adi_adrv9001_Ssi_Tx_TestMode_Status_Inspect(Instance->Adrv9001, AdiChannel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg, &s );

    /* Stop DMA */
    PhyStream_Disable( Instance, Port );

    if( (status != 0) || (s.dataError != 0) || (s.strobeAlignError != 0) )
      return PhyStatus_TestModeFailure;
  }
  else
  {
    StreamReq.Cyclic = false;
    StreamReq.Blocking = true;

    /* Transfer Sample from Rx DMA */
    status = PhyStream_Enable(Instance, &StreamReq);

    if( (status != 0) || ( Buf[0] != ADRV9001_TEST_MODE_PATTERN) )
      return PhyStatus_TestModeFailure;
  }

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_SetTestMode( phy_t *Instance, phy_port_t Port )
{
  /* Get ADI Channel Number */
  adi_common_ChannelNumber_e AdiChannel = PhyAdrv9001_GetAdiChannel( Port );

  if(PHY_IS_PORT_TX(Port))
  {
    adi_adrv9001_TxSsiTestModeCfg_t Cfg =
    {
        .fixedDataPatternToCheck = ADRV9001_TEST_MODE_PATTERN,
        .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_FIXED_PATTERN
    };

    return adi_adrv9001_Ssi_Tx_TestMode_Configure(Instance->Adrv9001, AdiChannel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg );
  }
  else
  {
    adi_adrv9001_RxSsiTestModeCfg_t Cfg =
    {
        .fixedDataPatternToTransmit = ADRV9001_TEST_MODE_PATTERN,
        .testData = ADI_ADRV9001_SSI_TESTMODE_DATA_FIXED_PATTERN
    };

    return adi_adrv9001_Ssi_Rx_TestMode_Configure(Instance->Adrv9001, AdiChannel, ADI_ADRV9001_SSI_TYPE_LVDS, ADI_ADRV9001_SSI_FORMAT_16_BIT_I_Q_DATA, &Cfg );
  }
}

phy_status_t PhyAdrv9001_RunSsiTestMode( phy_t *Instance, phy_port_t Port, uint8_t clkDelay, uint8_t dataDelay  )
{
  int32_t status;

  do
  {
    /* Prime State */
    if((status = PhyAdrv9001_ToRfPrimed( Instance, Port )) != 0) break;

    /* Set SSI Delay */
    if((status = PhyAdrv9001_SetSsiDelay( Instance, Port, clkDelay, dataDelay )) != 0 ) break;

    /* Set Test Mode */
    if((status = PhyAdrv9001_SetTestMode( Instance, Port )) != 0) break;

    /* Enable RF */
    if((status = PhyAdrv9001_ToRfEnabled( Instance, Port )) != 0) break;

    /* Inspect Test Mode */
    if((status = PhyAdrv9001_InspectTestMode( Instance, Port )) != 0) break;

  }while(0);

  /* Prime State */
  if( PhyAdrv9001_ToRfPrimed( Instance, Port ) != 0)
    status = PhyStatus_TestModeFailure;

  return status;
}

phy_status_t PhyAdrv9001_PerformSsiSweep( phy_t *Instance, phy_port_t Port, uint8_t results[8][8] )
{
  int32_t status;

  for(uint8_t clkDly = 0; clkDly < 8; clkDly++)
  {
    for(uint8_t dataDly = 0; dataDly < 8; dataDly++)
    {
      status = PhyAdrv9001_RunSsiTestMode( Instance, Port, clkDly, dataDly );

      results[clkDly][dataDly] = (status == 0)? 1 : 0;

      usleep(1000);
    }
  }

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_SetEnablePin( phy_t *Instance, phy_port_t Port, uint8_t Enabled )
{
  uint32_t regMask = (Port == PhyPort_Rx1)? AXI_ADRV9001_CTRL_RX1EN_MASK :
                     (Port == PhyPort_Rx2)? AXI_ADRV9001_CTRL_RX2EN_MASK :
                     (Port == PhyPort_Tx1)? AXI_ADRV9001_CTRL_TX1EN_MASK :
                     (Port == PhyPort_Tx2)? AXI_ADRV9001_CTRL_TX2EN_MASK : 0;

  uint32_t regVal;

  axi_io_read(AXI_ADRV9001_CTRL_BASE_ADDR, AXI_ADRV9001_CTRL_REG_OFFSET, &regVal);

  if(Enabled)
    regVal |= regMask;
  else
    regVal &= ~regMask;

  axi_io_write(AXI_ADRV9001_CTRL_BASE_ADDR, AXI_ADRV9001_CTRL_REG_OFFSET, regVal);

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_LnaEnable( phy_t *Instance, phy_port_t Port, bool Enable )
{
  int32_t Status = 0;

  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  if( Port == PhyPort_Rx1 )
  {
    if( Enable )
      Status = adi_adrv9001_gpio_OutputPinLevel_Set(Instance->Adrv9001, ADI_ADRV9001_GPIO_ANALOG_06, ADI_ADRV9001_GPIO_PIN_LEVEL_HIGH);
    else
      Status = adi_adrv9001_gpio_OutputPinLevel_Set(Instance->Adrv9001, ADI_ADRV9001_GPIO_ANALOG_06, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW);
  }
  else if( Port == PhyPort_Rx2 )
  {
    if( Enable )
      Status = adi_adrv9001_gpio_OutputPinLevel_Set(Instance->Adrv9001, ADI_ADRV9001_GPIO_ANALOG_05, ADI_ADRV9001_GPIO_PIN_LEVEL_HIGH);
    else
      Status = adi_adrv9001_gpio_OutputPinLevel_Set(Instance->Adrv9001, ADI_ADRV9001_GPIO_ANALOG_05, ADI_ADRV9001_GPIO_PIN_LEVEL_LOW);
  }

  if( Status == 0 )
    return PhyStatus_Success;
  else
    return PhyStatus_Adrv9001Error;
}

phy_status_t PhyAdrv9001_GetGainControlMode( phy_t *Instance, phy_port_t Port, uint32_t *Rssi_mdB )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  if( adi_adrv9001_Rx_Rssi_Read(Instance->Adrv9001, PhyAdrv9001_GetAdiChannel(Port), Rssi_mdB) != 0)
    return PhyStatus_Adrv9001Error;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_EnableDac( phy_t *Instance, uint8_t Id, bool Enable )
{
  adi_adrv9001_GpioSignal_e signal;

  adi_adrv9001_GpioCfg_t Cfg = {
      .polarity = ADI_ADRV9001_GPIO_POLARITY_NORMAL,
      .master = ADI_ADRV9001_GPIO_MASTER_ADRV9001
  };

  switch( Id )
  {
    case 0 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_0; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_00; break;
    case 1 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_1; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_01; break;
    case 2 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_2; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_02; break;
    case 3 : signal = ADI_ADRV9001_GPIO_SIGNAL_AUX_DAC_3; Cfg.pin = ADI_ADRV9001_GPIO_ANALOG_03; break;
    default: return PhyStatus_InvalidParameter;
  }

  if( adi_adrv9001_gpio_Configure(Instance->Adrv9001, signal, &Cfg) != 0)
    return PhyStatus_GpioError;

  if( adi_adrv9001_AuxDac_Configure(Instance->Adrv9001, Id, Enable) != 0 )
    return PhyStatus_DacError;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_SetDac( phy_t *Instance, uint8_t Id, float Voltage )
{
  uint16_t Value = (uint16_t)((((Voltage - 0.900)/ 1.700) * 4096.000) + 2048);

  if( adi_adrv9001_AuxDac_Code_Set(Instance->Adrv9001, Id, Value) != 0 )
    return PhyStatus_DacError;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_GetDac( phy_t *Instance, uint8_t Id, float *Voltage )
{
  uint16_t Value;

  if( adi_adrv9001_AuxDac_Code_Get(Instance->Adrv9001, Id, &Value) != 0 )
    return PhyStatus_DacError;

  *Voltage = 0.9000 + (((float)Value - 2048.000) / 4096.000) * 1.7;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_ResetSsiPort( phy_t *Instance, phy_port_t Port )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  uint32_t regMask = (Port == PhyPort_Rx1)? AXI_ADRV9001_CTRL_RX1RST_MASK :
                     (Port == PhyPort_Rx2)? AXI_ADRV9001_CTRL_RX2RST_MASK :
                     (Port == PhyPort_Tx1)? AXI_ADRV9001_CTRL_TX1RST_MASK :
                     (Port == PhyPort_Tx2)? AXI_ADRV9001_CTRL_TX2RST_MASK : 0;

  uint32_t regVal;

  axi_io_read(AXI_ADRV9001_CTRL_BASE_ADDR, AXI_ADRV9001_CTRL_REG_OFFSET, &regVal);

  regVal |= regMask;

  axi_io_write(AXI_ADRV9001_CTRL_BASE_ADDR, AXI_ADRV9001_CTRL_REG_OFFSET, regVal);

  regVal &= ~regMask;

  axi_io_write(AXI_ADRV9001_CTRL_BASE_ADDR, AXI_ADRV9001_CTRL_REG_OFFSET, regVal);

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_ToRfCalibrated( phy_t *Instance, phy_port_t Port )
{
  if( (Port >= PhyPort_Num) && (Port != PhyPort_All) )
    return PhyStatus_InvalidPort;

  adi_common_Port_e AdiPort;
  adi_common_ChannelNumber_e AdiChannel;

  phy_port_t p = (Port == PhyPort_All)? PhyPort_Rx1 : Port;

  do
  {
    AdiPort = PhyAdrv9001_GetAdiPort(p);
    AdiChannel = PhyAdrv9001_GetAdiChannel(p);

    /* Check if port enabled */
    if( PhyAdrv9001_IsPortEnabled( Instance, p ) == PhyStatus_Success )
    {
      /* Set Enable Pin */
      PhyAdrv9001_SetEnablePin( Instance, p, 0 );

      if(adi_adrv9001_Radio_Channel_ToCalibrated(Instance->Adrv9001, AdiPort, AdiChannel) != 0)
        return PhyStatus_InvalidRadioState;
    }

    p++;

  }while( (p < PhyPort_Num) && (Port == PhyPort_All) );

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_ToRfEnabled( phy_t *Instance, phy_port_t Port )
{
  phy_status_t status;

  if( (Port >= PhyPort_Num) && (Port != PhyPort_All) )
    return PhyStatus_InvalidPort;

  adi_adrv9001_ChannelEnableMode_e mode = ADI_ADRV9001_SPI_MODE;

  adi_common_Port_e AdiPort;
  adi_common_ChannelNumber_e AdiChannel;

  phy_port_t p = (Port == PhyPort_All)? PhyPort_Rx1 : Port;

  do
  {
    AdiPort = PhyAdrv9001_GetAdiPort(p);
    AdiChannel = PhyAdrv9001_GetAdiChannel(p);

    /* Check if port enabled */
    if( PhyAdrv9001_IsPortEnabled( Instance, p ) == PhyStatus_Success )
    {

      /* Enable LNA */
      if((status = PhyAdrv9001_LnaEnable( Instance, p, true )) != 0)
        return status;

      /* Set Enable Pin */
      PhyAdrv9001_SetEnablePin( Instance, p, 1 );

      /* Get Radio Enable Mode */
      if(adi_adrv9001_Radio_ChannelEnableMode_Get(Instance->Adrv9001, AdiPort, AdiChannel, &mode) != 0 )
        return PhyStatus_InvalidRadioState;

      /* Set mode using SPI */
      if( mode == ADI_ADRV9001_SPI_MODE )
      {
        /* Set Radio State */
        if(adi_adrv9001_Radio_Channel_ToRfEnabled(Instance->Adrv9001, AdiPort, AdiChannel) != 0)
          return PhyStatus_InvalidRadioState;
      }

      /* Synchronize SSI */
      PhyAdrv9001_ResetSsiPort( Instance, p );

    }

    p++;

  }while( (p < PhyPort_Num) && (Port == PhyPort_All) );

  usleep(10);

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_ToRfPrimed( phy_t *Instance, phy_port_t Port )
{
  phy_status_t status;

  if( (Port >= PhyPort_Num) && (Port != PhyPort_All) )
    return PhyStatus_InvalidPort;

  adi_common_Port_e AdiPort;
  adi_common_ChannelNumber_e AdiChannel;

  phy_port_t p = (Port == PhyPort_All)? PhyPort_Rx1 : Port;

  do
  {
    AdiPort = PhyAdrv9001_GetAdiPort(p);
    AdiChannel = PhyAdrv9001_GetAdiChannel(p);

    /* Check if port enabled */
    if( PhyAdrv9001_IsPortEnabled( Instance, p ) == PhyStatus_Success )
    {
      /* Disable LNA */
      if((status = PhyAdrv9001_LnaEnable( Instance, p, false )) != 0)
        return status;

      /* Set Enable Pin */
      PhyAdrv9001_SetEnablePin( Instance, p, 0 );

      /* Set Radio State */
      if(adi_adrv9001_Radio_Channel_ToPrimed(Instance->Adrv9001, AdiPort, AdiChannel) != 0)
        return PhyStatus_InvalidRadioState;
    }

    p++;

  }while( (p < PhyPort_Num) && (Port == PhyPort_All) );

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_GetRssi( phy_t *Instance, phy_port_t Port, uint32_t *Rssi_mdB )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  if( adi_adrv9001_Rx_Rssi_Read(Instance->Adrv9001, PhyAdrv9001_GetAdiChannel(Port), Rssi_mdB) != 0)
    return PhyStatus_Adrv9001Error;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_GetTemperature( phy_t *Instance, int16_t *Temp_C )
{
  if( adi_adrv9001_Temperature_Get(Instance->Adrv9001, Temp_C) != 0)
    return PhyStatus_Adrv9001Error;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_SetTxBoost( phy_t *Instance, phy_port_t Port, bool Enable )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  if( adi_adrv9001_Tx_OutputPowerBoost_Set(Instance->Adrv9001, PhyAdrv9001_GetAdiChannel(Port), Enable) != 0)
    return PhyStatus_Adrv9001Error;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_GetTxBoost( phy_t *Instance, phy_port_t Port, bool *Enable )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  if(adi_adrv9001_Tx_OutputPowerBoost_Get(Instance->Adrv9001, PhyAdrv9001_GetAdiChannel(Port), Enable) != 0)
    return PhyStatus_Adrv9001Error;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_SetTxAttenuation( phy_t *Instance, phy_port_t Port, uint16_t Attn_mdB )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  if( adi_adrv9001_Tx_Attenuation_Set(Instance->Adrv9001, PhyAdrv9001_GetAdiChannel(Port), Attn_mdB) != 0)
    return PhyStatus_Adrv9001Error;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_GetTxAttenuation( phy_t *Instance, phy_port_t Port, uint16_t *Attn_mdB )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  if( adi_adrv9001_Tx_Attenuation_Get(Instance->Adrv9001, PhyAdrv9001_GetAdiChannel(Port), Attn_mdB) != 0)
    return PhyStatus_Adrv9001Error;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_GetSampleRate( phy_t *Instance, phy_port_t Port, uint32_t *FreqHz )
{
  if( Port == PhyPort_Rx1 )
    *FreqHz = Instance->Profile->AdrvInit.rx.rxChannelCfg[0].profile.rxInterfaceSampleRate_Hz;
  else if( Port == PhyPort_Rx2 )
    *FreqHz = Instance->Profile->AdrvInit.rx.rxChannelCfg[1].profile.rxInterfaceSampleRate_Hz;
  else if( Port == PhyPort_Tx1 )
    *FreqHz = Instance->Profile->AdrvInit.tx.txProfile[0].txInterfaceSampleRate_Hz;
  else if( Port == PhyPort_Tx2 )
    *FreqHz = Instance->Profile->AdrvInit.tx.txProfile[1].txInterfaceSampleRate_Hz;
  else
    return PhyStatus_InvalidPort;

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_GetCarrierFrequency( phy_t *Instance, phy_port_t Port, uint64_t *FreqHz )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  adi_common_ChannelNumber_e  AdiChannel  = PhyAdrv9001_GetAdiChannel(Port);
  adi_common_Port_e           AdiPort     = PhyAdrv9001_GetAdiPort(Port);
  adi_adrv9001_Carrier_t      AdiCarrier;

  int32_t status = adi_adrv9001_Radio_Carrier_Inspect(Instance->Adrv9001, AdiPort, AdiChannel, &AdiCarrier);

  *FreqHz = AdiCarrier.carrierFrequency_Hz;

  return status;
}

phy_status_t PhyAdrv9001_SetCarrierFrequency( phy_t *Instance, phy_port_t Port, uint64_t FreqHz )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  adi_common_ChannelNumber_e  AdiChannel  = PhyAdrv9001_GetAdiChannel(Port);
  adi_common_Port_e           AdiPort     = PhyAdrv9001_GetAdiPort(Port);
  adi_adrv9001_Carrier_t      AdiCarrier = {
      .carrierFrequency_Hz = FreqHz,
      .intermediateFrequency_Hz = 0,
      .loGenOptimization = ADI_ADRV9001_LO_GEN_OPTIMIZATION_PHASE_NOISE
  };

  int32_t status;

  if((status = PhyAdrv9001_ToRfPrimed( Instance, Port )) != PhyStatus_Success)
    return status;

  if((status = PhyAdrv9001_ToRfCalibrated( Instance, Port )) != PhyStatus_Success)
    return status;

  if((status = adi_adrv9001_Radio_Carrier_Configure(Instance->Adrv9001, AdiPort, AdiChannel, &AdiCarrier)) != 0)
    return status;

  return status;
}

phy_status_t PhyAdrv9001_SetInternalLoopBack( phy_t *Instance, phy_port_t Port, bool Enabled )
{
  if( Port >= PhyPort_Num )
    return PhyStatus_InvalidPort;

  adi_common_ChannelNumber_e AdiChannel = PhyAdrv9001_GetAdiChannel(Port);

  if((Port == PhyPort_Tx1) || (Port == PhyPort_Tx2))
  {
    return adi_adrv9001_Ssi_Loopback_Set(Instance->Adrv9001, AdiChannel, ADI_ADRV9001_SSI_TYPE_LVDS, Enabled);
  }
  else
  {
    return PhyStatus_InvalidPort;
  }
}

phy_status_t PhyAdrv9001_ClearError( phy_t *Instance )
{
  adi_common_ErrorClear(&Instance->Adrv9001->common);

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_LoadProfile( phy_t *Instance )
{
  int32_t status;

  adrv9001_cfg_t Cfg = {
      .CarrierFreq = { Instance->Profile->Lo[0].Frequency, Instance->Profile->Lo[1].Frequency },
      .Init = &Instance->Profile->AdrvInit
  };

  /* Load Profile */
  if( Adrv9001_Initialize( Instance->Adrv9001, &Cfg ) != 0)
    return PhyStatus_Adrv9001Error;

  /* Calibrate SSI */
  for( int i = 0; i < PhyPort_Num; i++)
  {
    if((status = PhyAdrv9001_CalibrateSsiDelay(Instance, i)) != 0)
      return status;
  }

  return PhyStatus_Success;
}

phy_status_t PhyAdrv9001_Initialize( phy_t *Instance )
{
  int32_t status;

  /* Create Storage for Device */
  if((Instance->Adrv9001 = (adi_adrv9001_Device_t *) calloc(1, sizeof(adi_adrv9001_Device_t))) == NULL)
    return PhyStatus_MemoryError;

  /* Assign Hal Reference to Instance */
  Instance->Adrv9001->common.devHalInfo = (void*)&Instance->Adrv9001Hal;

  /* Enable Logging */
  Instance->Adrv9001->common.error.logEnable = 1;

  /* Load Default Profile */
  status = PhyAdrv9001_LoadProfile( Instance );

  printf("%s Version Information:\r\n","ADRV9002" );

  adi_adrv9001_SiliconVersion_t SiliconVer;
  if( adi_adrv9001_SiliconVersion_Get( Instance->Adrv9001, &SiliconVer ) != PhyStatus_Success)
    return status;

  printf("  -Silicon Version: %X%X\r\n", SiliconVer.major, SiliconVer.minor);

  adi_adrv9001_ArmVersion_t ArmVer;

  if( adi_adrv9001_arm_Version( Instance->Adrv9001, &ArmVer ) != PhyStatus_Success)
    return status;

  printf("  -Firmware Version: %u.%u.%u.%u\r\n",ArmVer.majorVer, ArmVer.minorVer, ArmVer.maintVer, ArmVer.rcVer);

  /* Get Version Info */
  adi_common_ApiVersion_t ApiVer;

  if(adi_adrv9001_ApiVersion_Get( Instance->Adrv9001, &ApiVer) != PhyStatus_Success)
    return status;

  printf("  -API Version: %lu.%lu.%lu\r\n\r\n", ApiVer.major,  ApiVer.minor, ApiVer.patch);

  return status;
}
