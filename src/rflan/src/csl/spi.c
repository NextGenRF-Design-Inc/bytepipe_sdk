/***************************************************************************//**
 *   @file   spi.c
 *   @brief  Implementation of the SPI Interface
 *   @author Antoniu Miclaus (antoniu.miclaus@analog.com)
********************************************************************************
 * Copyright 2020(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include <inttypes.h>
#include "spi.h"
#include <stdlib.h>
#include "error.h"

/**
 * @brief Initialize the SPI communication peripheral.
 * @param desc - The SPI descriptor.
 * @param param - The structure that contains the SPI parameters.
 * @return SUCCESS in case of success, FAILURE otherwise.
 */
int32_t spi_init(struct spi_desc **desc,
		 const struct spi_init_param *param)
{
	if (!param)
		return FAILURE;

	if ((param->platform_ops->init(desc, param)))
		return FAILURE;

	(*desc)->platform_ops = param->platform_ops;

	return SUCCESS;
}

/**
 * @brief Free the resources allocated by spi_init().
 * @param desc - The SPI descriptor.
 * @return SUCCESS in case of success, FAILURE otherwise.
 */
int32_t spi_remove(struct spi_desc *desc)
{
	return desc->platform_ops->remove(desc);
}

/**
 * @brief Write and read data to/from SPI.
 * @param desc - The SPI descriptor.
 * @param data - The buffer with the transmitted/received data.
 * @param bytes_number - Number of bytes to write/read.
 * @return SUCCESS in case of success, FAILURE otherwise.
 */
int32_t spi_write_and_read(struct spi_desc *desc,
			   uint8_t *data,
			   uint16_t bytes_number)
{
	return desc->platform_ops->write_and_read(desc, data, bytes_number);
}

/**
 * @brief  Iterate over head list and send all spi messages
 * @param desc - The SPI descriptor.
 * @param msgs - Array of messages.
 * @param len - Number of messages in the array.
 * @return SUCCESS in case of success, negativ error code otherwise.
 */
int32_t spi_transfer(struct spi_desc *desc, struct spi_msg *msgs, uint32_t len)
{
	int32_t  ret;
	uint32_t i;

	if (!desc || !desc->platform_ops)
		return -EINVAL;

	if (desc->platform_ops->transfer)
		return desc->platform_ops->transfer(desc, msgs, len);

	for (i = 0; i < len; i++) {
		if (msgs[i].rx_buff != msgs[i].tx_buff || !msgs[i].tx_buff)
			return -EINVAL;
		ret = spi_write_and_read(desc, msgs[i].rx_buff,
					 msgs[i].bytes_number);
		if (IS_ERR_VALUE(ret))
			return ret;
	}

	return ret;
}
