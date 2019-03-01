#include "OSAL.h"
#include "OSAL_Nv.h"
#include "App_cfg.h"
#include "string.h"
#include "hal_drivers.h"
#include "hal_key_cfg.h"
#include "hal_key.h"
#include "hal_audio.h"
#include "lcd_serial.h"
#include "TimeUtil.h"
#include "MacUtil.h"

#include "MenuLib_orphan.h"
#include "MenuLib_global.h"
#include "MineApp_MenuLib.h"
#include "MineApp_global.h"
#include "MineApp_Local.h"
#include "MineApp_MenuLibChinese.h"
#include "MenuChineseInputUtil.h"
#include "MenuAdjustUtil.h"
#include "MineApp_MP_Function.h"
#include "MenuLib_Nv.h"
#ifdef CELLSWITCH_DEBUG
#include "OnBoard.h"
#endif

#include "Lcd_aging_check.h"

static  uint8             sig_index = 0;
static  uint8             bat_index = FULL_BAT;

typedef struct
{
    uint8 ID;
    char* name;
    uint8 ParentID;
    uint8 FirstChildID;
    uint8 ChildNum;
    char* const __code *ItemName_CH;
    MenuOper_t oper;
} Orphan_node_t;

/*dial number*/

/*identical functions */
static void    menu_poweron_animation_display(void);
static void    menu_poweroff_animation_display(void);
static void    menu_initnwk_display(void);
//static void    menu_initnwk_onkey(uint8 keys, uint8 status);
static void    menu_showing_number_display(void);
static void    menu_showing_number_onkey(uint8 keys, uint8 status);
static void    menu_dialing_display(void);
static void    menu_dialing_onkey(uint8 keys, uint8 status);
static void    menu_talking_display(void);
static void    menu_talking_onkey(uint8 keys, uint8 status);
static void    menu_incomingcall_display(void);
static void    menu_incomingcall_onkey(uint8 keys, uint8 status);
static void    menu_missingcall_display(void);
static void    menu_sm_sending_display(void);
static void    menu_missingcall_onkey(uint8 keys, uint8 status);

static void    menu_callrecord_detail_onkey(uint8 keys, uint8 status);
static void    menu_incomingmessage_display(void);
static void    menu_incomingmessage_onkey(uint8 keys, uint8 status);

static void    menu_inputname_display(void);
static void    menu_inputname_onkey(uint8 keys, uint8 status);
static void    menu_inputnumber_display(void);
static void    menu_inputnumber_onkey(uint8 keys, uint8 status);
static void    menu_inputnumber_sms_onkey(uint8 keys, uint8 status);
static void    menu_inputnumber_contact_onkey(uint8 keys, uint8 status);

static void    menu_showmessage_display(void);
//static void    menu_showmessage_onkey(uint8 keys, uint8 status);
static void    menu_showquestion_display(void);
static void    menu_showquestion_onkey(uint8 keys, uint8 status);
static void    menu_canlendar_display(void);
static void    menu_canlendar_onkey(uint8 keys, uint8 status);
static void    menu_main_display(void);
static void    menu_main_onkey(uint8 keys, uint8 status);

/*internal functions */
#ifdef CELLSWITCH_DEBUG
void menu_ShowCellSwitch(uint16 panID);
#endif

