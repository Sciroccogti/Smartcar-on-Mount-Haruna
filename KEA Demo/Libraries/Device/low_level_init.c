/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2014
 *
 *    File name   : low_level_init.c
 *    Description : Low level init procedure
 *    Editor      : xhjiang
 *    Version     : rev. 1.0, 2018-04-10
 *
 *    $Revision: 16 $
 **************************************************************************/
#include <stdint.h>
#include "SKEAZ1284.h"
#include "system_SKEAZ1284.h"
#include "common.h"

extern uint32_t __vector_table[];

/*************************************************************************
 * Function Name: low_level_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: This function is used for low level initialization
 *
 *************************************************************************/
int __low_level_init(void)
{
  SCB_VTOR = (uint32_t)__vector_table; /* Set the interrupt vector table position */
  WDOG_TOVAL = 0xE803;                 /* setting timeout value */
  WDOG_CS2 = WDOG_CS2_CLK_MASK;        /* setting 1-kHz clock source */
  WDOG_CS1 = 0x23;                     /* Watchdog disabled,
                                          Watchdog interrupts are disabled. Watchdog resets are not delayed,
                                          Updates allowed. Software can modify the watchdog configuration registers within 128 bus clocks after performing the unlock write sequence,
                                          Watchdog test mode disabled,
                                          Watchdog disabled in chip debug mode,
                                          Watchdog enabled in chip wait mode,
                                          Watchdog enabled in chip stop mode. */
  Clock_Config();                      /* Set the clock */
  Get_Clk();                           /* Get the clock */
  return 1;
}
