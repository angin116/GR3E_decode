/*
  Ӳ����Э����ؽ����뿴�����̵�README��
 */

#include "12C5A60S2.h"

sbit test = P2^0;

unsigned char mk[3];	// ����ͨ����������Ϣ

void sendB(unsigned char dat)
{
    test = 1;
    SBUF=dat;
    while(!TI);
    TI=0;
    test = 0;
}

void t0() interrupt 1
{
    static unsigned long num=0;
    if(num++ == 100000)
    {
	num = 0;
	sendB(0x55);
    }
    test = 0;
}

void main()
{
    // ��ʼ���ж�
    ET0 = 1;
    ES=0;	//�ش����жϣ�����ѭ�����ò�ѯ��ʽ��������
    EA = 1;
    
    // ��ʼ�����ڣ�������115200
    AUXR|=0x40;//��ʱ��1������1Tģʽ
    TL1 = 0xCC;		//�趨��ʱ��ֵ
    TH1 = 0xFF;		//�趨��ʱ��ֵ
    TMOD&=0x0f;//�����4λ
    TMOD|=0x20;//t1�����ڷ�ʽ2
    TR1=1;
    SM0=0;
    SM1=1;
    REN=1;
    ET1 = 0;

    // ��ʼ��T0���ж�����20us
    AUXR &= 0x7F;		//��ʱ��0������12Tģʽ
    TMOD &= 0xF0; TMOD |= 0x02;	//��ʱ��0������8Ϊ�Զ���װģʽ
    TL0 = 216;
    TH0 = 216;
    TF0 = 0;		//���TF0��־
    TR0 = 1;		//��ʱ��0��ʼ��ʱ

    while(1)
    {
    }
}