/*Menu Defination */
static Orphan_node_t const __code Menu_Orphan[] =
{
    {
        .ID = MENU_ID_ROOT,
        .oper.display = NULL,
        .oper.on_key = NULL,
    }
    ,
    /*independent ID in the root, parrelled with main menu*/
    {
        .ID = MENU_ID_POWERON_ANIMATION,
        .oper.display = menu_poweron_animation_display,
        .oper.on_key = NULL,
    }
    ,
    {
        .ID = MENU_ID_POWEROFF_ANIMATION,
        .oper.display = menu_poweroff_animation_display,
        .oper.on_key = NULL,
    }
    ,
    {
        .ID = MENU_ID_INITNWK,
        .oper.display = menu_initnwk_display,
        .oper.on_key = NULL,
    }
    ,
    {
        .ID = MENU_ID_LONGTIME_CLOCK,
        .oper.display = NULL,
        .oper.on_key = NULL,
    }
    ,
    {
        .ID = MENU_ID_SHOWING_NUMBER,
        .oper.display = menu_showing_number_display,
        .oper.on_key = menu_showing_number_onkey
    }
    ,
    {
        .ID = MENU_ID_DIALING,
        .oper.display = menu_dialing_display,
        .oper.on_key = menu_dialing_onkey
    }
    ,
    {
        .ID = MENU_ID_TALKING,
        .oper.display = menu_talking_display,
        .oper.on_key = menu_talking_onkey
    }
    ,
    {
        .ID = MENU_ID_INCOMINGCALL,
        .oper.display = menu_incomingcall_display,
        .oper.on_key = menu_incomingcall_onkey
    }
    ,
    {
        .ID = MENU_ID_MISSINGCALL,
        .oper.display = menu_missingcall_display,
        .oper.on_key = menu_missingcall_onkey
    }
    ,
    {
        .ID = MENU_ID_INCOMINGSMS,
        .oper.display = menu_incomingmessage_display,
        .oper.on_key = menu_incomingmessage_onkey,
    }
    ,
    {
        .ID = MENU_ID_MISSINGMESSAGE,
        .oper.display = NULL,
        .oper.on_key = NULL
    }
    ,
    {
        .ID = MENU_ID_SM_SENDING,
        .oper.display = menu_sm_sending_display,
        .oper.on_key = NULL
    }
    ,
    {
        .ID = MENU_ID_CALLRECORD_DETAIL,
        .oper.display = NULL,
        .oper.on_key = menu_callrecord_detail_onkey
    }
    ,
    {
        .ID = MENU_ID_TYPEING,
        .oper.display = NULL,
        .oper.on_key = NULL
    }
    ,
    {
        .ID = MENU_ID_INPUTCHINESE,
        .oper.display = NULL,//menu_inputchinese_display,
        .oper.on_key = NULL,//menu_inputchinese_onkey
    }
    ,
    {
        .ID = MENU_ID_INPUTNAME,
        .oper.display = menu_inputname_display,
        .oper.on_key = menu_inputname_onkey
    }
    ,
    {
        .ID = MENU_ID_INPUTNUMBER_SMS,
        .oper.display = menu_inputnumber_display,
        .oper.on_key = menu_inputnumber_sms_onkey,
    }
    ,
    {
        .ID = MENU_ID_INPUTNUMBER_CONTACT,
        .oper.display = menu_inputnumber_display,
        .oper.on_key = menu_inputnumber_contact_onkey,
    }
    ,
    {
        .ID = MENU_ID_INPUTSYMBOL,
        .oper.display = NULL,
        .oper.on_key = NULL
    }
    ,
    {
        .ID = MENU_ID_SHOWMESSAGE,
        .oper.display = menu_showmessage_display,
        .oper.on_key = NULL//menu_showmessage_onkey
    }
    ,
    {
        .ID = MENU_ID_SHOWQUESTION,
        .oper.display = menu_showquestion_display,
        .oper.on_key = menu_showquestion_onkey
    }
    ,
    {
        .ID = MENU_ID_SHOWALERT,
        .oper.display = NULL,
        .oper.on_key = NULL
    }
    ,
    {
        .ID = MENU_ID_BUSY,
        .oper.display = NULL,
        .oper.on_key = NULL
    }
    ,
    {
        .ID = MENU_ID_ADJUSTVOLUME,
        .oper.display = menu_adjustvol_display,
        .oper.on_key = menu_adjustvol_onkey
    }
    ,
    {
        .ID = MENU_ID_ADJUSTTIME,
        .oper.display = menu_adjusttime_display,
        .oper.on_key = menu_adjusttime_onkey
    }
    ,
    {
        .ID = MENU_ID_ADJUSTDATE,
        .oper.display = menu_adjustdate_display,
        .oper.on_key = menu_adjustdate_onkey
    }
    ,
    {
        .ID = MENU_ID_CANLENDAR,
        .oper.display = menu_canlendar_display,
        .oper.on_key = menu_canlendar_onkey
    }
    ,
    {
        .ID = MENU_ID_MAIN,
        .oper.display = menu_main_display,
        .oper.on_key = menu_main_onkey
    }

};
/*------------------- functions ----------------------------*/
void    menu_orphan_nodeID_check()
{
    uint8 len = sizeof(Menu_Orphan)/sizeof(Menu_Orphan[0]);
    for(uint8 i =0; i<len; i++)
    {
        if(Menu_Orphan[i].ID != GetIDFromIdx(NODE_TYPE_ORPHAN,i))
        {
            LcdClearDisplay();
            LCD_Str_Print("Menu Node ID Incorrect!", 0, 0, TRUE);
            while(1);
        }
    }
}
void menu_orphan_handle_key(uint8 keys, uint8 status)
{
    if(GetTypeFromID(CurrentNodeID) == NODE_TYPE_ORPHAN)
    {
        uint8 idx = GetIdxFromID(CurrentNodeID);
        if(Menu_Orphan[idx].oper.on_key)
            Menu_Orphan[idx].oper.on_key(keys,status);
    }
}

void menu_orphan_display()
{
    if(GetTypeFromID(CurrentNodeID) == NODE_TYPE_ORPHAN)
    {
        uint8 idx = GetIdxFromID(CurrentNodeID);
        if(Menu_Orphan[idx].oper.display)
            Menu_Orphan[idx].oper.display();
    }
}

void menu_set_signal(uint8 idx)
{
    if(idx < MAX_SIG)
    {
        sig_index = idx;
    }
}
void menu_set_battery(uint8 idx)
{
    if(idx<MAX_BAT)
    {
        bat_index = idx;
    }
}

/*identical functions */
static void    menu_poweron_animation_display(void)
{
    LcdClearDisplay();
    LCD_Str_Print(POWERON_CHINA, 3, 1, TRUE);
}

static void    menu_poweroff_animation_display(void)
{
    LcdClearDisplay();
    LCD_Str_Print(POWEROFF_CHINA, 3, 1, TRUE);
}

static void    menu_initnwk_display(void)
{
    if(CurrentNodeID != MENU_ID_INITNWK)
        return;
    LcdClearDisplay();
    LCD_Str_Print(NWK_INIT_CHINA, 3, 1, TRUE);
}

