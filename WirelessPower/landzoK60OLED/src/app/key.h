#ifndef _KEY_H
#define _KEY_H
extern u8 key_Pressed;      //�����Ƿ񱻰���:1--�ǣ�0--��
extern u8 key_val;          //��ż�ֵ
extern u8 key_Flag;         //�����Ƿ��ѷſ���1--�ǣ�0--��
//���ü����߼���ֵ���������ֵ��ӳ��
//extern u8 key_Map[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

extern float input_val;

void Init_Key_Port();
void Check_Key(void);
void Key_Event(void);







#endif