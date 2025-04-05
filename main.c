#include <STC8G.H>
#include "intrins.H"

unsigned char num=0;
unsigned char sendnum[60];
unsigned char	sendnum1[30];

void Uart1_Init(void)		//9600bps@22.1184MHz
{
	SCON = 0x50;					//8位数据,可变波特率                                                                                    
	AUXR |= 0x40;					//定时器时钟1T模式
	AUXR &= 0xFE;					//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;					//设置定时器模式
	TL1 = 0xC0;						//设置定时初始值
	TH1 = 0xFD;						//设置定时初始值
	ET1 = 0;							//禁止定时器中断
	TR1 = 1;							//定时器1开始计时
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