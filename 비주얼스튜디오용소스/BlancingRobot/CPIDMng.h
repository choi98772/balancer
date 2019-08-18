#ifndef PID_MNG_H_
#define PID_MNG_H_

#include <Arduino.h>
#include "CAngleUtil.h"

/*
#define PID_KP (1.0f / 35.0f)
#define PID_KI 0.05f
#define PID_KD 6.7f
*/

#define LIMIT_AMOUNT 20.0f

void gSetCenter(float fCenter);

/*PID���� Ŭ����

P : �������, ������ �ٽ��� �Ǵ� ���� ����̴�. ��� �ʿ��� ��ŭ ����ؼ� ����� �����Ѵ�.  P��� * ����
I : ��������, ��������� �������� ������ �����ϱ� ��������� ������ ������ŭ �߰���� ���ؼ� ���� ������ ����� �����Ѵ�. I��� * ��������
D : �̺�����, �ް��� ȯ�溯ȭ�� ���� ���� �ż��� ��� ���� ���� ���� ����  D��� * (������� - ��������)


���� ������ ���̴� ��ó�� ������ �����ֵ� �����̸� �߿��� �����Դϴ�.
�뷱�� �κ��� �ϼ��ߴٸ� ���� ���� P���� ����� ã�ƾ� �մϴ�.
I, D�������� 0���� �л��¿��� ������ P���� ����� ã�� ������
I�� D���� ����� ã���ñ� �ٶ��ϴ�.

���� �����س��� ������ ���� ���� ȯ�濡 �´� ���Դϴ�. �Ȱ��� ȯ�汸���� �ƴѴ������� �翬�� ����� ���� ���� ���� �̴ϴ�.
*/
class CPIDMng
{
	float m_fOldAngle;	//������
	float m_fDestAngle;	//��ǥ��
	float m_fErrorAmount;	//��������

public:
	CPIDMng()
	{
		m_fOldAngle = 0.0f;
		m_fDestAngle = 0.0f;
		m_fErrorAmount = 0.0f;
	}
	
	static float PID_KP;	//���������
	static float PID_KI;	//����������
	static float PID_KD;	//�̺�������
	static float PWM_LIMIT;	//PWM ���Ѱ�

	//���� ����� ����
	static void SetKP(float kp);
	static void SetKI(float ki);
	static void SetKD(float kd);
	static void SetPWMLimit(float f);

	//���� ��ǥ�� ����
	void SetDestAngle(float fAngle)
	{
		m_fDestAngle = fAngle;
	}

	//PID���� �ʱ�ȭ
	void Init(float fDestAngle)
	{
		m_fOldAngle = 0.0f;
		m_fDestAngle = fDestAngle;
		m_fErrorAmount = 0.0f;
	}

	//����鸸 �ʱ�ȭ ��Ŵ
	void Reset()
	{
		m_fOldAngle = 0.0f;
		m_fErrorAmount = 0.0f;
	}

	//PID���� ����
	float PID(float fCurAngle)
	{
		//��ǥ���� ���簢�� ������ ����
		float fErrorAngle = m_fDestAngle - fCurAngle;

		//������ ������Ŵ
		m_fErrorAmount += fErrorAngle * 0.1f;

		/*PID���� ���Ŀ� ���� ��� ����
		��, �������� ���� ��� ���� �Ѱ谪���� Ŭ����(-LIMIT_AMOUNT ~ LIMIT_AMOUNT) �ϴµ�, �ƹ��� ������ ��� �ϴ��� ������ ��ǥ���� ������ �� ���� ������
		�ð��� ���� ���� ���� ������ �ʹ�Ŀ���� ��� �����ϱ� �����̴�.
		*/
		float fPID = (PID_KP * fErrorAngle) + (PID_KI * constrain(m_fErrorAmount, -LIMIT_AMOUNT, LIMIT_AMOUNT)) + (PID_KD * (fErrorAngle - m_fOldAngle));

		//������ �������� �����ϱ�
		m_fOldAngle = fErrorAngle;

		//PID���� pwm���� ������ ����
		return constrain(fPID, -PWM_LIMIT, PWM_LIMIT);
	}
};

#endif