// 比完赛晚上秋名山见 v3.0
// Last updated: 12-4-2019 By 张逸帆
// feat: PID

#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"
#include <math.h>

#define DSPEED (speed_error - lspeed_error)
#define ISPEED (speed_error + lspeed_error + llspeed_error)
#define DSTEER (offset - loffset)
#define ISTEER (offset + loffset + lloffset)

extern uint16_t AD1, AD2, AD3, AD4, ADV; // electromagnet sensor cache
extern int distance, mode, count; // mode：速度控制标志，-1为自动速度
extern float steer, offset, speed, expected_steer;

float target_spd();

// 速度控制环
float speed_loop(float target_speed);

// 转向控制环
float steer_loop(float P, float I, float D);

// 通用指数控制，speedmode控制速度，-1为默认，-2为关闭
void Control();

// 异常自动停车，赛时需要移除
void ChecktoStop();

#endif