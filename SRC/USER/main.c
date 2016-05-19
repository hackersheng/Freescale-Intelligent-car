/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @���ݵ��ӿƼ���ѧ ��Ϣ����ѧԺCCD�����
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*						Edited on 2014.9.11. Added ccd,uart,ftm,adc,lptmr driver. Used to test CCD.
*						Edited on 2014.9.21. Fixed FTM bug and added the pulse width transformed to duty
																 and added the sevro control duty.Tested the CCD_Deal 's time.
																 edited the adc.c on the hardwareaverage_32.

						Edited on 2014.10.27 added MMA8452 and L3G4200D
						Edited on 2014.11.06 added MMA7361L
							 
				MAP : 
							 ������ C5 A5
							 CCD:
								 CCD1 SI:C6    CLK:C10   AD:C0
								 CCD2 SI:C4    CLK:C8    AD:C1
								 CCD3 SI:C15   CLK:C17   AD:B2
							 ��� ��:D4 D5  ��:D6 D7
							 OLED
									SCL   			B23   //D0
									SDA         B21   //D1
									RST         B19   //RST
									DC          B17   //DC
							 L3G4200D     
									SCL:B0    SDA:B1
							 MMA7361
							 X:B10  Y:B11  Z:A17 GS:C9 SL:C11
							 ����
									RX:E24   TX:E25
							 KEY
							 S8:A4 S7:A16 S3:A12 S5:A13 S6:B9
							 
							 endlongp 3071	endlongd 200 angelset 225 amspeed 1260 speedp 170 speedi 60 speedd0
						turnp 340 turnd 620   lmd570 rmd200 threshold 10
						
						һ�� Amspeed 
						���� Amspeed 1450 SpeedP 140 I 70 TurnP 324 D620 LM250 RM325 zhijiaoflag 13
						����         1250        140   70       340  500   240   300             16
						===========================================================================================
						���� ���ԣ�����  edited on 7-26  ------- By ZX
						
						1.�ı��е�ṹ ������ ���Ҹı��صļܷ� ���ڵ�·����  ���� Ƕ�뵽��ģ�ײ���ȥ����ʦ�� 
						2.Ԫ�ص��Ż� ���ߵ��ж����� Ӧ������ߵ����� �ٽ��а���ɨ�� ���Ұ����Ա�Ϊ�� ���ߵĶ��ߴ���
							ֱ�ǵ�zhijiao_countӦ�����ٶȲ�һ�����ı� ��ֱ�ǿɳ���ʹ�ñ����� 
							����ڴ��Ĵ���
						3.ccd��ȡ���ߵ��㷨��Ҫ�Ż� ����Ӧ�ö���һЩ�����ͼ�� �ɼ��жϳ�����ʮ�� ͼ����Ҫ��ʮ�ֵĴ��� 
						4.��ŵĴ��� ��ͨpd �� ת�������� �ټ�������ϵ�� ���ʼ���ɲο�ȥ�걱�ʼ������� ��һ���˽�һ��˥����� ���ҿɼ��붯̬pd 
						5.����ccd�Ĵ�����Ҫ���� �µ��ɳ�����ccd�ж� ������ccd�������� Զǰհ��Ҫ���жϵ����� �ϰ��ɿ�Զǰհ �������Ҳ�ɿ�Զǰհ
						6.�ı������ǵĳ�ʼ�� �����˲��ɳ��Զ������Ƕ�������ֵ���д��� ���� ����һ��˥�� ��ͨ��ͨ�˲� 
						7.ȥ��CCD������ �������Ӳ����������

						��CCDͼ���㷨����ɲο��ھŽ�֣���Ṥҵ ���ʵļ������� 
						CCD������������Ϊ�ã�����1 һ����żӵ��� һ���ж�Ԫ�� һ���ֲ��������ǰհ
																 ����2 һ����żӵ��� һ���ж�Ԫ�� ���߿�����
						
*/
#include "includes.h" //���ͷ�ļ� �������� �궨�� �ⲿȫ�ֱ��� �������ڴ�ͷ�ļ���

