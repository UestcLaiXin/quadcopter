#include "pwm.h"

extern float motor1, motor2, motor3, motor4; //�ĸ�����ٶ�
extern u16 PWM_IN_CH[4];

void PWM_OUT_Init(void)
{
    //TODO �Ƿ�Ҫ���ֶ�ʱ��PWM���Ƶ�ʺ�ң����������PWM���Ƶ��һ�£�
    uint16_t arr = 1000 - 1;
    uint16_t psc = 1548 - 1; //54.27hz �� PWM
    //	uint16_t arr = 2500-1;
    //	uint16_t psc = 84-1;//400hz �� PWM

    RCC->APB1ENR |= 1 << 1; //TIM3 ʱ��ʹ��
    TIM3->CR1 &= ~(3 << 5); //���ض���ģʽ
    TIM3->CR1 &= ~(1 << 4); //��������������
    TIM3->SMCR &= ~(7 << 0); //ʱ����Դ����Ϊ�ڲ�ʱ��             /////////////////////
    TIM3->ARR = arr; //�趨�������Զ���װֵ
    TIM3->PSC = psc; //Ԥ��Ƶ������Ƶ

    TIM3->CCR1 = 0;
    TIM3->CCR2 = 0;
    TIM3->CCR3 = 0;
    TIM3->CCR4 = 0;

    //TIM3��ͨ��һ������ʼ��
    RCC->AHB1ENR |= 1 << 0; //ʹ�� PORTA ʱ��
    GPIO_Set(GPIOA, GPIO_Pin_6, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_NOPULL); //���ù���,�������������������
    GPIO_Set(GPIOA, GPIO_Pin_7, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_NOPULL); //���ù���,�������������������
    GPIO_AF_Set(GPIOA, 6, 2); //PA6,AF2
    GPIO_AF_Set(GPIOA, 7, 2); //PA7,AF2
    TIM3->CCMR1 &= ~(3 << 0); //CH1 ����Ϊ���ģʽ            ////////////////////////
    TIM3->CCMR1 |= 6 << 4; //CH1 PWM1 ģʽ
    TIM3->CCMR1 |= 1 << 3; //CH1 Ԥװ��ʹ��
    TIM3->CCER |= 1 << 0; //OC1 ���ʹ��
    TIM3->CCER &= ~(1 << 1); //OC1 �ߵ�ƽ��Ч
    //TIM3->CCER|=1<<1; //OC1 �͵�ƽ��Ч
    TIM3->CCMR1 &= ~(3 << 8); //CH2 ����Ϊ���ģʽ            ////////////////////////
    TIM3->CCMR1 |= 6 << 12; //CH2 PWM1 ģʽ
    TIM3->CCMR1 |= 1 << 11; //CH2 Ԥװ��ʹ��
    TIM3->CCER |= 1 << 4; //OC2 ���ʹ��
    TIM3->CCER &= ~(1 << 5); //OC2 �ߵ�ƽ��Ч
    //TIM3->CCER|=1<<5; //OC2 �͵�ƽ��Ч

    //TIM3��ͨ�������ĳ�ʼ��
    RCC->AHB1ENR |= 1 << 1; //ʹ�� PORTB ʱ��
    GPIO_Set(GPIOB, GPIO_Pin_0, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_NOPULL); //���ù���,�������������������
    GPIO_Set(GPIOB, GPIO_Pin_1, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_NOPULL); //���ù���,�������������������
    GPIO_AF_Set(GPIOB, 0, 2); //PB0,AF2
    GPIO_AF_Set(GPIOB, 1, 2); //PB1,AF2
    TIM3->CCMR2 &= ~(3 << 0); //CH3 ����Ϊ���ģʽ            ////////////////////////
    TIM3->CCMR2 |= 6 << 4; //CH3 PWM1 ģʽ
    TIM3->CCMR2 |= 1 << 3; //CH3 Ԥװ��ʹ��
    TIM3->CCER |= 1 << 8; //OC3 ���ʹ��
    TIM3->CCER &= ~(1 << 9); //OC3 �ߵ�ƽ��Ч
    //TIM3->CCER|=1<<9; //OC3 �͵�ƽ��Ч
    TIM3->CCMR2 &= ~(3 << 8); //CH2 ����Ϊ���ģʽ            ////////////////////////
    TIM3->CCMR2 |= 6 << 12; //CH2 PWM1 ģʽ
    TIM3->CCMR2 |= 1 << 11; //CH2 Ԥװ��ʹ��
    TIM3->CCER |= 1 << 12; //OC4 ���ʹ��
    TIM3->CCER &= ~(1 << 13); //OC4 �ߵ�ƽ��Ч
    //TIM3->CCER|=1<<13; //OC4 �͵�ƽ��Ч

    TIM3->CR1 |= 1 << 7; //ARPE ʹ��
    TIM3->CR1 |= 1 << 0; //ʹ�ܶ�ʱ�� 3
}

