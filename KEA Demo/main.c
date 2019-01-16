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

    while (1)
    {
        // 读取AD值并打印
        OLED_Clear(0x00);
        AD1 = ADC_Read(ADC0_SE1);
        AD2 = ADC_Read(ADC0_SE3);
        sprintf(spring_oled, "%d", AD1);
        OLED_Show_String(8, 16, 0, 20, 1, spring_oled, 0);
        sprintf(spring_oled, "%d", AD2);
        OLED_Show_String(8, 16, 80, 20, 1, spring_oled, 0);

        // 使用PD设置偏移量
        offset = (float)100 * (AD1 - AD2) / (AD1 + AD2 + 10);
        const int straight_adjust_thres = 30, turn_thres = 45;

        /*
	    if(fabs(offset)>straight_adjust_thres&&fabs(offset)<=turn_thres)//直道调整
	    {
      	    FTM_PWM_Duty(ftm0, ftm_ch0, (int)(mid-(offset>0?1:-1)*(fabs(offset)-straight_adjust_thres)*0.7));//乘数为转弯系数
    	    FTM_PWM_Duty(ftm2, ftm_ch1, speed);//除数为减速系数
    	}
    	else*/

        if (fabs(offset) > turn_thres)
        {                                                           //转弯的offset阈值
            FTM_PWM_Duty(ftm0, ftm_ch0, (int)(mid - offset * 2.6)); //乘数为转弯系数
            if (fabs(offset) > turn_thres + 20)
                FTM_PWM_Duty(ftm2, ftm_ch1, speed - fabs(offset) / 4.8); //除数为减速系数
            else
                FTM_PWM_Duty(ftm2, ftm_ch1, speed - fabs(offset) / 2.7); //除数为减速系数
        }
        else
        { //直行
            setMotor(speed);
            setSteer(mid);
        }
        //sprintf(spring_oled, "%.2f", offset);
        //OLED_Show_String(8,16,0,20,1,spring_oled,0);

        OLED_Refresh_Gram();
    }
}
