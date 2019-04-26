;// <h> Stack Configuration
;// <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h>

Stack_Size      EQU     0x00001000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h>

Heap_Size       EQU     0x00001000
BKP_KEY			EQU		0x8555aaa1
BKP_BASE_KEY	EQU		0x400010F0	
BKP_RG60_0		EQU		0x40001100	
BKP_RG60_1		EQU		0x40001110	
BKP_RG60_2		EQU		0x40001120	
	

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
 ;              EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
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
                
				DCD     FT_ERR_IF                 ;IRQ0
				DCD     CLK_IF                    ;IRQ1
				DCD     PVD_IF                    ;IRQ2
				DCD     RTC_IF                    ;IRQ3
				DCD     BKP_IF                    ;IRQ4
				DCD     EXTBUS_ERR_IF             ;IRQ5
				DCD     DMA_DONE_IF               ;IRQ6
				DCD     CCSDS_RX_IF               ;IRQ7
				DCD     CCSDS_TX_IF               ;IRQ8
				DCD     PORT_IF                   ;IRQ9
				DCD     SPW0_INT_IF               ;IRQ10
				DCD     SPW1_INT_IF               ;IRQ11
				DCD     TMR0_INT_IF               ;IRQ12
				DCD     TMR1_INT_IF               ;IRQ13
				DCD     TMR2_INT_IF               ;IRQ14
				DCD     TMR3_INT_IF               ;IRQ15
				DCD     CAN0_INT_IF               ;IRQ16
				DCD     CAN1_INT_IF               ;IRQ17
				DCD     UART0_INT_IF              ;IRQ18
				DCD     UART1_INT_IF              ;IRQ19
				DCD     MIL0_INT_IF               ;IRQ20
				DCD     MIL1_INT_IF               ;IRQ21
				DCD     MIL2_INT_IF               ;IRQ22
				DCD     MIL3_INT_IF               ;IRQ23
				DCD     SSP0_INT_IF               ;IRQ24
				DCD     SSP1_INT_IF               ;IRQ25
				DCD     SSP2_INT_IF               ;IRQ26
				DCD     SSP3_INT_IF               ;IRQ27
				DCD     SSP4_INT_IF               ;IRQ28
				DCD     SSP5_INT_IF               ;IRQ29
				DCD     CRC_INT_IF                ;IRQ30
				DCD     ECC_INT_IF                ;IRQ31

                AREA    EXECUTABLE_MEMORY_SECTION, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler			[WEAK]
                IMPORT  __main

;				LDR		R0, =BKP_BASE_KEY
;				LDR		R1, =BKP_KEY
;				STR		R1, [R0]	
;				
;				LDR		R0, =BKP_RG60_0
;				LDR		R1,[R0]
;				ORR		R1,R1,#0x10000000
;				STR		R1,[R0]
;				LDR		R0, =BKP_RG60_1
;				STR		R1,[R0]
;				LDR		R0, =BKP_RG60_2
;				STR		R1,[R0]
;				B		.
				
				LDR     R0,=__main
				BX      R0
                ENDP


                AREA    |.text|, CODE, READONLY

; Dummy Exception Handlers (infinite loops which can be modified)                

NMI_Handler     PROC
                EXPORT  NMI_Handler				[WEAK]
                B       .
                ENDP

HardFault_Handler PROC
                EXPORT  HardFault_Handler		[WEAK]
                B       .
                ENDP

SVC_Handler     PROC
                EXPORT  SVC_Handler				[WEAK]
                B       .
                ENDP

PendSV_Handler  PROC
                EXPORT  PendSV_Handler			[WEAK]
                B       .
                ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler			[WEAK]
                B       .
                ENDP

; External Interrupts

FT_ERR_IF       PROC
                EXPORT  FT_ERR_IF		[WEAK]
                B       .
                ENDP

CLK_IF          PROC
                EXPORT  CLK_IF			[WEAK]
                B       .
                ENDP

