#include <STC8G.H>
#include "intrins.H"

unsigned char num=0;
unsigned char sendnum[60];
unsigned char	sendnum1[30];

void Uart1_Init(void)		//9600bps@22.1184MHz
{
	SCON = 0x50;					//8λ����,�ɱ䲨����                                                                                    
	AUXR |= 0x40;					//��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE;					//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;					//���ö�ʱ��ģʽ
	TL1 = 0xC0;						//���ö�ʱ��ʼֵ
	TH1 = 0xFD;						//���ö�ʱ��ʼֵ
	ET1 = 0;							//��ֹ��ʱ���ж�
	TR1 = 1;							//��ʱ��1��ʼ��ʱ
	EA	=	1;
	ES	=	1;
}

void Delay1ms(unsigned int ms)	//@22.1184MHz
{
		unsigned char data i, j;
	while(ms--)
	{
		i = 29;
		j = 183;
		do
		{
			while (--j);
		} while (--i);
	}
}


void check()
{
	  unsigned char i, a, b;
    for(i = 0; i < 31; i++)
    {
        if((sendnum[i] == 0xaa) & (sendnum[i + 1] == 0xff) & (sendnum[i + 2] == 0x03) & (sendnum[i + 3] == 0x00)	&	(sendnum[i+28]	==	0x55)	&	(sendnum[i+29]	==	0xcc))
				{  
						a = i;
						 for(b = 0; b < 30; b++)
						{
							sendnum1[b] = sendnum[(b + a)];
						}
				}
		} 
}

void main()
{
    P1M0=0X00;    P1M1=0X00;
    P3M0=0X00;    P3M1=0X00;
    P5M0=0X00;    P5M1=0X00;
    Uart1_Init();
    while(1)
    {
        unsigned char i;
        check();
				P_SW1=0x40;
        REN=1;
        
        Delay1ms(100);
        REN=0;
        P_SW1=0x00;
        for(i=0;i<30;i++)
        {
            SBUF    =    sendnum1[i];
						while(TI);
						Delay1ms(2);
         }    
        
        Delay1ms(100);
                                                     
    }                                                  
}

void    zjghhgd()    interrupt    4
{
    unsigned char temp;
    temp	=	SBUF;
    if(RI)
    {    
        P_SW1=0x40;
        sendnum[num]    =    temp;
        num++;
        if(num>59)
        {
            num=0;
            sendnum[num]    =0x00;
        }

        RI=0;
    }    
    if(TI)
    {
			P_SW1=0x00;
        TI=0;
    }
}