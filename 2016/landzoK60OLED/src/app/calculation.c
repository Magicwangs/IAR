
#include "common.h"
#include "include.h"
#include "calculation.h"

/*********************************************************** 
函数名称：LCD_KEY_init
函数功能：
入口参数：
出口参数：无 
备 注： 
***********************************************************/
void LCD_KEY_init (void){
  
  
      pit_init_ms(PIT2, 5);                                  //初始化PIT2，定时时间为： 5ms ,按键用定时器
     /************************
     液晶屏初始化
    ************************/
    gpio_init (PORTC , 14, GPO, HIGH) ;
    gpio_init (PORTC , 15, GPO, HIGH) ;
    gpio_init (PORTC , 6,  GPO, HIGH) ;
    gpio_init (PORTC , 7,  GPO, HIGH) ;
    gpio_init (PORTB , 21, GPO, HIGH) ;
    gpio_init (PORTB , 22, GPO, HIGH) ;
    LCD_init() ;  
     /************************
     按键初始化
    ************************/
    gpio_Interrupt_init(PORTD , 8, GPI, EITHER) ;
    gpio_Interrupt_init(PORTD , 9, GPI, EITHER) ;
    gpio_Interrupt_init(PORTD , 10, GPI, EITHER) ;
    gpio_Interrupt_init(PORTD , 11, GPI, EITHER) ;
}






