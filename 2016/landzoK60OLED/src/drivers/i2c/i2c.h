/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
 * �ļ���       ��i2c.h
 * ����         ��I2Cͷ�ļ�
 *
 * ʵ��ƽ̨     ��
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��
 * �Ա���       ��
**********************************************************************************/

#include "i2c_cfg.h"

typedef enum I2Cn
{
    I2C0  = 0,
    I2C1  = 1
} I2Cn;

typedef enum MSmode
{
    MWSR =   0x00,  /* Master write  */
    MRSW =   0x01   /* Master read */
} MSmode;


void  I2C_init(I2Cn);                                         //��ʼ��I2C
void  I2C_WriteAddr(I2Cn, u8 SlaveID, u8 Addr, u8 Data);      //��ȡ��ַ�������
u8    I2C_ReadAddr(I2Cn, u8 SlaveID, u8 Addr);                //����ַ��д������





void  I2C_StartTransmission (I2Cn, u8 SlaveID, MSmode);       //��������