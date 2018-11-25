#include "common.h"
#include "main.h"

//虚拟示波器显示
void VirOSC(UARTn uartn)//如果调用UART_Putint16，则里面的参数需要强制类型转换
{

  UART_Putchar(uartn, 0x03);
  UART_Putchar(uartn, 0xFC);
//以上为数据帧头
  
//  UART_Putint16(uartn, (int16_t) gyro_z);//motor_left_target1
//  UART_Putint16(uartn, (int16_t) offset_cal);//real_left_speed
//  UART_Putint16(uartn, (int16_t) acce_y);//motor_left_out_vir//保持与期望值量级相同,便于观察gyro_z
  UART_Putint16(uartn, (int16_t) offset_out);
//  UART_Putint16(uartn, (int16_t) real_left_speed);
//  UART_Putint16(uartn, (int16_t) real_right_speed);
//  UART_Putint16(uartn, (int16_t) (real_left_speed+real_right_speed));//(real_left_speed+real_right_speed)
  
//根据需要的波形数量添加或删减
//将要显示的变量定义为全局变量
  
//以下为数据帧尾
  UART_Putchar(uartn, 0xFC);
  UART_Putchar(uartn, 0x03);
}
/***********************************************************************************************
*
* @brief    KBI_Interrupt(UINT8 ch). Callback function called by the KBI module on each interrupt
* @param    KBI channel
* @return   none
*
************************************************************************************************/
void KBI_Interrupt(uint8 ch)
{
  switch(ch)
  {
    case 0://(uint8)KBI0_P0
    break;
    case (uint8)KBI1_P20://52
      GPIO_Turn(G1);
    break;
    case (uint8)KBI1_P21://53
      GPIO_Turn(G2);
    break;
  }
}
/***********************************************************************************************
*
* @brief    UART_Interrupt(UINT8 ch). Callback function called by the UART module on each interrupt
* @param    UART channel
* @return   none
*
************************************************************************************************/
void UART_Interrupt(uint8 ch)
{
  uint8 data_get;
  switch(ch)
  {
    case 0:
      UART_Getchar(uart0, &data_get);
      data_getstring[0] = data_get;
      if(data_getstring[0]=='s')//蓝牙发送s人工停止车模运行
      {
        flag_stop = 1;
        flag_run = 0;
      }
      //UART_Putstr(uart0, data_getstring);
      break;
    case 1:
      break;
    case 2:
      break;
  }
}

void PIT_Interrupt(uint8 ch)
{
  Get_Gyro();
//  gyro_x=mpu_gyro_x;
  gyro_y=mpu_gyro_y;//上下坡
  if(car_version)  gyro_z=mpu_gyro_z+50;//转弯
  else gyro_z=mpu_gyro_z+17;
  Get_AccData();
  acce_x = mpu_acc_x;
  acce_y = mpu_acc_y;
  acce_z = mpu_acc_z;
  
  pit0_count++;
  pit0_flag_stop_en++;
  if(pit0_flag_stop_en>2010) pit0_flag_stop_en = 2010;
  
  count_unround++;
  if(count_unround>((int)count_unround_low*100) && count_unround<((int)count_unround_high*100))
    state_unround = 1;
  else state_unround = 0;
  count_slow++;
  if(count_slow>((int)count_slow_low*100) && count_slow<((int)count_slow_high*100))
    flag_slow = 1;
  else flag_slow = 0;
  
  if(pit0_count%80 == 0)
  {
    GPIO_Turn(G2);//绿灯闪烁表示正常工作
    #ifdef OLED_RUN_SHOW
    flag_show_oled = 1;//若要观察虚拟示波器波形,最好禁止OLED显示
//    OLED_Myshow();//必须放在while中,否则OLED刷新过慢会造成卡中断,改变实际中断周期
    #endif
    pit0_count = 0;
  }
  #ifdef BLUESEND//头文件中定义
//UART发送数据
//  UART_Putstr(uart0,"SEU_smartcar");
    flag_bluesend = 1;
//    VirOSC();//虚拟示波器    不要放在中断里
  #endif
  if(pit0_flag_stop_en>2000 && (forbid_stop==0))//2s禁止停车
  {
    if((Pin(I4)==0)||(Pin(C6)==0)||(Pin(E4)==0)) flag_stop = 1;
  }
  pit0_flag_start++;//1s内逐渐加速

  if(pit0_flag_start>100) pit0_flag_start = 100;
    
  Control();

}

int main(void)
{
  while((1280*ex_clk_khz) != (256*ics_clk_khz));//确保时钟配置无误
  Initial_All();

  LCD_Init();
//  LCD_Clear(WHITE);
//  char num_char[4]="adc";
//  LCD_Show_String(6,8,0,0,num_char,0,BLACK,WHITE);
//  LCD_Fill_CharImage(20,20,40,30,test,BLACK,WHITE);
//  LCD_Fill_Image(60,60,54,25,gImage_baidu);
//  LCD_Fill_Image(0,0,64,80,gImage_zhuoqing);//
  // LCD_Fill_Image(64,0,64,80,gImage_zhuoqing);//
  // LCD_Fill_Image(0,80,64,80,gImage_zhuoqing);//
  // LCD_Fill_Image(64,80,64,80,gImage_zhuoqing);//
  
  OLED_Init();
//  OLED_Picture(128,64,&show[0]);//?????????
  Soft_Delay_ms(100);
  OLED_Clear(0x00);

//  float flash_write_float = 12.3;
//  double flash_write_double = 13.4;
//  FLASH_EraseSector(254);
//  
//  FLASH_WriteFloat(254,flash_write_float,4);
//  FLASH_WriteDouble(254,flash_write_double,16);
//  
//  sprintf(spring_oled,"RR:%d",ad_right2);//
//  OLED_Show_String(8,16,64,16,1,spring_oled,1);
//  float flash_read_float = 0;
//  double flash_read_d = 0;
//  
//  flash_read_float = flash_read(254,4,float);
//  flash_read_d = flash_read_double(254,16);
//
////sprintf浮点数无法使用,需要修改为
////  sprintf(spring_oled, "F:%.2f",flash_read_float);//
//	sprintf(spring_oled,"F:%d.%03d",(int)flash_read_float,(int)((flash_read_float-(int)flash_read_float)*1000));
//  OLED_Show_String(8,16,0,30,1,spring_oled,1);
//	sprintf(spring_oled,"D:%d.%03d",(int)flash_read_d,(int)((flash_read_d-(int)flash_read_d)*1000));
////  sprintf(spring_oled, "D:%f",flash_read_d);//
//  OLED_Show_String(8,16,0,50,1,spring_oled,1);
//  Soft_Delay_ms(2000);
  IIC_init();
  InitMPU6050();
//



  //开启菜单调试
  RTC_Init();
  RTC_Timeset(5);
  while(1)//两个键同时按下持续0.5s完成调试
  {
    if(Pin(G4) && Pin(G5)) menu_debug_finish = RTC_Check();
    else RTC_Clear();
//    if(menu_show_choose) Menu_Debug_LCD();
////    else 
    
    Menu_Debug();
    if(menu_debug_finish) break;
  }
  //-----菜单调试结束指示,RGB跑马灯循环1次
  oled_menu_pagenum = 0;
  oled_position_cursory = 16;
  OLED_Clear(0x00);
  LCD_DisplayOff();
  LCD_Sleep();
  
  while(Pin(G4));
  while(Pin(G5));//等待按键放开
  
  //赋值另一半c_ad
  c_ad5 = -c_ad4;
  c_ad6 = -c_ad3;
  c_ad7 = -c_ad2;
  c_ad8 = -c_ad1;
  offset_c78 = -offset_c12;
  offset_c67 = -offset_c23;
  offset_c56 = -offset_c34;

//  offset_c34 = -offset_c12;
  min_offset = -max_offset;
  
  //写flash扇区
  //-----------------------------------------------
  menu_array_length = sizeof(menu_array)/sizeof(float);//确保正确的长度

  int menu_array_count_temp = 0,menu_flash_count_temp = 0;

  while(menu_array_count_temp < menu_array_length)
  {
    flash_num8[menu_flash_count_temp] = (uint8)(100*(*menu_array[menu_array_count_temp]));//???
    menu_flash_count_temp++;
    flash_num8[menu_flash_count_temp] = (uint8)((uint16)(*menu_array[menu_array_count_temp]*100)>>8);//???,????uint16
    menu_flash_count_temp++;
    menu_array_count_temp++;
  }

  FLASH_EraseSector(255);

  FLASH_WriteSector(255,(const uint8 *)flash_num8,(2*menu_array_length+4-(menu_array_length*2)%4),0);//每个十六位数据长两个字节
  //--------------------------------------------------------------------------
  Pout(G1,1);
  Pout(G2,1);
  Pout(G3,1);
//  Soft_Delay_ms(200);
//  Pout(G1,0);
//  Soft_Delay_ms(200);
//  Pout(G1,1);
//  Pout(G2,0);
//  Soft_Delay_ms(200);
//  Pout(G2,1);
//  Pout(G3,0);
//  Soft_Delay_ms(200);  
//  Pout(G3,1);
  
//  Pout(G1,0);
//  Soft_Delay_ms(200);  
//  Pout(G1,1);
//  Pout(G2,0);
//  Soft_Delay_ms(200);  
//  Pout(G2,1);
//  Pout(G3,0);
//  Soft_Delay_ms(200);
//  Pout(G3,1);

  //---------------------------

  
//环岛设置
  round_count = 0;//环岛位清空
  uint8 round_size_int = (uint8)undefine_para;
  uint8 round_dir_int = (uint8)flash_round_dir;
  uint8 round_size_array = 0;
  round_array_length = sizeof(round_size)/sizeof(char);
  for(round_size_array=0;round_size_array<round_array_length;round_size_array++)
  {
    round_size[round_size_array] = round_size_int & (0x01);
    round_dir[round_size_array] = round_dir_int & (0x01);
    round_dir_int = round_dir_int>>1;
    round_size_int = round_size_int>>1;
  }

  
  Pout(G1,0);//红灯常亮,等待发车

//环岛标志读取
  RTC_Timeset(5);
  RTC_Clear();                          //清空当前RTC
  menu_debug_finish = 0;
  while(1)//下键按下持续0.5s完成调试
  {
    if(Pin(G5)) menu_debug_finish = RTC_Check();
    else RTC_Clear();
    Round_Read_Menu();
    if(menu_debug_finish) break;
  }
  //-----菜单调试结束指示,RGB跑马灯循环1次
  oled_menu_pagenum = 0;
  OLED_Clear(0x00);
  //写flash
  menu_array_length = sizeof(ad_flag_round)/sizeof(uint16_t);
  menu_array_count_temp = 0;
  menu_flash_count_temp = 0;

  while(menu_array_count_temp < menu_array_length)
  {
    flash_round_write[menu_flash_count_temp] = (uint8)(ad_flag_round[menu_array_count_temp]);
    menu_flash_count_temp++;
    flash_round_write[menu_flash_count_temp] = (uint8)((uint16)(ad_flag_round[menu_array_count_temp])>>8);
    menu_flash_count_temp++;
    menu_array_count_temp++;
  }

  FLASH_EraseSector(254);
  FLASH_WriteSector(254,(const uint8 *)flash_round_write,20,0);
  while(Pin(G5));//等待按键放开
  
  while(!Pin(G4))
  {
//    GPIO_Turn(G1);
//    int i = 10;//红灯闪烁，等待发车
//    while(i)
//    {
//      if(Pin(G4)) break;
//      Soft_Delay_ms(20);
//      i--;
//    }
  }
  flag_speed_choose = 2*Pin(H5)+Pin(H2);//8*Pin(C5)+4*Pin(H7)+
  motor_set_speed_temp = 72+4*flag_speed_choose;
  motor_set_speed = motor_set_speed_temp;
  forbid_stop = Pin(H7);
  flag_speed_up = Pin(C5);
  flag_run = 1;//修改标志位
  flag_stop = 0;
  preround_delay = 2000;
  Pout(G1,1);
  Soft_Delay_ms(500);
  RTC_Timeset((uint16)(run_time_set*10));
  RTC_Clear();                          //清空当前RTC
  UART_RX_IRQ_Enable(uart0);
  Enable_Interrupt(INT_PIT_CH0); 	/* Enable Interrupts */

  while(1)
  {
    if(1 == RTC_Check()) flag_stop = 1;
//    flag_stop = RTC_Check();
    if(flag_bluesend)
    {
      VirOSC(uart0);
      flag_bluesend = 0;
    }
    if(flag_show_oled)
    {
      OLED_Myshow();
      flag_show_oled = 0;
    }
//    if(Pin(G4))
//    {
//      Soft_Delay_ms(500);
//      flag_run = 1;//?????
//      flag_stop = 0;
//      data_getstring[0] = '0';
//    }
//    if(Pin(G5)||flag_stop)
//    {
//      flag_run = 0;//?????
//      flag_stop = 1;
//      motor_left_error = 0;//?????
//      pre_motor_left_error = 0;
//      ppre_motor_left_error = 0;
//      lastactive_area = 0;
//      position_area = 0;
//      motor_right_out = 0;
//      motor_left_out = 0;
//    }
  }
	return 1;
}

