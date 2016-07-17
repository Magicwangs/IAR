#ifndef _KEY_H
#define _KEY_H
extern u8 key_Pressed;      //按键是否被按下:1--是，0--否
extern u8 key_val;          //存放键值
extern u8 key_Flag;         //按键是否已放开：1--是，0--否
//设置键盘逻辑键值与程序计算键值的映射
//extern u8 key_Map[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

extern float input_val;

void Init_Key_Port();
void Check_Key(void);
void Key_Event(void);







#endif