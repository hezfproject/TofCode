/**************************************************************************************************
  Filename:       hal_dma.c
  Revised:        $Date: 2011/08/10 17:46:29 $
  Revision:       $Revision: 1.1 $

  Description:    This file contains the interface to the DMA.


  Copyright 2007-2009 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
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

/*********************************************************************
 * INCLUDES
 */

#include "hal_types.h"
#include "hal_defs.h"
#include "hal_dma.h"
#include "hal_mcu.h"
#include "hal_uart.h"

#if (defined HAL_IRGEN) && (HAL_IRGEN == TRUE)
#include "hal_irgen.h"
#endif

#if (defined HAL_SPI) && (HAL_SPI == TRUE)
#include "hal_spi.h"
#endif


#if ((defined HAL_DMA) && (HAL_DMA == TRUE))

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

halDMADesc_t dmaCh0;
halDMADesc_t dmaCh1234[4];

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/******************************************************************************
 * @fn      HalDMAInit
 *
 * @brief   DMA Interrupt Service Routine
 *
 * @param   None
 *
 * @return  None
 *****************************************************************************/
void HalDmaInit( void )
{
  HAL_DMA_SET_ADDR_DESC0( &dmaCh0 );
  HAL_DMA_SET_ADDR_DESC1234( dmaCh1234 );
#if (HAL_UART_DMA || \
   ((defined HAL_SPI) && (HAL_SPI == TRUE))  || \
   ((defined HAL_IRGEN) && (HAL_IRGEN == TRUE)))
  DMAIE = 1;
#endif
}

#if (HAL_UART_DMA || \
   ((defined HAL_SPI) && (HAL_SPI == TRUE))  || \
   ((defined HAL_IRGEN) && (HAL_IRGEN == TRUE)))
/******************************************************************************
 * @fn      HalDMAInit
 *
 * @brief   DMA Interrupt Service Routine
 *
 * @param   None
 *
 * @return  None
 *****************************************************************************/
HAL_ISR_FUNCTION( halDmaIsr, DMA_VECTOR )
{
  extern void HalUARTIsrDMA(void);

  DMAIF = 0;

#if HAL_UART_DMA
  if (HAL_DMA_CHECK_IRQ(HAL_DMA_CH_TX))
  {
    HalUARTIsrDMA();
  }
#endif // HAL_UART_DMA

#if (defined HAL_SPI) && (HAL_SPI == TRUE)
  if ( HAL_DMA_CHECK_IRQ( HAL_DMA_CH_RX ) )
  {
    HAL_DMA_CLEAR_IRQ( HAL_DMA_CH_RX );
    HalSpiRxIsr();
  }

  if ( HAL_DMA_CHECK_IRQ( HAL_DMA_CH_TX ) )
  {
    HAL_DMA_CLEAR_IRQ( HAL_DMA_CH_TX );
    HalSpiTxIsr();
  }
#endif // (defined HAL_SPI) && (HAL_SPI == TRUE)

#if (defined HAL_IRGEN) && (HAL_IRGEN == TRUE)
  if ( HAL_IRGEN == TRUE && HAL_DMA_CHECK_IRQ( HAL_IRGEN_DMA_CH ) )
  {
    HAL_DMA_CLEAR_IRQ( HAL_IRGEN_DMA_CH );
    HalIrGenDmaIsr();
  }
#endif // (defined HAL_IRGEN) && (HAL_IRGEN == TRUE)
}
#endif
#endif  // #if ((defined HAL_DMA) && (HAL_DMA == TRUE))
/******************************************************************************
******************************************************************************/
