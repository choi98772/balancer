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

//�ش� ������ �������� ���ʿ� �ִ��� �����ʿ� �ִ��� Ȯ��
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