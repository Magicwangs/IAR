/******************** (C) COPYRIGHT 2011 蓝宙电子工作室 ********************
 * 文件名       ：isr.c
 * 描述         ：中断处理例程
 *
 * 实验平台     ：
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：蓝宙电子工作室
 * 淘宝店       ：http://landzo.taobao.com/
**********************************************************************************/



/******************** (C) COPYRIGHT 2011 蓝宙电子工作室 ********************
 * 文件名       ：isr.c
 * 描述         ：中断处理例程
 *
 * 实验平台     ：
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：蓝宙电子工作室
 * 淘宝店       ：http://landzo.taobao.com/
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

extern int pwm_gain;
extern double pwm_gain_temp;
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
*                             蓝宙电子工作室
*
*  函数名称：USART1_IRQHandler
*  功能说明：串口1 中断 接收 服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*  备    注：
*************************************************************************/


void USART1_IRQHandler(void)
{
    uint8 ch;

    DisableInterrupts;		    //关总中断

    //接收一个字节数据并回发
    ch = uart_getchar (UART1);      //接收到一个数据
    uart_sendStr  (UART1, "\n你发送的数据为：");
    uart_putchar (UART1, ch);       //就发送出去

    EnableInterrupts;		    //开总中断
}
/*************************************************************************
*                             蓝宙嵌入式开发工作室
*
*  函数名称：PIT1_IRQHandler
*  功能说明：PIT1定时中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-9-18    已测试
*  备    注：
*************************************************************************/
u8 PIT1Count = 0 ;
void PIT1_IRQHandler(void)
{
   PIT_Flag_Clear(PIT1);       //清中断标志位
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PIT0_IRQHandler
*  功能说明：PIT0 定时中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-18    已测试
*  备    注：
*************************************************************************/



void PIT0_IRQHandler(void)
{
   DisableInterrupts; 
            if(I0_present_max>=18)  //2.5为有效值
            {
              gpio_set(PORTB ,5,1);//高电平关闭
              Power_Flag=0;
            }
            
            TimeCount++;
            if(TimeCount==4000)
            {
              Refresh_Present();
              TimeCount=0;
              U0_present_max=0;
              I0_present_max=0;
               pwm_gain=pwm_gain_temp;
            }
             
            
  EnableInterrupts;
  PIT_Flag_Clear(PIT0);       //清中断标志位
}



/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：SysTick_Handler
*  功能说明：系统滴答定时器中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-18    已测试
*  备    注：ucos里用得到
*************************************************************************/
void SysTick_Handler(void)
{
    //    OSIntEnter();
    //    OSTimeTick();
    //    OSIntExit();
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PendSV_Handler
*  功能说明：PendSV（可悬起系统调用）中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-15    已测试
*  备    注：uC/OS用来切换任务
*************************************************************************/
void PendSV_Handler(void)
{
}
/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：HardFault_Handler
*  功能说明：硬件上访中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-4    已测试
*  备    注：可以用LED闪烁来指示发生了硬件上访
*************************************************************************/
void HardFault_Handler(void)
{
    while (1)
    {
        printf("\n****硬件上访错误!!!*****\r\n\n");
    }
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PORTA_IRQHandler
*  功能说明：PORTA端口中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-1-25    已测试
*  备    注：引脚号需要自己初始化来清除
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




    u8  n = 0;    //引脚号
    n = 8;
    if(PORTD_ISFR & (1 << n))         //PTDn触发中断
    {
        PORTD_ISFR  |= (1 << n);        //写1清中断标志位
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
    if(PORTD_ISFR & (1 << n))         //PTDn触发中断
    {
        PORTD_ISFR  |= (1 << n);        //写1清中断标志位
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
    if(PORTD_ISFR & (1 << n))         //PTDn触发中断
    {
        PORTD_ISFR  |= (1 << n);        //写1清中断标志位
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
    if(PORTD_ISFR & (1 << n))         //PTDn触发中断
    {
        PORTD_ISFR  |= (1 << n);        //写1清中断标志位
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
*                             蓝宙电子工作室
*
*  函数名称：PORTB_IRQHandler
*  功能说明：PORTB端口中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-1-25    已测试
*  备    注：引脚号需要根据自己初始化来修改
*************************************************************************/
void PORTB_IRQHandler()
{

  //  PORT_PCR_REG(PORTB , 10) |= PORT_PCR_ISF(1);
    u8  n = 0;    //引脚号
    n = 0;
    if(PORTB_ISFR & (1 << n))         //PTB0触发中断
    {
        PORTB_ISFR  |= (1 << n);        //写1清中断标志位
        /*  用户任务  */

    }

    n = 10;
    if(PORTB_ISFR & (1 << n))         //PTB10触发中断
    {
        PORTB_ISFR  |= (1 << n);        //写1清中断标志位
        /*  用户任务  */



    }
}


/*************************************************************************
*                             蓝宙嵌入式开发工作室
*
*  函数名称：PIT2_IRQHandler
*  功能说明：PIT2定时中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-9-18    已测试
*  备    注：
*************************************************************************/

void PIT2_IRQHandler(void)
{
   PIT_Flag_Clear(PIT2);       //清中断标志位


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
*                             蓝宙电子工作室
*
*  函数名称：FTM0_IRQHandler
*  功能说明：FTM0输入捕捉中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-25
*  备    注：引脚号需要根据自己初始化来修改，参考现有的代码添加自己的功能
*************************************************************************/


void FTM0_IRQHandler()
{
  
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：FTM1_IRQHandler
*  功能说明：FTM1输入捕捉中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-25
*  备    注：引脚号需要根据自己初始化来修改，参考现有的代码添加自己的功能
*************************************************************************/


void FTM1_IRQHandler()
{
    u8 s = FTM1_STATUS;             //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
    FTM1_STATUS = 0x00;               //清中断标志位
    
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
*                             蓝宙电子工作室
*
*  函数名称：DMA_CH4_Handler
*  功能说明：DMA通道4的中断服务函数
*  参数说明：是采集摄像头数据，本数据位摄像头AD数据，可以采集到300个点。
             设置标志位能够及时搬移。
*  函数返回：无
*  修改时间：2012-3-18    已测试
*  备    注：
*************************************************************************/
u8  DMA_Over_Flg = 0 ;     //行采集完成标志位
void DMA_CH4_Handler(void)
{
  //DMA通道4
  //  u16 i ;
 //    uart_putchar(UART0,0XAA);
    DMA_IRQ_CLEAN(DMA_CH4) ;
    DMA_IRQ_DIS(DMA_CH4);
    DMA_DIS(DMA_CH4);
    DMA_Over_Flg = 1 ;
  //   disable_irq(PORTC+87);

}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：DMA_CH4_Handler
*  功能说明：DMA通道4的中断服务函数
*  参数说明：是采集摄像头数据，本数据位摄像头AD数据，可以采集到300个点。
             设置标志位能够及时搬移。
*  函数返回：无
*  修改时间：2012-3-18    已测试
*  备    注：
*************************************************************************/
void DMA_CH0_Handler(void)
{

    //DMA通道0
    printf("DMA_CH0_Handler\n");
    DMA_IRQ_CLEAN(DMA_CH0);                             //清除通道传输中断标志位    (这样才能再次进入中断)
    DMA_EN(DMA_CH0);                                    //使能通道CHn 硬件请求      (这样才能继续触发DMA传输)
  //   uart_putchar(UART0,0XAA);
}



volatile u8 LPT_INT_count = 0;
void  LPT_Handler(void)
{
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;   //清除LPTMR比较标志
    LPT_INT_count++;                    //中断溢出加1
}