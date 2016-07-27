/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
 * �ļ���       ��isr.c
 * ����         ���жϴ�������
 *
 * ʵ��ƽ̨     ��
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��������ӹ�����
 * �Ա���       ��http://landzo.taobao.com/
**********************************************************************************/



/******************** (C) COPYRIGHT 2011 ������ӹ����� ********************
 * �ļ���       ��isr.c
 * ����         ���жϴ�������
 *
 * ʵ��ƽ̨     ��
 * ��汾       ��
 * Ƕ��ϵͳ     ��
 *
 * ����         ��������ӹ�����
 * �Ա���       ��http://landzo.taobao.com/
**********************************************************************************/



#include "common.h"
#include "include.h"


extern int mod;

u8 TIME0flag_5ms  = 0 ;
u8 TIME0flag_10ms = 0 ;
u8 TIME0flag_15ms = 0 ;
u8 TIME0flag_20ms = 0 ;
u8 TIME0flag_80ms = 0 ;
u8 TIME1flag_1s   = 0 ;
u8 TIME1flag_3s   = 0 ;

u16  TimeCount = 0 ;
extern int Power_Flag;
int sin_count=0;

extern double U0_present_value;
extern double I0_present_value;
extern double U2_present_value;

extern int U0_sample;
extern int I0_sample;
extern int U0_sample_max;
extern int I0_sample_max;

extern double U0_present_max;
extern double I0_present_max;

extern int test_flag;
extern int start_flag;
extern int ftm_flag;
   
extern void Refresh_Present();
extern void PID_control();
extern void PWM_Control();


/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�USART1_IRQHandler
*  ����˵��������1 �ж� ���� ������
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-14    �Ѳ���
*  ��    ע��
*************************************************************************/


