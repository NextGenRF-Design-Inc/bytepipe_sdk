
#include <stdio.h>
#include <string.h>
#include <xil_io.h>
#include "axi_adrv9001.h"
#include "adrv9001.h"
#include "xscugic.h"


extern XScuGic          xInterruptController;



/* Register Address */
#define ADRV9001_TX1_MODE_ADDR            ((0 ) << 2)
#define ADRV9001_TX2_MODE_ADDR            ((1 ) << 2)
#define ADRV9001_RX1_MODE_ADDR            ((2 ) << 2)
#define ADRV9001_RX2_MODE_ADDR            ((3 ) << 2)
#define ADRV9001_RSTN_ADDR                ((4 ) << 2)

#define ADRV9001_TX1_ENABLE_DLY_ADDR      ((5 ) << 2)
#define ADRV9001_TX2_ENABLE_DLY_ADDR      ((6 ) << 2)
#define ADRV9001_RX1_ENABLE_DLY_ADDR      ((7 ) << 2)
#define ADRV9001_RX2_ENABLE_DLY_ADDR      ((8 ) << 2)

#define ADRV9001_TX1_DISABLE_DLY_ADDR     ((9 ) << 2)
#define ADRV9001_TX2_DISABLE_DLY_ADDR     ((10) << 2)
#define ADRV9001_RX1_DISABLE_DLY_ADDR     ((11) << 2)
#define ADRV9001_RX2_DISABLE_DLY_ADDR     ((12) << 2)

#define ADRV9001_DGPIO_DIR_ADDR           ((13) << 2)
#define ADRV9001_DGPIO_IO_ADDR            ((14) << 2)

#define ADRV9001_CH1_DATA_ADDR            ((15) << 2)
#define ADRV9001_CH2_DATA_ADDR            ((16) << 2)

#define ADRV9001_TX1_DATA_PATH_ADDR       ((17) << 2)
#define ADRV9001_TX2_DATA_PATH_ADDR       ((18) << 2)

#define ADRV9001_CAPTURE_CONTROL_CNT_ADDR ((19) << 2)

#define ADRV9001_MSPI_ADDR                ((21) << 2)

#define ADRV9001_ID_ADDR                  ((31) << 2)




static void AxiAdrv9001_SpiIrq( axi_adrv9001_t *Instance )
{
  Instance->SpiDone = 1;

  XScuGic_Disable(&xInterruptController, Instance->IrqId);
}

int32_t AxiAdrv9001_Initialize(axi_adrv9001_t *Instance, axi_adrv9001_init_t *Init )
{
  Instance->Base = Init->Base;
  Instance->IrqId = Init->IrqId;

  uint32_t regVal = Xil_In32(Instance->Base + ADRV9001_ID_ADDR);

  if(XScuGic_Connect(&xInterruptController, Instance->IrqId, (XInterruptHandler)AxiAdrv9001_SpiIrq, Instance ) != 0)
    return Adrv9001Status_IrqErr;

  if(regVal == 0x12345678)
    return 0;
  else
    return -1;
}

void AxiAdrv9001_ResetbPinSet(axi_adrv9001_t *Instance, uint8_t Level )
{
  Xil_Out32(Instance->Base + ADRV9001_RSTN_ADDR, Level);
}

static void AxiAdrv9001_MspiWrite(axi_adrv9001_t *Instance, uint8_t Value, uint8_t Start )
{
  uint32_t regValue = (uint32_t)Value + ((uint32_t)Start << 8);

  Xil_Out32(Instance->Base + ADRV9001_MSPI_ADDR, regValue);
}

static bool AxiAdrv9001_MspiRead(axi_adrv9001_t *Instance, uint8_t *Value )
{
  uint32_t regValue = Xil_In32(Instance->Base + ADRV9001_MSPI_ADDR);

  if( (regValue & 0x100) == 0x100)
  {
    *Value = (uint8_t)regValue;
    return true;
  }
  else
  {
    return false;
  }
}

int32_t AxiAdrv9001_MspiTransfer(axi_adrv9001_t *Instance, uint8_t *TxBuf, uint8_t *RxBuf, uint32_t Length)
{
  Instance->SpiDone = 0;

  XScuGic_Enable(&xInterruptController, Instance->IrqId);

  for( int i = 0; i < Length; i++ )
  {
    if( i == (Length - 1))
      AxiAdrv9001_MspiWrite(Instance, TxBuf[i], 1);
    else
      AxiAdrv9001_MspiWrite(Instance, TxBuf[i], 0);
  }

  while( Instance->SpiDone == 0 );

  uint8_t regValue;
  for( int i = 0; i < Length; i++ )
  {
    if( AxiAdrv9001_MspiRead( Instance, &regValue) == false)
      return Adrv9001Status_SpiErr;

    if( RxBuf != NULL )
      RxBuf[i] = regValue;
  }

  return Adrv9001Status_Success;
}

void AxiAdrv9001_SetDisableDelay(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t SampleCnt )
{
  uint32_t tmp = (uint32_t)SampleCnt;
  
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Instance->Base + ADRV9001_TX1_DISABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Instance->Base + ADRV9001_TX2_DISABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Instance->Base + ADRV9001_RX1_DISABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Instance->Base + ADRV9001_RX2_DISABLE_DLY_ADDR, tmp);
  }
}

