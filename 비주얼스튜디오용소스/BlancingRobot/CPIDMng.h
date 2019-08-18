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

/*PID제어 클래스

P : 비례제어, 제어의 핵심이 되는 제어 상수이다. 제어가 필요한 만큼 비례해서 제어값을 산출한다.  P상수 * 오차
I : 적분제어, 비례제어의 정밀하지 못함을 보충하기 위한제어로 누적된 오차만큼 추가제어를 통해서 좀더 정밀한 제어값을 제공한다. I상수 * 오차누적
D : 미분제어, 급격한 환경변화에 대해 좀더 신속한 제어를 위한 제어 값을 제공  D상수 * (현재오차 - 이전오차)


위의 설명에서 보이는 것처럼 비례제어가 가장주된 제어이며 중요한 제어입니다.
밸런싱 로봇을 완성했다면 가장 먼저 P제어 상수를 찾아야 합니다.
I, D제어상수를 0으로 둔상태에서 최적의 P제어 상수를 찾은 다음에
I와 D제어 상수를 찾으시기 바랍니다.

제가 설정해놓은 값들은 단지 저의 환경에 맞는 값입니다. 똑같은 환경구성이 아닌다음에는 당연히 제대로 제어 되지 않을 겁니다.
*/
class CPIDMng
{
	float m_fOldAngle;	//이전각
	float m_fDestAngle;	//목표각
	float m_fErrorAmount;	//오차누적

public:
	CPIDMng()
	{
		m_fOldAngle = 0.0f;
		m_fDestAngle = 0.0f;
		m_fErrorAmount = 0.0f;
	}
	
	static float PID_KP;	//비례제어상수
	static float PID_KI;	//적분제어상수
	static float PID_KD;	//미분제어상수
	static float PWM_LIMIT;	//PWM 제한값

	//각종 상수값 설정
	static void SetKP(float kp);
	static void SetKI(float ki);
	static void SetKD(float kd);
	static void SetPWMLimit(float f);

	//제어 목표각 설정
	void SetDestAngle(float fAngle)
	{
		m_fDestAngle = fAngle;
	}

	//PID제어 초기화
	void Init(float fDestAngle)
	{
		m_fOldAngle = 0.0f;
		m_fDestAngle = fDestAngle;
		m_fErrorAmount = 0.0f;
	}

	//제어값들만 초기화 시킴
	void Reset()
	{
		m_fOldAngle = 0.0f;
		m_fErrorAmount = 0.0f;
	}

	//PID제어 수행
	float PID(float fCurAngle)
	{
		//목표각과 현재각의 오차를 구함
		float fErrorAngle = m_fDestAngle - fCurAngle;

		//오차를 누적시킴
		m_fErrorAmount += fErrorAngle * 0.1f;

		/*PID제어 공식에 따라서 제어값 산출
		단, 적분제어 값의 경우 일정 한계값까지 클리핑(-LIMIT_AMOUNT ~ LIMIT_AMOUNT) 하는데, 아무리 정밀한 제어를 하더라도 완전한 목표값에 도달할 수 없기 때문에
		시간이 지날 수록 누적 오차가 너무커져서 제어에 실패하기 때문이다.
		*/
		float fPID = (PID_KP * fErrorAngle) + (PID_KI * constrain(m_fErrorAmount, -LIMIT_AMOUNT, LIMIT_AMOUNT)) + (PID_KD * (fErrorAngle - m_fOldAngle));

		//현재의 오차값을 저장하기
		m_fOldAngle = fErrorAngle;

		//PID값을 pwm제한 값으로 매핑
		return constrain(fPID, -PWM_LIMIT, PWM_LIMIT);
	}
};

#endif