/**
*  @name   : main
*  @author : ZX
*  @tips   : ������
*/
int main(void)
{
	DisableInterrupts(); //�ر����ж�
	
	ALL_Init(); //����ģ���ʼ��
	
	/**
	*@function Flash����洢 ������
	*/
	Data_Read();
	
	/**
	*@Tips!!!!  У׼����
	*/
	/*
	if(EndlongP == 65535)   EndlongP = 3200;
	if(EndlongD == 65535)   EndlongD = 250;
	*/
	/*
	if(Angle_Set ==  45)  Angle_Set = 140;
	if(black_threshold == 65535) black_threshold = 58000;
	if(podao_threshold == 65535) podao_threshold = 800;
	if(AmSpeed ==  20)    AmSpeed = 1900;
	if(Speed_P ==  0)    Speed_P = 120;
	if(Speed_I ==  0)    Speed_I = 0;
	if(Speed_D ==  65535)    Speed_D = 0;
	if(S_bar_cnt == 45)   S_bar_cnt = 20000;
	if(DirectionP ==  40) DirectionP = 400;
	if(DirectionD ==  45) DirectionD = 600;
	if(zhijiao_count == 255) zhijiao_count = 13;	
	if(Threshold ==  10)    Threshold = 35;
	if(Threshold2 ==  255)    Threshold2 = 35;
	//if(Mid_Threshold ==  255)    Mid_Threshold = 40;
	if(LeftMotorDead == 10) LeftMotorDead = 250;
	if(RightMotorDead == 40) RightMotorDead = 250;
	
	if(Obstacle_Switch == 255)    Obstacle_Switch = 1;
	if(Single_Mid_Switch == 255)    Single_Mid_Switch = 1;
	if(Zhijiao_Switch == 255)    Zhijiao_Switch = 1;
	if(Podao_Switch == 255)    Podao_Switch = 1;
	if(StartSwitch == 255)    StartSwitch = 3;
	if(obstacle_conut == 255) obstacle_conut = 1;
	if(StopTime == 65535) StopTime = 1000;
	if(Auto == 255) Auto = 0;
	if(Up_Threshold == 255) Up_Threshold = 30;
	if(GYRO_Turn == 65535) GYRO_Turn = 350;
	
	*/
	/*
	Speed_P=130;
	S_bar_cnt = 20000;
	DirectionP = 380;
	DirectionD = 550;
	zhijiao_count = 13;
	Threshold = 30;
	Threshold2 = 35;
	Mid_Threshold = 40;
	Up_Threshold = 30;
  */
	//AmSpeed = 1600;
	//DirectionP = 360;
	//DirectionD = 700;
	EnableInterrupts(); //�����ж�
	while(1)
	{
		#if(TESTTIME==ON) //��ѡ����Գ�������ʱ�� �������´��� ��ʾ�����۲�IO��
			PAout(11) = 1;
			//Get_Angel();
			//RD_TSL(Pixels1);
			//RD_TSL(Pixels1
		  Get_Value();
			Get_Angel(); //��ȡ�Ƕ�
		  Balace_Control(); //�Ƕ�PD����
			PAout(11) = 0;
			//Get_Angel();
			//RD_TSL(Pixels1);
			//RD_TSL(Pixels1);
		  Get_Value(); 
		  Get_Angel(); //��ȡ�Ƕ�
		  Balace_Control(); //�Ƕ�PD����
			//
		
		#else	//��������
			if(UartOK) //ÿ5ms����һ��
			{
			  /*
				if(CCDflag == 1)
					SendImageData(Pixels1); //����CCDͼ��
			  else if(CCDflag == 2)
			   	SendImageData(Pixels3);
				*/
			//	Data_Send_Senser(GYRO_X,ACC_Z_Fix,ACC_Y_Fix,GYRO_Y,Angle_ACC*1000,Angle*1000,0); //������λ�������������ݿ�����
			  Data_Send_Senser(LeftSpeedNow,RightSpeedNow,CarSpeed,GYRO_Y,ccd_average,mid_ave,0); //������λ�������������ݿ�����
			// Data_Send_Senser(LeftBlack,RightBlack,LeftBlack2,RightBlack2,(LeftBlack+RightBlack)/2,obstacle_width,0); //������λ�������������ݿ�����
				//Serial_Send(Angle_Speed,Angle_IGYRO,Angle_ACC,Angle);
				//Data_Send_Senser(SpeedP,SpeedI,Speed,SpeedPWM,endlong,Angle,Angle_IGYRO); //������λ�������������ݿ�����
				UartOK = 0;
			} 
			if(DisPlay) //ÿ50msˢ��һ����ʾ��  OLEDʹ��ֱ�ӵ���Oled.h�������ʾ����
			{			
				
		   MenuDisplay();
			//	menu();
				DisPlay = 0;
			}
		 KeyScan(); //����Flash ����ȷ�Ͻ��󱣴����
		#endif
	}
}
/**
*  @name   : assert_failed
*  @author : ZX
*  @tips   :
*/
void assert_failed(uint8_t* file, uint32_t line)
{
	//����ʧ�ܼ��
	UART_printf("assert_failed:line:%d %s\r\n",line,file);
	while(1);
}