void Control(void)
{

//  buffer_gyro_y[9] = gyro_y;
//  gyro_sum = array_sum(buffer_gyro_y,10);
//  buffer_left_move(buffer_gyro_y,10);
//  if(abs(gyro_sum)>3000) flag_updown = 1;
//  else flag_updown = 0;
  
//最大值匹配L_R 435,440,555,440,530
  //    475,445,550,455,540
  //655,665,625,715,655
  if(sensor_version==0)
  {
    ad_right2 = (int)(ADC_Read(ADC0_SE2)*0.954); //A6------------------------------------------------------------------------
    ad_right1 = (int)(ADC_Read(ADC0_SE3)*0.874); //A7-------------------------------------------------------------------------
    ad_center = ADC_Read(ADC0_SE1); //A1
    ad_left1 = (int)(ADC_Read(ADC0_SE9)*0.94); //C1
    ad_left2 = (int)(ADC_Read(ADC0_SE10)*0.954);//C2
  }
  else
  {
    ad_center = ADC_Read(ADC0_SE1); //A1
    ad_right2 = (int)(ADC_Read(ADC0_SE2)*1.0); //A6
    ad_right1 = (int)(ADC_Read(ADC0_SE3)*1); //A7
    ad_left1 = (int)(ADC_Read(ADC0_SE9)*1); //C1
    ad_left2 = (int)(ADC_Read(ADC0_SE10)*1);//C2
  }
  ad_total = ad_center+ad_right2+ad_right1+ad_left1+ad_left2;
  //五电感算法
  //AD最大值计算
  uint16_t max_ad = 0;
  if(ad_left2 >= ad_left1) max_ad = ad_left2;
  else max_ad = ad_left1;
  if(ad_center >= max_ad) max_ad = ad_center;
  else ;//max_ad = max_ad
  if(ad_right1 >= max_ad) max_ad = ad_right1;
  else ;
  if(ad_right2 >= max_ad) max_ad = ad_right2;
  else ;
  
  char area;//区块判断
  
  //分区
  if((lastactive_area == 0)&&(max_ad <= outrange_admax)) area=NONE_SIT;//初始化
  else if(max_ad <= outrange_admax) area=OUTRANGE_SIT;
  else if(max_ad == ad_right2) area=LEFT_LEFT;            //1
  else if(max_ad == ad_left2) area=RIGHT_RIGHT;          //8
  else if(max_ad == ad_center)
  {
    if(ad_left1>=ad_right1) area=CENTER_RIGHT;               //5
    else if(ad_right1>ad_left1) area=CENTER_LEFT;            //4
    else area=OUTRANGE_SIT;
  }
  else if(max_ad == ad_left1)
  {
    if(ad_left2>=ad_center) area=MIDRIGHT_RIGHT;            //7
    else if(ad_center>ad_left2) area=MIDRIGHT_LEFT;           //6
    else area=OUTRANGE_SIT;
  }
  else if(max_ad == ad_right1)
  {
    if(ad_right2>=ad_center) area=MIDLEFT_LEFT;            //2
    else if(ad_center>ad_right2) area=MIDLEFT_RIGHT;             //3
    else area=OUTRANGE_SIT;
  }
  else area=OUTRANGE_SIT;
  

  
  //偏移计算
  switch(area)
  {
    case 1:        offset_cal = c_ad1*((float)(ad_right2-ad_right1)/(ad_right2))+offset_c12;break;
    case 2:        offset_cal = (((float)(ad_right1-ad_right2)/(ad_right1)+offset_c12)*(ad_right2-ad_center)/(ad_right1-ad_center))+((-(float)(ad_right2-ad_center)/(ad_right1)+offset_c23)*(ad_right1-ad_right2)/(ad_right1-ad_center));break;

    case 3:        offset_cal = (((float)(ad_right2-ad_center)/(ad_right1)+offset_c23)*(ad_center-ad_right1)/(ad_right2-ad_right1))+((-(float)(ad_center-ad_right1)/(ad_center)+offset_c34)*(ad_right2-ad_center)/(ad_right2-ad_right1));break;

    case 4:        offset_cal = (((float)(ad_center-ad_right1)/(ad_center)+offset_c34)*(ad_right1-ad_left1)/(ad_center-ad_left1))+((-(float)(ad_right1-ad_left1)/(ad_center)+offset_c45)*(ad_center-ad_right1)/(ad_center-ad_left1));break;
    case 5:        offset_cal = (((float)(ad_center-ad_left1)/(ad_center)+offset_c56)*(ad_left1-ad_right1)/(ad_center-ad_right1))+((-(float)(ad_left1-ad_right1)/(ad_center)+offset_c45)*(ad_center-ad_left1)/(ad_center-ad_right1));break;

    case 6:        offset_cal = (((float)(ad_left2-ad_center)/(ad_left1)+offset_c67)*(ad_center-ad_left1)/(ad_left2-ad_left1))+((-(float)(ad_center-ad_left1)/(ad_center)+offset_c56)*(ad_left2-ad_center)/(ad_left2-ad_left1));break;

    case 7:        offset_cal = (((float)(ad_left1-ad_left2)/(ad_left2)+offset_c78)*(ad_left2-ad_center)/(ad_left1-ad_center))+((-(float)(ad_left2-ad_center)/(ad_left1)+offset_c67)*(ad_left1-ad_left2)/(ad_left1-ad_center));break;
    case 8:        offset_cal = c_ad8*((float)(ad_left2-ad_left1)/(ad_left2))+offset_c78;break;

    case 9:        offset_cal = offset_cal;break;
   // default :     offset_cal = pre_offset;
  }
  
  if(offset_cal>max_offset) offset_cal = max_offset;
  else if(offset_cal<min_offset) offset_cal = min_offset;

  
  //丢线寻线
  if(area == 9)
  {
    if(lastactive_area == 0) offset_cal= 0;//初始化
    else if(lastactive_area >= 5) offset_cal = min_offset;
    else offset_cal = max_offset;
  }

  delay_updown_count += gyro_y;
  
////3电感算法
//  //AD最大值计算
//  uint16_t max_ad = 0;
//  if(ad_left2 >= ad_center) max_ad = ad_left2;
//  else max_ad = ad_center;
//  if(ad_right2 >= max_ad) max_ad = ad_right2;
//  else ;//max_ad = max_ad
//
//  
//  char area;//区块判断
//  
//  //分区
//  if((lastactive_area == 0)&&(max_ad <= 100)) area=NONE_SIT;//初始化
//  else if(max_ad <= 100) area=OUTRANGE_SIT;
//  else if(max_ad == ad_left2) area=RIGHT_RIGHT;          //4
//  else if(max_ad == ad_right2) area=LEFT_LEFT;            //1
//  else if(max_ad == ad_center)
//  {
//    if(ad_left2>=ad_right2) area=CENTER_RIGHT;               //3
//    else if(ad_right2>ad_left2) area=CENTER_LEFT;            //2
//    else area=OUTRANGE_SIT;
//  }
//  else area=OUTRANGE_SIT;
    //偏移计算
//  switch(area)
//  {
//    case 1:        offset_cal = c_ad1*((float)(ad_right2-ad_center)/(ad_right2))+offset_c12;break;
//    case 2:        offset_cal = (((float)(ad_center-ad_right2)/(ad_center)+offset_c12)*(ad_right2-ad_left2)/(ad_center-ad_left2))+((-(ad_right2-ad_left2)/(ad_center)+offset_c23)*(ad_center-ad_right2)/(ad_center-ad_left2));break;
//    case 3:        offset_cal = (((float)(ad_center-ad_left2)/(ad_center)+offset_c34)*(ad_left2-ad_right2)/(ad_center-ad_right2))+((-(ad_left2-ad_right2)/(ad_center)+offset_c23)*(ad_center-ad_left2)/(ad_center-ad_right2));break;
//    case 4:        offset_cal = c_ad8*((float)(ad_left2-ad_left1)/(ad_left2))+offset_c34;break;
//    
//    case 5:        offset_cal = offset_cal;break;
//  }
//
//  
//  if(offset_cal>max_offset) offset_cal = max_offset;
//  else if(offset_cal<min_offset) offset_cal = min_offset;

  //offset
//  char area;//区块判断
//  offset_cal=100*(ad_right2-ad_left2)/(float)(ad_right2+ad_left2);
//  if(fabs(offset_cal)<20) area=1;//1直道
//  else if(fabs(offset_cal)<60) area=2;//2小弯
//  else if(fabs(offset_cal)<300) area=3;//3
//  else area=OUTRANGE_SIT;
//  if(max_ad<150) area=OUTRANGE_SIT;
//  //丢线寻线
//  if(area == 5)
//  {
//    if(lastactive_area == 0) offset_cal= 0;//初始化
//    else if(offset_precal <= 0) offset_cal = min_offset;
//    else offset_cal = max_offset;
//  }
//  offset_precal = offset_cal;  
//  switch(area)
//  {  
//    case 1:        offset_out = p1*offset_cal + (int)(d1 * (offset_cal-offset_precal));break;
//    case 2:        offset_out = p2*offset_cal + (int)(d2 * (offset_cal-offset_precal));break;
//    case 3:        offset_out = p3*offset_cal + (int)(d3 * (offset_cal-offset_precal));break;
//    
//    case 5:        offset_cal = offset_cal;break;
//  }
//
  
//  if(area==3) Pout(C7,1);
//  else Pout(C7,0);
  

  
//  offset_vertical = ad_right1-ad_left1;
//
//  offset_verint = (int)offset_vertical;
//  
// 
//  if(((area==2)||(area==3))&&((offset_verint>150)||(offset_verint<-150)))
//  {
//    if(flag_offset_double==0)
//    {
//      flag_offset_double = 1;
//    }
//    else
//    {
//      offset_double_count = 0;
//    }
//  }
//  if(flag_offset_double==1)
//  {
//    offset_double_count++;
//    offset_cal*=2;
//    Pout(C7,1);
//  }
//  if(offset_double_count>70)
//  {
//    flag_offset_double = 0;
//    offset_double_count = 0;
//  }
//  if(flag_offset_double == 0) Pout(C7,0);
  
  //偏移纠正
  
//  if(area != 9 && area != 0)//有效分区内
//  {
//    if(area == (pre_area-1))//偏移向左,需要更大的右偏纠正
//	{
//      c_ad[area] *= (offset_area[area]/offset_cal)
//      offset_cal = offset_area[area];
//	}
//  }
//  
//  pre_area = area;
  

  

  //元素判断

  

  //编码器变量
  uint16_t motor_get_value_left;
  uint16_t motor_get_value_right;
  uint8_t motor_get_direction_left;
  uint8_t motor_get_direction_right;

  motor_get_direction_left = Pin(E1);
  motor_get_direction_right = Pin(H6);
   
  if(motor_get_direction_left != get_left_dir) //上次的方向与此次不同,直接将这个周期的脉冲数计0
    motor_get_value_left = 0;
  else  motor_get_value_left = FTM_Pulse_Get(ftm0);//编码器数值读取

  get_left_speed = motor_get_value_left;
  get_left_dir = motor_get_direction_left;//将读取到的方向存储
  
  if(get_left_dir)   real_left_speed = get_left_speed;//计算带符号脉冲数
  else real_left_speed = -get_left_speed;

  if(motor_get_direction_right != get_right_dir) //上次的方向与此次不同,直接将这个周期的脉冲数计0
    motor_get_value_right = 0;
  else  motor_get_value_right = FTM_Pulse_Get(ftm1);//编码器数值读取
  
  get_right_speed = motor_get_value_right;
  get_right_dir = motor_get_direction_right;//将读取到的方向存储
  
  if(get_right_dir)   real_right_speed = -get_right_speed;//计算带符号脉冲数
  else real_right_speed = get_right_speed;
  
  FTM_Count_Clean(ftm0);//编码器数值清零
  FTM_Count_Clean(ftm1);//编码器数值清零
    
//  //直道、环岛判断
//  if((ad_center>(ad_flag_round[round_count]/6*5))&&(flag_round==1)&&(flag_state==0))
//  {
//    flag_round = 0;//出环设定40ms
//    flag_round_out = 1;//置位出环标志,一定时间内不允许入环判定
//  }
//
//  if((ad_center>ad_flag_round[round_count])&&(flag_state==0))
//  {
//    flag_round_count ++;
//    if(flag_round != 1) offset_cal/=4;//减小P,硬化转角控制
//    if(flag_round_count)//取进入特征值判定后两次与离开特征值判定当次,总计三次判断环岛方向
//    {//&&ad_left1>(ad_center/5*2) && ad_right1>(ad_center/5*2) && ad_right2>(ad_center/5) && ad_left2>(ad_center/5)
//      flag_round_mid = outrange_admax;//ad_center/5*2;///5*3
//      if(ad_left2>flag_round_mid && ad_left1>flag_round_mid) round_state_count++;
//      else if(ad_right2>flag_round_mid && ad_right1>flag_round_mid) round_state_count++;
//      else
//      {
//        flag_round_mid = ad_center/10*7;
//        if(ad_left1>flag_round_mid && ad_right1>flag_round_mid)
//        {
//          flag_round_mid = ad_center/5*2;
//          if(ad_left2>flag_round_mid || ad_left1>flag_round_mid)
//            round_state_count++;
//        }
//      }
//    }
//    round_out_count = 0;
//  }
//  else//出
//  {
//    round_out_count +=(real_left_speed+real_right_speed);
//    if(round_out_count>700)
//    {
//      if((flag_round_count>1)&&(flag_round == 0)&&(flag_round_out == 0))//入环设定50ms
//      {
//        flag_round_mid = outrange_admax;//ad_center/5*2;
//        if(ad_left2>flag_round_mid && ad_left1>flag_round_mid) round_state_count++;
//        else if(ad_right2>flag_round_mid && ad_right1>flag_round_mid) round_state_count++;
//        if(round_state_count>0)
//        {
//          if(round_dir[round_count])     flag_state = 1;
//          else flag_state = 2;
//        }
//
//        if(flag_state == 2||flag_state == 1)   flag_round = 1;
//      }
//      else if((flag_round_count>1)&&(flag_round == 1)&&(flag_round_noout==0))
//      {
//        flag_round = 0;//出环设定40ms
//        flag_round_out = 1;//置位出环标志,一定时间内不允许入环判定
//  //      if(round_count<(round_array_length-1)) round_count++;//环岛计数+1
//  //      else round_count = 0;//超过最大环岛数,从0开始重新计数
//      }
//      flag_round_count = 0;//清空圆环标志计数
//      round_state_count=0;
//    }
//  }
//
//  
//  if(flag_round_out==1) flag_round_delay++;//判定出环后延迟一定时间才允许判定下一个入环
//  if(flag_state == 2||flag_state == 1) 
//  {
//    //flag_round_float +=gyro_z/100;//flag_round_delay +=(real_left_speed+real_right_speed);//判定入环后延迟一定时间才允许判定出环
//    flag_round_delay++;//=gyro_z/100;
//  }
////  if((abs(flag_round_delay) > 800)&&(flag_state == 2||flag_state == 1))//4m速度下的经过1/2个最小R50圆环路径时间//3500
//  if((flag_round_delay > 50)&&(flag_state == 2||flag_state == 1))
//  {
//    flag_state = 0;//离开判定环岛区
//    flag_round_delay = 0;//计时清空
//    flag_round_float = 0;
//    flag_round_noout = 1;//不允许出环判定
//  }
//  else if((flag_round_delay > 150)&& (flag_round_out==1) &&(round_cfg==1))//5
//  {
//    flag_state = 0;//离开判定环岛区
//    flag_round_delay = 0;//计时清空
//    flag_round_out = 0;//出环标志清空,可以判定下一次入环
//    if(round_count<(round_array_length-1)) round_count++;//环岛计数+1
//    else round_count = 0;//超过最大环岛数,从0开始重新计数
//  }
//  else if((flag_round_delay > 150)&& (flag_round_out==1) &&(round_cfg==0))
//  {
//
//      flag_state = 0;//离开判定环岛区
//      flag_round_delay = 0;//计时清空
//      flag_round_out = 0;//出环标志清空,可以判定下一次入环
//      if(round_count<(round_array_length-1)) round_count++;//环岛计数+1
//      else round_count = 0;//超过最大环岛数,从0开始重新计数
//  }
//  
//  if(flag_round_noout) delay_round_noout++;
//  if(delay_round_noout > 100)
//  {
//    flag_round_out = 0;//出环标志清空,可以判定下一次入环
//    delay_round_noout = 0;
//    flag_round_noout = 0;
//  }
//  if((flag_round_out==1)&& (flag_round_delay>0) &&(flag_round_delay<25)&&(round_cfg2==1))
//  {
//    round_cfg2_flag=1;
//  }
//  
//  if(round_size[round_count] == 0)//小环打最大角R50-60  //50的前轮的尾部在切线采样，70的后轮电机在切线采样（按照小环给），100后轮在切线（按照大环给）
//  {
//    if(flag_state == 2 && flag_round)
//      offset_cal = min_offset;//-(real_left_speed+real_right_speed)/10*3;
//    else if(flag_state == 1 && flag_round)
//      offset_cal = max_offset;//+(real_left_speed+real_right_speed)/10*3;
//  }
//  else//大环R65以上打最大角的一半
//  {
//    if(flag_state == 2 && flag_round)
//      offset_cal = -25;
//    else if(flag_state == 1 && flag_round)
//      offset_cal = 25;
//    if(round_count==3) offset_cal = 0;
//  }
//  
//  if(flag_state==2 && flag_round)//左环消右转偏差
//  {
//    if(offset_cal>0) offset_cal = 0;
//    
//  }
//  else if (flag_state==1 && flag_round)
//  {
//    if(offset_cal<0) offset_cal = 0;
//  }
////  if(flag_round && (max_ad>ad_flag_round[round_count]) && round_cfg2 && (round_cfg2_flag==0))//双环岛第二个甩出环
////  {
////    round_cfg2_flag = 1;
////  }
//  
////  if(flag_round_out==1) offset_cal/=4;//出环硬化P
//  if(round_cfg2_flag==1)  round_cfg2_delay++;
//  if((round_cfg2_flag==1)&&(round_cfg2_delay<20))
//  {
//    if(round_size[round_count] == 0)//小环打最大角R50-60
//    {
//      if((round_dir[round_count]==0))
//        offset_cal = -120;
//      else if((round_dir[round_count]==1))
//        offset_cal = 120;
//    }
//    else//大环R65以上打最大角的一半
//    {
//      if((round_dir[round_count]==0))
//        offset_cal = min_offset/6*3;
//      else if((round_dir[round_count]==1))
//        offset_cal = max_offset/6*3;
//    }
//    round_cfgbeep = 1;
//  }
//  else
//  {
//    round_cfg2_flag = 0;
//    round_cfg2_delay = 0;
//    round_cfgbeep = 0;
//  }
//  
//////Pout(C7,round_cfgbeep);
//  Pout(C7,flag_round);
////  if(flag_round_delay)  Pout(C7,1);//出入环利用蜂鸣器标志
////  else Pout(C7,0);
//  //大环标志Pout(C7,flag_round && (round_size[round_count]));
  
  
  if(area != 1 && area != 8 && area != 9)
  {
    count_straight++;
  }
  else
  {
    count_straight = 0;
  }
  if(count_straight>3000) count_straight = 3000;
  if(count_straight>100) flag_straight_en = 1;
  else flag_straight_en = 0;
  //直道、环岛判断
  //环岛处理
//双黄蛋----------------------------------------------------------------------------------------------------------------------

//  if(round_count == 1)  round_cfg=1;
//  else round_cfg=0;
//  if(round_count == 2) round_cfg2 = 1;
//  else round_cfg2 = 0;


  
  
  //判环区
  if((ad_center>=ad_flag_round[round_count])&&(state == STATE_NOTROUND||state == STATE_PREROUND)&&(state_unround==0))
  {
    state = STATE_PREROUND;
    preround_delay = 0;//如果又一次检测到标志值,清空延时,重新计次
  }
  else if((ad_center<ad_flag_round[round_count]) && (state == STATE_PREROUND))//离开判环区 滤波保证正确进入环岛的时间
  {
    preround_delay += (real_left_speed+real_right_speed);
    if(preround_delay>2000) preround_delay =2000;//限幅
    if(preround_delay>100) state = STATE_SETROUND;//开始打角1700
  }
  
  if(state==STATE_SETROUND)
  {
    setround_delay++;
    if(setround_delay<50)
    {
      if(round_size[round_count])
      {
        //坡道不处理
      }
      else
      {
        if(round_dir[round_count]) offset_cal = max_offset;//右环
        else offset_cal = min_offset;  
      }
    }
    else state = STATE_INROUND;
  }
  else setround_delay = 0;
  
  if(state==STATE_INROUND)
  {
    inround_delay++;
    if(inround_delay>100)
    {
      inround_delay = 0;
      state = STATE_ROUND;
    }
  }
  else inround_delay = 0;
  
  if(state == STATE_ROUND)
  {
    round_delay++;
    if(round_size[round_count])
    {
      //坡道
      if(round_delay > time_updown)
      {
        round_delay = 0;
        state = STATE_NOTROUND;
      }
    }
    else
    {
      //正常环岛
      if(round_delay > 600)
      {
        round_delay = 0;
        state = STATE_NOTROUND;
      }
    }
  }
  else round_delay = 0;
  
  if((state == STATE_ROUND) && (max_ad>=ad_flag_round[round_count]/10*7))
  {
    state = STATE_OUTROUND;

  }
  
  if(state == STATE_OUTROUND)
  {
    outround_delay++;
    if(round_cfg==0)
    {
      if(outround_delay>100)
      {
        outround_delay = 0;
        state = STATE_NOTROUND;
        round_count++;
      }
    }
    else
    {
      if(outround_delay>5)
      {
        outround_delay = 0;
        state = STATE_NOTROUND;
        round_count++;
      }
    }
    if(round_cfg2)
    {
      if(outround_delay<50)
      {
        if(round_dir[round_count]) offset_cal = max_offset;//右环
        else offset_cal = min_offset;
      }
    }
//    if(round_size[round_count] == 0)
//    {
//      offset_cal=offset_cal/2*3;
//    }
  }
  
  if(state== STATE_SETROUND || state== STATE_OUTROUND)  Pout(C7,1);//出入环利用蜂鸣器标志|| state== STATE_INROUND 
  else Pout(C7,0);
  
  
  //offset_cal等同于已经乘了p系数
// ----------------------------------------------------------------------------------------------------------------
//  if(area == 1||area == 8||area == 9)  offset_out = offset_cal;
//  else if(area == 2 || area == 3)
//  {
//    if(offset_cal-offset_precal<0) offset_out = offset_cal;
//    else offset_out = offset_cal + (offset_kd * (offset_cal-offset_precal));
//  }
//  else if(area == 6 || area == 7)
//  {
//    if(offset_cal-offset_precal>0) offset_out = offset_cal;
//    else offset_out = offset_cal + (offset_kd * (offset_cal-offset_precal));
//  }

  offset_ei += offset_cal;
  if(area==4||area==5) offset_ei = 0;
  else if(area <= 3 && pre_area >= 6) offset_ei = 0;
  else if(area >= 6 && pre_area <= 3) offset_ei = 0;
  if(offset_ei>200) offset_ei = 200;
  else if(offset_ei<-200) offset_ei = -200;
  
  offset_ed = (offset_cal-offset_precal);

  offset_out = offset_cal + offset_kd * offset_ed + offset_ki * offset_ei;
  offset_int = (int)(offset_out);
//  offset_verint=(int)(100*(ad_right2*ad_right2-ad_left2*ad_left2)/(float)(ad_right2+ad_left2));
//  offset_verint = (int)offset_out;
  offset_precal = offset_cal;
  //数据处理

  pre_motor_left_target = motor_left_target;
  pre_motor_right_target = motor_right_target;

  flag_prestate = flag_state;
  
  
  if(round_count==3) count_out_round_three++;
  if(count_out_round_three>8000) count_out_round_three = 8000;
  if(count_out_round_three >out_round_count_three_low*100 && count_out_round_three<out_round_count_three_high*100) flag_round_updown = 1;
  else flag_round_updown = 0;
  
  if(pit0_flag_start<100)
  {
    motor_set_speed = motor_set_speed_temp*pit0_flag_start/100;
  }
  else if(flag_round_updown)
  {
    motor_set_speed = speed_inupdown;
  }
  else if(flag_slow)
  {
    motor_set_speed = speed_slow;//减速带------------------------------------------------------------------------------------------------------------
  }
  else if(state == STATE_PREROUND)
  {
    if(round_cfg)
      motor_set_speed = 40;
    else motor_set_speed = 80;
    if(round_size[round_count]==0)  motor_set_speed = motor_set_speed_temp;//100
    else motor_set_speed = speed_inupdown;//坡道减速
  }
  else if(state == STATE_ROUND)
  {
    if(round_size[round_count]==0)  motor_set_speed = speed_round;//100
    else motor_set_speed = speed_inupdown;//坡道减速
  }
  else if(flag_speed_up==1 && area != 4 && area != 5 && area != 3 && area != 6 && (real_right_speed+real_left_speed>(motor_set_speed_temp+motor_set_speed_temp)))
  {
    motor_set_speed = -100;
  }
//  else if(area != 4 && area != 5 && (real_right_speed+real_left_speed>(motor_set_speed_temp+motor_set_speed_temp)))
//  {
//    motor_set_speed = 80;
//  }
  else if(flag_speed_up==1 && flag_straight_en == 1) motor_set_speed = motor_set_speed_temp+count_unround_low;
//  else if(area == 3)
//  {
//    if((real_left_speed>(motor_set_speed-offset_cal))&&(real_right_speed>(motor_set_speed-offset_cal))) motor_set_speed = 2*motor_set_speed-2*offset_out-(real_left_speed+real_right_speed)/2;
//    else motor_set_speed = motor_set_speed_temp;
//  }
//  else if(area == 6)
//  {
//    if((real_left_speed>(motor_set_speed+offset_cal))&&(real_right_speed>(motor_set_speed+offset_cal))) motor_set_speed = 2*motor_set_speed+2*offset_out-(real_left_speed+real_right_speed)/2;
//    else motor_set_speed = motor_set_speed_temp;
//  }
  //----------------------------------------------------------------------------------------------------------------------
  else if(state == STATE_OUTROUND)
  {
    if(round_size[round_count]==0)  motor_set_speed = motor_set_speed_temp;
    else motor_set_speed = motor_set_speed_temp;//坡道减速
  }
//  else if(area == 4||area == 5) motor_set_speed = 84;
//  else if(real_left_speed>65 && real_right_speed>65 &&(area == 3||area == 6||area == 2||area == 7)) motor_set_speed = 40;
  else
  {
    motor_set_speed = motor_set_speed_temp;
  }
  

  
  //---------------------------------------------------------------------------------------------------------------------------
  if(flag_stop||area == NONE_SIT||area == OUTRANGE_SIT)//丢线停止)
  {
    motor_left_target = 0;
    motor_right_target = 0;
  }
  else if(lastactive_area==0)
  {
    motor_left_target = motor_set_speed;
    motor_right_target = motor_set_speed;
  }
  else
  {
    motor_left_target = motor_set_speed;
    motor_right_target = motor_set_speed;
    offset_and_speed = offset_out;// / ((float)(real_right_speed + real_left_speed)/2)/(float)motor_set_speed;
    if(offset_out>0)
    {
      motor_right_target = motor_right_target - offset_out;
      motor_left_target = motor_left_target + offset_out;
//      motor_right_target = motor_right_target*motor_left_p_right + motor_left_d_right;
//      motor_left_target = motor_left_target;
    }
    else 
    {
      //串行PID
      motor_right_target = motor_right_target*motor_left_p_right + motor_left_d_right - offset_out;
      motor_left_target = motor_left_target + offset_out;
      //并行PID
//      motor_right_target = motor_right_target*motor_left_p_right + motor_left_d_right;
//      motor_left_target = motor_left_target;
    }
  }

  //电机控制PID------------------------------------------------------------------------------
  ppre_motor_left_error = pre_motor_left_error;
  pre_motor_left_error = motor_left_error;
  motor_left_error = motor_left_target - real_left_speed;
//  if(motor_left_error>10)//加速p控制量
//  {
//    motor_left_error = 10;
//  }

  ppre_motor_right_error = pre_motor_right_error;
  pre_motor_right_error = motor_right_error;
  motor_right_error = motor_right_target - real_right_speed;
//  if(motor_right_error>10)
//  {
//    motor_right_error = 10;
//  }
  
//增量式
//  motor_left_ep = motor_left_error - pre_motor_left_error;
//  motor_left_ei = motor_left_error;
//  motor_left_ed = motor_left_error - 2*pre_motor_left_error + ppre_motor_left_error;
  //位置式
  motor_left_ep = motor_left_error;
  motor_left_ei += motor_left_error;
  if(motor_left_ei>50) motor_left_ei=50;
  else if(motor_left_ei<-50) motor_left_ei=-50;
  motor_left_ed = motor_left_error - pre_motor_left_error;
//  分段积分I
//  if((motor_left_error<(motor_left_target/2))&&(motor_left_error<-(motor_left_target/2)))
//  motor_left_out += motor_kp * motor_left_ep + motor_ki * motor_left_ei + motor_kd * motor_left_ed;
//  else  motor_left_out += motor_kp * motor_left_ep  + motor_kd * motor_left_ed;
  
//  if(motor_left_error<=0) motor_left_ei=0;
  
  //分段比例P
//  if((motor_left_error<(motor_set_speed/2))&&(motor_left_error>-(motor_set_speed/2)))
//  motor_left_out += motor_kp * motor_left_ep + motor_ki * motor_left_ei + motor_kd * motor_left_ed;
//  else  motor_left_out += motor_kp * motor_left_ep + motor_ki * motor_left_ei + motor_kd * motor_left_ed;
//  
//  if((flag_stop==0))
//    motor_left_out += motor_kp * motor_left_ep + motor_ki * motor_left_ei + motor_kd * motor_left_ed;
//  else 
//    motor_left_out += motor_kp * motor_left_ep + motor_kd * motor_left_ed;
  
  //棒棒控制
//  if(motor_left_error>80) motor_left_out = 800;
  
//  if(motor_left_error<0)
  
//  motor_left_out += 60 * motor_left_ep + motor_ki * motor_left_ei + motor_kd * motor_left_ed;
//  else  motor_left_out += motor_kp * motor_left_ep + motor_ki * motor_left_ei + motor_kd * motor_left_ed;
//  motor_left_out += motor_kp * motor_left_ep + motor_ki * motor_left_ei + motor_kd * motor_left_ed;
//  motor_right_ep = motor_right_error - pre_motor_right_error;
//  motor_right_ei = motor_right_error;
//  motor_right_ed = motor_right_error - 2*pre_motor_right_error + ppre_motor_right_error;
  
  motor_right_ep = motor_right_error;
  motor_right_ei += motor_right_error;
  if(motor_right_ei>50) motor_right_ei=50;
  else if(motor_right_ei<-50) motor_right_ei=-50;
  motor_right_ed = motor_right_error - pre_motor_right_error;
//  if((motor_right_error<(motor_right_target/2))&&(motor_right_error<-(motor_right_target/2))) 
//  motor_right_out += motor_kp * motor_right_ep + motor_ki * motor_right_ei + motor_kd * motor_right_ed;
//  else  motor_right_out += motor_kp * motor_right_ep + motor_kd * motor_right_ed;
  
//  if(motor_right_error<=0) motor_right_ei=0;
//  if((motor_right_error<(motor_set_speed/2))&&(motor_right_error>-(motor_set_speed/2))) 
  
//  motor_right_out += motor_kp * motor_right_ep + motor_ki * motor_right_ei + motor_kd * motor_right_ed;
//  else  motor_right_out += motor_kp * motor_right_ep + motor_ki * motor_right_ei + motor_kd * motor_right_ed;
  
//  if((flag_stop==0))
//    motor_right_out += motor_kp * motor_right_ep + motor_ki * motor_right_ei + motor_kd * motor_right_ed;
//  else
//    motor_right_out += motor_kp * motor_right_ep + motor_kd * motor_right_ed;
  
  

    
  if(state == STATE_ROUND)
  {
    offset_kd = 0;
  }
  else offset_kd = 15.12;


  
  if((flag_stop==0)&&(offset_and_speed<20)&&(offset_and_speed>-20))
    motor_left_out = motor_kp * motor_left_ep + motor_ki * motor_left_ei + motor_kd * motor_left_ed;
  else if(state == STATE_ROUND  && (round_size[round_count]==0)) motor_left_out = motor_kp * motor_left_ep  + motor_ki * motor_left_ei;
  else
    motor_left_out = motor_kp * motor_left_ep;//*3
  


  if((flag_stop==0)&&(offset_and_speed<20)&&(offset_and_speed>-20))
    motor_right_out = motor_kp * motor_right_ep + motor_ki * motor_right_ei + motor_kd * motor_right_ed;
  else if(state == STATE_ROUND && (round_size[round_count]==0)) motor_right_out = motor_kp * motor_right_ep + motor_ki * motor_right_ei;
  else  motor_right_out = motor_kp * motor_right_ep;
  
//  if(motor_right_error>30) motor_right_out = 700;
//  if(motor_right_error<0)
//  motor_right_out += 60 * motor_right_ep + motor_ki * motor_right_ei + motor_kd * motor_right_ed;
//  else  motor_right_out += motor_kp * motor_right_ep + motor_ki * motor_right_ei + motor_kd * motor_right_ed;
//  motor_right_out += motor_kp * motor_right_ep + motor_ki * motor_right_ei + motor_kd * motor_right_ed;
//
//  if(motor_left_out > MAX_SPEED_DUTY) motor_left_out = MAX_SPEED_DUTY;
//  else if(motor_left_out < MIN_SPEED_DUTY) motor_left_out = MIN_SPEED_DUTY;
//
//  if(motor_right_out > MAX_SPEED_DUTY) motor_right_out = MAX_SPEED_DUTY;
//  else if(motor_right_out < MIN_SPEED_DUTY) motor_right_out = MIN_SPEED_DUTY;

  
//  if(motor_left_error>=50) motor_left_out = 430+(motor_left_error-50)*10;
//  else if(motor_left_error<=-50) motor_left_out = -290+(motor_left_error+50)*10;
//  else motor_left_out = motor_left_error*8.6;
//  motor_left_out += motor_left_target*10;
//  if(motor_left_out>900) motor_left_out = 900;
//  
//  if(motor_right_error>=50) motor_right_out = 430+(motor_right_error-50)*10;
//  else if(motor_right_error<=-50) motor_right_out = -290+(motor_right_error+50)*10;
//  else motor_right_out = motor_right_error*8.6;
//  motor_right_out += motor_right_target*10;
//  if(motor_right_out>900) motor_right_out = 900;
  
  if(max_ad > outrange_admax) lastactive_area = area;//有效区域
  pre_area = area;
//  if(motor_left_error>60)
//  {
//    SetMotor(990,(int)motor_right_out+70);
//  }
//  else if(motor_right_error>60)
//  {
//    SetMotor((int)motor_left_out+70,990);
//  }
  //if(area == OUTRANGE_SIT) SetMotor(0,0); 

//    if(flag_stop==0)
//    {
      if(offset_out>0)
      {
//        if(real_right_speed<1) SetMotor(1000,0);
//        else 
//          if(area == 1||(area == 2)||(area == 9)||(state == STATE_ROUND && (round_size[round_count]==0)))
////          SetMotor(1000,-1000);
////        else if(area == 2)
//          SetMotor(1000,(int)(motor_right_out+motor_right_target*5.4));
//        else
          SetMotor((int)(motor_left_out+motor_left_target*5.4),(int)(motor_right_out+motor_right_target*5.4));
      }
      else
      {
//        if(real_left_speed<1) SetMotor(0,1000);
//        else 
//          if((area==7||area == 8||area == 9)||((state == STATE_ROUND && (round_size[round_count]==0))))
////          SetMotor(-1000,1000);
////        else if(area == 7)
//          SetMotor((int)(motor_left_out+motor_left_target*5.4),1000);
//        else
          SetMotor((int)(motor_left_out+motor_left_target*5.4),(int)(motor_right_out+motor_right_target*5.4));
      }
//    }
//      SetMotor((int)(motor_left_out+motor_left_target*5.4),(int)(motor_right_out+motor_right_target*5.4));
//    if(flag_stop==0) SetMotor((int)(motor_left_out),(int)(motor_right_out));
//  else SetMotor((int)(motor_left_out),(int)(motor_right_out));
//    else SetMotor(0,0);//新车空载右轮对应关系26-100//58-200//93-300//126-400//154-500//              左轮//26.5-100//62-200////100-300//167-500//带载后大约60-300
    /*
mathematica一次曲线拟合
data = {{0, 0}, {13, 100}, {29, 200}, {46.5, 300}, {63, 400}, {77, 
    500}};
model = a*t;(*^2+b*t;*)
fit = FindFit[data, model, {a}, t]
modelf = Function[{t}, Evaluate[model /. fit]]
Plot[modelf[t], {t, 0, 80}, Epilog -> Map[Point, data]]

*/
//  SetMotor(60,60);//静态工作点
}

