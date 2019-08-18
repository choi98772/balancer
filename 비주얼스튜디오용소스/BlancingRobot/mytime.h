#ifndef MYTIME_H_
#define MYTIME_H_

//�ð� ���� ��ƿ��Ƽ�Դϴ�.
class CMyTime
{
	long m_lTime;
public:
	CMyTime()
	{
		m_lTime = millis();
	}

	//���� �ð�ĸ��
	void Capture()
	{
		m_lTime = millis();
	}

	//������ capture�� �������� ���Լ��� ȣ���ϴ� ���������� ��� �ð��� �ʴ����� ����
	float GetPassedTime()
	{
		long lpassedTime = millis() - m_lTime;

		return (float)lpassedTime * (1.0f / 1000.0f);
	}
};
#endif