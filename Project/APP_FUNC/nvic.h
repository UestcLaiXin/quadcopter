#ifndef NVIC_H
#define NVIC_H
#include "includes.h"
//����ԭ�ӡ�����STM32F4����ָ��-�Ĵ����汾_V1.1��

//��������
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group);
void MY_NVIC_Init(u8 NVIC_PreemptionPriority, u8 NVIC_SubPriority, u8 NVIC_Channe, u8 NVIC_Group);
void Ex_NVIC_Config(u8 GPIOx, u8 BITx, u8 TRIM);
#endif