static void    menu_showing_number_display(void)
{
    uint8 x_start;

    LcdClearDisplay();
    x_start = (num_buf.len < LCD_LINE_WIDTH) ? ((LCD_LINE_WIDTH-num_buf.len)/2): 0;
    //LCD_Str_Print(num_buf.p , 0 , 3, FALSE);
    LCD_Memory_Print(num_buf.p, num_buf.len, x_start, 2);
}
static void    menu_showing_number_onkey(uint8 keys, uint8 status)
{
    switch(keys)
    {
    case HAL_KEY_0:
    case HAL_KEY_1:
    case HAL_KEY_2:
    case HAL_KEY_3:
    case HAL_KEY_4:
    case HAL_KEY_5:
    case HAL_KEY_6:
    case HAL_KEY_7:
    case HAL_KEY_8:
    case HAL_KEY_9:
        if(num_buf.len < NMBRDIGIT - 1)
        {
            num_buf.p[num_buf.len++] = Key2ASCII(keys);
            num_buf.p[num_buf.len] = '\0';
            menu_display();
        }
        break;
    case HAL_KEY_CALL:
        Buffer_Copy(&incoming_buf,&num_buf);
        menu_Dial(incoming_buf);
        break;
    case HAL_KEY_SELECT:
        Stack_Push(&global_stack, CurrentNodeID, NULL);
        shortcuts_flag = TRUE;
        menu_JumptoMenu(MENU_ID_INPUTNAME);
        break;
    case HAL_KEY_BACKSPACE:
        num_buf.p[--num_buf.len]  = '\0';
        if(num_buf.len == 0)
        {
            menu_JumptoMenu(MENU_ID_MAIN);
        }
        else
        {
            menu_display();
        }
        break;
    default:
        break;
    }
}
static void    menu_dialing_display(void)
{
    //uint8 len;
    LcdClearDisplay();
    LCD_Signal_Show(sig_index);
    LCD_Battery_Show(bat_index);
    LCD_Str_Print(CALLING_CHINA, 3, 1, TRUE);
    //len = (uint8)osal_strlen((char*)C_num);
    Contact_Node c_node;
    if(ZSuccess==menu_Contact_SearchContactByNum(&c_node, NULL, incoming_buf.p))
    {
        LCD_Str_Print(c_node.name, (16-strlen((char *)c_node.name))/2, 2, TRUE);
    }
    else
    {
        if(incoming_buf.len <= LCD_LINE_WIDTH)
            LCD_Str_Print(incoming_buf.p, (16-incoming_buf.len)/2, 2, TRUE);
        else
            LCD_Str_Print(incoming_buf.p, 0, 2, TRUE);
    }
}
static void    menu_dialing_onkey(uint8 keys, uint8 status)
{
    switch(keys)
    {
    case HAL_KEY_CALL:
        SET_ON_CALLINGWAIT();
        break;
    case HAL_KEY_RIGHT:
    case HAL_KEY_LEFT:
        menu_JumpandMark(MENU_ID_ADJUSTVOLUME);
        //osal_start_timerEx(MineApp_Function_TaskID, MINEAPP_MENULIB_EVENT, 1000);
        MineApp_StartMenuLibEvt(1000);
        break;
    default:
        break;
    }

}
static void    menu_talking_display(void)
{
    LcdClearDisplay();
    LCD_Signal_Show(sig_index);
    LCD_Battery_Show(bat_index);
    LCD_Str_Print(VOICING_CHINA, 4, 1, TRUE);

    Contact_Node c_node;
    uint16 num_len;

    char *pNmbr = MineApp_NodeAttr.peer_termnmbr.Nmbr;
    if(pNmbr!=NULL && (num_len = strlen(pNmbr))!=0)
    {
        if(ZSuccess == menu_Contact_SearchContactByNum(&c_node, NULL, pNmbr))
        {
            LCD_Str_Print(c_node.name, (16 - strlen((char*)c_node.name)) / 2, 2, TRUE);
        }
        else
        {
            LCD_Str_Print(pNmbr, (16 - num_len) / 2, 2, TRUE);
        }
    }
#ifdef CELLSWITCH_DEBUG
    menu_ShowCellSwitch(_NIB.nwkPanId);
#endif
}
static void    menu_talking_onkey(uint8 keys, uint8 status)
{
    switch(keys)
    {
    case HAL_KEY_RIGHT:
    case HAL_KEY_LEFT:
        menu_JumpandMark(MENU_ID_ADJUSTVOLUME);
        //osal_start_timerEx(MineApp_Function_TaskID, MINEAPP_MENULIB_EVENT, 1000);
        MineApp_StartMenuLibEvt(1000);
        break;
    default:
        break;
    }
}
static void    menu_incomingcall_display(void)
{

    LcdClearDisplay();
    LCD_Signal_Show(sig_index);
    LCD_Battery_Show(bat_index);
    LCD_Str_Print(INCOMING_CHINA, 6, 2, TRUE);

    Contact_Node c_node;
    if(ZSuccess==menu_Contact_SearchContactByNum(&c_node, NULL, incoming_buf.p))
    {
        LCD_Str_Print(c_node.name, (LCD_LINE_WIDTH-strlen((char*)c_node.name))/2, 1, TRUE);
    }
    else
    {
        LCD_Str_Print(incoming_buf.p, (LCD_LINE_WIDTH-incoming_buf.len)/2, 1, TRUE);
    }

}
static void    menu_incomingcall_onkey(uint8 keys, uint8 status)
{
    Record new_record;

    switch(keys)
    {
    case HAL_KEY_CALL:
        menu_JumptoMenu(MENU_ID_TALKING);
        if(incoming_buf.p != NULL)
        {
            GetTimeChar(new_record.time);
            osal_memcpy(new_record.num, incoming_buf.p, incoming_buf.len+1);
            //Clr_Num_Buf();
            Add_CallRecord(MENU_ID_CALLRECORD_ANSWEREDCALL, &new_record);
        }
        //Buffer_Free(&incoming_buf);
        break;
    case HAL_KEY_CANCEL:
        //menu_JumptoMenu(MENU_ID_MAIN);
        if(incoming_buf.p != NULL)
        {
            GetTimeChar(new_record.time);
            osal_memcpy(new_record.num, incoming_buf.p, incoming_buf.len+1);
            Add_CallRecord(MENU_ID_CALLRECORD_ANSWEREDCALL, &new_record);
        }
        //Buffer_Free(&incoming_buf);
        break;
    default:
        break;
    }


}

