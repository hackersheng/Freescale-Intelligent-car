/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @���ݵ��ӿƼ���ѧ ��Ϣ����ѧԺCCD�����
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _INCLUDES_H_
#define _INCLUDES_H_

//�⺯��ͷ�ļ�
//--------LIB------------
#include "sys.h"
#include "gpio.h"
#include "delay.h"
#include "wdog.h"
#include "pit.h"
#include "dma.h"
#include "ftm.h"
#include "lptm.h"
#include "adc.h"
#include "tsi.h"
#include "dac.h"
#include "spi.h"
#include "i2c.h"
#include "pdb.h"
#include "sd.h"
#include "flash.h"
#include "stdio.h"
#include "uart.h"

//�����Լ������ͷ�ļ�
//------HARDWARE---------
#include "Data.h" //��ͷ�ļ����������Ӳ��ͷ�ļ� ��Ҫ�õ����ⲿȫ�ֱ���

#define  Inline  __forceinline //ǿ������


//����CPU
//Ĭ��Ϊ512
#define MDK60DN512

//����Ϊ512��Ϊ256
#ifndef MDK60DN512
	#define MDK60DN256
#endif

#define TESTTIME OFF

#define ON       1
#define OFF      0 

#define TRUE     1
#define FALSE    0

//function
#define abs(x)      ( (x)>0?(x):-(x) )
#define max(a,b)    ( (a)>(b)?(a):(b))
#define min(a,b)    ( (a)<(b)?(b):(a))

#define MA 3

#endif
