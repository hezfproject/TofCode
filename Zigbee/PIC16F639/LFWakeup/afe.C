#include "afe.h"



_LFSTATE LFSTATE;



void delay(unsigned int c)
{
	unsigned int a,b;
	for(a=c;a>0;a--)
		for(b=110;b>0;b--);
}


/*********************************
**
**		ʱ��ѡ��
**
************************************/
void OSC_INIT(void)
{
	//OSCCON = 0x41;	//1M,
//	OSCCON = 0x71;	//8M,
//	OSCCON = 0x11;	//125K,
//	while(!HTS);	//�ȶ�  32K����ʹ��
	//OSCCON = 0x00;	//31K,	����Լʮ��uA
	//while(!LTS);	//�ȶ�  32K����ʹ��
	//while(!OSTS);
	OSCCON = 0x40;	//1M,	
	while(!HTS);
}


/***************************************
**
**		�˿ڳ�ʼ�����ص�һЩ�ĵ�Ķ���	
**		���������
**	
***************************************/

void PORT_INIT(void)
{
	CMCON0 = 0x07;	//�Ƚ�����
	PCON = 0x0;		//���ó���ѹ���Ѻ�BOR
	VRCON = 0x20;	//��Vref
	T1CON = 0x00;
	WDA = 0xFB;		//RA2����; 
	WPUDA = 0x00;	//��ֹ������

		
	TRISA = 0x04;	//RA2-in
	//RA1,RA5 out
	TRISA &= ~0x22;	//RA5-out
	TRISC = 0x0C;		//RC3-in	RC2-in
	TRISC &= 0x01;		//RC0-out
	PORTA = 0xFF;
	PORTC = 0xFF;
}
	

/****************************************
**
**		��ʼ�����빦�ܣ�ռ�� T1��
**
*****************************************/
void INT_AFE_INIT(void)
{	

	INTEDG = 1;		//������
	INTE = 1;		// INT�жϿ�

	LFSTATE = LFidle;

	GIE	= 1;		//
}




/**********************************************
**
**		���ݽ��룬���ж��е���
**		250uS��250uS�ͱ�ʾ0
**		250uS��500uS�ͱ�ʾ1
**		RA2/INT  ��������Ч
**
***********************************************/

void AFE_RECEIVER(void)
{

	switch(LFSTATE)
 		{
   			case LFidle:	//��ʱ����ʼ������
				

				LFSTATE = LFstart;
				break;
				
			case LFstart:	//ȷ�����ϸ������Ϳ�ʼ����һ��

				LFSTATE = LFaddr;
				break;

			case LFaddr:

				LFSTATE = LFdata;
				break;
				
			case LFdata:	//...........�������ݶ�Ӧ������

				LFSTATE = LFidle;
				break;
				
			default:
           		LFSTATE = LFidle;
              break;
		}


}





