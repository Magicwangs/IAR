���س������ʾ��
�����ߺ�/LANDZO
�������/LANDZO
Ϊ��/LANDZO
����/LANDZO

���߷�����

  #define OLED_SCL  PTD1_OUT
  #define OLED_SDA  PTD3_OUT
  #define OLED_RST  PTC15_OUT
  #define OLED_DC   PTD2_OUT
  #define OLED_CS   PTD4_OUT

4��SPIʹ��˵����
VBT ���ڲ�DC-DC��ѹ��3.3~4.3V�����ʹ��5V��ѹ��Ϊ���������һ��100~500ŷ�ĵ���
VCC ���ڲ��߼���ѹ 1.8~6V
GND ��

BS0 �͵�ƽ
BS1 �͵�ƽ
BS2 �͵�ƽ

CS  Ƭѡ�ܽ�
DC  ��������ѡ��ܽ�
RES ģ�鸴λ�ܽ�
D0��SCLK�� ��ʱ�ӽţ���MCU����
D1��MOSI�� ����������������ݽţ���MCU����

D2 ����
D3-D7 �� �͵�ƽ �� Ҳ�����գ��������Ϊ�͵�ƽ
RD  �͵�ƽ ��Ҳ�����գ��������Ϊ�͵�ƽ
RW  �͵�ƽ ��Ҳ�����գ��������Ϊ�͵�ƽ
RD  �͵�ƽ ��Ҳ�����գ��������Ϊ�͵�ƽ