static void    menu_missingcall_display(void)
{
    uint8 len;

    LcdClearDisplay();
    len = LCD_ID_Show(missed_call_amount, 3, 2);
    LCD_Str_Print(MISSED_CALL_MENU_CHINA, len+3, 2, TRUE);
}

static void    menu_missingcall_onkey(uint8 keys, uint8 status)
{
    switch(keys)
    {
    case HAL_KEY_SELECT:
        shortcuts_flag = TRUE;
        missed_call_amount = 0;
        menu_JumptoMenu(MENU_ID_CALLRECORD_MISSEDCALL);
        break;
    case HAL_KEY_BACKSPACE:
        shortcuts_flag = FALSE;
        missed_call_amount = 0;
        menu_JumptoMenu(MENU_ID_MAIN);
        break;
    case HAL_KEY_CANCEL:
        shortcuts_flag = FALSE;
        missed_call_amount = 0;
        break;
    default:
        break;
    }
    //Buffer_Free(&incoming_buf);
}

static void    menu_callrecord_detail_onkey(uint8 keys, uint8 status)
{
    switch(keys)
    {
    case HAL_KEY_CALL:
        Buffer_Copy(&incoming_buf, &num_buf);
        menu_Dial(incoming_buf);
        break;
    case HAL_KEY_SELECT:
        menu_JumptoMenu(MENU_ID_SHOWING_NUMBER);
        break;
    case HAL_KEY_BACKSPACE:
        menu_JumpBackWithMark();
        break;
    default:
        break;
    }
}


static void    menu_incomingmessage_display(void)
{
#if 0
    LcdClearDisplay();
    LCD_SMS_ICON_Show(LCD_LINE_WIDTH-4, 0);
    LCD_Str_Print(NEW_SMS_CHINA, 5, 1, TRUE);
    LCD_Str_Print(CHECK_CHINA, 0, 3, TRUE);
    LCD_Str_Print(CANCEL_CHINA, 12, 3, TRUE);
#endif    
}

static void    menu_incomingmessage_onkey(uint8 keys, uint8 status)
{
    switch(keys)
    {
    case HAL_KEY_SELECT:
        if(NearLastNodeID != MENU_ID_SHORTMESSAGE_INCOMINGBOX)
            shortcuts_flag = TRUE;
        menu_JumptoMenu(MENU_ID_SHORTMESSAGE_INCOMINGBOX);
        break;
    case HAL_KEY_BACKSPACE:
        menu_JumptoMenu(MENU_ID_MAIN);
        break;
    default:
        break;
    }
}

static void    menu_sm_sending_display(void)
{
    LcdClearDisplay();
    LCD_Str_Print(SM_SENDING_CHINA, 3, 1, TRUE);
}

static void    menu_inputname_display(void)
{
#if 0
    if(NULL== data_buf.p)
    {
        if(NULL == Buffer_Init(&data_buf, MAX_NAME_LEN+1))
            return;
    }
    menu_inputchinese_display();
    LCD_ShowCursor(0, 1);
    if(data_buf.len > 0)
    {
        LCD_SMS_Print(data_buf.p, data_buf.len, 0, 1);
        LCD_ShowCursor(data_buf.len, 1);
    }
 #endif   
}

