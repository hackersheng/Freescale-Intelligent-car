/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @���ݵ��ӿƼ���ѧ ��Ϣ����ѧԺCCD�����
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.5
* @Date   : Found date 2014.9.7
*/
#include "Encoder.h"

uint8_t dummy1, dummy2;

static const uint32_t DMA_PORT_TriggerSourceTable[] = 
{
    PORTA_DMAREQ,
    PORTB_DMAREQ,
    PORTC_DMAREQ,
    PORTD_DMAREQ,
    PORTE_DMAREQ,
};

/**
 * @brief  DMA �������������ʼ��     
 * @param  dmaChl :DMAͨ����
 * @param  instance :�˿ں� ����HW_GPIOA
 * @param  pinIndex :���ź�
 * @retval None
 */
static void DMA_PulseCountInit(uint32_t dmaChl, GPIO_Type *GPIOx, uint32_t instance, uint16_t pinIndex)
{
		GPIO_InitTypeDef GPIO_InitStructure1;
		DMA_InitTypeDef DMA_InitStruct1 = {0};
    /* ����2·���� ����ΪDMA���� */
		GPIO_InitStructure1.GPIOx = GPIOx;
		GPIO_InitStructure1.GPIO_Pin = pinIndex;
		GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DMA_RISING;
		GPIO_Init(&GPIO_InitStructure1);
    //GPIO_QuickInit(instance, pinIndex, kGPIO_Mode_IFT);
    /* ����ΪDMA�����ش��� */
    //GPIO_ITDMAConfig(instance, pinIndex, kGPIO_DMA_RisingEdge, true);
    /* ����DMA */
    DMA_InitStruct1.chl = dmaChl;  
    DMA_InitStruct1.chlTriggerSource = DMA_PORT_TriggerSourceTable[instance];
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal; 
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = DMA_CITER_ELINKNO_CITER_MASK; /* ���ֵ */
    
    DMA_InitStruct1.sAddr = (uint32_t)&dummy1;
    DMA_InitStruct1.sLastAddrAdj = 0; 
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = (uint32_t)&dummy2; 
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 0; 
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    DMA_Init(&DMA_InitStruct1);
    /* �������� */
    DMA_EnableRequest(dmaChl);
}
/**
*  @name   : Encoder_Init
*  @author : ZX
*  @tips   : 
*/
void Encoder_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;
	
	GPIO_InitStructure1.GPIOx = Left_DDR;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = Left_DRR_pin;
	GPIO_Init(&GPIO_InitStructure1);
	
	GPIO_InitStructure1.GPIOx = right_DDR;
	GPIO_InitStructure1.GPIO_InitState = Bit_RESET;
	GPIO_InitStructure1.GPIO_IRQMode = GPIO_IT_DISABLE;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin = right_DDR_Pin;
	GPIO_Init(&GPIO_InitStructure1);
	
	DMA_PulseCountInit(HW_DMA_CH0,PTC,HW_GPIOC,5);
	DMA_PulseCountInit(HW_DMA_CH1,PTA,HW_GPIOA,5);
}
/**
*  @name   : Read_Left_CNT
*  @author : ZX
*  @tips   : 
*/
uint32_t Read_Left_CNT(void)
{
		uint32_t value;
    /* CH0 */
    value = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH0);
    /* ������� */
    DMA_CancelTransfer();
    DMA_SetMajorLoopCounter(HW_DMA_CH0, DMA_CITER_ELINKNO_CITER_MASK);
    /* ��ʼ��һ�δ��� */
    DMA_EnableRequest(HW_DMA_CH0);
		return value;
}
/**
*  @name   : Read_Right_CNT
*  @author : ZX
*  @tips   : 
*/
uint32_t Read_Right_CNT(void)
{
		uint32_t value;
    /* CH1 */
    value = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH1);
    /* ������� */
    DMA_CancelTransfer();
    DMA_SetMajorLoopCounter(HW_DMA_CH1, DMA_CITER_ELINKNO_CITER_MASK);
    /* ��ʼ��һ�δ��� */
    DMA_EnableRequest(HW_DMA_CH1);
		return value;
}

/**
*  @name   : Read_CNT
*  @author : ZX
*  @tips   : 
*/
void Read_CNT(void)
{
	  /* ����DMA �ǵ������� ������Ҫ�����ֵ��һ�� */
    /* CH0 */
    LeftSpeedNow = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH0);
    /* CH1 */
    RightSpeedNow = DMA_CITER_ELINKNO_CITER_MASK - DMA_GetMajorLoopCount(HW_DMA_CH1);
    /* ������� */
    DMA_CancelTransfer();
    DMA_SetMajorLoopCounter(HW_DMA_CH0, DMA_CITER_ELINKNO_CITER_MASK);
    DMA_SetMajorLoopCounter(HW_DMA_CH1, DMA_CITER_ELINKNO_CITER_MASK);
    /* ��ʼ��һ�δ��� */
    DMA_EnableRequest(HW_DMA_CH0);
    DMA_EnableRequest(HW_DMA_CH1);
}
