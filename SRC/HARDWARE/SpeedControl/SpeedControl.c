#include "SpeedControl.h"

/*PID*/
uint16_t EndlongP = 4000;//467;
uint16_t EndlongI = 0;
uint16_t EndlongD = 250;//75;

uint16_t DirectionP = 65;
uint16_t DirectionA = 50;
uint16_t DirectionD = 6;

uint16_t DirectionPL = 65;
uint16_t DirectionIL = 0;
uint16_t DirectionDL = 0;
uint16_t DirectionPR = 65;
uint16_t DirectionIR = 0;
uint16_t DirectionDR = 6;

uint16_t GYRO_Turn;

uint16_t Speed_P = 34;
uint16_t Speed_I = 41;
uint16_t Speed_D = 0;

/*ֱ��PWM*/
int16_t  endlong;
int16_t  Last_endlong;

/*�ٶ�PWM*/
int16_t  PWMLeft;
int16_t  PWMRight;
int16_t  PWMLEFTERROR=0;
int16_t  PWMRIGHTERROR=0;

/*�ٶȿ���*/
uint16_t  AmSpeed = 135;
uint16_t  Save_speed = 0;
int16_t   SpeedError; 
int16_t   SpeedLastError;
int16_t   SpeedLastLastError;
int16_t   LastSpeed;
int16_t   LastCarSpeed;
int16_t   Speed;
float     SpeedP;
float     SpeedI;
float     SpeedD;
uint16_t  LeftMotorDead = 300;
uint16_t  RightMotorDead = 300;

/*�ٶȲɼ�*/
int16_t LeftSpeedNow;
int16_t RightSpeedNow;
int32_t LeftSpeedSigma = 0;
int32_t RightSpeedSigma = 0;
float CarSpeed ;
/**motor**/
uint32_t  MotorPool=0;

/***�������***/
int16_t  Direction=0;
int16_t  lastDirection=0;

int16_t  DirectionL=0;
int16_t  lastDirectionL=0;
int16_t  DirectionR=0;
int16_t  lastDirectionR=0;

uint16_t A = 0,B = 0,C = 0;

/**
* @name: Balace_Control
* @author: ZX
* @tips :
*/
void Balace_Control(void)
{
	int16_t f_nspeed;
	
	Last_endlong = endlong; //������һ�ε���ֵ

	if(Stop) {
		EndlongP = 0;
		EndlongD = 0;
	}

	f_nspeed = EndlongP * (Angle) + EndlongD * (Angle_Speed);
	
	endlong = f_nspeed;
	
	if(endlong > EndlongMax)	endlong = EndlongMax;
	if(endlong < EndlongMin)	endlong = EndlongMin;
}
/**
* @name: PWM_Out
* @author: ZX
* @tips :
*/
void PWM_Out(void)
{
	SpeedOut(1,PWMLeft);
	SpeedOut(2,PWMRight);
}

/**
* @name: Speed_Control
* @author: ZX
* @tips :
*/
void Speed_ControlOut(int16_t MotorEndlong,int16_t MotorSpeed,int16_t MotorTurn)
{
	//GYRO_Y = Read_Gyro_Y();
	PWMLeft  = MotorEndlong - MotorSpeed - MotorTurn;//- GYRO_Y_Fix * GYRO_Turn/100;
	PWMRight = MotorEndlong - MotorSpeed + MotorTurn;// + GYRO_Y_Fix * GYRO_Turn/100;
	
	//�����������
	if(!Stop) {
		if(PWMLeft > 0)		  PWMLeft  += LeftMotorDead;
		else						    PWMLeft  -= LeftMotorDead;
		
		if(PWMRight > 0)    PWMRight += RightMotorDead;
		else							  PWMRight -= RightMotorDead;
	}
	
	//����޷�
	if(PWMLeft > PWMMax) {
		PWMLeft = PWMMax;
	}
	else if(PWMLeft < PWMMin) {
		PWMLeft = PWMMin;
	}
	
	if(PWMRight > PWMMax) {
		PWMRight = PWMMax; 
	}
	else if(PWMRight < PWMMin) {
		PWMRight = PWMMin;
	}
	
	PWM_Out();
}

#define AANGPERIODFAV  (5) 
/**
* @name: AAangPWMOut
* @author: ZX
* @tips :  NewAngPWM -------------- ��ǰ������PWM
*					 LastAangPWM ------------ ��һ�ε�����PWM
*          PeriodCount ------------ ƽ������
*/
int16_t AAangPWMOut(int16_t NewAangPWM ,int16_t LastAangPWM,uint8_t PeriodCount)
{
    int16_t  AangPWMfav ;
    int16_t  AangOUT ;
    AangPWMfav = NewAangPWM - LastAangPWM ;
    AangOUT = AangPWMfav *(PeriodCount)/AANGPERIODFAV + LastAangPWM ;
    
    return AangOUT ;  
}

#define SPEEDPERIODFAV  (100)
/**
* @name: SpeedPWMOut
* @author: ZX
* @tips :  NewspeedPWM ------------- ��ǰ������PWM
*					 LastspeedPWM ------------ ��һ�ε�����PWM
*          PeriodCount ------------- ƽ������
*/
int16_t SpeedPWMOut(int16_t NewspeedPWM ,int16_t LastspeedPWM,uint8_t PeriodCount)
{
    int16_t  speedPWMfav ;
    int16_t  SpeedOUT ;
    speedPWMfav = NewspeedPWM - LastspeedPWM ;
    SpeedOUT = speedPWMfav *(PeriodCount)/SPEEDPERIODFAV + LastspeedPWM ;
    
    return SpeedOUT ;
}