static bool    menu_inputname_output_handle(uint8 keys, uint8 input_status)
{
#if 0
    if(input_status == OUTPUT_STATUS)
    {
        if(keys == HAL_KEY_BACKSPACE)
        {
            if(data_buf.len == 0)
            {
                LCD_CloseCursor();
                Buffer_Free(&data_buf);
                Stack_Pop(&global_stack, &CurrentNodeID, NULL);
                menu_display();
                //menu_JumptoMenu(NearLastNodeID);
            }
            else
            {
                if(data_buf.p[data_buf.len-1] > 0x80)
                {
                    data_buf.p[--data_buf.len] = '\0';
                    data_buf.p[--data_buf.len] = '\0';
                    LCD_Clear(data_buf.len, 1, data_buf.len+2, 1);
                }
                else
                {
                    data_buf.p[--data_buf.len] = '\0';
                    LCD_Clear(data_buf.len, 1, data_buf.len+1, 1);
                }
                LCD_ShowCursor(data_buf.len%LCD_LINE_WIDTH, 1);
            }

            return TRUE;
        }
        else if(keys == HAL_KEY_SELECT)
        {
            if(data_buf.len > 0)
            {
                uint8 contact_num = 0;
                LCD_CloseCursor();
#ifdef NEW_DOUBLE_NVID_OP
                menu_Contact_ReadContactNum(&contact_num);
#else
                osal_nv_read(MINEAPP_NV_CONTACT, 0, 1, &contact_num);
#endif
                if(contact_num >= MAX_CONTACT_NUM)
                {
                    strcpy((char *)g_jump_buf,FULL_CONTACTLIST_CHINA);
                    menu_JumptoMenu(MENU_ID_SHOWMESSAGE);
                    MineApp_StartMenuLibEvt(1000);
                }
                else
                {
#ifdef NEW_DOUBLE_NVID_OP
                    if(strlen((char*)num_buf.p) < NMBRDIGIT && strlen((char*)data_buf.p) < MAX_NAME_LEN)
                    {
                        Contact_Node Node;
                        strcpy((char*)Node.num, (char*)num_buf.p);
                        strcpy((char*)Node.name,(char*)data_buf.p);
                        menu_Contact_AddContact(&Node);
                    }
#else
                    osal_nv_write(MINEAPP_NV_CONTACT, contact_num*sizeof(Contact_Node)+1, num_buf.len+1, num_buf.p);
                    osal_nv_write(MINEAPP_NV_CONTACT, contact_num*sizeof(Contact_Node)+1+NMBRDIGIT, data_buf.len+1, data_buf.p);
                    ++contact_num;
                    osal_nv_write(MINEAPP_NV_CONTACT, 0, 1, &contact_num);
                    //shortcuts_flag = TRUE;
#endif
                    Buffer_Free(&data_buf);
                    Stack_Clear(&global_stack);
                    menu_JumptoMenu(MENU_ID_CONTACTLIST);
                }
            }
            return TRUE;
        }
    }
    return FALSE;
#endif    
}


static void    menu_inputname_onkey(uint8 keys, uint8 status)
{
    static uint8 input_status = OUTPUT_STATUS;
    uint8 *output_p = NULL;

    //it will return after handle the character of name
    if(menu_inputname_output_handle(keys, input_status))
        return;

    //input new character for name of contact
    input_status = menu_inputchinese_onkey(keys, status);

    //if the status of input function is OUTPUT_STATUS, the new character
    //should be print on the LCD
    if(input_status == OUTPUT_STATUS)
    {
        output_p = menu_ChineseOutput();

        if(menu_ChineseOutput_Length() > 0)
        {
            uint8 len_output = 0;

            if((data_buf.len >= (MAX_NAME_LEN-1)) || ((data_buf.len >= (MAX_NAME_LEN-2))&&(output_p[0]>0x80)))
            {
                strcpy((char *)g_jump_buf, NAME_INPUT_FULL_CHINA);
                menu_JumpandMark(MENU_ID_SHOWMESSAGE);
                return;
            }
            len_output = osal_strlen((char*)output_p);
            osal_memcpy(data_buf.p+data_buf.len, output_p, len_output);
            data_buf.p[data_buf.len+len_output] = '\0';
            LCD_Str_Print(output_p, data_buf.len, 1, TRUE);
            data_buf.len += len_output;
            menu_ChineseOutputClear();
        }
        LCD_ShowCursor(data_buf.len%LCD_LINE_WIDTH, 1);
    }
}

static void    menu_inputnumber_display(void)
{
    LCD_Clear(8, 0, 16, 3);
    LCD_Str_Print(INPUT_NUM_CHINA, 0, 0, TRUE);
    LCD_Str_Print(num_buf.p , 0 , 3, FALSE);
    LCD_ShowCursor(LCD_LINE_WIDTH-1, 3);
}

static void    menu_inputnumber_sms_onkey(uint8 keys, uint8 status)
{
    if(keys == HAL_KEY_SELECT)
    {
        if(num_buf.len > 0)
        {
            LCD_CloseCursor();
            //set shortmessage sending status
            SET_ON_SM_SENDING();
            Stack_Pop(&global_stack, &CurrentNodeID, NULL);
            menu_JumptoMenu(MENU_ID_SM_SENDING);
        }
    }
    else
    {
        menu_inputnumber_onkey( keys,  status);
    }
}
static void    menu_inputnumber_contact_onkey(uint8 keys, uint8 status)
{
    if(keys == HAL_KEY_SELECT)
    {
        if(num_buf.len > 0)
        {
            LCD_CloseCursor();
            Stack_Push(&global_stack, CurrentNodeID, NULL);
            menu_JumptoMenu(MENU_ID_INPUTNAME);
        }
    }
    else
    {
        menu_inputnumber_onkey( keys,  status);
    }

}

