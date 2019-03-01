/**************************************************************************************************
Filename:       med.h
Revised:        $Date: 2010/12/24 06:48:25 $
Revision:       $Revision: 1.18 $

Description:    This file contains the the Mac EndDevice Application protypes and definitions
**************************************************************************************************/

#ifndef MACENDDEVICEAPP_H
#define MACENDDEVICEAPP_H

#ifdef __cplusplus
extern "C"
{
#endif

	/**************************************************************************************************
	* INCLUDES
	**************************************************************************************************/
#include "hal_types.h"
#include "hal_defs.h"
#include "App_cfg.h"
#include "AppProtocol.h"
	/**************************************************************************************************
	*                                        User's  Defines
	**************************************************************************************************/  

#ifdef DEBUG
#define MED_KEY_INT_ENABLED     FALSE       
#else
#define MED_KEY_INT_ENABLED     TRUE        
#endif
	/*
	* FALSE = Key Polling
	* TRUE  = Key interrupt
	*
	* Notes: Key interrupt will not work well with 2430 EB because
	*        all the operations using up/down/left/right switch will
	*        no longer work. Normally S1 + up/down/left/right is used
	*        to invoke the switches but on the 2430 EB board,  the
	*        GPIO for S1 is used by the LCD.
	*/


#define APS_DST_ENDPOINT      MINEAPP_ENDPOINT //should be the same to the full-stack app setting, now is MINE_APP_ENDPOINT.
#define APS_SRC_ENDPOINT      0x20  //FIXME:Get from sniffer, If not so, the full stack node will reject the pkt.
#define APS_CLUSTER_ID          CARD_CLUSTERID //should be the same to the full-stack app setting, now is MINE_APP_CLUSTERID.
#define APS_PROFILE_ID           MINEAPP_APS_PROFID //should be the same to the full-stack setting.


	/**************************************************************************************************
	* CONSTANTS
	**************************************************************************************************/

	/* Event IDs */
//#define MED_KEY_EVENT      0x0001
#define MED_ALERT_EVENT   0x0001
#define MED_BLAST_EVENT   0x0002
#define MED_SLEEP_EVENT   0x0004
#ifdef USE_STATE_UNINIT
#define MED_UNINIT_EVENT  0x0008
#endif

#define MED_SEND_RECVRETREAT_EVT   0x0010
#define MED_SEND_KEYCONFIRM_EVT   0x0020

	//#define MED_POLL_EVENT     0x0008
	//#define MED_URGENT_EVENT 0x0010

	/**************************************************************************************************
	* GLOBALS
	**************************************************************************************************/
	extern uint8 MED_TaskId;

	/*********************************************************************
	* FUNCTIONS
	*/

	/*
	* Task Initialization for the Mac Sample Application
	*/
	extern void MED_Init( uint8 task_id );

	/*
	* Task Event Processor for the Mac Sample Application
	*/
	extern uint16 MED_ProcessEvent( uint8 task_id, uint16 events );

	/*
	* Handle keys
	*/
	extern void MED_HandleKeys( uint16 keys, uint8 shift );

	/*
	* Handle power saving
	*/
	// extern void MED_PowerMgr (uint8 mode);

	/*********************************************************************
	*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* MACENDDEVICEAPP_H */
