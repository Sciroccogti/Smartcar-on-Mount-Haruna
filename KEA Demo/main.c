// 比完赛晚上秋名山见 v3.1
// Last updated: 12-4-2019 By 张逸帆
// feat: 适配新赛道

#include "main.h"

void PIT_Interrupt(uint8 ch)
{
    //GPIO_Turn(G1);
    Refresh();
    ChecktoStop();
    Control();
}

int main(void)
{
    Init();
    while(1)
    {
        MYOledShow();
    }
}