/*********************************************************** 
函数名称：LCD_KEY
函数功能：液晶显示和按键程序
入口参数：c	:  显示的字符
出口参数：无 
备 注： 
***********************************************************/
extern u8 key8flg ;
extern u8 key9flg ;
extern u8 key10flg ;
extern u8 key11flg ;
extern u8 keyflg  ;
extern u8 TIME0flag_80ms  ;
extern u16 ASPeed0 ,ASPeed1 ,ASPeed2;
extern u16 ASPeed3 ,ASPeed4 ,ASPeed5 ;
void KYELCD(void){
  

static u8 t8rmp0 = 0,t8rmp1 = 0;
static u8 AupDown  ;
u8 AspeedS0, AspeedS1, AspeedS2, AspeedS3;

   if(key8flg >= 1)  {
         
          keyflg = 0 ;
          key8flg -- ;
          if(AupDown > 0)
          {
            AupDown -- ;
          }
        }
      

         if(key11flg >= 1)  {
          if(AupDown < 5)
          {
            AupDown ++ ;
          }
          keyflg = 0 ;
          key11flg -- ;
        }

      /*********************
      160ms程序执行代码段
      *********************/
      if(TIME0flag_80ms == 1)
      {
        TIME0flag_80ms = 0 ; 
        
     
           if(AupDown == 0)
           {
                if(key9flg >= 1)  {
                 keyflg = 0 ;
                 ASPeed0 += key9flg ;
                 key9flg = 0 ;            
              }
              if(key10flg >= 1){
                  keyflg = 0 ; 
                  if((ASPeed0-key10flg) < 0)
                  {
                    ASPeed0 = 0 ; 
                    key10flg = 0 ;
                  } else 
                  {
                    ASPeed0 -= key10flg ; 
                    key10flg = 0 ;
                  }
              }
           }           
           else if(AupDown == 1)
           {

             
              if(key9flg >= 1)  {
                 keyflg = 0 ;
                 ASPeed1 += key9flg ;
                 key9flg = 0 ;            
              }
              if(key10flg >= 1){
                  keyflg = 0 ; 
                  if((ASPeed1-key10flg) < 0)
                  {
                    ASPeed1 = 0 ; 
                    key10flg = 0 ;
                  } else 
                  {
                    ASPeed1 -= key10flg ; 
                    key10flg = 0 ;
                  }
              }
           }
           else if(AupDown == 2)
           {
                if(key9flg >= 1)  {
                 keyflg = 0 ;
                 ASPeed2 += key9flg ;
                 key9flg = 0 ;            
              }
              if(key10flg >= 1){
                  keyflg = 0 ; 
                  if((ASPeed2-key10flg) < 0)
                  {
                    ASPeed2 = 0 ; 
                    key10flg = 0 ;
                  } else 
                  {
                    ASPeed2 -= key10flg ; 
                    key10flg = 0 ;
                  }
              }
           }
           else if(AupDown == 3)
           {
                if(key9flg >= 1)  {
                 keyflg = 0 ;
                 ASPeed3 += key9flg ;
                 key9flg = 0 ;            
              }
              if(key10flg >= 1){
                  keyflg = 0 ; 
                  if((ASPeed3-key10flg) < 0)
                  {
                    ASPeed3 = 0 ; 
                    key10flg = 0 ;
                  } else 
                  {
                    ASPeed3 -= key10flg ; 
                    key10flg = 0 ;
                  }
              }
           }
           else if(AupDown == 4)
           {
                if(key9flg >= 1)  {
                 keyflg = 0 ;
                 ASPeed4 += key9flg ;
                 key9flg = 0 ;            
              }
              if(key10flg >= 1){
                  keyflg = 0 ; 
                  if((ASPeed4-key10flg) < 0)
                  {
                    ASPeed4 = 0 ; 
                    key10flg = 0 ;
                  } else 
                  {
                    ASPeed4 -= key10flg ; 
                    key10flg = 0 ;
                  }
              }
           }
           else if(AupDown == 5)
           {
                if(key9flg >= 1)  {
                 keyflg = 0 ;
                 ASPeed5 += key9flg ;
                 key9flg = 0 ;            
              }
              if(key10flg >= 1){
                  keyflg = 0 ; 
                  if((ASPeed5-key10flg) < 0)
                  {
                    ASPeed5 = 0 ; 
                    key10flg = 0 ;
                  } else 
                  {
                    ASPeed5 -= key10flg ; 
                    key10flg = 0 ;
                  }
              }
           }
        
        
           



              
        if(t8rmp0 == 1)
        {
             
          t8rmp0 = 0 ;

           if(AupDown == 0)
            LCD_set_XY(0, 0) ;
           else if(AupDown == 1)
            LCD_set_XY(0, 1) ; 
           else if(AupDown == 2)
            LCD_set_XY(0, 2) ; 
           else if(AupDown == 3)
            LCD_set_XY(0, 3) ; 
           else if(AupDown == 4)
            LCD_set_XY(0, 4) ; 
           else if(AupDown == 5)
            LCD_set_XY(0, 5) ; 
            for(t8rmp1 = 0 ; t8rmp1 < 83 ;t8rmp1 ++)
                  {  LCD_write_byte(0, 1);   }   
        }
        else{
 
             t8rmp0 = 1 ; 
 
              LCD_set_XY(0, 0) ;
              LCD_write_char('L');
              LCD_write_char('a');
              LCD_write_char('n');
              LCD_write_char('d');
              LCD_write_char('z');
              LCD_write_char('o'); 
             /**************
              第二行显示内容
             ************* */
       
              AspeedS0 = (u8) ((ASPeed1/1000)  + 48 );
              AspeedS1 = (u8) (((ASPeed1%1000)/100) +48 ) ;
              AspeedS2 = (u8) (((ASPeed1%100)/10)  + 48 ); 
              AspeedS3 = (u8) ((ASPeed1%10)  + 48 );

              LCD_set_XY(0, 1) ;
              LCD_write_char('S');
              LCD_write_char('p');
              LCD_write_char('e');
              LCD_write_char('e');
              LCD_write_char('d');
              LCD_write_char('0');
              LCD_write_char(':');
              LCD_write_char(AspeedS0);
              LCD_write_char(AspeedS1);
              LCD_write_char(AspeedS2);
              LCD_write_char(AspeedS3);
             /**************
              第三行显示内容
            **************/ 

              AspeedS0 = (u8) ((ASPeed2/1000)  + 48 );
              AspeedS1 = (u8) (((ASPeed2%1000)/100) +48 ) ;
              AspeedS2 = (u8) (((ASPeed2%100)/10)  + 48 ); 
              AspeedS3 = (u8) ((ASPeed2%10)  + 48 );
              

             LCD_set_XY(0, 2) ;  
              LCD_write_char('S');
              LCD_write_char('p');
              LCD_write_char('e');
              LCD_write_char('e');
              LCD_write_char('d');
              LCD_write_char('1');
              LCD_write_char(':');
              LCD_write_char(AspeedS0);
              LCD_write_char(AspeedS1);
              LCD_write_char(AspeedS2);
              LCD_write_char(AspeedS3);
             /**************
              第四行显示内容
             **************/ 
              AspeedS0 = (u8) ((ASPeed3/1000)  + 48 );
              AspeedS1 = (u8) (((ASPeed3%1000)/100) +48 ) ;
              AspeedS2 = (u8) (((ASPeed3%100)/10)  + 48 ); 
              AspeedS3 = (u8) ((ASPeed3%10)  + 48 );
             LCD_set_XY(0, 3) ;
              LCD_write_char('S');
              LCD_write_char('p');
              LCD_write_char('e');
              LCD_write_char('e');
              LCD_write_char('d');
              LCD_write_char('2');
              LCD_write_char(':');
              LCD_write_char(AspeedS0);
              LCD_write_char(AspeedS1);
              LCD_write_char(AspeedS2);
              LCD_write_char(AspeedS3);
             /**************
              第五行显示内容
             **************/ 
              AspeedS0 = (u8) ((ASPeed4/1000)  + 48 );
              AspeedS1 = (u8) (((ASPeed4%1000)/100) +48 ) ;
              AspeedS2 = (u8) (((ASPeed4%100)/10)  + 48 ); 
              AspeedS3 = (u8) ((ASPeed4%10)  + 48 );
             LCD_set_XY(0, 4) ;
              LCD_write_char('S');
              LCD_write_char('p');
              LCD_write_char('e');
              LCD_write_char('e');
              LCD_write_char('d');
              LCD_write_char('3');
              LCD_write_char(':');
              LCD_write_char(AspeedS0);
              LCD_write_char(AspeedS1);
              LCD_write_char(AspeedS2);
              LCD_write_char(AspeedS3);
             /**************
              第六行显示内容
             **************/  
              AspeedS0 = (u8) ((ASPeed5/1000)  + 48 );
              AspeedS1 = (u8) (((ASPeed5%1000)/100) +48 ) ;
              AspeedS2 = (u8) (((ASPeed5%100)/10)  + 48 ); 
              AspeedS3 = (u8) ((ASPeed5%10)  + 48 );
              LCD_set_XY(0, 5) ;
              LCD_write_char('S');
              LCD_write_char('p');
              LCD_write_char('e');
              LCD_write_char('e');
              LCD_write_char('d');
              LCD_write_char('4');
              LCD_write_char(':');
              LCD_write_char(AspeedS0);
              LCD_write_char(AspeedS1);
              LCD_write_char(AspeedS2);
              LCD_write_char(AspeedS3);
        }
      }

}
/*********************************************************** 
函数名称：LCDTIME
函数功能：液晶显示和
入口参数：c	:  显示的字符
出口参数：无 
备 注： 
***********************************************************/
void LCDTIME(void){
  
              
              u8 AspeedS0, AspeedS1, AspeedS2, AspeedS3;
              LCD_set_XY(0, 0) ;
              LCD_write_char('L');
              LCD_write_char('a');
              LCD_write_char('n');
              LCD_write_char('d');
              LCD_write_char('z');
              LCD_write_char('o'); 
             /**************
              第二行显示内容
             ************* */
       
              AspeedS0 = (u8) ((ASPeed1/1000)  + 48 );
              AspeedS1 = (u8) (((ASPeed1%1000)/100) +48 ) ;
              AspeedS2 = (u8) (((ASPeed1%100)/10)  + 48 ); 
              AspeedS3 = (u8) ((ASPeed1%10)  + 48 );

              LCD_set_XY(0, 1) ;
              LCD_write_char('S');
              LCD_write_char('p');
              LCD_write_char('e');
              LCD_write_char('e');
              LCD_write_char('d');
              LCD_write_char('0');
              LCD_write_char(':');
              LCD_write_char(AspeedS0);
              LCD_write_char(AspeedS1);
              LCD_write_char(AspeedS2);
              LCD_write_char(AspeedS3);
             /**************
              第三行显示内容
            **************/ 

              AspeedS0 = (u8) ((ASPeed2/1000)  + 48 );
              AspeedS1 = (u8) (((ASPeed2%1000)/100) +48 ) ;
              AspeedS2 = (u8) (((ASPeed2%100)/10)  + 48 ); 
              AspeedS3 = (u8) ((ASPeed2%10)  + 48 );
              

             LCD_set_XY(0, 2) ;  
              LCD_write_char('S');
              LCD_write_char('p');
              LCD_write_char('e');
              LCD_write_char('e');
              LCD_write_char('d');
              LCD_write_char('1');
              LCD_write_char(':');
              LCD_write_char(AspeedS0);
              LCD_write_char(AspeedS1);
              LCD_write_char(AspeedS2);
              LCD_write_char(AspeedS3);
             /**************
              第四行显示内容
             **************/ 
              AspeedS0 = (u8) ((ASPeed3/1000)  + 48 );
              AspeedS1 = (u8) (((ASPeed3%1000)/100) +48 ) ;
              AspeedS2 = (u8) (((ASPeed3%100)/10)  + 48 ); 
              AspeedS3 = (u8) ((ASPeed3%10)  + 48 );
             LCD_set_XY(0, 3) ;
              LCD_write_char('S');
              LCD_write_char('p');
              LCD_write_char('e');
              LCD_write_char('e');
              LCD_write_char('d');
              LCD_write_char('2');
              LCD_write_char(':');
              LCD_write_char(AspeedS0);
              LCD_write_char(AspeedS1);
              LCD_write_char(AspeedS2);
              LCD_write_char(AspeedS3);
             /**************
              第五行显示内容
             **************/ 
              AspeedS0 = (u8) ((ASPeed4/1000)  + 48 );
              AspeedS1 = (u8) (((ASPeed4%1000)/100) +48 ) ;
              AspeedS2 = (u8) (((ASPeed4%100)/10)  + 48 ); 
              AspeedS3 = (u8) ((ASPeed4%10)  + 48 );
             LCD_set_XY(0, 4) ;
              LCD_write_char('S');
              LCD_write_char('p');
              LCD_write_char('e');
              LCD_write_char('e');
              LCD_write_char('d');
              LCD_write_char('3');
              LCD_write_char(':');
              LCD_write_char(AspeedS0);
              LCD_write_char(AspeedS1);
              LCD_write_char(AspeedS2);
              LCD_write_char(AspeedS3);
             /**************
              第六行显示内容
             **************/  
              AspeedS0 = (u8) ((ASPeed5/1000)  + 48 );
              AspeedS1 = (u8) (((ASPeed5%1000)/100) +48 ) ;
              AspeedS2 = (u8) (((ASPeed5%100)/10)  + 48 ); 
              AspeedS3 = (u8) ((ASPeed5%10)  + 48 );
              LCD_set_XY(0, 5) ;
              LCD_write_char('S');
              LCD_write_char('p');
              LCD_write_char('e');
              LCD_write_char('e');
              LCD_write_char('d');
              LCD_write_char('4');
              LCD_write_char(':');
              LCD_write_char(AspeedS0);
              LCD_write_char(AspeedS1);
              LCD_write_char(AspeedS2);
              LCD_write_char(AspeedS3);
        
      
}









 