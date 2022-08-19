
#include <stdio.h>
#include <string.h>
#include <xil_io.h>
#include "axi_adrv9001.h"



/* Register Address */
#define ADRV9001_CTRL_ADDR                ((0) << 2)
#define ADRV9001_DGPIO_ADDR               ((1) << 2)
#define ADRV9001_DGPIO_DIR_ADDR           ((2) << 2)
#define ADRV9001_TX1_DATA_PATH_ADDR       ((3) << 2)
#define ADRV9001_TX2_DATA_PATH_ADDR       ((4) << 2)
#define ADRV9001_TX1_DATA_ADDR            ((5) << 2)
#define ADRV9001_TX2_DATA_ADDR            ((6) << 2)
#define ADRV9001_RX1_DATA_ADDR            ((7) << 2)
#define ADRV9001_RX2_DATA_ADDR            ((8) << 2)

#define ADRV9001_ID_ADDR                  ((15) << 2)

/* Control Register Mask */
#define ADRV9001_CTRL_TX1EN_MASK          (0x00000001)
#define ADRV9001_CTRL_TX2EN_MASK          (0x00000002)
#define ADRV9001_CTRL_RX1EN_MASK          (0x00000004)
#define ADRV9001_CTRL_RX2EN_MASK          (0x00000008)
#define ADRV9001_CTRL_RSTN_MASK           (0x00000100)


int32_t AxiAdrv9001_Initialize( uint32_t Base )
{
  uint32_t regVal = Xil_In32(Base + ADRV9001_ID_ADDR);

  if(regVal == 0x12345678)
    return 0;
  else
    return -1;
}

void AxiAdrv9001_ResetbPinSet( uint32_t Base, uint8_t Level )
{
  uint32_t regVal = Xil_In32(Base + ADRV9001_CTRL_ADDR);

  if(Level)
    regVal |= ADRV9001_CTRL_RSTN_MASK;
  else
    regVal &= ~ADRV9001_CTRL_RSTN_MASK;

  Xil_Out32(Base + ADRV9001_CTRL_ADDR, regVal);
}

void AxiAdrv9001_SetEnablePin( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, bool Enabled )
{
  uint32_t regMask;

  if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_1) )
    regMask = ADRV9001_CTRL_RX1EN_MASK;
  else if( (Port == ADI_RX) && (Channel == ADI_CHANNEL_2) )
    regMask = ADRV9001_CTRL_RX2EN_MASK;
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_1) )
    regMask = ADRV9001_CTRL_TX1EN_MASK;
  else if( (Port == ADI_TX) && (Channel == ADI_CHANNEL_2) )
    regMask = ADRV9001_CTRL_TX2EN_MASK;
  else
    regMask = 0;

  uint32_t regVal = Xil_In32(Base + ADRV9001_CTRL_ADDR);

  if(Enabled)
    regVal |= regMask;
  else
    regVal &= ~regMask;

  Xil_Out32(Base + ADRV9001_CTRL_ADDR, regVal);
}

void AxiAdrv9001_SetDgpio( uint32_t Base, uint32_t Value )
{
  Xil_Out32(Base + ADRV9001_DGPIO_ADDR, Value);
}

void AxiAdrv9001_GetDgpio( uint32_t Base, uint32_t *Value )
{
  *Value = Xil_In32(Base + ADRV9001_DGPIO_ADDR);
}

void AxiAdrv9001_SetDgpioDir( uint32_t Base, uint32_t Value )
{
  Xil_Out32(Base + ADRV9001_DGPIO_DIR_ADDR, Value);
}

void AxiAdrv9001_GetDgpioDir( uint32_t Base, uint32_t *Value )
{
  *Value = Xil_In32(Base + ADRV9001_DGPIO_DIR_ADDR);
}

void AxiAdrv9001_SetDataPath( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t Value )
{
  if( Port == ADI_TX )
  {
    if( Channel == ADI_CHANNEL_1 )
      Xil_Out32(Base + ADRV9001_TX1_DATA_PATH_ADDR, (uint32_t)Value);
    else
      Xil_Out32(Base + ADRV9001_TX2_DATA_PATH_ADDR, (uint32_t)Value);
  }
}

void AxiAdrv9001_GetDataPath( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, axi_adrv9001_data_path_t *Value )
{
  uint32_t tmp;

  if( Port == ADI_TX )
  {
    if( Channel == ADI_CHANNEL_1 )
      tmp = Xil_In32(Base + ADRV9001_TX1_DATA_PATH_ADDR);
    else
      tmp = Xil_In32(Base + ADRV9001_TX2_DATA_PATH_ADDR);
  }
  else
  {
    tmp = AXI_ADRV9001_DATA_PATH_AXIS;
  }

  *Value = (axi_adrv9001_data_path_t)tmp;
}

void AxiAdrv9001_SetData( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint32_t Value )
{
	if( Port == ADI_TX )
	{
		if( Channel == ADI_CHANNEL_1 )
			Xil_Out32(Base + ADRV9001_TX1_DATA_ADDR, Value);
		else
			Xil_Out32(Base + ADRV9001_TX2_DATA_ADDR, Value);
	}
	else
	{
		if( Channel == ADI_CHANNEL_1 )
			Xil_Out32(Base + ADRV9001_RX1_DATA_ADDR, Value);
		else
			Xil_Out32(Base + ADRV9001_RX2_DATA_ADDR, Value);
	}
}

void AxiAdrv9001_GetData( uint32_t Base, adi_common_Port_e Port, adi_common_ChannelNumber_e Channel, uint32_t *Value )
{
	if( Port == ADI_TX )
	{
		if( Channel == ADI_CHANNEL_1 )
			*Value = Xil_In32(Base + ADRV9001_TX1_DATA_ADDR);
		else
			*Value = Xil_In32(Base + ADRV9001_TX2_DATA_ADDR);
	}
	else
	{
		if( Channel == ADI_CHANNEL_1 )
			*Value = Xil_In32(Base + ADRV9001_RX1_DATA_ADDR);
		else
			*Value = Xil_In32(Base + ADRV9001_RX2_DATA_ADDR);
	}
}