/***********************
  设置电机
  参数  MotorDuty :占空比
*************************/
void  SetMotor(int motorduty_left, int motorduty_right)
{
  //对传入占空比进行限幅
  if(motorduty_left > MAX_SPEED_DUTY)
  {
    motorduty_left = MAX_SPEED_DUTY;
  }
  else if(motorduty_left<MIN_SPEED_DUTY)
  {
    motorduty_left = MIN_SPEED_DUTY;
  }
  if(motorduty_left < 0) 
  {
    if(drive_version)
    {
      FTM_PWM_Duty(ftm2,ftm_ch1,-motorduty_left);
      FTM_PWM_Duty(ftm2,ftm_ch0,0);
    }
    else
    {
      FTM_PWM_Duty(ftm2,ftm_ch5,-motorduty_left);
      FTM_PWM_Duty(ftm2,ftm_ch4,0);
    }
  }
  else 
  {
    if(drive_version)
    {
      FTM_PWM_Duty(ftm2,ftm_ch1,0);
      FTM_PWM_Duty(ftm2,ftm_ch0,motorduty_left);
    }
    else
    {
      FTM_PWM_Duty(ftm2,ftm_ch5,0);
      FTM_PWM_Duty(ftm2,ftm_ch4,motorduty_left);
    }
  }
  
  if(motorduty_right > MAX_SPEED_DUTY)
  {
    motorduty_right = MAX_SPEED_DUTY;
  }
  else if(motorduty_right<MIN_SPEED_DUTY)
  {
    motorduty_right = MIN_SPEED_DUTY;
  }
  if(motorduty_right < 0) 
  {
    if(drive_version)
    {
      FTM_PWM_Duty(ftm2,ftm_ch5,-motorduty_right);
      FTM_PWM_Duty(ftm2,ftm_ch4,0);
    }
    else
    {
      FTM_PWM_Duty(ftm2,ftm_ch1,-motorduty_right);
      FTM_PWM_Duty(ftm2,ftm_ch0,0);

    }
  }
  else 
  {
    if(drive_version)
    {
      FTM_PWM_Duty(ftm2,ftm_ch5,0);
      FTM_PWM_Duty(ftm2,ftm_ch4,motorduty_right);
    }
    else
    {
      FTM_PWM_Duty(ftm2,ftm_ch1,0);
      FTM_PWM_Duty(ftm2,ftm_ch0,motorduty_right);
    }
  }
}