static void    menu_inputnumber_onkey(uint8 keys, uint8 status)
{

    switch(keys)
    {
    case HAL_KEY_0:
    case HAL_KEY_1:
    case HAL_KEY_2:
    case HAL_KEY_3:
    case HAL_KEY_4:
    case HAL_KEY_5:
    case HAL_KEY_6:
    case HAL_KEY_7:
    case HAL_KEY_8:
    case HAL_KEY_9:
        if(num_buf.len < NMBRDIGIT-1)
        {
            num_buf.p[num_buf.len++] = Key2ASCII(keys);
            num_buf.p[num_buf.len] = '\0';
            menu_display();
        }
        break;
        /*
        case HAL_KEY_SELECT:
        if(num_buf.len > 0)
        {
        LCD_CloseCursor();

        if(NearLastNodeID == MENU_ID_SHORTMESSAGE_WRITINGBOX)
        {
        //set shortmessage sending status
        SET_ON_SM_SENDING();
        Stack_Pop(&global_stack, &CurrentNodeID, NULL);
        menu_JumptoMenu(MENU_ID_SM_SENDING);
        }
        #ifdef SMS_TEMPLATE
        else if(NearLastNodeID == MENU_ID_SMS_TEMPLATE_HANDLE)
        {
        //set shortmessage sending status
        SET_ON_SM_SENDING();
        Stack_Pop(&global_stack, &CurrentNodeID, NULL);
        menu_JumptoMenu(MENU_ID_SM_SENDING);
        }
        #endif
        #ifdef SMS_SENDBOX
        else if(NearLastNodeID == MENU_ID_SMS_EDIT_HANDLE)
        {
        //set shortmessage sending status
        SET_ON_SM_SENDING();
        Stack_Pop(&global_stack, &CurrentNodeID, NULL);
        menu_JumptoMenu(MENU_ID_SM_SENDING);
        }
        else if(NearLastNodeID == MENU_ID_SMS_SENDBOX_HANDLE)
        {
        //set shortmessage sending status
        SET_ON_SM_SENDING();
        Stack_Pop(&global_stack, &CurrentNodeID, NULL);
        menu_JumptoMenu(MENU_ID_SM_SENDING);
        }
        #endif
        else if(NearLastNodeID == MENU_ID_MAIN ||NearLastNodeID == MENU_ID_CONTACT_HANDLE)
        {
        //Stack_Pop(&global_stack, &CurrentNodeID, &node_info);
        Stack_Push(&global_stack, CurrentNodeID, NULL);
        //shortcuts_flag = TRUE;
        menu_JumptoMenu(MENU_ID_INPUTNAME);
        }
        else
        {
        Stack_Clear(&global_stack);
        menu_JumptoMenu(MENU_ID_MAIN);
        }
        }
        break;
        */
    case HAL_KEY_BACKSPACE:
        if(num_buf.len == 0)
        {
            Buffer_Clear(&num_buf);
            if(NearLastNodeID == MENU_ID_SHORTMESSAGE_WRITINGBOX)
            {
                Stack_Pop(&global_stack, &CurrentNodeID, NULL);
                //Pop_PipeLine();
                menu_display();
            }
#ifdef SMS_TEMPLATE
            else if(NearLastNodeID == MENU_ID_SMS_TEMPLATE_HANDLE)
            {
                Stack_Pop(&global_stack, &CurrentNodeID, NULL);
                Stack_Pop(&global_stack, &CurrentNodeID, NULL);
                menu_display();
            }
#endif
#ifdef SMS_SENDBOX
            else if(NearLastNodeID == MENU_ID_SMS_SENDBOX_HANDLE)
            {
                Stack_Pop(&global_stack, &CurrentNodeID, NULL);
                Stack_Pop(&global_stack, &CurrentNodeID, NULL);
                menu_display();
            }
#endif
            //else if(NearLastNodeID == MENU_ID_CONTACT_HANDLE)
            //else if(menu_GetJumpMark() == MENU_ID_CONTACTLIST)
            else
            {
                NearLastNodeID = CurrentNodeID;
                Stack_Pop(&global_stack, &CurrentNodeID, &node_info);
                menu_display();
                //menu_JumpBackWithMark();
            }
        }
        else
        {
            num_buf.p[--num_buf.len]  = '\0';
            menu_display();
        }
        break;
    default:
        break;
    }
}


static void    menu_showmessage_display(void)
{
    LcdClearDisplay();
    /* show message in global buffer, and jump back with mark after 1 second */
    LCD_Str_Print((uint8 *)g_jump_buf, (LCD_LINE_WIDTH-osal_strlen((char *)g_jump_buf))/2, 1, TRUE);
    //osal_start_timerEx(MineApp_Function_TaskID,MINEAPP_MENULIB_EVENT, 500);
    MineApp_StartMenuLibEvt(500);

}

