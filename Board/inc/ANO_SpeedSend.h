#ifndef ANO_SPEEDSEND_H
#define ANO_SPEEDSEND_H


#include "stm32f10x.h"

void ANO_Send_Data(u16 speed);
void ANO_Send_Byte(u8 testdatatosend[], u8 _cnt);

#endif