void Initial_All(void)
{
  //核心板3色RGB LED初始化,高电平灭,低电平亮
  GPIO_Init(G1,GPO,HIGH);//R
  GPIO_Init(G2,GPO,HIGH);//G
  GPIO_Init(G3,GPO,HIGH); //B
  //按键初始化
  GPIO_Init(G4,GPI,LOW);//button2,上方,自带下拉,不能使能上拉,按下为高电平
  GPIO_Init(G5,GPI,LOW);//button1,下方,自带下拉,不能使能上拉,按下为高电平
  GPIO_Init(I0,GPI,HIGH);//摇杆按键,左侧,自带上拉,可以使能上拉,按下为低电平
  KBI_Init(KBI1_P20,IRQ_RISING);
  KBI_Init(KBI1_P21,IRQ_RISING);
  Port_Ioflt(FLTKBI1,FLTLPO2);//近似1kHz/2=500Hz低通滤波
  ADC_Init(ADC0_SE11, ADC_12bit); //SWX
  ADC_Init(ADC0_SE8, ADC_12bit);  //SWY
  //霍尔开关
  GPIO_Init(I4,GPI,HIGH);//左侧霍尔开关,stop3
  GPIO_Init(C6,GPI,HIGH);//中间霍尔开关,stop1
  GPIO_Init(E4,GPI,HIGH);//右侧霍尔开关,stop2
  //拨码开关
  GPIO_Init(C5,GPI,LOW);//4,按下为高电平
  GPIO_Init(H7,GPI,LOW);//3,按下为高电平
  GPIO_Init(H5,GPI,LOW);//2,按下为高电平
  GPIO_Init(H2,GPI,LOW);//1,按下为高电平
  //蜂鸣器
  GPIO_Init(C7,GPO,LOW);
  //SD卡
  GPIO_Init(G0,GPI,HIGH);//card detect 0为有卡,1为无卡
  uint32 baud = spi_init(spi1,USE_PCS,MASTER,1*1000*100);//100kHz
  //FLASH
  FLASH_Init();
  uint8 flash_count = 0;
  uint8 flash_data_num = sizeof(flash_num16)/sizeof(uint16);
  //读flash扇区数据
  while(flash_count < flash_data_num)
  {
    flash_num16[flash_count] = flash_read(255,2*flash_count,uint16);
    flash_count++;
  }
  menu_array_length = sizeof(menu_array)/sizeof(float);
  //更新flash数据到全局变量中
  #if (LOAD_FLASH == 1)
  while(menu_array_length)
  {
    menu_array_length--;
    *menu_array[menu_array_length] = (float)flash_num16[menu_array_length]/100;
  }
  #endif
  flash_count = 0;
  flash_data_num = sizeof(flash_round_read)/sizeof(uint16);
  //读flash扇区数据
  while(flash_count < flash_data_num)
  {
    flash_round_read[flash_count] = flash_read(254,2*flash_count,uint16);
    flash_count++;
  }
  menu_array_length = sizeof(menu_round_array)/sizeof(float);
  //更新flash数据到全局变量中
  #if (LOAD_FLASH == 1)
  while(menu_array_length)
  {
    menu_array_length--;
    *menu_round_array[menu_array_length] = flash_round_read[menu_array_length];
  }
  #endif
  
  //ADC
  ADC_Init(ADC0_SE1, ADC_12bit); //A1
  ADC_Init(ADC0_SE2, ADC_12bit); //A6
  ADC_Init(ADC0_SE3, ADC_12bit); //A7
  ADC_Init(ADC0_SE9, ADC_12bit); //C1
  ADC_Init(ADC0_SE10, ADC_12bit);//C2
  //UART串口(蓝牙)
  UART_Init(uart0, 9600, RXTX_B0B1);
  UART_SetCallback(UART_Interrupt);
  NVIC_SetPriority(UART0_IRQn, 0x02);//如果我们不对优先级进行配置的话，则默认相应中断源的向量号越低其优先级越高
  UART_RX_IRQ_Disable(uart0);
  //FTM 电机PWM输出
  FTM_PWM_Init(ftm2,ftm_ch0,F0,14000,0); //PWM1 左后
  FTM_PWM_Init(ftm2,ftm_ch1,F1,14000,0); //PWM2 左前
  FTM_PWM_Init(ftm2,ftm_ch4,G6,14000,0); //PWM3 右后
  FTM_PWM_Init(ftm2,ftm_ch5,G7,14000,0); //PWM4 右前
  //FTM 脉冲计数
  FTM_Pulse_Init(ftm0,  FTM_PS_1, TCLK1);//左轮
  FTM_Pulse_Init(ftm1,  FTM_PS_1, TCLK2);//右轮
  GPIO_Init(E1,GPI,HIGH);
  GPIO_Init(H6,GPI,HIGH);
  //PIT定时器
  #if (SIGNAL_DETECT == 1)
  PIT_Init1(pit0,5000);//单位us,5ms
  #else
  PIT_Init1(pit1,2000);//单位us,2ms
  #endif
  PIT_SetCallback(PIT_Interrupt);	/* Set the PIT callback function to be called on time 0.2s */
  Disable_Interrupt(INT_PIT_CH0);
  KBI_SetCallback(KBI_Interrupt);	/* Set the KBI callback function to be called on each button press */
  Enable_Interrupt(INT_KBI1); 	/* Enable KBI0 Interrupts */
  //
}
void OLED_Myshow(void)
{
  OLED_Clear(0x00);
  
//AD
  sprintf(spring_oled, "LL:%d",ad_left2);//
  OLED_Show_String(8,16,0,0,1,spring_oled,0);
  sprintf(spring_oled," M:%d",ad_center);//
  OLED_Show_String(8,16,0,16,1,spring_oled,0);
  sprintf(spring_oled,"RR:%d",ad_right2);//
  OLED_Show_String(8,16,0,32,1,spring_oled,0);  
  sprintf(spring_oled," L:%d",ad_left1);//
  OLED_Show_String(8,16,64,0,1,spring_oled,0);
  sprintf(spring_oled," R:%d",ad_right1);//
  OLED_Show_String(8,16,64,16,1,spring_oled,0);

  
////电机偏差
//  sprintf(spring_oled,"taL:%d",(int)motor_left_error);
//  OLED_Show_String(8,16,0,32,1,spring_oled,0);
//  sprintf(spring_oled,"taR:%d",(int)motor_right_error);
//  OLED_Show_String(8,16,64,32,1,spring_oled,0);
////编码器实际速度,带方向
//  sprintf(spring_oled,"spL:%d",real_left_speed);
//  OLED_Show_String(8,16,0,48,1,spring_oled,0);
//  sprintf(spring_oled,"spR:%d",real_right_speed);
//  OLED_Show_String(8,16,64,48,1,spring_oled,0);
//电感计算的赛道偏差
  sprintf(spring_oled, "off:%d",offset_int);
  OLED_Show_String(8,16,0,48,1,spring_oled,0);
//最后一次有效分区
  sprintf(spring_oled,"area:%d",lastactive_area);
  OLED_Show_String(8,16,64,32,1,spring_oled,0);
//  if(lastactive_area==8) 
//  {
//    Disable_Interrupt(INT_PIT_CH0);
//    while(1);
//    OLED_Show_String(8,16,64,32,1,spring_oled,0);
//  }
  sprintf(spring_oled,"d:%d",(int)count_unround);
  OLED_Show_String(8,16,64,48,1,spring_oled,0);
  
  OLED_Refresh_Gram();
}
//LCD菜单显示,待优化
void Menu_Debug_LCD(void)
{
  swx=ADC_Read(ADC0_SE11);
  swy=ADC_Read(ADC0_SE8);
  
  if(Pin(G5))  lcd_enter_page = 0;
  else if(Pin(G4)) lcd_enter_page = 1;
  else if(!Pin(I0)) menu_show_choose = !menu_show_choose;
  while(!Pin(I0));
  
  Pout(G1,!Pin(G4));
  Pout(G2,!Pin(G5));
  Pout(G3,Pin(I0));
  uint8 lcd_cursorx = 0;
  uint8 array_num = 0;
  array_num = (lcd_position_cursory-12)/24+lcd_menu_pagenum*3;
  uint8 num_length = 0;
  num_length = Num_Length((int)((*menu_array_float[array_num])*100));
  if(!lcd_enter_page) lcd_position_cursorx = num_length;
  if(swy>OLED_XY_HIGH)
  {
    if(lcd_enter_page)
    {
      *menu_array_float[array_num]+= My_Pow(10,(lcd_position_cursorx-1))/100;
    }
    else
    {
      if(lcd_position_cursory>12) lcd_position_cursory-=24;
      else
      {
        lcd_position_cursory = 60;
        if(lcd_menu_pagenum !=0)   lcd_menu_pagenum -= 1;
        else lcd_menu_pagenum = 0;
      }
    }
  }
  else if(swy<OLED_XY_LOW)
  {
    if(lcd_enter_page)
    {
      *menu_array_float[array_num]-= My_Pow(10,(lcd_position_cursorx-1))/100;
      num_length = Num_Length((int)((*menu_array_float[array_num])*100));
      if(lcd_position_cursorx >= num_length) lcd_position_cursorx=num_length;
    }
    else
    {
      if(lcd_position_cursory<60) lcd_position_cursory+=24;
      else
      {
        lcd_position_cursory = 12;
//        if(lcd_menu_pagenum <0)lcd_menu_pagenum += 1;
//        else
          lcd_menu_pagenum = 0;
      }
    }
  }
  else if(swx>OLED_XY_HIGH)
  {
    if(lcd_enter_page)
    {
      if(lcd_position_cursorx<=1)lcd_position_cursorx=1;
      else lcd_position_cursorx-=1;
    }
  }
  else if(swx<OLED_XY_LOW)
  {
    if(lcd_enter_page)
    {
      if(lcd_position_cursorx>=num_length) lcd_position_cursorx=num_length;
      else lcd_position_cursorx+=1;
    }
  }
  
  lcd_cursorx = 16+18+(num_length-lcd_position_cursorx)*6;

    sprintf(spring_menu,"%c%c:%d/100",menu_array_string_float[9*lcd_menu_pagenum],menu_array_string_float[9*lcd_menu_pagenum+1],(int)(100*(*menu_array_float[3*lcd_menu_pagenum])));
    LCD_Show_String(6,12,16,0,spring_menu,0,BLACK,WHITE);
    sprintf(spring_menu,"%c%c:%d/100",menu_array_string_float[9*lcd_menu_pagenum+3],menu_array_string_float[9*lcd_menu_pagenum+4],(int)(100*(*menu_array_float[3*lcd_menu_pagenum+1])));
    LCD_Show_String(6,12,16,24,spring_menu,0,BLACK,WHITE);
    sprintf(spring_menu,"%c%c:%d/100",menu_array_string_float[9*lcd_menu_pagenum+6],menu_array_string_float[9*lcd_menu_pagenum+7],(int)(100*(*menu_array_float[3*lcd_menu_pagenum+2])));
    LCD_Show_String(6,12,16,48,spring_menu,0,BLACK,WHITE);




  if(lcd_enter_page)
  {
    LCD_Show_String(6,12,lcd_cursorx,lcd_position_cursory-12,"_",1,BLACK,WHITE);
    LCD_Fill(12,lcd_position_cursory-12,127,lcd_position_cursory,WHITE);
  }
  else LCD_Fill(0,0,6,60,WHITE);
  LCD_Show_String(6,12,0,lcd_position_cursory-12,"~",0,BLACK,WHITE);

  
  while(swx<OLED_XY_LOW||swx>OLED_XY_HIGH||swy<OLED_XY_LOW||swy>OLED_XY_HIGH)
  {
    OLED_Fill(0,52,64,63,0,0);
    swx=ADC_Read(ADC0_SE11);
    swy=ADC_Read(ADC0_SE8);
    sprintf(spring_menu,"btnx:%d",swx);
    OLED_Show_String(6,12,0,52,1,spring_menu,1);
    sprintf(spring_menu,"btny:%d",swy);
    OLED_Show_String(6,12,64,52,1,spring_menu,1);

  }
  

}


