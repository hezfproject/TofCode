/**************************************************************************************************
  Filename:       hal_adc.h
  Revised:        $Date: 2010/08/02 23:44:18 $
  Revision:       $Revision: 1.11 $

  Description:    This file contains the interface to the ADC Service.


  Copyright 2005-2007 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

#ifndef HAL_ADC_H
#define HAL_ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "hal_board.h"

/**************************************************************************************************
 * MACROS
 **************************************************************************************************/



/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

/* Resolution */
#define HAL_ADC_RESOLUTION_8       0x01
#define HAL_ADC_RESOLUTION_10      0x02
#define HAL_ADC_RESOLUTION_12      0x03
#define HAL_ADC_RESOLUTION_14      0x04

/* Channels */
#define HAL_ADC_CHANNEL_0          0x00
#define HAL_ADC_CHANNEL_1          0x01
#define HAL_ADC_CHANNEL_2          0x02
#define HAL_ADC_CHANNEL_3          0x03
#define HAL_ADC_CHANNEL_4          0x04
#define HAL_ADC_CHANNEL_5          0x05
#define HAL_ADC_CHANNEL_6          0x06
#define HAL_ADC_CHANNEL_7          0x07
#define HAL_ADC_CHANNEL_A0A1    0x08    /* AIN0,AIN1 */
#define HAL_ADC_CHANNEL_A2A3    0x09    /* AIN2,AIN3 */
#define HAL_ADC_CHANNEL_A4A5    0x0a    /* AIN4,AIN5 */
#define HAL_ADC_CHANNEL_A6A7    0x0b    /* AIN6,AIN7 */
#define HAL_ADC_CHANNEL_GND     0x0c    /* GND */
#define HAL_ADC_CHANNEL_VREF    0x0d    /* Positive voltage reference */
#define HAL_ADC_CHANNEL_TEMP    0x0e    /* Temperature sensor */
#define HAL_ADC_CHANNEL_VDD3    0x0f    /* VDD/3 */
#define HAL_ADC_CHANNEL_BITS    0x0f    /* Bits [3:0] */

/* reference */
#define HAL_ADC_REF_125V    0x00    /* Internal 1.25V Reference */
#define HAL_ADC_REF_AIN7    0x40    /* AIN7 Reference */
#define HAL_ADC_REF_AVDD    0x80    /* AVDD_SOC Pin Reference */
#define HAL_ADC_REF_DIFF    0xc0    /* AIN7,AIN6 Differential Reference */

/* Limits */
#define HAL_ADC_VDD_LIMIT_0        0x00
#define HAL_ADC_VDD_LIMIT_1        0x01
#define HAL_ADC_VDD_LIMIT_2        0x02
#define HAL_ADC_VDD_LIMIT_3        0x03
#define HAL_ADC_VDD_LIMIT_4        0x04
#define HAL_ADC_VDD_LIMIT_5        0x05
#define HAL_ADC_VDD_LIMIT_6        0x06
#define HAL_ADC_VDD_LIMIT_7        0x07
#define HAL_ADC_VDD_LIMIT_8        0x08

/**************************************************************************************************
 *                                            TYPEDEFS
 **************************************************************************************************/


/**************************************************************************************************
 *                                        GLOBAL VARIABLES
 **************************************************************************************************/


/**************************************************************************************************
 *                                        FUNCTIONS - API
 **************************************************************************************************/

/*
 * Initialize ADC Service
 */
extern void HalAdcInit ( void );

/*
 * Read value from a specified ADC Channel at the given resolution
 */
extern uint16 HalAdcRead ( uint8 channel, uint8 resolution );

extern uint16 HalAdcRead2 ( uint8 channel, uint8 resolution, uint8 reference);

extern  bool HalAdcCheckVdd (uint8 limit);

#if ((defined HAL_AUDIO) && (HAL_AUDIO == TRUE)) || (defined MINE_CONTROLLER)|| (defined GASMONITOR_PROJECT)
extern uint8  HalAdcMeasureVdd (void);
extern uint8 HalAdcPhoneCheckVdd(uint8 limit);
#endif
/**************************************************************************************************
**************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
