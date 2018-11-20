/**************************************************
 *
 * Copyright 2012 IAR Systems. All rights reserved.
 *
 * $Revision: 488 $
 *
 **************************************************/

;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:ROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)               ; Top of Stack
        DCD     __iar_program_start       ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler
        ; External Interrupts
        DCD     Default_Handler           ; 0:  Reserved interrupt 16/0
        DCD     Default_Handler           ; 1:  Reserved interrupt 17/1
        DCD     Default_Handler           ; 2:  Reserved interrupt 18/2
        DCD     Default_Handler           ; 3:  Reserved interrupt 19/3
        DCD     Default_Handler           ; 4:  Reserved interrupt 20/4
        DCD     FTMRE_IRQHandler          ; 5:  Command complete and error interrupt
        DCD     PMC_IRQHandler            ; 6:  Low-voltage detect, low-voltage warning
        DCD     IRQ_IRQHandler            ; 7:  External interrupt
        DCD     I2C0_IRQHandler           ; 8:  Single interrupt vector for all sources
        DCD     I2C1_IRQHandler           ; 9:  Single interrupt vector for all sources
        DCD     SPI0_IRQHandler           ;10:  Single interrupt vector for all sources
        DCD     SPI1_IRQHandler           ;11:  Single interrupt vector for all sources
        DCD     UART0_SCI0_IRQHandler     ;12:  UART0 Status and Error interrupt
        DCD     UART1_SCI1_IRQHandler     ;13:  UART1 Status and Error interrupt
        DCD     UART2_SCI2_IRQHandler     ;14:  UART2 Status and Error interrupt
        DCD     ADC0_IRQHandler           ;15:  ADC conversion complete interrupt
        DCD     ACMP0_IRQHandler          ;16:  Analog comparator 0 interrupt
        DCD     FTM0_IRQHandler           ;17:  Single interrupt vector for all sources
        DCD     FTM1_IRQHandler           ;18:  Single interrupt vector for all sources
        DCD     FTM2_IRQHandler           ;19:  Single interrupt vector for all sources
        DCD     RTC_IRQHandler            ;20:  RTC overflow
        DCD     ACMP1_IRQHandler          ;21:  Analog comparator 1 interrupt
        DCD     PIT_CH0_IRQHandler        ;22:  PIT CH0 overflow
        DCD     PIT_CH1_IRQHandler        ;23:  PIT CH1 overflow
        DCD     KBI0_IRQHandler           ;24:  Keyboard interrupt0
        DCD     KBI1_IRQHandler           ;25:  Keyboard interrupt1
        DCD     Default_Handler           ;26:  Reserved interrupt 42/26
        DCD     ICS_IRQHandler            ;27:  Clock loss of lock Interrupt
        DCD     WDG_IRQHandler            ;28:  WDG timeout interrupt
        DCD     PWT_IRQHandler            ;29:  Single interrupt vector for all sources
        DCD     MSCAN_Rx_IRQHandler       ;30:  MSCAN Rx Interrupt
        DCD     MSCAN_Tx_IRQHandler       ;31:  MSCAN Tx, Err and Wake-up interrupt



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;

      PUBWEAK NMI_Handler
      PUBWEAK HardFault_Handler
      PUBWEAK MemManage_Handler
      PUBWEAK BusFault_Handler
      PUBWEAK UsageFault_Handler
      PUBWEAK SVC_Handler
      PUBWEAK DebugMon_Handler
      PUBWEAK PendSV_Handler
      PUBWEAK SysTick_Handler
      PUBWEAK FTMRE_IRQHandler
      PUBWEAK PMC_IRQHandler
      PUBWEAK IRQ_IRQHandler
      PUBWEAK I2C0_IRQHandler
      PUBWEAK I2C1_IRQHandler
      PUBWEAK SPI0_IRQHandler
      PUBWEAK SPI1_IRQHandler
      PUBWEAK UART0_SCI0_IRQHandler
      PUBWEAK UART1_SCI1_IRQHandler
      PUBWEAK UART2_SCI2_IRQHandler
      PUBWEAK ADC0_IRQHandler
      PUBWEAK ACMP0_IRQHandler
      PUBWEAK FTM0_IRQHandler
      PUBWEAK FTM1_IRQHandler
      PUBWEAK FTM2_IRQHandler
      PUBWEAK RTC_IRQHandler
      PUBWEAK ACMP1_IRQHandler
      PUBWEAK PIT_CH0_IRQHandler
      PUBWEAK PIT_CH1_IRQHandler
      PUBWEAK KBI0_IRQHandler
      PUBWEAK KBI1_IRQHandler
      PUBWEAK ICS_IRQHandler
      PUBWEAK WDG_IRQHandler
      PUBWEAK PWT_IRQHandler
      PUBWEAK MSCAN_Rx_IRQHandler
      PUBWEAK MSCAN_Tx_IRQHandler

      SECTION .text:CODE:REORDER:NOROOT(1)
      THUMB

NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler
FTMRE_IRQHandler
PMC_IRQHandler
IRQ_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
UART0_SCI0_IRQHandler
UART1_SCI1_IRQHandler
UART2_SCI2_IRQHandler
ADC0_IRQHandler
ACMP0_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
RTC_IRQHandler
ACMP1_IRQHandler
PIT_CH0_IRQHandler
PIT_CH1_IRQHandler
KBI0_IRQHandler
KBI1_IRQHandler
ICS_IRQHandler
WDG_IRQHandler
PWT_IRQHandler
MSCAN_Rx_IRQHandler
MSCAN_Tx_IRQHandler
Default_Handler

        B       Default_Handler
        END
