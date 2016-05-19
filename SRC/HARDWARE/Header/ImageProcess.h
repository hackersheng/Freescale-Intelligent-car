/**
*                   === Threshold Studio ===
* @name   : Freescale Intelligent Car Racing
* @author : ZhaoXiang @���ݵ��ӿƼ���ѧ ��Ϣ����ѧԺCCD�����
* @Tips   : Based on MK60DN212 200Mhz Core Speed and 100Mhz Bus Speed
* @Lib    : CH_Lib v2.4
* @Date   : Found date 2014.9.7
*/
#ifndef _IMAGEPROCESS_H_
#define _IMAGEPROCESS_H_


#include "Data.h"

void GetBlackString(uint8_t *line);
void GetBlackString2(uint8_t *line);
void errorcalculate(void);
void GetBlack(uint8_t *line);
void GetBlack2(uint8_t *line);
void PictureADSort(uint8_t* image);
void PictureDiff(uint8_t *image);
uint16_t CCD_Average(uint8_t *image);
void path_element(void);

#endif
