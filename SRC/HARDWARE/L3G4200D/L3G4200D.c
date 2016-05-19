/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @���ݵ��ӿƼ���ѧ ��Ϣ����ѧԺCCD�����
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#include "L3G4200D.h"

//GYRO
/**
*  @name   : Single_WriteL3G4200D
*  @author : ZX
*/
void Single_WriteL3G4200D(I2C_Type* I2Cx,uint8_t REG_Address,uint8_t REG_data)
{
	//���͵�ַ
	I2C_GenerateSTART(I2Cx);
	I2C_Send7bitAddress(I2Cx,L3G4200_Addr,I2C_MASTER_WRITE);
	I2C_WaitAck(I2Cx);
	//������Ҫд��ļĴ�����ַ
	I2C_SendData(I2Cx,REG_Address);
	I2C_WaitAck(I2Cx);
	//��������
	I2C_SendData(I2Cx,REG_data);
	I2C_WaitAck(I2Cx);
	//����
	I2C_GenerateSTOP(I2Cx);
	while(I2C_IsLineBusy(I2Cx) == TRUE);
}

/**
*  @name   : Single_ReadL3G4200D
*  @author : ZX
*/
uint8_t Single_ReadL3G4200D(I2C_Type* I2Cx,uint8_t REG_Address)
{  	
		uint8_t result;
    //���ʹӻ���ַ
		I2C_GenerateSTART(I2Cx);
		I2C_Send7bitAddress(I2Cx,L3G4200_Addr,I2C_MASTER_WRITE);
    I2C_WaitAck(I2Cx);
		//��Ҫ��ȡ�ļĴ���
    I2C_SendData(I2Cx, REG_Address);
    I2C_WaitAck(I2Cx);
		//���¿�ʼ
		I2C_GenerateRESTART(I2Cx);
	  //������
    I2C_Send7bitAddress(I2Cx,L3G4200_Addr+1,I2C_MASTER_READ);
    I2C_WaitAck(I2Cx);
		//����KinetisΪ����ģʽ
		I2C_SetMasterMode(I2Cx,I2C_MASTER_READ);
    I2C_GenerateAck(I2Cx);
    result = I2Cx->D;
    I2C_WaitAck(I2Cx);
    //����ֹͣ�ź�
    I2C_GenerateSTOP(I2Cx);
		//��ȡ����
    result =  I2Cx->D;
    return result;
}

/**
*  @name   : InitL3G4200D
*  @author : ZX
*/
void InitL3G4200D(void)
{
	I2C_InitTypeDef I2C_InitStruct2;
	I2C_InitStruct2.I2CxMAP = L3G4200D_I2C_PIN_MAP;
	I2C_InitStruct2.I2C_ClockSpeed = I2C_CLOCK_SPEED_400KHZ;
	I2C_Init(&I2C_InitStruct2);

	/*Single_WriteL3G4200D(L3G4200D_I2C_PORT,CTRL_REG1, 0x0f);   //800Hz????,110hz cut-off
	Single_WriteL3G4200D(L3G4200D_I2C_PORT,CTRL_REG2, 0x00);   //��ͨ�˲�  ,1Hz  
	Single_WriteL3G4200D(L3G4200D_I2C_PORT,CTRL_REG3, 0x08);   //
	Single_WriteL3G4200D(L3G4200D_I2C_PORT,CTRL_REG4, 0x30);   //+-500dps BDU = 1 BLE = 1;
	Single_WriteL3G4200D(L3G4200D_I2C_PORT,CTRL_REG5, 0x00);   //�˲�ʹ��
	*/
	Single_WriteL3G4200D(L3G4200D_I2C_PORT,CTRL_REG1, 0xFF);   //800Hz���� 110hz cut-off
	//Single_WriteL3G4200D(L3G4200D_I2C_PORT,CTRL_REG2, 0x06);   //��ͨ�˲�  ,1Hz  
	Single_WriteL3G4200D(L3G4200D_I2C_PORT,CTRL_REG3, 0x08);   //
	Single_WriteL3G4200D(L3G4200D_I2C_PORT,CTRL_REG4, 0xD0); //f0 2000dps //D0 //+-500dps BDU = 1 BLE = 1;
	//Single_WriteL3G4200D(L3G4200D_I2C_PORT,CTRL_REG5, 0x12);   //�˲�ʹ��
}	
/**
*  @name   : Read_Gyro_X
*  @author : ZX
*/
int Read_Gyro_X(void)
{
	uint8_t data_H;
	uint8_t data_L;
	//float temp;
	int dis_data;
	
	
	data_L = Single_ReadL3G4200D(L3G4200D_I2C_PORT,OUT_X_L);
	data_H = Single_ReadL3G4200D(L3G4200D_I2C_PORT,OUT_X_H);

	dis_data = (data_H<<8) + data_L;  
	//temp = (float)dis_data;
	
	return dis_data;
}
/**
*  @name   : Read_Gyro_Y
*  @author : ZX
*/
int Read_Gyro_Y(void)
{
	//float temp;
	int dis_data;
	uint8_t data_H;
	uint8_t data_L;
	
	data_L = Single_ReadL3G4200D(L3G4200D_I2C_PORT,OUT_Y_L);
	data_H = Single_ReadL3G4200D(L3G4200D_I2C_PORT,OUT_Y_H);

	dis_data = (data_H<<8) + data_L;  
	//temp = (float)dis_data*0.07;
	
	return dis_data;
}
/**
*  @name   : Read_Gyro_Z
*  @author : ZX
*/
int Read_Gyro_Z(void)
{
	//float temp;
	int dis_data;
	uint8_t data_H;
	uint8_t data_L;
	
	data_L = Single_ReadL3G4200D(L3G4200D_I2C_PORT,OUT_Z_L);
	data_H = Single_ReadL3G4200D(L3G4200D_I2C_PORT,OUT_Z_H);

	dis_data = (data_H<<8) + data_L;  
	//temp = (float)dis_data*0.07;
	
	return dis_data;
}