#define TURNPERIODFAV  (10)
/**
* @name: SpeedPWMOut
* @author: ZX
* @tips :  NewturnPWM -------------- ��ǰת�������PWM
*					 LastturnPWM ------------- ��һ��ת�������PWM
*          PeriodCount ------------- ƽ������
*/
int16_t TurnPWMOut(int16_t NewturnPWM ,int16_t LastturnPWM,uint8_t PeriodCount)
{
    int16_t  turnPWMfav ;
    int16_t  turnOUT ;
    turnPWMfav = NewturnPWM - LastturnPWM ;
    turnOUT = turnPWMfav *(PeriodCount)/TURNPERIODFAV + LastturnPWM ;
                
    return turnOUT ;
}

/**
* @name: GetMotorSpeed
* @author: ZX
* @tips :
*/
void GetMotorSpeed(void)
{
	 /*RightSpeedNow  = Read_Left_CNT(); 	//��ȡ�����ٶ�
	 LeftSpeedNow   =  Read_Right_CNT(); //��ȡ�����ٶ�
	 */
	//���������������෴���� ��ת��Ӹ���
	/*if(PWMLeft < 0) LeftSpeedNow = -LeftSpeedNow;
	LeftSpeedSigma += LeftSpeedNow;
	
	if(PWMRight < 0) RightSpeedNow = -RightSpeedNow;
	RightSpeedSigma += RightSpeedNow;*/
	/*
	if(Left_in == 1)
	{
		LeftSpeedNow   =  Read_Right_CNT(); //��ȡ�����ٶ�
	}
	else
	{
		//LeftSpeedNow   =  -Read_Right_CNT(); //��ȡ�����ٶ�
	}
	
	
	if(Right_in == 1)
	{
		RightSpeedNow  = -Read_Left_CNT(); 	//��ȡ�����ٶ�
		LeftSpeedNow   =  -Read_Right_CNT();
	}
	else
	{
		RightSpeedNow  = Read_Left_CNT(); 	//��ȡ�����ٶ�
	}*/
	
	RightSpeedNow = Read_Left_CNT(); 	//��ȡ�����ٶ�
	LeftSpeedNow  = Read_Right_CNT();
	
	LeftSpeedSigma += LeftSpeedNow;
	RightSpeedSigma += RightSpeedNow;
	
}

/**
* @name: Speed_Stratage
* @author: ZX
* @tips :  
*/
void Speed_Stratage(void)
{
	if(Start == 1)
	{
		if(abs(offset) > 20)
			Save_speed = AmSpeed;
		else
			Save_speed  = AmSpeed;
		
		if(black_area)
		{
			
			Save_speed = AmSpeed * 0.8;
		}
		
		if(shizi)
		{
			Save_speed = AmSpeed * 1.1;
		}
		
		if(podao)
		{
			Save_speed = AmSpeed * 0.8;
		}
		
		
	}
	else
	{
		Save_speed = 0;
	}
}



/**
* @name: SpeedCalculate
* @author: ZX
* @tips :  P�Ӵ��ٶȻ��� ����Ӱ��ֱ��
*/
void SpeedCalculate(void)
{
	CarSpeed = (LeftSpeedSigma + RightSpeedSigma) / 2.0; //��ǰ�ٶ�
	LeftSpeedSigma = 0;
	RightSpeedSigma = 0;

	SpeedError = Save_speed - CarSpeed; //�����ٶ�ƫ��ˢ��
	SpeedLastLastError = CarSpeed - LastCarSpeed; //���ϴ��ٶ�ƫ��ˢ��
	LastCarSpeed = CarSpeed; //�ϴ��ٶ�ƫ��ֵˢ��

	SpeedP =  (float)(SpeedError * Speed_P / 10.0 );// / 10.0);
	SpeedD =  (float)(SpeedLastLastError * Speed_D / 100.0);
	SpeedI += (float)(SpeedError * Speed_I / 1000.0);
  
	
  if(SpeedI > SpeedMax) {
		SpeedI = 9000;
	}
	else if(SpeedI < SpeedMin) {
		SpeedI = -9000;
	}
	
	LastSpeed = Speed;
	Speed = SpeedP + SpeedI + SpeedD;

	if(Speed > SpeedMax) {	
		Speed = SpeedMax;
	}
	else if(Speed < SpeedMin) {
		Speed = SpeedMin;
	}
}

float T = 0.4; //һ��˥��ϵ��
int16_t DirectionOut = 0;
int16_t LastDirectionOut = 0;
void TurnCalculate(void)
{ 
  float  turn_P;
  float  turn_D;
	//float  turn_A;
	int16_t diff = 0,diff_two_order = 0;
	
	if(single)
	{
		diff = offset2;
		diff_two_order = offset_two_order2;
	}
	else
	{
		
		diff = offset;
		if(!zhijiao)
			diff_two_order = offset_two_order;
	}
	
	lastDirection = Direction;
	LastDirectionOut = DirectionOut;
	
	turn_P = (float)(diff * A / 10.0);
  turn_D = (float)(diff_two_order * B);
 // turn_A = (float)(diff * diff * DirectionA / 100.0);
	
	Direction = (int16_t)(turn_P + turn_D);// + turn_A);
	
	DirectionOut = T * Direction + (1-T) * LastDirectionOut; //һ��˥�����

	if(DirectionOut > DirectionMax) {	
		DirectionOut = DirectionMax;
	}
	else if(DirectionOut < DirectionMin) {
		DirectionOut = DirectionMin;
  }
}
