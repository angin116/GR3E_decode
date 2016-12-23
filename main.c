/*
  Ӳ����Э����ؽ����뿴�����̵�README��
 */

#include "12C5A60S2.h"

sbit test0 = P2^0;
sbit test1 = P2^1;
sbit ch1 = P1^2;
sbit ch2 = P1^1;
sbit ch3 = P1^0;

typedef enum {
    false=0,
    true
} bool;

unsigned char ch1_num=0, ch2_num=0, ch3_num=0;	//�������ͨ����⵽�ߵ�ƽ����
unsigned char ch1_mk, ch2_mk, ch3_mk;	// ����ͨ����������Ϣ����ΪҪ���͵�����
bool flag_ready = false;	//����׼���ñ�־
bool flag_miss = false;		//�źŶ�ʧ��־
unsigned char step = 0;	// 0-��ǰ���ڲɼ�����ͨ���źš�
			//1-����ͨ���ź��Ѿ��ɼ���ɣ����ڵȴ��µ����ڵ�����

void sendB(unsigned char dat)
{
    SBUF=dat;
    while(!TI);
    TI=0;
}

void t0() interrupt 1	//20usһ���ж�
{
    static unsigned char over_num = 0;	//����������⵽����ͨ�����ǵ͵�ƽ�Ĵ���
    test0 = 1;
    switch(step)
    {
    case 0:	//�ɼ��źţ�������һ��˵���Ѿ�����������һ·Ϊ�ߵ�ƽ
	//����ͨ������
	if(ch1)
	{
	    ch1_num++;
	    over_num = 0;
	}
	else if(ch2)
	{
	    ch2_num++;
	    over_num = 0;
	}
	else if(ch3)
	{
	    ch3_num++;
	    over_num = 0;
	}
	else	//����ͨ������0���ܿ���������ͨ���źŽ���
	{
	    over_num ++;
	    if(over_num == 5)	//����50������ͨ�����ǵ͵�ƽ��ȷ�ϱ������źŽ���
	    {
		if(ch1_num < 25 || ch2_num < 25 || ch3_num < 25)	//��һ·ͨ������С��500us����Ϊ�ź������⣬�����źŶ�ʧ��Ϣ
		    flag_miss = true;
		else
		    flag_ready = true;	//֪ͨ�����̷������ݣ��˱���������������
		step = 1;	//����ȴ��׶�
	    }
	}
	break;
    case 1:	//�ɼ���ɣ��ȴ��¸����ڵ���
	if(ch1 | ch2 | ch3)	//�Ѿ���⵽��ͨ��Ϊ�ߵ�ƽ���µ��źŵ���
	{
	    step = 0;
	    over_num = 0;
	    ch1_num = 0;
	    ch2_num = 0;
	    ch3_num = 0;
	    if(ch1) ch1_num++;
	    else if(ch2) ch2_num++;
	    else if(ch3) ch3_num++;
	}
	break;
    }
    test0 = 0;
}

void main()
{
    bool first_frame = true;	//��һ֡��־
    // ��ʼ���ж�
    ET0 = 1;
    ES=0;	//�ش����жϣ�����ѭ�����ò�ѯ��ʽ��������
    EA = 1;
    
    // ��ʼ�����ڣ�������115200
    PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
    SCON = 0x50;		//8λ����,�ɱ䲨����
    AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
    AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
    TMOD &= 0x0F;		//�����ʱ��1ģʽλ
    TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
    TL1 = 0xF3;		//�趨��ʱ��ֵ
    TH1 = 0xF3;		//�趨��ʱ����װֵ
    ET1 = 0;		//��ֹ��ʱ��1�ж�
    TR1 = 1;		//������ʱ��1

    // ��ʼ��T0���ж�����20us
    AUXR &= 0x7F;		//��ʱ��0������12Tģʽ
    TMOD &= 0xF0; TMOD |= 0x02;	//��ʱ��0������8Ϊ�Զ���װģʽ
    TL0 = 216;
    TH0 = 216;
    TF0 = 0;		//���TF0��־
    TR0 = 1;		//��ʱ��0��ʼ��ʱ

    while(1)
    {
	if(flag_ready)
	{
	    test1 = ~test1;
	    if(first_frame)
	    {
		first_frame = false;
		flag_ready = false;
	    }
	    else
	    {
		//�����������Ϣת��������ֵ
		ch1_mk = ch1_num * 2;
		ch2_mk = ch2_num * 2;
		ch3_mk = ch3_num * 2;
		sendB(0x01);	//һ֡������ʼ��־
		sendB(ch1_mk);
		sendB(ch2_mk);
		sendB(ch3_mk);
		flag_ready = false;
	    }
	}
	if(flag_miss)
	{
	    sendB(0x02);
	    flag_miss = false;
	}
    }
}