void PWM_IN_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // Enable GPIOA and TIM5 clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    // GPIO configuration. Push-pull alternate function
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // GPIO alternate function configuration
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);

    // Close TIM5
    TIM_DeInit(TIM5);
    // TIM5 configuration. Prescaler is 84, period is 0xFFFF, and counter mode is up
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

    // TIM5 input configuration. Capture on rising edge and filter value is 0x0B
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0B;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0B;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0B;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0B;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

    // NVIC initialization
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // TIM5 interrupt configuration
    TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);
    TIM_ITConfig(TIM5, TIM_IT_CC2, ENABLE);
    TIM_ITConfig(TIM5, TIM_IT_CC3, ENABLE);
    TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);

    // Enable TIM5
    TIM_Cmd(TIM5, ENABLE);

    //     //�Ĵ��������������⣬����û��PWM������Ҳ����벶���ж�
    //     uint32_t arr = 0xffff;
    //     uint16_t psc = 84 - 1; //��1Mhz��Ƶ�ʼ�����ÿ0xffffus����һ�μ����������ж�

    //     RCC->APB1ENR |= 1 << 3; //TIM5 ʱ��ʹ��
    //     RCC->AHB1ENR |= 1 << 0; //ʹ�� PORTA ʱ��

    //     //TIM5->CH1   PA0
    //     GPIO_Set(GPIOA, GPIO_Pin_0, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_UP);
    //     GPIO_AF_Set(GPIOA, 0, 2); //PA0,AF2
    //     //TIM5->CH2   PA1
    //     GPIO_Set(GPIOA, GPIO_Pin_1, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_UP);
    //     GPIO_AF_Set(GPIOA, 1, 2); //PA1,AF2
    //     //TIM5->CH3   PA2
    //     GPIO_Set(GPIOA, GPIO_Pin_2, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_UP);
    //     GPIO_AF_Set(GPIOA, 2, 2); //PA2,AF2
    //     //TIM5->CH4   PA3
    //     GPIO_Set(GPIOA, GPIO_Pin_3, GPIO_Mode_AF, GPIO_OType_PP, GPIO_Fast_Speed, GPIO_PuPd_UP);
    //     GPIO_AF_Set(GPIOA, 3, 2); //PA3,AF2

    //     TIM5->ARR = arr; //�趨�������Զ���װֵ
    //     TIM5->PSC = psc; //Ԥ��Ƶ��

    //     //TIM5->CH1   PA0
    //     TIM5->CCMR1 |= 1 << 0; //CC1S=01 ѡ������� IC1 ӳ�䵽 TI1 ��
    //     TIM5->CCMR1 |= 0 << 4; //IC1F=0000 ���������˲��� ���˲�
    //     TIM5->CCMR1 |= 0 << 2; //IC1PSC=00 ���������Ƶ,����Ƶ
    //     TIM5->CCER |= 0 << 1; //CC1P=0 �����ز���
    //     TIM5->CCER |= 1 << 0; //CC1E=1 �������������ֵ������Ĵ�����
    //     //TIM5->CH2   PA1
    //     TIM5->CCMR1 |= 1 << 8; //CC2S=01 ѡ������� IC2 ӳ�䵽 TI2 ��
    //     TIM5->CCMR1 |= 0 << 12; //IC2F=0000 ���������˲��� ���˲�
    //     TIM5->CCMR1 |= 0 << 10; //IC2PSC=00 ���������Ƶ,����Ƶ
    //     TIM5->CCER |= 0 << 5; //CC2P=0 �����ز���
    //     TIM5->CCER |= 1 << 4; //CC2E=1 �������������ֵ������Ĵ�����
    //     //TIM5->CH3   PA2
    //     TIM5->CCMR2 |= 1 << 0; //CC3S=01 ѡ������� IC3 ӳ�䵽 TI3 ��
    //     TIM5->CCMR2 |= 0 << 4; //IC3F=0000 ���������˲��� ���˲�
    //     TIM5->CCMR2 |= 0 << 2; //IC3PSC=00 ���������Ƶ,����Ƶ
    //     TIM5->CCER |= 0 << 9; //CC3P=0 �����ز���
    //     TIM5->CCER |= 1 << 8; //CC3E=1 �������������ֵ������Ĵ�����
    //     //TIM5->CH4   PA3
    //     TIM5->CCMR2 |= 1 << 8; //CC4S=01 ѡ������� IC4 ӳ�䵽 TI4 ��
    //     TIM5->CCMR2 |= 0 << 12; //IC4F=0000 ���������˲��� ���˲�
    //     TIM5->CCMR2 |= 0 << 10; //IC4PSC=00 ���������Ƶ,����Ƶ
    //     TIM5->CCER |= 0 << 13; //CC4P=0 �����ز���
    //     TIM5->CCER |= 1 << 12; //CC4E=1 �������������ֵ������Ĵ�����

    //     TIM5->EGR = 1 << 0; //������Ʋ��������¼�,ʹд�� PSC ��ֵ������Ч,
    //     //���򽫻�Ҫ�ȵ���ʱ������Ż���Ч!
    // //    TIM5->SR &= ~(1 << 0); //��������жϱ�־λ
    //     MY_NVIC_Init(3, 3, TIM5_IRQn, 2); //��2,��ռ���ȼ�3����Ӧ���ȼ�0
    //     TIM5->DIER |= 0x0F << 1; //������ 1��2��3��4 �ж�
    //     TIM5->CR1 |= 0x01; //ʹ�ܶ�ʱ�� 5
}

