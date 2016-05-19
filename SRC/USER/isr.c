/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @���ݵ��ӿƼ���ѧ ��Ϣ����ѧԺCCD�����
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "Data.h"

uint8_t IntegrationTime = 0;

uint8_t  Time = 0;
uint8_t  DisPlayTime = 0,DisPlay = 0;
uint8_t  UartTime = 0,UartOK = 0;
uint16_t StartTime = 0;  //������ֱ��ʱ��	
uint8_t  Start = 0;	//������־λ

uint8_t  AngleOutPeriod = 0;
uint8_t  SpeedOutPeriod = 0;

int16_t  AnglePWM = 0;
int16_t  SpeedPWM = 0;
int16_t  TurnPWM = 0;
int16_t  TurnPWML = 0;
int16_t  TurnPWMR = 0;
uint8_t  CCDCount = 0;
uint8_t  SpeedCount = 0;
uint8_t  TurnCount=0;

uint8_t  SpeedSwitch = 0;
uint8_t  TurnSwitch = 0;

uint16_t blacktime = 0;
uint16_t podao_time;
uint16_t infrared_value = 0;
uint16_t podao_value = 0;
uint16_t podao_flag = 0;
uint16_t podao = 0;
uint16_t black_threshold = 3300;
uint16_t podao_threshold = 900;
uint16_t podao_start_time = 0;
uint8_t  podao_start = 0;
uint16_t podao_angle = 3;
uint16_t podao_last_time = 1500;


uint16_t podao_angle_time = 0;
uint8_t  CCD_Mode = 0;// ccdɨ��ģʽ 0Ϊ��Ų����� ���߰��� �ϰ����
																	// 1Ϊ��Ųɿ�� ���߰��� �ϰ����

uint8_t Start_Mode = 0; //0ģʽΪ������������� 1ģʽΪ�رռ��ͣ�� 2ģʽΪ�ر�����ͣ�� ��ʱ����

/**
*  @name   : ISR_Init
*  @author : ZX
*  @tips   : �жϳ�ʼ��
*/
void ISR_Init(void)
{
	PIT_InitTypeDef PIT0_Structure;
	PIT0_Structure.PITx = PIT0;
	PIT0_Structure.PIT_Interval = 1; //1ms
	PIT_Init(&PIT0_Structure);
	PIT_ITConfig(PIT0,PIT_IT_TIF,ENABLE);
	NVIC_EnableIRQ(PIT0_IRQn);
}

