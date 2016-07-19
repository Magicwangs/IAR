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




void Pot_Show(int x,int y,double final_adc);
void OLED_Light();
void Refresh_Present();
void Refresh_Key();
void PID_control();
void Curent_PID();
void Voltage_PID();




void main()
{
  DisableInterrupts;                             //禁止总中断

  /*********************************************************
  初始化程序
  *********************************************************/

   uart_init (UART0 , 115200);                      //初始化UART0，输出脚PTA15，输入脚PTA14，串口频率 9600
   gpio_init (PORTA , 16, GPO,HIGH);
   
   Init_Key_Port();
   
   pit_init_us(PIT0, 300);                                    //初始化PIT0，定时时间为： 300us
   
   OLED_Init();
   adc_init(ADC0, AD9);  //B1
   adc_init(ADC0,AD10); //A7
   adc_init(ADC0,AD12);  //B2
   
   FTM_PWM_init(FTM0, CH2 ,20000,0);
   
   FTM_PWM_init(FTM0, CH3 ,20000,0);
   
   //dac_once_init(DAC0,VDDA);

   
   //pit_init_us(PIT1, 300);  

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
  
  OLED_P6x8Str(0,0,"U1:");
  Pot_Show(25,0,U1_present_value);
  OLED_P6x8Str(65,0,"I1:");
  Pot_Show(90,0,I1_Vol_present_value);
  
  OLED_P6x8Str(0,1,"U2:");
  Pot_Show(25,1,U2_present_value);
  //OLED_P6x8Str(65,0,"I2:");
  //Pot_Show(90,0,);  
  
  
  
  OLED_P6x8Str(0,2,"I10:");
  Pot_Show(25,2,I_Set);
  OLED_P6x8Str(65,2,"U20:");
  Pot_Show(90,2,U20);
  
  
  OLED_P6x8Str(0,3,"P_i:");
  Pot_Show(25,3,Kp_i);
  OLED_P6x8Str(65,3,"P_u:");
  Pot_Show(90,3,Kp_u);
  
  OLED_P6x8Str(0,4,"I_i:");
  Pot_Show(25,4,Ki_i);
  OLED_P6x8Str(65,4,"I_u:");
  Pot_Show(90,4,Ki_u);  
  
  OLED_P6x8Str(0,5,"PWM_I:");
  Pot_Show(35,5,PWM_Current/10);
  
  OLED_P6x8Str(0,6,"PWM_U:");
  Pot_Show(35,6,PWM_Voltage/10);
  
  OLED_P6x8Str(70,6,"Mod:");
  Pot_Show(92,6,modeflag);

  
}


void Refresh_Key()
{
  
  Pot_Show(25,2,I_Set);

  Pot_Show(90,2,U20); 

  Pot_Show(25,3,Kp_i);

  Pot_Show(90,3,Kp_u);

  Pot_Show(25,4,Ki_i);

  Pot_Show(90,4,Ki_u);
  
}

void Refresh_Present()
{
    
  OLED_Fill_up(0x00);
  
  OLED_P6x8Str(0,0,"U1:");
  Pot_Show(25,0,U1_present_value);
  OLED_P6x8Str(65,0,"I1:");
  Pot_Show(90,0,I1_Vol_present_value);
  
  OLED_P6x8Str(0,1,"U2:");
  Pot_Show(25,1,U2_present_value);
  
  Pot_Show(35,5,PWM_Current/10);
  
  Pot_Show(35,6,PWM_Voltage/10);
  
  Pot_Show(92,6,modeflag);
}



void Pot_Show(int x,int y,double final_adc)
{
  u8  adc_num[]="00.00" ;
  
  int int_adc_1=0;
  int int_adc_2=0;
  int pot_adc_1=0;
  int pot_adc_2=0;
  int adc_temp=0;
  
  adc_temp=final_adc*100;
  int_adc_1=adc_temp/1000;
  int_adc_2=adc_temp/100-int_adc_1*10;
  pot_adc_1=adc_temp/10-int_adc_1*100-int_adc_2*10;
  pot_adc_2=adc_temp-int_adc_1*1000-int_adc_2*100-pot_adc_1*10;
        
  sprintf(adc_num, "%d%d.%d%d",int_adc_1,int_adc_2,pot_adc_1,pot_adc_2); 
  
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