/*----------------------------------------------------------------
菜单调试
所有参数均在退出菜单后写入Flash(ROM)中
默认使用五向开关，xy轴使用AD口采集作为坐标
所有浮点数按照100倍比例显示，
*/
void Menu_Debug(void)
{
  //保证摇杆一次只能推动一格
  swx=ADC_Read(ADC0_SE11);//x轴摇杆AD读值一次
  swy=ADC_Read(ADC0_SE8);//y轴摇杆AD读值一次

  if(!Pin(I0)) oled_enter_page = !oled_enter_page;
  //按键按下指示
  Pout(G3,Pin(I0));//进入参数辑菜单，返回参数选择菜单
  while(!Pin(I0));

  card_detect = !Pin(G0);//读取SD卡状态，显示在OLED屏右上角，有SD卡为1，无SD卡为0
  uint8 oled_cursorx = 0;
  uint8 array_num = 0;
  array_num = (oled_position_cursory-OLED_START_CURSORY)/OLED_FONT_HEIGHT+oled_menu_pagenum*OLED_MENU_PER_PAGE_COUNT;
  uint8 num_length = 0;
  num_length = Num_Length((int)((*menu_array[array_num])*100));
  if(!oled_enter_page) oled_position_cursorx = num_length;//默认初始为最高位
  if(swy>OLED_XY_HIGH)//向上
  {
    if(oled_enter_page)
    {
      *menu_array[array_num]+= My_Pow(10,(oled_position_cursorx-1))/100;
    }
    else
    {
      if(oled_position_cursory>OLED_START_CURSORY) oled_position_cursory-=OLED_FONT_HEIGHT;
      else
      {
        oled_position_cursory = OLED_START_CURSORY+(OLED_MENU_PER_PAGE_COUNT-1)*OLED_FONT_HEIGHT;
      }
    }
  }
  else if(swy<OLED_XY_LOW)//向下
  {
    if(oled_enter_page)
    {
      *menu_array[array_num]-= My_Pow(10,(oled_position_cursorx-1))/100;
      num_length = Num_Length((int)((*menu_array[array_num])*100));//更新数字位数
      if(oled_position_cursorx >= num_length) oled_position_cursorx=num_length;
    }
    else
    {
      if(oled_position_cursory<OLED_START_CURSORY+(OLED_MENU_PER_PAGE_COUNT-1)*OLED_FONT_HEIGHT) oled_position_cursory+=OLED_FONT_HEIGHT;
      else
      {
        oled_position_cursory = OLED_START_CURSORY;
      }
    }
  }
  else if(swx>OLED_XY_HIGH)//向右
  {
    if(oled_enter_page)
    {
      if(oled_position_cursorx<=1)oled_position_cursorx=1;
      else oled_position_cursorx-=1;
    }
    else
    {
      if(oled_menu_pagenum < OLED_MENU_ALL_PAGE_COUNT-1)oled_menu_pagenum += 1;
      else oled_menu_pagenum = 0;
    }
  }
  else if(swx<OLED_XY_LOW)//向左
  {
    if(oled_enter_page)
    {
      if(oled_position_cursorx>=(num_length+1)) oled_position_cursorx=(num_length+1);
      else oled_position_cursorx+=1;
    }
    else
    {
      if(oled_menu_pagenum !=0)   oled_menu_pagenum -= 1;
      else oled_menu_pagenum = OLED_MENU_ALL_PAGE_COUNT-1;
    }
  }
  
  oled_cursorx = 16+18+(num_length-oled_position_cursorx)*6;

  sprintf(spring_menu,"%c%c:%d/100",menu_array_string[9*oled_menu_pagenum],menu_array_string[9*oled_menu_pagenum+1],(int)(100*(*menu_array[3*oled_menu_pagenum])));
  OLED_Show_String(6,12,16,16,1,spring_menu,0);
  sprintf(spring_menu,"%c%c:%d/100",menu_array_string[9*oled_menu_pagenum+3],menu_array_string[9*oled_menu_pagenum+4],(int)(100*(*menu_array[3*oled_menu_pagenum+1])));
  OLED_Show_String(6,12,16,28,1,spring_menu,0);
  sprintf(spring_menu,"%c%c:%d/100",menu_array_string[9*oled_menu_pagenum+6],menu_array_string[9*oled_menu_pagenum+7],(int)(100*(*menu_array[3*oled_menu_pagenum+2])));
  OLED_Show_String(6,12,16,40,1,spring_menu,0);

  
  sprintf(spring_menu,"sd:%d",card_detect);
  OLED_Show_String(6,12,104,0,1,spring_menu,0);
  
  sprintf(spring_menu,"sd:%d",flash_num16[3*oled_menu_pagenum]);
  OLED_Show_String(6,12,72,16,1,spring_menu,0);
  sprintf(spring_menu,"sd:%d",flash_num16[3*oled_menu_pagenum+1]);
  OLED_Show_String(6,12,72,28,1,spring_menu,0);
  sprintf(spring_menu,"sd:%d",flash_num16[3*oled_menu_pagenum+2]);
  OLED_Show_String(6,12,72,40,1,spring_menu,0);

  if(oled_enter_page)  OLED_Reverse(6,12,oled_cursorx,oled_position_cursory,0);
  OLED_Show_String(6,12,0,oled_position_cursory,1,"~~",0);
  sprintf(spring_menu,"btnx:%d",swx);
  OLED_Show_String(6,12,0,52,1,spring_menu,0);
  sprintf(spring_menu,"btny:%d",swy);
  OLED_Show_String(6,12,64,52,1,spring_menu,0);
  
  while(swx<OLED_XY_LOW||swx>OLED_XY_HIGH||swy<OLED_XY_LOW||swy>OLED_XY_HIGH)//上一次推动了摇杆，等待摇杆释放
  {
    OLED_Clear(0x00);
    swx=ADC_Read(ADC0_SE11);
    swy=ADC_Read(ADC0_SE8);
    sprintf(spring_menu,"btnx:%d",swx);
    OLED_Show_String(6,12,0,52,1,spring_menu,1);
    sprintf(spring_menu,"btny:%d",swy);
    OLED_Show_String(6,12,64,52,1,spring_menu,1);
  }
  
  OLED_Refresh_Gram();
}