uint16_t Test = 0; //��⵽�Ժ��ӳ�һ��ʱ���ٿ������
uint16_t StopFlag = 0;  //��⵽ͣ���źź���ʱһ��ʱ��
uint16_t  StopTime = 15000; //��ʱ��ʱ��
/**
*  @name   : PIT0_IRQHandler
*  @author : ZX
*  @tips   : ���ڶ�ʱ���жϷ�����
*/
void PIT0_IRQHandler(void)
{
	PIT_ClearITPendingBit(PIT0,PIT_IT_TIF);//����жϱ�־λ
	
	DisableInterrupts(); //�ر����ж�
	
	if(StartSwitch == 1)		//�������ģʽ �������ͣ��
	{ 											
		if(Start == 0) 
		{ //�ȴ���ⷢ��
				if(Start_in == 1)  //��ⲻ�� ����
				{
					Start = 1; //����
					start_angle_time = 300;
				}
				else //��⵽ �ȴ�
				{
					Start = 0;
				}
		}
		else if(Start == 1) //�ȴ����ͣ��
		{
			Test ++;
			if(Test >= StopTime)
			{
				Test = 0;
				StopFlag = 1;
			}
			if(Start_in == 0 && StopFlag == 1) 
			{ //��⵽ ����ͣ��ģʽ ������ʱ����ⲻ����ͣ��
				/*if(++StopFlag >= StopTime) { //��⵽ͣ���ź��Ժ��ӳ�2sͣ��
					Start = 2; //ͣ��ģʽ
					StopFlag = 0;
				*/
				Start = 2;
			}
				
		}
		else if(Start == 2)  //�쳣״̬�µı���
		{
			if(Start_in == 1) //��⵽����
			{
				Start = 3;
			}
		}
		
		if(Start == 3)
		{
			SpeedOut(1,0);
			SpeedOut(2,0);
		}
		
		if(Stop == 1)
		{
			SpeedOut(1,0);
			SpeedOut(2,0);
		}
		
	}
	else if(StartSwitch == 2) //ֻ������� �ر�ͣ��
	{
		if(Start == 0) { //�ȴ���ⷢ��
				if(Start_in == 1)  //��ⲻ�� ����
				{
					Start = 1; //����
					start_angle_time = 200;
				}
				else //��⵽ �ȴ�
				{
					Start = 0;
				}
		}
		if(Start == 1)
		if(Stop == 1) //�쳣״̬�µı���  
		{
			SpeedOut(1,0);
			SpeedOut(2,0);
		}
		
	}
	else if(StartSwitch == 3) //�رռ�� ��ʱ����
	{
			if(Start == 0) {
				StartTime++;
				if(StartTime == 3000)
				{
					Start = 1;
					start_angle_time = 200;
				}
		  }
			if(Start == 1)
				if(Stop == 1) //�쳣״̬�µı���
				{
					SpeedOut(1,0);
					SpeedOut(2,0);
				}
	}
	
	Time++;           //ƽ�����ʱ��
	DisPlayTime++;    //��ʾˢ��ʱ��
	UartTime++;       //����ˢ��ʱ��
	
	SpeedOutPeriod++;
	TurnCount++;
	
	if(podao_start == 0)
		podao_start_time++;

	if(podao_start_time >= 6000)
	{
		podao_start_time = 0;
		podao_start = 1;
	}
	
	//ÿ1ms����������һ��
	//����ʽ���Ʒ�ʽ
	SpeedPWM = SpeedPWMOut(Speed,LastSpeed,SpeedOutPeriod); //�ٶ�pwm���
	
	if((Start != 2 && Stop == 0 && StartSwitch!=1) || (StartSwitch == 1 && Start != 3 && Stop == 0)) //�ȴ� ����
		Speed_ControlOut(endlong,SpeedPWM,DirectionOut); //�ٶ��ں�pwm���
	
	if(Time==5) { 
		Time= 0; //���� 
	}
	
	//�Ƕȿ���
	else if(Time==1) {
		AngleCalculate(); //��ȡ�Ƕ�
		Balace_Control(); //�Ƕ�PD����
	}
	
	//�ٶȿ���
	else if(Time==2) {	
			GetMotorSpeed(); //�ٶȲɼ�  
			SpeedCount++;
	  	if(SpeedCount == 20) {  //100ms�ٶȿ��Ƽ���
			  SpeedCalculate(); //�ٶ�pid����
			  SpeedCount = 0;//50ms��־����
				SpeedOutPeriod = 0;
		  }
	}
	
	//CCD�ɼ��������
	else if(Time==3) {
		CCDCount++;
		if(CCDCount==2) {
			Read_CCD1(); //�ع���� CCD�ɼ�
			Read_CCD3();
			GetBlackString(Pixels1); //�� Ѳ˫�� ֱ�� �ϰ�
			GetBlackString2(Pixels3); //���� ʮ��
			Speed_Stratage();  // �ٶȲ���
			path_element(); //Ԫ��
			errorcalculate();  //ƫ�����
			TurnCalculate(); //�������pid����
			CCDCount = 0;	
			TurnCount = 0;
		}
	}
	
	else if(Time==4) {
		
	}
	
	//�����������
	if(DisPlayTime==50) {
		DisPlay = 1;
		DisPlayTime = 0;
	}
	
	if(UartTime==2) {
		UartOK = 1;
		UartTime = 0;
	}
	
	if(Zhijiao_Switch && !podao) 
	{ //ֱ��
			if(++blacktime >= 200)
			{
				blacktime = 200;
				if(Black1_in == 1 && Black2_in == 1) // black
				{
						if(black_area == 0) 
						{
							black_area = 1;
							black_angle_time = 100;
							//Beep();
						}
						else  
						{
							black_area = 0;
							zhijiao_cnt = 0;
							black_angle_time = 0;
							//NoBeep();
						}
						blacktime = 0;
				}
			}
	}
	
	// �µ� �Թܲ���ô׼ȷ
		if(Podao_Switch && podao_start) 
		{
				if(++podao_flag>500) 
				{
					podao_flag = 500;
					podao_value = ADC_QuickReadValue(Infrared_Podao_Map);
					if(podao_value < podao_threshold) 
					{
						if(podao==0)  
						{
							podao = 1;
							podao_angle_time = 200;
						}
						
						podao_flag = 0; 
					} 
				}
			
			 if(podao) 
			 {
				 if(++podao_time > podao_last_time)//1.5s
				 {
					 podao_time = 0;
					 podao = 0; 
					 podao_angle_time = 0;
				 }
				 //Obstacle_Switch = 0;
				// Single_Mid_Switch = 0;
			 }
	 }
	
	if( obstacle || zhijiao || podao || mid_black || shizi || black_area) Beep();
  else           NoBeep();
	
	EnableInterrupts();
}