// TIM5_CAPTURE_STA[i]:0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
// TIM5_CAPTURE_OVF[i]:0,δ���;1,���.
// TIM5_CAPTURE_VAL[i][0]:�����ض�Ӧ������ֵ
// TIM5_CAPTURE_VAL[i][1]:�½��ض�Ӧ������ֵ
// PWM_IN_CH[i]:ͨ��i+1������PWMλ��
u8 TIM5_CAPTURE_STA[4];
u16 TIM5_CAPTURE_OVF[4];
u16 TIM5_CAPTURE_VAL[4][2];
void TIM5_PWM_IN_IRQ(void)
{
    u8 i;
    //ͨ��i+1�ж�
    for (i = 0; i < 4; i++) {
        if (TIM5->SR & (1 << (i + 1))) {
            TIM5->SR &= ~((0x0001 << (i + 1))); //����жϱ�־
            if (TIM5_CAPTURE_STA[i]) { //�Ѳ��񵽸ߵ�ƽ��˵����ʱ�����½���
                TIM5_CAPTURE_STA[i] = 0; //���Ĳ���״̬
                TIM5_CAPTURE_VAL[i][1] = *(&(TIM5->CCR1) + i); //��ȡ�½��ض�Ӧ������ֵ
                PWM_IN_CH[i] = TIM5_CAPTURE_VAL[i][1] - TIM5_CAPTURE_VAL[i][0] + TIM5_CAPTURE_OVF[i] * 0xffff; //����������
                TIM5_CAPTURE_OVF[i] = 0; //�����������
                TIM5->CCER &= ~(0x0001 << (1 + 4 * i)); //CCxP=00 ͨ�������ز���
            } else { //����������
                TIM5_CAPTURE_STA[i] = 1; //���Ĳ���״̬
                TIM5_CAPTURE_VAL[i][0] = *(&(TIM5->CCR1) + i); //��ȡ�����ض�Ӧ������ֵ
                TIM5->CCER |= (0x0001 << (1 + 4 * i)); //CCxP=01 ͨ���½��ز���
            }
        }
    }
}

//�⺯���涨ʱ���ж�
// // Capture status of channels
// unsigned char TIM5CH1_CAPTURE_STA = 1;
// unsigned char TIM5CH2_CAPTURE_STA = 1;
// unsigned char TIM5CH3_CAPTURE_STA = 1;
// unsigned char TIM5CH4_CAPTURE_STA = 1;

// // Rising edge/falling edge data
// uint16_t TIM5CH1_Rise, TIM5CH1_Fall,
//     TIM5CH2_Rise, TIM5CH2_Fall,
//     TIM5CH3_Rise, TIM5CH3_Fall,
//     TIM5CH4_Rise, TIM5CH4_Fall;

// // Overflow processing variable
// uint16_t TIM5_T;

// // Four-channel remote control initial value
// extern uint16_t PWMInCh1, PWMInCh2, PWMInCh3, PWMInCh4;

// void TIM5_PWM_IN_IRQ(void)
// {
//     // CH1 - AIL - Roll
//     // Capture the interrupt
//     if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET) {
//         // Clear interrupt flag
//         TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
//         // Capture the rising edge
//         if (TIM5CH1_CAPTURE_STA == 1) {
//             // Get the data of rising edge
//             TIM5CH1_Rise = TIM_GetCapture1(TIM5);
//             // Change capture status to falling edge
//             TIM5CH1_CAPTURE_STA = 0;
//             // Set to capture on falling edge
//             TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling);
//         }
//         // Capture the falling edge
//         else {
//             // Get the data of falling edge
//             TIM5CH1_Fall = TIM_GetCapture1(TIM5);
//             // Change capture status to rising edge
//             TIM5CH1_CAPTURE_STA = 1;

//             // Overflow processing
//             if (TIM5CH1_Fall < TIM5CH1_Rise) {
//                 TIM5_T = 65535;
//             } else {
//                 TIM5_T = 0;
//             }