void AxiAdrv9001_SetEnableDelay(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t SampleCnt )
{
  uint32_t tmp = (uint32_t)SampleCnt;
    
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Instance->Base + ADRV9001_TX1_ENABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Instance->Base + ADRV9001_TX2_ENABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    Xil_Out32(Instance->Base + ADRV9001_RX1_ENABLE_DLY_ADDR, tmp);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    Xil_Out32(Instance->Base + ADRV9001_RX2_ENABLE_DLY_ADDR, tmp);
  }
}

void AxiAdrv9001_GetDisableDelay(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t *SampleCnt )
{
  uint32_t tmp;
  
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_TX1_DISABLE_DLY_ADDR);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_TX2_DISABLE_DLY_ADDR);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_RX1_DISABLE_DLY_ADDR);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_RX2_DISABLE_DLY_ADDR);
  }
  
  *SampleCnt = (uint16_t)tmp;
}

void AxiAdrv9001_GetEnableDelay(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint16_t *SampleCnt )
{
  uint32_t tmp;
  
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_TX1_ENABLE_DLY_ADDR);
  }
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_TX2_ENABLE_DLY_ADDR);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_RX1_ENABLE_DLY_ADDR);
  }
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
  {
    tmp = Xil_In32(Instance->Base + ADRV9001_RX2_ENABLE_DLY_ADDR);
  }
  
  *SampleCnt = (uint16_t)tmp;  
}

void AxiAdrv9001_SetEnableMode(axi_adrv9001_t *Instance, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, adi_adrv9001_ChannelEnableMode_e mode )
{
  if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
    Xil_Out32(Instance->Base + ADRV9001_TX1_MODE_ADDR, (uint32_t)mode);
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
    Xil_Out32(Instance->Base + ADRV9001_TX2_MODE_ADDR, (uint32_t)mode);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
    Xil_Out32(Instance->Base + ADRV9001_RX1_MODE_ADDR, (uint32_t)mode);
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
    Xil_Out32(Instance->Base + ADRV9001_RX2_MODE_ADDR, (uint32_t)mode);
}

void AxiAdrv9001_SetCaptureControlCnt(axi_adrv9001_t *Instance, uint32_t Value )
{
  Xil_Out32(Instance->Base + ADRV9001_CAPTURE_CONTROL_CNT_ADDR, Value);
}

void AxiAdrv9001_GetCaptureControlCnt(axi_adrv9001_t *Instance, uint32_t *Value )
{
  *Value = Xil_In32(Instance->Base + ADRV9001_CAPTURE_CONTROL_CNT_ADDR);
}


void AxiAdrv9001_SetDgpio(axi_adrv9001_t *Instance, uint32_t Value )
{
  Xil_Out32(Instance->Base + ADRV9001_DGPIO_IO_ADDR, Value);
}

void AxiAdrv9001_GetDgpio(axi_adrv9001_t *Instance, uint32_t *Value )
{
  *Value = Xil_In32(Instance->Base + ADRV9001_DGPIO_IO_ADDR);
}

void AxiAdrv9001_SetDgpioDir(axi_adrv9001_t *Instance, uint32_t Value )
{
  Xil_Out32(Instance->Base + ADRV9001_DGPIO_DIR_ADDR, Value);
}

void AxiAdrv9001_GetDgpioDir(axi_adrv9001_t *Instance, uint32_t *Value )
{
  *Value = Xil_In32(Instance->Base + ADRV9001_DGPIO_DIR_ADDR);
}

void AxiAdrv9001_SetTxDataPath(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t Value )
{
  if( Channel == ADI_CHANNEL_1 )
    Xil_Out32(Instance->Base + ADRV9001_TX1_DATA_PATH_ADDR, (uint32_t)Value);
  else
    Xil_Out32(Instance->Base + ADRV9001_TX2_DATA_PATH_ADDR, (uint32_t)Value);
}

void AxiAdrv9001_GetTxDataPath(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t *Value )
{
  if( Channel == ADI_CHANNEL_1 )
    *Value = Xil_In32(Instance->Base + ADRV9001_TX1_DATA_PATH_ADDR);
  else
    *Value = Xil_In32(Instance->Base + ADRV9001_TX2_DATA_PATH_ADDR);
}

void AxiAdrv9001_SetTxData(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, uint32_t Value )
{
  if( Channel == ADI_CHANNEL_1 )
    Xil_Out32(Instance->Base + ADRV9001_CH1_DATA_ADDR, Value);
  else
    Xil_Out32(Instance->Base + ADRV9001_CH2_DATA_ADDR, Value);
}

void AxiAdrv9001_GetRxData(axi_adrv9001_t *Instance, adi_common_ChannelNumber_e Channel, uint32_t *Value )
{
  if( Channel == ADI_CHANNEL_1 )
    *Value = Xil_In32(Instance->Base + ADRV9001_CH1_DATA_ADDR);
  else
    *Value = Xil_In32(Instance->Base + ADRV9001_CH2_DATA_ADDR);
}