/* question should be put into g_buf  first*/
static void    menu_showquestion_display(void)
{
    LcdClearDisplay();
    switch(menu_GetJumpMark())
    {
    case    MENU_ID_SETTINGS:
    {
        LCD_Str_Print((uint8 *)g_jump_buf, (LCD_LINE_WIDTH-osal_strlen((char *)g_jump_buf))/2, 1, TRUE);
        break;
    }
    case   MENU_ID_CALLRECORD_DELETE:
    {
        uint8 sel_item = g_jump_buf[0];
        LCD_Str_Print((uint8 *)DELETE_ALL_CHINA, 4, 0, TRUE);
        if(sel_item == 0)   // delete missed call?
        {
            LCD_Str_Print((uint8 *)DELETE_MISSEDCALL_CHINA, 4, 1, TRUE);
        }
        else if(sel_item == 1) // delete answered call?
        {
            LCD_Str_Print((uint8 *)DELETE_ANSWEREDCALL_CHINA, 4, 1, TRUE);
        }
        else if(sel_item == 2)  // delete dialed call?
        {
            LCD_Str_Print((uint8 *)DELETE_DIALEDCALL_CHINA, 4, 1, TRUE);
        }
        break;
    }
    }
    LCD_Str_Print(CONFIRM_CHINA, 0, 3, TRUE);
    LCD_Str_Print(CANCEL_CHINA, 12, 3, TRUE);
}

static void    menu_showquestion_onkey(uint8 keys, uint8 status)
{
    if(keys ==  HAL_KEY_SELECT)
    {
        switch(menu_GetJumpMark())
        {
        case    MENU_ID_SETTINGS:
        {
            set_info_t set_info;
            MP_SettingInformation_GetDefault(&set_info);
            MP_SettingInformation_Handout(&set_info);
            MP_SettingInformation_WriteFlash(&set_info);

            strcpy((char *)g_jump_buf,SETTED_CHINA);
            menu_JumptoMenu(MENU_ID_SHOWMESSAGE);

            break;
        }
        case   MENU_ID_CALLRECORD_DELETE:
        {
            uint8 delete_sel = g_jump_buf[0];
            uint8 pos = 0;
            if(delete_sel == 0)   //missed call
            {
                osal_nv_write(MINEAPP_NV_MISSED,0,1,&pos);
            }
            else if(delete_sel == 1)   //answered call
            {
                osal_nv_write(MINEAPP_NV_ANSWERED,0,1,&pos);
            }
            else if(delete_sel == 2)   //dialed call
            {
                osal_nv_write(MINEAPP_NV_DIALED,0,1,&pos);
            }
            strcpy((char *)g_jump_buf,DELETED_CHINA);
            menu_JumptoMenu(MENU_ID_SHOWMESSAGE);
            break;
        }
        }

    }
    else if(keys == HAL_KEY_BACKSPACE)
    {
        menu_JumpBackWithMark();
    }
}

static void    menu_canlendar_display(void)
{
#if 0
    uint8 date_str[DATE_LEN];
    uint8 week_str[16];

    LcdClearDisplay();
    GetDateChar(date_str);
    GetWeekChar(week_str);

    LCD_Str_Print(TODAYIS_CHINA, (LCD_LINE_WIDTH-osal_strlen(TODAYIS_CHINA))/2, 0, TRUE);
    LCD_Str_Print((uint8 *)date_str, (LCD_LINE_WIDTH-osal_strlen((char *)date_str))/2, 1, TRUE);
    LCD_Str_Print((uint8 *)week_str, (LCD_LINE_WIDTH-osal_strlen((char *)week_str))/2, 2, TRUE);
    LCD_Str_Print(CONFIRM_CHINA, 0, 3, TRUE);
#endif    
}
static void    menu_canlendar_onkey(uint8 keys, uint8 status)
{
    if(keys == HAL_KEY_SELECT)
    {
        menu_JumpBackWithMark();
    }
}
static void    menu_main_display(void)
{

#if 1
        Lcd_aging();    
#else
    uint16 len;

    LcdClearDisplay();
    LCD_Signal_Show(sig_index);
    LCD_Battery_Show(bat_index);

#ifdef NEW_DOUBLE_NVID_OP
    len = MAX_SMS_NUM/2*SMS_NV_LEN;
    osal_nv_read(MINEAPP_NV_SMS2, len, sizeof(uint8), &unread_sms);
#else
    len = sizeof(uint8)+ MAX_SMS_NUM*SMS_NV_LEN;
    osal_nv_read(MINEAPP_NV_SMS, len, sizeof(uint8), &unread_sms);
#endif
    if(unread_sms > 0)
        LCD_SMS_ICON_Show(LCD_LINE_WIDTH-4, 0);
    /*
    osal_nv_read(MINEAPP_NV_SMS, sizeof(uint8)+ MAX_SMS_NUM*SMS_NV_LEN, sizeof(uint8), &new_sms_flag);
    if(new_sms_flag)
    {
    LCD_SMS_ICON_Show();
    }*/
    Menu_UpdateTime();
    Buffer_Free(&data_buf);
    Buffer_Clear(&num_buf);
    Buffer_Clear(&incoming_buf);
    ///Stack_Clear(&tree_stack);
    Stack_Clear(&global_stack);
    if(Menu_GetNwkStat() == NWK_STAT_INIT)
    {
        LCD_Str_Print_Pixel(NONWK_CHINA, (LCD_LINE_WIDTH-osal_strlen(NONWK_CHINA))/2, 1.5*LCD_LINE_HIGH);
    }
    else //if normal or cell switch, show normal on screen
    {
        LCD_Str_Print_Pixel(LOG_CHINA, (LCD_LINE_WIDTH-osal_strlen(LOG_CHINA))/2, 1.5*LCD_LINE_HIGH);
    }
    menu_ChineseOutputClear();

    //if(HalGetPadLockEnable())
    {
        if(HalGetPadLockStat() == PADLOCK_LOCKED)
        {
            //LCD_BigAscii_Print(0x94, 3, 0);  //key icon
            LCD_Str_Print(OPENLOCK_CHINA, 0, 3, TRUE);
            //LCD_Str_Print(time_str, 5, 1, TRUE);
            if(unread_sms > 0)
                LCD_SMS_ICON_Show(LCD_LINE_WIDTH-4, 0);
            return ;
        }
        else if( HalGetPadLockStat() == PADLOCK_MID)
        {
            //LCD_BigAscii_Print(0x94, 3, 0);
            LCD_Line_Clear(1);
            LCD_Line_Clear(2);
            LCD_Str_Print(REQUESTSTAR_CHINA, 1, 1, TRUE);
            LCD_Str_Print(OPENLOCK_CHINA, 0, 3, TRUE);
            if(unread_sms > 0)
                LCD_SMS_ICON_Show(LCD_LINE_WIDTH-4, 0);
            return;
        }
        else if(HalGetPadLockStat() == PADLOCK_ALERT)
        {
            //LCD_BigAscii_Print(0x94, 3, 0);
            LCD_Line_Clear(1);
            LCD_Line_Clear(2);
            LCD_Str_Print(HOWTOUNLOCK1_CHINA, 0, 1, TRUE);
            LCD_Str_Print(HOWTOUNLOCK2_CHINA, 0, 2, TRUE);
            if(unread_sms > 0)
                LCD_SMS_ICON_Show(LCD_LINE_WIDTH-4, 0);
            return;
        }
    }

    {
        //LCD_Str_Print(time_str, 4, 0, TRUE);
        LCD_Str_Print(FUNCTIONLIST_CHINA, 0, 3, TRUE);
        LCD_Str_Print(CONTACTLIST_CHINA, 10, 3, TRUE);
        //LCD_BigAscii_Print(0xB0, 7, 3);
    }
#endif
}