//             // Falling edge time minus rising edge time to get high-level time
//             PWMInCh1 = TIM5CH1_Fall - TIM5CH1_Rise + TIM5_T;
//             // Set to capture on rising edge
//             TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising);
//         }
//     }

//     // CH2 - ELE - Pitch
//     if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET) {
//         TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);

//         if (TIM5CH2_CAPTURE_STA == 1) {
//             TIM5CH2_Rise = TIM_GetCapture2(TIM5);
//             TIM5CH2_CAPTURE_STA = 0;
//             TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Falling);
//         } else {
//             TIM5CH2_Fall = TIM_GetCapture2(TIM5);
//             TIM5CH2_CAPTURE_STA = 1;
//             if (TIM5CH2_Fall < TIM5CH2_Rise) {
//                 TIM5_T = 65535;
//             } else {
//                 TIM5_T = 0;
//             }
//             PWMInCh2 = TIM5CH2_Fall - TIM5CH2_Rise + TIM5_T;
//             TIM_OC2PolarityConfig(TIM5, TIM_ICPolarity_Rising);
//         }
//     }

//     // CH3 - THR - Acc_t
//     if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET) {
//         //        SendStr("ok1");
//         TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
//         if (TIM5CH3_CAPTURE_STA == 1) {
//             TIM5CH3_Rise = TIM_GetCapture3(TIM5);
//             TIM5CH3_CAPTURE_STA = 0;
//             TIM_OC3PolarityConfig(TIM5, TIM_ICPolarity_Falling);
//         } else {
//             TIM5CH3_Fall = TIM_GetCapture3(TIM5);
//             TIM5CH3_CAPTURE_STA = 1;
//             if (TIM5CH3_Fall < TIM5CH3_Rise) {
//                 TIM5_T = 65535;
//             } else {
//                 TIM5_T = 0;
//             }
//             PWMInCh3 = TIM5CH3_Fall - TIM5CH3_Rise + TIM5_T;
//             TIM_OC3PolarityConfig(TIM5, TIM_ICPolarity_Rising);
//         }
//     }

//     // CH4 - RUD -Yaw
//     if (TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET) {
//         TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);

//         if (TIM5CH4_CAPTURE_STA == 1) {
//             TIM5CH4_Rise = TIM_GetCapture4(TIM5);
//             TIM5CH4_CAPTURE_STA = 0;
//             TIM_OC4PolarityConfig(TIM5, TIM_ICPolarity_Falling);
//         } else {
//             TIM5CH4_Fall = TIM_GetCapture4(TIM5);
//             TIM5CH4_CAPTURE_STA = 1;
//             if (TIM5CH4_Fall < TIM5CH4_Rise) {
//                 TIM5_T = 65535;
//             } else {
//                 TIM5_T = 0;
//             }
//             PWMInCh4 = TIM5CH4_Fall - TIM5CH4_Rise + TIM5_T;
//             TIM_OC4PolarityConfig(TIM5, TIM_ICPolarity_Rising);
//         }
//     }
//     //		TIM3->CCR1=PWMInCh1*0.054;//PWM���
//     //		TIM3->CCR2=PWMInCh2*0.054;//PWM���
//     //		TIM3->CCR3=PWMInCh3*0.054;//PWM���
//     //		TIM3->CCR4=PWMInCh4*0.054;//PWM���

//     //			TIM3->CCR1=PWMInCh1*0.054;//PWM���
//     //			TIM3->CCR2=PWMInCh3*0.054;//PWM���
//     //			TIM3->CCR3=PWMInCh3*0.054;//PWM���
//     //			TIM3->CCR4=PWMInCh3*0.054;//PWM���

// //    SendPWMIN(0xF1, &TIM5CH1_CAPTURE_STA, &TIM5_T, &TIM5CH1_Rise, &TIM5CH1_Fall, &PWMInCh1);
// //    SendPWMIN(0xF2, &TIM5CH2_CAPTURE_STA, &TIM5_T, &TIM5CH2_Rise, &TIM5CH2_Fall, &PWMInCh2);
// //    SendPWMIN(0xF3, &TIM5CH3_CAPTURE_STA, &TIM5_T, &TIM5CH3_Rise, &TIM5CH3_Fall, &PWMInCh3);
// //    SendPWMIN(0xF4, &TIM5CH4_CAPTURE_STA, &TIM5_T, &TIM5CH4_Rise, &TIM5CH4_Fall, &PWMInCh4);
// }

void PWM_OUT(void)
{
    TIM3->CCR1 = motor2 * PWM_IN_TO_OUT;
    TIM3->CCR2 = motor4 * PWM_IN_TO_OUT;
    TIM3->CCR3 = motor3 * PWM_IN_TO_OUT;
    TIM3->CCR4 = motor1 * PWM_IN_TO_OUT;
}
