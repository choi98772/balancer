#ifndef MYTIME_H_
#define MYTIME_H_

//시간 측정 유틸리티입니다.
class CMyTime
{
	long m_lTime;
public:
	CMyTime()
	{
		m_lTime = millis();
	}

	//현재 시간캡쳐
	void Capture()
	{
		m_lTime = millis();
	}

	//이전에 capture한 시점부터 이함수를 호출하는 시점까지의 경과 시간을 초단위로 구함
	float GetPassedTime()
	{
		long lpassedTime = millis() - m_lTime;

		return (float)lpassedTime * (1.0f / 1000.0f);
	}
};
#endif