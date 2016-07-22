/******************** (C) COPYRIGHT 2011 蓝宙电子工作室 ********************
 * 文件名       ：main.c
 * 描述         ：工程模版实验
 *
 * 实验平台     ：landzo电子开发版
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/

**********************************************************************************/

#include "include.h"


/*************************
设置系统的全局变量
*************************/


extern  u8  LPT_INT_count ;
extern  u8  DMA_Over_Flg ;            //采集完成标志位
extern  u8  LinADCout ;

extern u8 TIME0flag_5ms   ;
extern u8 TIME0flag_10ms  ;
extern u8 TIME0flag_15ms  ;
extern u8 TIME0flag_20ms  ;
extern u8 TIME0flag_80ms ;
extern u8 TIME1flag_1s ;
extern u8 TIME1flag_3s ;


extern double I1_Vol_present_value;
extern double U1_present_value;
extern double U2_present_value;


extern double I_Set;
extern double U20;


int test_flag=0;

double Kp_i=20.0;
double Ki_i=0.3;
double Kd_i=0.0;

double Kp_u=20.0;
double Ki_u=0.5;
double Kd_u=0.0;

double PWM_Out=0.0;

int start_flag=0;

double Voltage_Error[2]={0,0};
double Current_Error[2]={0,0};

double PWM_Voltage_temp=640;
double PWM_Current_temp=640;

int modeflag=0;
int ftm_flag=0;

char buf[20];

unsigned int PWM_Voltage=640,PWM_Current=640;




void Pot_Show(int x,int y,int final_adc);
void OLED_Light();
void Refresh_Present();
void Refresh_Key();
void PID_control();
void Curent_PID();
void Voltage_PID();


int pwm_now=400;
int freq_now=180000;

void main()
{
  DisableInterrupts;                             //禁止总中断

  /*********************************************************
  初始化程序
  *********************************************************/

   uart_init (UART0 , 115200);                      //初始化UART0，输出脚PTA15，输入脚PTA14，串口频率 9600
   gpio_init (PORTA , 16, GPO,HIGH);
   
   Init_Key_Port();
   
   //pit_init_us(PIT0, 300);                                    //初始化PIT0，定时时间为： 300us
   
   OLED_Init();
   adc_init(ADC0, AD9);  //B1
   adc_init(ADC0,AD10); //A7
   adc_init(ADC0,AD12);  //B2
   
   FTM_PWM_init(FTM0, CH2 ,20000,0);
   
   FTM_PWM_init(FTM0, CH3 ,20000,0);
   
   //dac_once_init(DAC0,VDDA);

   
   //pit_init_us(PIT1, 300);  

   FTM0_Init(freq_now ,pwm_now);
   
   EnableInterrupts;			                    //开总中断


   //FTM0_Duty_Change(200);
   OLED_Light();
   
   
   /******************************************
    执行程序
    ******************************************/
    while(1)
    {
        Key_Event(); 
    }

}


void OLED_Light()
{
  OLED_Fill(0x00);  //液晶屏全黑
  Dly_ms(50);
  
  OLED_P6x8Str(0,0,"pwm:");
  Pot_Show(25,0,pwm_now);
  OLED_P6x8Str(0,2,"freq:");
  Pot_Show(35,2,freq_now);
  
}


void Refresh_Key()
{


  

  if(key_val==1||key_val==4||key_val==2||key_val==5)
  {
      Pot_Show(35,2,freq_now);
  FTM0_Freq_Change(freq_now);
      Pot_Show(25,0,pwm_now);
  FTM0_Duty_Change(pwm_now);
  }
  

  
}

void Refresh_Present()
{

}



void Pot_Show(int x,int y,int final_adc)
{
  u8  adc_num[]="00.00" ;
  
  int int_adc_1=0;
  int int_adc_2=0;
  int pot_adc_1=0;
  int pot_adc_2=0;
  int adc_temp=0;
        
  sprintf(adc_num, "%d",final_adc); 
  
  OLED_P6x8Str(x,y,adc_num);  
}


void PID_control()
{

  if(start_flag==1)
  {
    if(ftm_flag==0)
    {
      FTM0_Init(20000 ,640);
      PWM_Current_temp=640;
      PWM_Voltage_temp=640;
    }
    ftm_flag=1;
    
    if(modeflag==0)
    {
      Curent_PID();
      FTM0_Duty_Change(PWM_Current);
    }
    else
    {
      Voltage_PID();
      FTM0_Duty_Change(1000-PWM_Voltage);
    }
  }

}



void Curent_PID()
{
  Current_Error[1] = Current_Error[0];
  Current_Error[0] = I_Set-I1_Vol_present_value;
  PWM_Current_temp = (Current_Error[0]-Current_Error[1])*Kp_i+Current_Error[0]*Ki_i + PWM_Current_temp;
  if(PWM_Current_temp >950) PWM_Current_temp=950;
  if(PWM_Current_temp <50) PWM_Current_temp=50;
  PWM_Current = (int)PWM_Current_temp;
}

void Voltage_PID()
{
  Voltage_Error[1]=Voltage_Error[0];
  Voltage_Error[0]=U20-U2_present_value;
  
  PWM_Voltage_temp=(Voltage_Error[0]-Voltage_Error[1])*Kp_u+Voltage_Error[0]*Ki_u+PWM_Voltage_temp;
  if(PWM_Voltage_temp >950) PWM_Voltage_temp=950;
  if(PWM_Voltage_temp <50) PWM_Voltage_temp=50;
  
  PWM_Voltage = (int)PWM_Voltage_temp;
  
}










