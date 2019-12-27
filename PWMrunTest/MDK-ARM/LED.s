LED0 EQU 0x422101a0 
RCC_APB2ENR EQU 0x40021018
GPIOA_CRH EQU 0x40010804



Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp







                AREA    RESET, DATA, READONLY

__Vectors       DCD     __initial_sp               ; Top of Stack
                DCD     Reset_Handler              ; Reset Handler
                    
                    
                AREA    |.text|, CODE, READONLY
                    
                THUMB
                REQUIRE8
                PRESERVE8
                    
                ENTRY
Reset_Handler 
                BL LED_Init
MainLoop        BL LED_ON
                BL Delay
                BL LED_OFF
                BL Delay
                
                B MainLoop
             
LED_Init
                PUSH {R0,R1, LR}
                
                LDR R0,=RCC_APB2ENR
                ORR R0,R0,#0x04
                LDR R1,=RCC_APB2ENR
                STR R0,[R1]
                
                LDR R0,=GPIOA_CRH
                BIC R0,R0,#0x0F
                LDR R1,=GPIOA_CRH
                STR R0,[R1]
                
                LDR R0,=GPIOA_CRH
                ORR R0,R0,#0x03
                LDR R1,=GPIOA_CRH
                STR R0,[R1]
                
                MOV R0,#1 
                LDR R1,=LED0
                STR R0,[R1]
             
                POP {R0,R1,PC}

             
LED_ON
                PUSH {R0,R1, LR}    
                
                MOV R0,#0 
                LDR R1,=LED0
                STR R0,[R1]
             
                POP {R0,R1,PC}
             
LED_OFF
                PUSH {R0,R1, LR}    
                
                MOV R0,#1 
                LDR R1,=LED0
                STR R0,[R1]
             
                POP {R0,R1,PC}             
             
Delay
                PUSH {R0,R1, LR}
                
                MOVS R0,#0
                MOVS R1,#0
                MOVS R2,#0
                
DelayLoop0        
                ADDS R0,R0,#1

                CMP R0,#330
                BCC DelayLoop0
                
                MOVS R0,#0
                ADDS R1,R1,#1
                CMP R1,#330
                BCC DelayLoop0

                MOVS R0,#0
                MOVS R1,#0
                ADDS R2,R2,#1
                CMP R2,#15
                BCC DelayLoop0
                
                
                POP {R0,R1,PC}    
             
    ;         NOP
             END