#include "include.h"
#include "key.h"
#define keyin (PTE_BYTE0_IN&0x0f)
u8 key_Pressed;      //按键是否被按下:1--是，0--否
u8 key_val;          //存放键值
u8 key_Flag;         //按键是否已放开：1--是，0--否


float input_val;
u8 pot_flag;

//设置键盘逻辑键值与程序计算键值的映射
//u8 key_Map[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
u8 key_Map[] = {1,2,3,10,4,5,6,11,7,8,9,12,14,0,15,13};
u8 str[]={'0','\0','0','0','0','0'};
u16 key_n=0;

double I_Set=0.5;
double U20=29.0;

extern int modeflag;
extern double Kp_i;
extern double Ki_i;


extern double Kp_u;
extern double Ki_u;

extern int start_flag;

extern void Refresh_Key();
    
    
void Init_Key_Port(void)
{    
  
  
  
    gpio_init(PORTE,0,GPI,1);
    gpio_init(PORTE,1,GPI,1); 
    gpio_init(PORTE,2,GPI,1); 
    gpio_init(PORTE,3,GPI,1); 

    gpio_init(PORTE,4,GPO,0);
    gpio_init(PORTE,5,GPO,0);
    gpio_init(PORTE,6,GPO,0);
    gpio_init(PORTE,7,GPO,0);
    key_Flag = 0;       
    key_Pressed = 0;   
    key_val = 0;
    
    input_val=0;
    pot_flag=0;
    key_n=0;
    
}
/*********************************************
* Check_Key(),检查按键，确认键值
*********************************************/
/*******************************************
函数名称：Check_Key
功    能：扫描键盘的IO端口，获得键值
参    数：无
返回值  ：无
********************************************/
void Check_Key(void)
{
    u8 row ,col;
    static u8 tmp1,tmp2;
    
    tmp1 = 0x80;
    for(row = 0;row < 4;row++)              //行扫描
    {
        PTE_BYTE0_OUT = 0xf0;                      //P1.4~P1.7输出全1
        PTE_BYTE0_OUT -= tmp1;                      //P1.4~p1.7输出四位中有一个为0
        tmp1 >>=1;                              
        if ((keyin  & 0x0f) < 0x0f)           //是否P1IN的P1.0~P1.3中有一位为0
        {                       
            tmp2 = 0x01;                         // tmp2用于检测出那一位为0  
            for(col = 0;col < 4;col++)              // 列检测 
            {          
                if((keyin & tmp2) == 0x00)           // 是否是该列,等于0为是
                {          
                    key_val = key_Map[row * 4 + col];  // 获取键值
                    return;                         // 退出循环
                }
                tmp2 <<= 1;                        // tmp2右移1位 
            }
        }
    }
    
    while(gpio_get(PORTE,0)==0||gpio_get(PORTE,1)==0||gpio_get(PORTE,2)==0||gpio_get(PORTE,3)==0);
}
void Key_Event(void)
{     
    u8 tmp;
    
    
    PTE_BYTE0_OUT &= 0x00;              // 设置P1OUT全为0，等待按键输入
    tmp = keyin;                 // 获取 p1IN
    if ((key_Pressed == 0x00)&&((tmp & 0x0f) < 0x0f)) //如果有键按下
    {                     
        key_Pressed = 1;        // 如果有按键按下，设置key_Pressed标识
        time_delay_ms(15) ;             //消除抖动
        Check_Key();            // 调用check_Key(),获取键值 
    }     
    else if ((key_Pressed == 1)&&((tmp & 0x0f) == 0x0f)) //如果按键已经释放
    {                     
        key_Pressed = 0;        // 清除key_Pressed标识
        key_Flag    = 1;        // 设置key_Flag标识
       
        
        switch(key_val)
        {
        case 1:I_Set+=0.1;break;
        case 4:I_Set-=0.1;break;
          
        case 2:Kp_i+=1;break;
        case 5:Kp_i-=1;break;
        
        case 3:Ki_i+=0.01;break;
        case 6:Ki_i-=0.01;break;
        
        case 7:U20+=0.1;break;
        case 14:U20-=0.1;break; 
        
        case 8:Kp_u+=1;break;
        case 0:Kp_u-=1;break;
        
        case 9:Ki_u+=0.01;break;
        case 15:Ki_u-=0.01;break;
        
        case 10:I_Set=1.0;U20=29.0;Kp_i=20.0;Ki_i=0.3;Kp_u=20.0;Ki_u=0.5;break;
        
        case 13:start_flag=1;break;
        case 12:modeflag=1;break;
        
        default:break;
        
        }
        
        key_val=11;
        
        Refresh_Key();
    }
    else 
    {
        asm("nop");
    }

}


void My_Key()
{
       switch(key_val)
        {
        case 0:
          str[key_n]='0';
          str[key_n+1]='\0';
          key_n++;
          OLED_P6x8Str(55,4,str);
          if(pot_flag==0)
          {
            input_val=input_val*10;
          }
          else
          {
            pot_flag++;
          }
          
          key_val=0;
          break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
          str[key_n]=key_val%10+'0';
          str[key_n+1]='\0';
          key_n++;
          OLED_P6x8Str(55,4,str);
          
          if(pot_flag==0)
          {
            input_val=input_val*10+key_val;
          }
          else
          {
            input_val=input_val+key_val/(pow(10,pot_flag));
            pot_flag++;
          }
          
          key_val=0;
          break;
        case 10:
          key_n=0;
          str[0]='0';
          str[1]='\0';
          OLED_Fill_page(0x00);//黑屏
          OLED_P6x8Str(55,4,str);
          input_val=0;
          pot_flag=0;
          
          key_val=0;
          break;
        case 14:
          str[key_n]='.';
          str[key_n+1]='\0';
          key_n++;
          OLED_P6x8Str(55,4,str);
          pot_flag=1;
          
          key_val=0;
          break;
        }
        
}

