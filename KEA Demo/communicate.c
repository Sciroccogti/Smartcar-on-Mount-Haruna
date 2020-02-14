#include "communicate.h"

char oled[4][20];

// 自定义显示函数，输出AD，speed，steer，isRing，count
void MYOledShow()
{
    if (Pin(H7))
    {
        OLED_Clear(0x00);
        if(oled[0][0]) sprintf(oled[0], "L:%5d R:%5d", AD1, AD4);
        OLED_Show_String(8, 16, 0, 0, 1, oled[0], 0);
        sprintf(oled[0], '\0');
        // OLED_Refresh_Gram_Page(0, 0);
        if(oled[1][0]) sprintf(oled[1], "off:%.2f m:%d", offset, mode);
        OLED_Show_String(8, 16, 0, 16, 1, oled[1], 0);
        sprintf(oled[1], '\0');
        // OLED_Refresh_Gram_Page(0, 16);
        // sprintf(spring_oled, "V:%5d R%d F%d", ADV, isRing, speedmode);
        OLED_Show_String(8, 16, 0, 32, 1, oled[2], 0);
        sprintf(oled[2], '\0');
        // OLED_Refresh_Gram_Page(0, 32);
        if(oled[3][0]) sprintf(oled[3], "C:%d S:%2.2f", count, speed);
        OLED_Show_String(8, 16, 0, 48, 1, oled[3], 0);
        sprintf(oled[3], '\0');
        // OLED_Refresh_Gram_Page(0, 48);
        OLED_Refresh_Gram();
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

void Send_Wave(void *waveaddr, uint32_t wavesize)
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