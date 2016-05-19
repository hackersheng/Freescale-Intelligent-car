/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @���ݵ��ӿƼ���ѧ ��Ϣ����ѧԺCCD�����
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "Data.h"
/**
*  @name   : ALL_Init
*  @author : ZX
*  @tips   : 
*/
void ALL_Init(void)
{
  SystemClockSetup(
									ClockSource_EX50M,
									CoreClock_100M);
											//��ʼ��ϵͳʱ�� ʹ���ⲿ50M���� PLL��Ƶ��200M
	
	Uart_Init(); 				//���ڳ�ʼ
	
	DelayInit();				//��ʼ����ʱģ��
	
	#if(TESTTIME==OFF)
		ISR_Init();       //���ڶ�ʱ���жϳ�ʼ��
	#endif
	
	Encoder_Init();			//���������ٳ�ʼ��
	
	CCD1_Init();		    //CCD1��ʼ��
	
	OLED_Init();        //OLED��ʼ��
	 
//	OLED2_Init();       //OLED2��ʼ��
	
	KeyInit();					//������ʼ��
	
 //DelayMs(2);
	
	InitL3G4200D();     //�����ǳ�ʼ��
	
	MMA7361_Init(); 		//���ٶȼƳ�ʼ��
	
	Motor_Init();       //�����ʼ��
	
	//LED_Init();       //ָʾ�Ƴ�ʼ��
	
	LED_Test_Init();    //���ԵƳ�ʼ��
	
	DFlash_Init();      //DFlash��ʼ��
	
	UART_printf("Init Success!");
}


