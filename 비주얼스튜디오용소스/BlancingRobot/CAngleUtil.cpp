#include "CAngleUtil.h"

float gGetErrorAngle(float fCurAngle, float fDestAngle)
{
	float fAngleDelta;

	if (fCurAngle > fDestAngle)
		fAngleDelta = fCurAngle - fDestAngle;
	else
		fAngleDelta = fDestAngle - fCurAngle;


	float fDiffAngle = 360.0f - fAngleDelta;


	if (fDiffAngle < fAngleDelta)
		return fDiffAngle;
	return fAngleDelta;

}

//해당 각도가 기준점의 왼쪽에 있는지 오른쪽에 있는지 확인
int gCheckAngleSide(float fCurAngle, float fDestAngle)
{
	float fAngleDelta = fDestAngle - fCurAngle;

	if (fAngleDelta > 0.0f)
	{
		float a = fAngleDelta;
		float b = 360.0f - fAngleDelta;

		if (a < b)
			return ANGLE_SIDE_LEFT;
		return ANGLE_SIDE_RIGHT;
	}

	float a = -fAngleDelta;
	float b = 360.0f + fAngleDelta;

	if (a < b)
		return ANGLE_SIDE_RIGHT;
	return ANGLE_SIDE_LEFT;
}