void USART1_IRQHandler(void)
{
    uint8 ch;

    DisableInterrupts;		    //�����ж�

    //����һ���ֽ����ݲ��ط�
    ch = uart_getchar (UART1);      //���յ�һ������
    uart_sendStr  (UART1, "\n�㷢�͵�����Ϊ��");
    uart_putchar (UART1, ch);       //�ͷ��ͳ�ȥ

    EnableInterrupts;		    //�����ж�
}
/*************************************************************************
*                             ����Ƕ��ʽ����������
*
*  �������ƣ�PIT1_IRQHandler
*  ����˵����PIT1��ʱ�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-9-18    �Ѳ���
*  ��    ע��
*************************************************************************/
u8 PIT1Count = 0 ;
void PIT1_IRQHandler(void)
{
   PIT_Flag_Clear(PIT1);       //���жϱ�־λ
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�PIT0_IRQHandler
*  ����˵����PIT0 ��ʱ�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-18    �Ѳ���
*  ��    ע��
*************************************************************************/



void PIT0_IRQHandler(void)
{
   DisableInterrupts; 
  
  I0_present_max=I0_sample_max*3.3/4095;
  U0_present_max=U0_sample_max*3.3/4095;
  
   if(I0_present_max>=3.5)  //2.5Ϊ��Чֵ
  {
    gpio_set(PORTB ,5,0);
    Power_Flag=0;
  }
  
   TimeCount++;
   if(TimeCount==3000)
   {
     Refresh_Present();
     TimeCount=0;
     U0_present_max=0;
     I0_present_max=0;

   }

  EnableInterrupts;
  PIT_Flag_Clear(PIT0);       //���жϱ�־λ
}



/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�SysTick_Handler
*  ����˵����ϵͳ�δ�ʱ���жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-18    �Ѳ���
*  ��    ע��ucos���õõ�
*************************************************************************/
void SysTick_Handler(void)
{
    //    OSIntEnter();
    //    OSTimeTick();
    //    OSIntExit();
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�PendSV_Handler
*  ����˵����PendSV��������ϵͳ���ã��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-15    �Ѳ���
*  ��    ע��uC/OS�����л�����
*************************************************************************/
void PendSV_Handler(void)
{
}
/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�HardFault_Handler
*  ����˵����Ӳ���Ϸ��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-4    �Ѳ���
*  ��    ע��������LED��˸��ָʾ������Ӳ���Ϸ�
*************************************************************************/
void HardFault_Handler(void)
{
    while (1)
    {
        printf("\n****Ӳ���Ϸô���!!!*****\r\n\n");
    }
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�PORTA_IRQHandler
*  ����˵����PORTA�˿��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-1-25    �Ѳ���
*  ��    ע�����ź���Ҫ�Լ���ʼ�������
*************************************************************************/

   u16   key8cout = 0 ;
   u8    key8start = 0 ;
   u8    key8flg = 0 ;
   u16   key9cout = 0 ;
   u8    key9start = 0 ;
   u8    key9flg = 0 ;
   u16   key10cout = 0 ;
   u8    key10start = 0 ;
   u8    key10flg = 0 ;
   u16   key11cout = 0 ;
   u8    key11start = 0 ;
   u8    key11flg = 0 ;


void PORTD_IRQHandler(){




    u8  n = 0;    //���ź�
    n = 8;
    if(PORTD_ISFR & (1 << n))         //PTDn�����ж�
    {
        PORTD_ISFR  |= (1 << n);        //д1���жϱ�־λ
        if( key8cout == 0 )
        {
          key8start = 1 ;
        } else if( key8cout > 13)
        {
          key8cout = 0 ;
          key8start = 0 ;
          key8flg ++ ;
        }
    }

    n = 9;
    if(PORTD_ISFR & (1 << n))         //PTDn�����ж�
    {
        PORTD_ISFR  |= (1 << n);        //д1���жϱ�־λ
        if( key9cout == 0 )
        {
          key9start = 1 ;
        } else if( key9cout > 13)
        {
          key9cout = 0 ;
          key9start = 0 ;
          key9flg ++ ;
        }
    }

    n = 10;
    if(PORTD_ISFR & (1 << n))         //PTDn�����ж�
    {
        PORTD_ISFR  |= (1 << n);        //д1���жϱ�־λ
        if( key10cout == 0 )
        {
          key10start = 1 ;
        } else if( key10cout > 13)
        {

          key10cout = 0 ;
          key10start = 0 ;
          key10flg ++ ;

        }
    }
    n = 11;
    if(PORTD_ISFR & (1 << n))         //PTDn�����ж�
    {
        PORTD_ISFR  |= (1 << n);        //д1���жϱ�־λ
        if( key11cout == 0 )
        {
          key11start = 1 ;
        } else if( key11cout > 13)
        {
          key11cout = 0 ;
          key11start = 0 ;
          key11flg ++ ;
        }
    }
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�PORTB_IRQHandler
*  ����˵����PORTB�˿��жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-1-25    �Ѳ���
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸�
*************************************************************************/
void PORTB_IRQHandler()
{

  //  PORT_PCR_REG(PORTB , 10) |= PORT_PCR_ISF(1);
    u8  n = 0;    //���ź�
    n = 0;
    if(PORTB_ISFR & (1 << n))         //PTB0�����ж�
    {
        PORTB_ISFR  |= (1 << n);        //д1���жϱ�־λ
        /*  �û�����  */

    }

    n = 10;
    if(PORTB_ISFR & (1 << n))         //PTB10�����ж�
    {
        PORTB_ISFR  |= (1 << n);        //д1���жϱ�־λ
        /*  �û�����  */



    }
}


/*************************************************************************
*                             ����Ƕ��ʽ����������
*
*  �������ƣ�PIT2_IRQHandler
*  ����˵����PIT2��ʱ�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-9-18    �Ѳ���
*  ��    ע��
*************************************************************************/

void PIT2_IRQHandler(void)
{
   PIT_Flag_Clear(PIT2);       //���жϱ�־λ


   if(key8start)
   {
     key8cout ++ ;
   }
   if( key8cout > 150)
   {
     key8cout = 0 ;
     key8start = 0 ;
   }

   if(key9start)
   {
     key9cout ++ ;
   }
   if( key9cout > 150)
   {
     key9cout = 0 ;
     key9start = 0 ;
   }

   if(key10start)
   {
     key10cout ++ ;
   }
   if( key10cout > 150)
   {
     key10cout = 0 ;
     key10start = 0 ;
   }

   if(key11start)
   {
     key11cout ++ ;
   }
   if( key11cout > 150)
   {
     key11cout = 0 ;
     key11start = 0 ;
   }

}
/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�FTM0_IRQHandler
*  ����˵����FTM0���벶׽�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-25
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸ģ��ο����еĴ�������Լ��Ĺ���
*************************************************************************/


void FTM0_IRQHandler()
{
  
}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�FTM1_IRQHandler
*  ����˵����FTM1���벶׽�жϷ�����
*  ����˵������
*  �������أ���
*  �޸�ʱ�䣺2012-2-25
*  ��    ע�����ź���Ҫ�����Լ���ʼ�����޸ģ��ο����еĴ�������Լ��Ĺ���
*************************************************************************/


void FTM1_IRQHandler()
{
    u8 s = FTM1_STATUS;             //��ȡ��׽�ͱȽ�״̬  All CHnF bits can be checked using only one read of STATUS.
    FTM1_STATUS = 0x00;               //���жϱ�־λ
    
  I0_sample=ad_mid(ADC0, AD9,ADC_12bit);
  U0_sample=ad_mid(ADC0, AD12,ADC_12bit);
  
  if(U0_sample>U0_sample_max)
  {
   U0_sample_max=U0_sample;
  }
  
  if(I0_sample>I0_sample_max)
  {
    I0_sample_max=I0_sample;
  }
   
  PWM_Control();
}



volatile u8  pit_flag = 0;
volatile u32 dma_int_count = 0;


/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�DMA_CH4_Handler
*  ����˵����DMAͨ��4���жϷ�����
*  ����˵�����ǲɼ�����ͷ���ݣ�������λ����ͷAD���ݣ����Բɼ���300���㡣
             ���ñ�־λ�ܹ���ʱ���ơ�
*  �������أ���
*  �޸�ʱ�䣺2012-3-18    �Ѳ���
*  ��    ע��
*************************************************************************/
u8  DMA_Over_Flg = 0 ;     //�вɼ���ɱ�־λ
void DMA_CH4_Handler(void)
{
  //DMAͨ��4
  //  u16 i ;
 //    uart_putchar(UART0,0XAA);
    DMA_IRQ_CLEAN(DMA_CH4) ;
    DMA_IRQ_DIS(DMA_CH4);
    DMA_DIS(DMA_CH4);
    DMA_Over_Flg = 1 ;
  //   disable_irq(PORTC+87);

}

/*************************************************************************
*                             ������ӹ�����
*
*  �������ƣ�DMA_CH4_Handler
*  ����˵����DMAͨ��4���жϷ�����
*  ����˵�����ǲɼ�����ͷ���ݣ�������λ����ͷAD���ݣ����Բɼ���300���㡣
             ���ñ�־λ�ܹ���ʱ���ơ�
*  �������أ���
*  �޸�ʱ�䣺2012-3-18    �Ѳ���
*  ��    ע��
*************************************************************************/
void DMA_CH0_Handler(void)
{

    //DMAͨ��0
    printf("DMA_CH0_Handler\n");
    DMA_IRQ_CLEAN(DMA_CH0);                             //���ͨ�������жϱ�־λ    (���������ٴν����ж�)
    DMA_EN(DMA_CH0);                                    //ʹ��ͨ��CHn Ӳ������      (�������ܼ�������DMA����)
  //   uart_putchar(UART0,0XAA);
}



volatile u8 LPT_INT_count = 0;
void  LPT_Handler(void)
{
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;   //���LPTMR�Ƚϱ�־
    LPT_INT_count++;                    //�ж������1
}