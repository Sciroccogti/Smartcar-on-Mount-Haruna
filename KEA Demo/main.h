// 比完赛晚上秋名山见 v3.0
// Last updated: 12-4-2019 By 张逸帆
// refactor: 改变整体文件结构，将大多数常量移至本文件
// 命名规范参见https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/naming/#
// const常量请以 k 开头，大小写混合
// 函数请以大写开头，大小写混合

#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include "driver.h"
#include "communicate.h"
#include "control.h"

// 调参常改常量
extern const int kTopSpeed;    //  速度上限
extern const int kMidSteer; // 舵机物理中值
extern const float kStraightSpeed, kCornerSpeed;
extern const int kOutMin, kOutMax;
// const int kTotalLap = 1;       //  圈数（资格赛）

void PIT_Interrupt(uint8 ch); // 中断函数

void Refresh();// 刷新数据

void Init();

#endif