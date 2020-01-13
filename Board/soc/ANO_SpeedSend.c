#include "stm32f10x.h"
#include "ANO_SpeedSend.h"
#include "usart.h"


//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

u8 testdatatosend[50];	//�������ݻ���


void ANO_Send_Data(u16 speed)	//�����û����ݣ�������3�����ݣ��ֱ��Ӧ���̳�3������

{

	u8 _cnt=0;
	
	testdatatosend[_cnt++]=0xAA;
	testdatatosend[_cnt++]=0xAA;
	//testdatatosend[_cnt++]=0x06;//0x05Ϊ���ݷ���Դ��������ο�����Э�飬���ֽ��û������������
	//testdatatosend[_cnt++]=0xAF;//0xAFΪ����Ŀ�ĵأ�AF��ʾ��λ����������ο�����Э��
	testdatatosend[_cnt++]=0xF1;//0xF1����ʾ��֡ΪF1�û��Զ���֡����Ӧ�߼������F1����֡
	testdatatosend[_cnt++]=0;//���ֽڱ�ʾ���ݳ��ȣ�������=0����������ٸ�ֵ�������Ͳ����˹����㳤����

	testdatatosend[_cnt++]=BYTE1(speed);//��Ҫ���͵����ݷ������ͻ�����
	testdatatosend[_cnt++]=BYTE0(speed);
	
	testdatatosend[3] = _cnt-4;//_cnt�����������ݳ��ȣ���5Ϊ��ȥ֡��ͷ5���������ֽ�

	u8 sum = 0;	//����Ϊ����sumУ���ֽڣ���0xAAҲ�������ֽڣ�һֱ��sum�ֽ�ǰһ�ֽ�

	for(u8 i=0;i<_cnt;i++)
		sum += testdatatosend[i];

	testdatatosend[_cnt++]=sum;	//��sumУ�����ݷ������һ�ֽ�

	ANO_Send_Byte(testdatatosend, _cnt);	//���÷������ݺ���

}


void ANO_Send_Byte(u8 testdatatosend[], u8 _cnt)
{
	for (int i = 0; i < _cnt; i++)
		Usart_SendByte(UART4, testdatatosend[i]);
}
