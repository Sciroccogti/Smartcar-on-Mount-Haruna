// 比完赛晚上秋名山见 v3.0
// Last updated: 12-4-2019 By 张逸帆
// test: 关闭电机倒转

#ifndef DRIVER_H
#define DRIVER_H

#include "main.h"

extern const FTMn encoder_port; // 编码器接口

// 舵机打角设定，0为打直，绝对值最大160，左负右正
void SetSteer(float dir);

// 电机电压设定，支持直接设置负数
void SetMotor(float floats);

void SetMotor_d(float s);

// 刷新数据
void Update();

#endif