// 比完赛晚上秋名山见 v2.0
// Last updated: 12-4-2019 By 张逸帆
// feat: 增加蓝牙，OLED使用局刷

#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include "main.h"

extern uint16_t AD1, AD2, AD3, AD4, ADV;
extern float offset, speed;

// uint8 data_getstring[2]; // bluetooth cache
char spring_oled[20]; // a line on OLED

// 自定义显示函数，输出AD，speed，steer，isRing，count
void MYOledShow()
{
    if (Pin(H7))
    {
        OLED_Clear(0x00);
        sprintf(spring_oled, "L:%5d R:%5d", AD1, AD4);
        OLED_Show_String(8, 16, 0, 0, 1, spring_oled, 0);
        OLED_Refresh_Gram_Page(0, 0);
        sprintf(spring_oled, "offset:%.2f", offset);
        OLED_Show_String(8, 16, 0, 16, 1, spring_oled, 0);
        OLED_Refresh_Gram_Page(0, 16);
        // sprintf(spring_oled, "V:%5d R%d F%d", ADV, isRing, speedmode);
        // OLED_Show_String(8, 16, 0, 32, 1, spring_oled, 0);
        // OLED_Refresh_Gram_Page(0, 32);
        sprintf(spring_oled, "C:%3d S:%d isR=%d", count, (int)speed, Pin(H2));
        OLED_Show_String(8, 16, 0, 48, 1, spring_oled, 0);
        OLED_Refresh_Gram_Page(0, 48);
    }
}

// void UART_Interrupt(uint8 ch)
// {
//     uint8 data_get;
//     UART_Getchar(uart2, &data_get);
//     data_getstring[0] = data_get;
//     if (data_getstring[0] == 's') //蓝牙发送s人工停止车模运行
//     {
//         GPIO_Turn(G1);
//     }
//     UART_Putstr(uart2, data_getstring);
// }

void Send_Wave(uint8 *waveaddr, uint32 wavesize)
{
#define CMD_WARE 3
    uint8_t cmdf[2] = {CMD_WARE, ~CMD_WARE}; /* 帧头 */
    uint8_t cmdr[2] = {~CMD_WARE, CMD_WARE}; /* 帧尾*/
    UART_Putbuff(uart2, cmdf, sizeof(cmdf)); /* 发送帧头 */
    UART_Putbuff(uart2, waveaddr, wavesize); /* 发送数据 */
    UART_Putbuff(uart2, cmdr, sizeof(cmdr)); /* 发送帧尾 */
}

// 封装蓝牙接收
uint8 GetBluetooth()
{
    uint8 data;
    UART_Getchar(uart2, &data);
    return data;
}

#endif