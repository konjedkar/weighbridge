/***************************************************************************//**
 *   @file   AD7780.cpp
 *   @brief  Implementation of AD7780 Driver.
 *   @author DNechita (dan.nechita@analog.com)
********************************************************************************
 * Copyright 2012(c) Analog Devices, Inc.
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
 *
********************************************************************************
 *   SVN Revision: 809
 *******************************************************************************/
 
/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "AD7780.h"	
#include <DSPI.h>

/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/

/***************************************************************************//**
 * @brief Initializes the communication peripheral and checks if the device is
 *        present.
 *
 * @return initStatus - The result of the initialization procedure.
 *                      Example: 0x0 - SPI peripheral was not initialized.
 *                               0x1 - SPI peripheral was initialized.
*******************************************************************************/
char AD7780::Init(void)
{
    unsigned char ad7780Status = 0;
    char          initStatus   = 1;    
    
    /* Set PDRST, FILTER and GAIN pins as output. */
    pinMode(AD7780_PDRST_PIN, OUTPUT);
    pinMode(AD7780_FILTER_PIN, OUTPUT);
    pinMode(AD7780_GAIN_PIN, OUTPUT);
    /* Set the initial value of the PDRST, FILTER and GAIN pins. */
    digitalWrite(AD7780_PDRST_PIN, HIGH);   // Places AD7780 in power-up mode.
    digitalWrite(AD7780_FILTER_PIN, LOW);   // The update rate is set to 16.7 Hz.
    digitalWrite(AD7780_GAIN_PIN, HIGH);    // Gain is set to 1.
    /* Initialize SPI communication */
	oDspi.begin();
	oDspi.setMode(DSPI_MODE3);
	oDspi.setSpeed(DEVICE_SPI_CLK_FREQ);
    initStatus = WaitRdyGoLow();
    if(!initStatus)
    {
        return 0;
    }
    ReadSample(&ad7780Status);
    if((ad7780Status & (AD7780_STAT_ID1 | AD7780_STAT_ID0)) != AD7780_ID_NUMBER)
    {
        return 0;
    }
    
    return initStatus;
}

/***************************************************************************//**
 * @brief Waits for DOUT/RDY pin to go low.
 *
 * @return None.
*******************************************************************************/
char AD7780::WaitRdyGoLow(void)
{
    unsigned long timeout = 0xFFFFF;

    while(digitalRead(AD7780_RDY_PIN) && timeout)
    {
        timeout--;
    }
    if(timeout == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/***************************************************************************//**
 * @brief Reads a 24-bit sample from the ADC.
 *
 * @param pStatus - Stores the 8-bit status that is read for every conversion.
 *
 * @return convSample - The 24-bit sample.
*******************************************************************************/
long AD7780::ReadSample(unsigned char* pStatus)
{
    unsigned char rxBuff[4]  = {0, 0, 0, 0};
    long          convSample = 0;
    
    oDspi.transfer(4, rxBuff, rxBuff);
    convSample = ((long)rxBuff[0] << 16) + (rxBuff[1] << 8) + rxBuff[2];
    *pStatus = rxBuff[3];
    
    return convSample;
}

/***************************************************************************//**
 * @brief Converts the 24-bit raw value to volts.
 *
 * @param rawSample - 24-bit raw sample(offset binary).
 * @param vRef - The reference voltage.
 * @param gain - The gain. Accepted values: 1, 128.
 *
 * @return voltage - The voltage obtained from the raw value.
*******************************************************************************/
float AD7780::ConvertToVoltage(long rawSample, float vRef, unsigned char gain)
{
    float voltage = 0;
    
    voltage = (((float)rawSample / (1ul << 23)) - 1) * vRef / gain;
    
    return voltage;
}