static void    menu_main_onkey(uint8 keys, uint8 status)
{
#if 0
    //if(HalGetPadLockEnable())
    {
        NearLastNodeID = CurrentNodeID;
        uint8 stat = HalGetPadLockStat();
        if(stat == PADLOCK_LOCKED || stat == PADLOCK_ALERT)
        {
            if(keys == HAL_KEY_SELECT)
            {
                HalSetPadLockStat(PADLOCK_MID);
                MineApp_StartMenuLibEvt(3000);
            }
            else
            {
                HalSetPadLockStat(PADLOCK_ALERT);
                MineApp_StartMenuLibEvt(1000);
            }
            menu_display();
            return;
        }
        else if(stat == PADLOCK_MID)
        {
            if(keys == HAL_KEY_STAR)
            {
                HalSetPadLockStat(PADLOCK_UNLOCKED);
                //osal_stop_timerEx(MineApp_Function_TaskID, MINEAPP_MENULIB_EVENT);
                MineApp_StopMenuLibEvt();
            }
            else
            {
                HalSetPadLockStat(PADLOCK_ALERT);
                //osal_start_timerEx(MineApp_Function_TaskID, MINEAPP_MENULIB_EVENT, 1000);
                MineApp_StartMenuLibEvt(1000);
            }
            menu_display();
            return;
        }
    }

    /* other situations are normal*/
    {
        switch(keys)
        {
        case HAL_KEY_0:
        case HAL_KEY_1:
        case HAL_KEY_2:
        case HAL_KEY_3:
        case HAL_KEY_4:
        case HAL_KEY_5:
        case HAL_KEY_6:
        case HAL_KEY_7:
        case HAL_KEY_8:
        case HAL_KEY_9:
            num_buf.p[num_buf.len++] = Key2ASCII(keys);
            num_buf.p[num_buf.len] = '\0';
            menu_JumptoMenu(MENU_ID_SHOWING_NUMBER);
            break;
        case HAL_KEY_CALL:
            shortcuts_flag = TRUE;
            menu_JumptoMenu(MENU_ID_CALLRECORD_DIALEDCALL);
            break;
        case HAL_KEY_SELECT:
            menu_JumptoMenu(MENU_ID_FUNCTIONLIST);
            break;
        case HAL_KEY_BACKSPACE:
            shortcuts_flag = TRUE;
            menu_JumptoMenu(MENU_ID_CONTACTLIST);
            break;
        default:
            break;
        }
    }
 #endif
}


#ifdef CELLSWITCH_DEBUG
void menu_ShowCellSwitch(uint16 panID)
{
    LCD_Line_Clear(3);
    uint8 start_pos = 3;

    LCD_Str_Print(MP_BASESTATION_CHINA, start_pos, 3, TRUE);

    uint8 p[8];
    _itoa(panID, (byte *)p, 10);
    LCD_Str_Print(p, start_pos+5, 3, TRUE);
}
#endif