PVD_IF          PROC
                EXPORT  PVD_IF			[WEAK]
                B       .
                ENDP
                
RTC_IF          PROC
                EXPORT  RTC_IF			[WEAK]
                B       .
                ENDP

BKP_IF          PROC
                EXPORT  BKP_IF			[WEAK]
                B       .
                ENDP

EXTBUS_ERR_IF   PROC
                EXPORT  EXTBUS_ERR_IF	[WEAK]
                B       .
                ENDP

DMA_DONE_IF     PROC
                EXPORT  DMA_DONE_IF		[WEAK]
                B       .
                ENDP

CCSDS_RX_IF     PROC
                EXPORT  CCSDS_RX_IF		[WEAK]
                B       .
                ENDP

CCSDS_TX_IF     PROC
                EXPORT  CCSDS_TX_IF		[WEAK]
                B       .
                ENDP

PORT_IF         PROC
                EXPORT  PORT_IF			[WEAK]
                B       .
                ENDP

SPW0_INT_IF     PROC
                EXPORT  SPW0_INT_IF		[WEAK]
                B       .
                ENDP

SPW1_INT_IF     PROC
                EXPORT  SPW1_INT_IF		[WEAK]
                B       .
                ENDP

TMR0_INT_IF     PROC
                EXPORT  TMR0_INT_IF		[WEAK]
                B       .
                ENDP

TMR1_INT_IF     PROC
                EXPORT  TMR1_INT_IF		[WEAK]
                B       .
                ENDP

TMR2_INT_IF     PROC
                EXPORT  TMR2_INT_IF		[WEAK]
                B       .
                ENDP

TMR3_INT_IF     PROC
                EXPORT  TMR3_INT_IF		[WEAK]
                B       .
                ENDP

CAN0_INT_IF     PROC
                EXPORT  CAN0_INT_IF		[WEAK]
                B       .
                ENDP

CAN1_INT_IF     PROC
                EXPORT  CAN1_INT_IF		[WEAK]
                B       .
                ENDP

UART0_INT_IF    PROC
                EXPORT  UART0_INT_IF	[WEAK]
                B       .
                ENDP

UART1_INT_IF    PROC
                EXPORT  UART1_INT_IF	[WEAK]
                B       .
                ENDP

MIL0_INT_IF     PROC
                EXPORT  MIL0_INT_IF		[WEAK]
                B       .
                ENDP

MIL1_INT_IF     PROC
                EXPORT  MIL1_INT_IF		[WEAK]
                B       .
                ENDP
                
MIL2_INT_IF     PROC
                EXPORT  MIL2_INT_IF		[WEAK]
                B       .
                ENDP

MIL3_INT_IF     PROC
                EXPORT  MIL3_INT_IF		[WEAK]
                B       .
                ENDP

SSP0_INT_IF     PROC
                EXPORT  SSP0_INT_IF		[WEAK]
                B       .
                ENDP

SSP1_INT_IF     PROC
                EXPORT  SSP1_INT_IF		[WEAK]
                B       .
                ENDP

SSP2_INT_IF     PROC
                EXPORT  SSP2_INT_IF		[WEAK]
                B       .
                ENDP

SSP3_INT_IF     PROC
                EXPORT  SSP3_INT_IF		[WEAK]
                B       .
                ENDP

SSP4_INT_IF     PROC
                EXPORT  SSP4_INT_IF		[WEAK]
                B       .
                ENDP

SSP5_INT_IF     PROC
                EXPORT  SSP5_INT_IF		[WEAK]
                B       .
                ENDP

CRC_INT_IF      PROC
                EXPORT  CRC_INT_IF		[WEAK]
                B       .
                ENDP

ECC_INT_IF      PROC
                EXPORT  ECC_INT_IF		[WEAK]
                B       .
                ENDP

                ALIGN



; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
               EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


              END
