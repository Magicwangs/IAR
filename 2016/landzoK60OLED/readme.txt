下载程序后显示：
安徽芜湖/LANDZO
蓝宙电子/LANDZO
为您/LANDZO
服务/LANDZO

接线方法：

  #define OLED_SCL  PTD1_OUT
  #define OLED_SDA  PTD3_OUT
  #define OLED_RST  PTC15_OUT
  #define OLED_DC   PTD2_OUT
  #define OLED_CS   PTD4_OUT

4线SPI使用说明：
VBT 供内部DC-DC电压，3.3~4.3V，如果使用5V电压，为保险起见串一个100~500欧的电阻
VCC 供内部逻辑电压 1.8~6V
GND 地

BS0 低电平
BS1 低电平
BS2 低电平

CS  片选管脚
DC  命令数据选择管脚
RES 模块复位管脚
D0（SCLK） ，时钟脚，由MCU控制
D1（MOSI） ，主输出从输入数据脚，由MCU控制

D2 悬空
D3-D7 ， 低电平 ， 也可悬空，但最好设为低电平
RD  低电平 ，也可悬空，但最好设为低电平
RW  低电平 ，也可悬空，但最好设为低电平
RD  低电平 ，也可悬空，但最好设为低电平