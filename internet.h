/***************************************************************************//**
 *   @file   AD7780.h
 *   @brief  Header file of AD7780 Driver.
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
#ifndef __AD7780_H__
#define __AD7780_H__

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <DSPI.h>

/******************************************************************************/
/******************* Macros and Constants Definitions *************************/
/******************************************************************************/
/* Communication definitions */
#define DEVICE_SPI_CLK_FREQ     1000000	// 1MHz SPI clock frequency */

/******************************************************************************/
/************************** AD7780 Definitions ********************************/
/******************************************************************************/

/* DOUT/RDY pin */
#define AD7780_RDY_PIN          (PIN_DSPI0_SS + 2) // MISO pin

/* PDRST pin */
#define AD7780_PDRST_PIN        PIN_DSPI0_SS

/* FILTER pin */
#define AD7780_FILTER_PIN       36  // IC1/RTCC/INT1/RD8

/* GAIN pin */
#define AD7780_GAIN_PIN         37  // C1IN+/AN5/CN7/RB5

/* Status bits */
#define AD7780_STAT_RDY         (1 << 7) // Ready bit.
#define AD7780_STAT_FILTER      (1 << 6) // Filter bit.
#define AD7780_STAT_ERR         (1 << 5) // Error bit.
#define AD7780_STAT_ID1         (1 << 4) // ID bits.
#define AD7780_STAT_ID0         (1 << 3) // ID bits.
#define AD7780_STAT_GAIN        (1 << 2) // Gain bit.
#define AD7780_STAT_PAT1        (1 << 1) // Status pattern bits.
#define AD7780_STAT_PAT0        (1 << 0) // Status pattern bits.

#define AD7780_ID_NUMBER        0x08

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/

class AD7780
{
private:
	DSPI0 oDspi;
    
public:
    /*! Initializes the communication peripheral and checks if the device is
    present. */
    char Init(void);
	
	/*! Waits for DOUT/RDY pin to go low. */
    char WaitRdyGoLow(void);
    
    /*! Reads a 24-bit sample from the ADC. */
    long ReadSample(unsigned char* pStatus);
    
    /*! Converts the 24-bit raw value to volts. */
    float ConvertToVoltage(long rawSample, float vRef, unsigned char gain);
};

#endif	//__AD7780_H__