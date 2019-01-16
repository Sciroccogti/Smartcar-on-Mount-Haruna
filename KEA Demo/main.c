//  2019年1月65日 张逸帆

#include "main.h"
void PIT_Interrupt(uint8 ch)
{
    GPIO_Turn(G2);
    GPIO_Turn(G3);
}

int main(void)
{
    MYInit();
    int nowspeed = speed;
    int inRing = 0;

    while (1)
    {
        // 读取AD值并打印
        OLED_Clear(0x00);
        AD1 = ADC_Read(ADC0_SE1);
        AD1 = AD1 > 15 ? AD1 - 15 : (AD1 > 7 ? AD1 - 7 : AD1); // 对AD值的偏差做了调整！！
        AD2 = ADC_Read(ADC0_SE3);
        AD3 = ADC_Read(ADC0_SE2);
        AD4 = ADC_Read(ADC0_SE9);
        sprintf(spring_oled, "%3d %3d %3d %3d", AD1, AD2, AD3, AD4);
        OLED_Show_String(8, 16, 0, 0, 1, spring_oled, 0);

        //count = FTM_Pulse_Get(ftm1); //编码器数值读取
        //FTM_Count_Clean(ftm1); //编码器数值清零

        // 使用PD设置偏移量
        offset = (float)100 * (AD1 - AD4) / (AD1 + AD4 + 10);
        const int straight_adjust_thres = 30, turn_thres = 45;

        if (inRing)
        {
        }

        if (AD1 + AD4 <= 20 && nowspeed) // 出赛道自动停车，赛时需要移除
        {
            Soft_Delay_ms(500);
            nowspeed = 0;
            setMotor(0);
        }
        else 
	    if(fabs(offset)>straight_adjust_thres&&fabs(offset)<=turn_thres)//直道调整
	    {
      	    setSteer((int)(mid-(offset>0?1:-1)*(fabs(offset)-straight_adjust_thres)*0.7));//乘数为转弯系数
    	    //FTM_PWM_Duty(ftm2, ftm_ch1, speed);//除数为减速系数
    	}
    	else
            if (fabs(offset) > turn_thres)
        {                                        //转弯的offset阈值
            setSteer((int)(mid - offset * 2.6)); //乘数为转弯系数
            if (fabs(offset) > turn_thres + 20)
                setMotor(speed - fabs(offset) / 4.8); //除数为减速系数
            else
                setMotor(speed - fabs(offset) / 2.7); //除数为减速系数
        }
        else
        { //直行
            setMotor(speed);
            setSteer(mid);
        }
        sprintf(spring_oled, "%.2f", offset);
        OLED_Show_String(8, 16, 0, 32, 1, spring_oled, 0);

        OLED_Refresh_Gram();
    }
}
