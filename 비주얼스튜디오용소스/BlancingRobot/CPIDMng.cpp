#include "CPIDMng.h"

float BL_CENTER_ANGLE = 0.0f;

float CPIDMng::PID_KP = 5.8f;
float CPIDMng::PID_KI = 21.8f;
float CPIDMng::PID_KD = 10.4f;
float CPIDMng::PWM_LIMIT = 255.0f;

void gSetCenter(float fCenter) {BL_CENTER_ANGLE = fCenter;}

void CPIDMng::SetKP(float kp)
{
	PID_KP = kp;
}

void CPIDMng::SetKI(float ki)
{
	PID_KI = ki;
}

void CPIDMng::SetKD(float kd)
{
	PID_KD = kd;
}

void CPIDMng::SetPWMLimit(float f)
{
	PWM_LIMIT = f;
}