void Round_Read_Menu(void)
{
  ad_center = ADC_Read(ADC0_SE1);
  swy=ADC_Read(ADC0_SE8);

  //按键按下指示
  Pout(G1,!Pin(G4));//读取AD值

  uint8 array_num = 0;
  array_num = (oled_position_cursory-16)/12+oled_menu_pagenum*3;

  if(Pin(G4))
  {
    *menu_round_array[array_num] = ad_center;
//    if(oled_position_cursory<40) oled_position_cursory+=12;
//    else
//    {
//      oled_position_cursory = 16;
//      if(oled_menu_pagenum <1)oled_menu_pagenum += 1;
//      else oled_menu_pagenum = 0;
//    }
    OLED_Clear(0x00);
  }
  else if(!Pin(I0))
  {
    *menu_round_array[array_num] = 4100;//大于4096,不可能满足,等效于取消后续环岛判定
    OLED_Clear(0x00);
  }
  while(Pin(G4));//等待按键放开
  while(!Pin(I0));
  
  if(swy>OLED_XY_HIGH)//向上
  {
    if(oled_position_cursory>16) oled_position_cursory-=12;
    else
    {
      oled_position_cursory = 40;
      if(oled_menu_pagenum !=0)   oled_menu_pagenum -= 1;
      else oled_menu_pagenum = 1;
    }
  }
  else if(swy<OLED_XY_LOW)//向下
  {
    if(oled_position_cursory<40) oled_position_cursory+=12;
    else
    {
      oled_position_cursory = 16;
      if(oled_menu_pagenum <1)oled_menu_pagenum += 1;
      else oled_menu_pagenum = 0;
    }
  }

  sprintf(spring_menu,"%c%c:%d",menu_array_round_string[9*oled_menu_pagenum],menu_array_round_string[9*oled_menu_pagenum+1],*menu_round_array[3*oled_menu_pagenum]);
  OLED_Show_String(6,12,16,16,1,spring_menu,0);
  sprintf(spring_menu,"%c%c:%d",menu_array_round_string[9*oled_menu_pagenum+3],menu_array_round_string[9*oled_menu_pagenum+4],*menu_round_array[3*oled_menu_pagenum+1]);
  OLED_Show_String(6,12,16,28,1,spring_menu,0);
  sprintf(spring_menu,"%c%c:%d",menu_array_round_string[9*oled_menu_pagenum+6],menu_array_round_string[9*oled_menu_pagenum+7],*menu_round_array[3*oled_menu_pagenum+2]);
  OLED_Show_String(6,12,16,40,1,spring_menu,0);

  sprintf(spring_menu,"sd:%d",flash_round_read[3*oled_menu_pagenum]);
  OLED_Show_String(6,12,72,16,1,spring_menu,0);
  sprintf(spring_menu,"sd:%d",flash_round_read[3*oled_menu_pagenum+1]);
  OLED_Show_String(6,12,72,28,1,spring_menu,0);
  sprintf(spring_menu,"sd:%d",flash_round_read[3*oled_menu_pagenum+2]);
  OLED_Show_String(6,12,72,40,1,spring_menu,0);

  OLED_Show_String(6,12,0,oled_position_cursory,1,"~~",0);
  sprintf(spring_menu,"ad:%d",ad_center);
  OLED_Show_String(6,12,0,52,1,spring_menu,0);
  sprintf(spring_menu,"btny:%d",swy);
  OLED_Show_String(6,12,64,52,1,spring_menu,0);
  
  while(swx<OLED_XY_LOW||swx>OLED_XY_HIGH||swy<OLED_XY_LOW||swy>OLED_XY_HIGH)//推动了摇杆，等待摇杆释放
  {
    OLED_Clear(0x00);
    ad_center = ADC_Read(ADC0_SE1);
    swy=ADC_Read(ADC0_SE8);
    sprintf(spring_menu,"ad:%d",ad_center);
    OLED_Show_String(6,12,0,52,1,spring_menu,1);
    sprintf(spring_menu,"btny:%d",swy);
    OLED_Show_String(6,12,64,52,1,spring_menu,1);
  }
  
  OLED_Refresh_Gram();
}

uint8 Num_Length(int num)
{
  uint8 length=0;
  if(num<0)
  {
    num=-num;
    length++;
  }
  do
  {
    num/=10;
    length++;
  }
  while(num!=0);
  return length;
}

uint8 Num_float_Length(float num)
{
  uint8 length = 0;
  return length;
}

double My_Pow(double m,int8 n)
{
  double result=1;
  if(n>0)
  {
    while(n--)result*=m;
  }
  else
  {
    while(n++)result/=m;
  }
  return result;
}

void float3_sprintf(float num, char *buf)
{
  sprintf(buf,"F:%d.%03d",(int)num,(int)((num-(int)num)*1000));
}

void buffer_left_move(int16 *buffer , int buf_len)
{
  int i;
  char tmp = buffer[0];
  for(i = 1 ; i < buf_len ; i++)
  {
    buffer[i-1] = buffer[i];
  }
  buffer[buf_len-1] = tmp;
}

int array_sum(int16 array[],int n)                   //求和函数
{
  int sum=0;
  for(int i=0;i<n;i++)
  {
    sum+=array[i];
  }
  return sum;
}
