/**************************************************************************************************
  Filename:       med_Osal.c
  Revised:        $Date: 2009/07/03 06:05:36 $
  Revision:       $Revision: 1.3 $

  Description:    This file contains function that allows user setup tasks

**************************************************************************************************/

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "hal_types.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "OSAL_Custom.h"
#include "OnBoard.h"
#include "mac_api.h"

/* HAL */
#include "hal_drivers.h"

/* Application */
#include "med.h"

/*********************************************************************
 * GLOBAL VARIABLES
 */

const pTaskEventHandlerFn tasksArr[] = {
  macEventLoop,
  MED_ProcessEvent,
  Hal_ProcessEvent,
};

const uint8 tasksCnt = sizeof( tasksArr ) / sizeof( tasksArr[0] );
uint16 *tasksEvents;
/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osalInitTasks
 *
 * @brief   This function invokes the initialization function for each task.
 *
 * @param   void
 *
 * @return  none
 */
void osalInitTasks( void )
{
  uint8 taskID = 0;

  tasksEvents = (uint16 *)osal_mem_alloc( sizeof( uint16 ) * tasksCnt);
  osal_memset( tasksEvents, 0, (sizeof( uint16 ) * tasksCnt));

  macTaskInit( taskID++ );
  MED_Init( taskID++ );
  Hal_Init( taskID++ );
}

/**************************************************************************************************
**************************************************************************************************/
