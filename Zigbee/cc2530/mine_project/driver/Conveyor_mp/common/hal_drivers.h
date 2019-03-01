/**************************************************************************************************
  Filename:       hal_drivers.h
  Revised:        $Date: 2011/08/10 17:48:14 $
  Revision:       $Revision: 1.1 $

  Description:    This file contains the interface to the Drivers service.


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

#ifndef HAL_DRIVER_H
#define HAL_DRIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

	/**************************************************************************************************
	 * INCLUDES
	 **************************************************************************************************/

	/**************************************************************************************************
	 * MACROS
	 **************************************************************************************************/
	 
	/**************************************************************************************************
	 * CONSTANTS
	 **************************************************************************************************/
#define HAL_KEY_EVENT         0x0001
#define HAL_LED_BLINK_EVENT   0x0002
#define HAL_SLEEP_TIMER_EVENT 0x0004

#if (defined HAL_AUDIO) && (HAL_AUDIO == TRUE)
#define HAL_AUDIO_START_EVT    0x0008
#define HAL_AUDIO_STOP_EVT     0x0010
#define HAL_RING_EVENT         0x0020
#define HAL_VOICEBELL_EVENT    0x0080
#endif
#if ((defined HAL_AUDIO) && (HAL_AUDIO == TRUE))
#define HAL_LCD_PERSIST_EVT     0x0040
#endif

	/**************************************************************************************************
	 * TYPEDEFS
	 **************************************************************************************************/


	/**************************************************************************************************
	 * GLOBAL VARIABLES
	 **************************************************************************************************/
	extern uint8 Hal_TaskID;

	/**************************************************************************************************
	 * FUNCTIONS - API
	 **************************************************************************************************/

	/**************************************************************************************************
	 * Serial Port Initialization
	 **************************************************************************************************/
	extern void Hal_Init ( uint8 task_id );

	/*
	 * Process Serial Buffer
	 */
	extern uint16 Hal_ProcessEvent ( uint8 task_id, uint16 events );

	/*
	 * Process Polls
	 */
	extern void Hal_ProcessPoll ( void );

	/*
	 * Initialize HW
	 */
	extern void HalDriverInit ( void );

#if ((defined HAL_AUDIO) && (HAL_AUDIO == TRUE))

	void HalResetBackLightEvent ( void );

	void Hal_SetKeyActive ( bool val );

	void Hal_SetCellSwitchStat ( bool val );

	bool Hal_AllowSleep ( void );
#endif

	uint8 Hal_SendDataToAir ( uint8 *p, uint8 len, uint16 dstPan, uint16 dstaddr, bool ack, bool retrans );


	void Hal_EndVoiceBell ( void );

	void Hal_StartVoiceBell ( uint8 bell_name );

        bool Hal_IsVoiceBellPlaying(void);

	void Hal_RingStart ( uint8 ringname, uint8 openflag );

	void Hal_RingStop ( void );

	/**************************************************************************************************
	**************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
