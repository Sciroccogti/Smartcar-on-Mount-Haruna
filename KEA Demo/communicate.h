// 比完赛晚上秋名山见 v3.0
// Last updated: 12-4-2019 By 张逸帆
// feat: 增加蓝牙，OLED使用局刷

#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include "main.h"

// uint8 data_getstring[2]; // bluetooth cache
extern char oled[4][20]; // a line on OLED

// 自定义显示函数，输出AD，speed，steer，isRing，count
void MYOledShow();

// void UART_Interrupt(uint8 ch);

void Send_Wave(uint8 *waveaddr, uint32 wavesize);

// 封装蓝牙接收
uint8 GetBluetooth();

#endif