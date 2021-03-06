#ifndef MINEAPP_MENULIB_H
#define MINEAPP_MENULIB_H

#include "hal_types.h"
#include "AppProtocolWrapper.h"

//#define SMS_MAX_NUM 6
#define MAX_CALL_NUM     10
#define MAX_CONTACT_NUM     40
#define MAX_SMS_NUM                       10
#define MAX_SMS_NUM_TEMPLATE    3//5
#ifdef SMS_SENDBOX
#define MAX_SMS_NUM_SENDBOX     5
#endif
#define NWK_STAT_INIT 	0
#define NWK_STAT_NORMAL 1
#define NWK_STAT_SWITCH 2
enum KeyStatus
{
	KEY_SHORT_PRESS,
      KEY_LONG_PRESS,
};
enum menu_msg
{
     MSG_INIT_NWK,
     MSG_INIT_MAIN,
     MSG_INCOMING_CALL,
     MSG_DIALING_SUCCESS,
     MSG_MISSED_CALL,
     MSG_NO_POWER,
     MSG_PAD_LOCK,
     MSG_VOICE_FINISH,
     MSG_SMS_SUCCESS,
     MSG_SMS_FAILED,
     MSG_SMS_INCOMING,
     MSG_POLL_END,
     MSG_POLL_START,
     MSG_REFRESH_SCREEN,
     MSG_POWERON_ANIM,
     MSG_POWEROFF_ANIM,
};

extern bool    Menu_Get_SMS_Full_Ring_Flag(void);
extern void    Menu_Set_SMS_Full_Ring_Flag(bool flag);
extern uint8* Menu_Get_SMS_Data(void);
extern uint8   Menu_Get_SMS_Len(void);
extern uint8 Menu_Init(void);
extern uint8* Menu_GetNumBuf(void);
extern uint8* Menu_GetDialNumBuf(void);
extern void Menu_handle_key(uint8 keys, uint8 status);
extern void Menu_handle_msg(uint8 MSG, uint8 *p, uint8 len);

extern void Menu_UpdateTime(void);
extern void Menu_UpdateSignal(uint8 level);
extern void Menu_UpdateBattery(uint8 level);
#ifdef 	RSSI_INFORMATION
extern void Menu_UpdateRSSI(uint8 lqi);
extern void Menu_UpdateLinkFlag(bool flag);
#endif
#ifdef PACKAGE_INFORMATION
extern void Menu_UpdatePackage( uint16 recvpagenum, uint16 errpacknum);
#endif
extern uint8 Save_New_SMS(APPWrapper_t*sms);
extern uint8 Get_SMS_Quantity(void);

extern void Menu_UpdateNwkLogo(void);
extern void Menu_SearchNwkFinish(void);
extern void Menu_ProcessMenuLibEvt(void);

extern void Menu_SetNwkStat(uint8 val);
extern uint8 Menu_GetNwkStat(void);
#endif
