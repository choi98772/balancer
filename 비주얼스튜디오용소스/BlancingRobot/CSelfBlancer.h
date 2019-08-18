#ifndef SELF_BALANCER_H_
#define SELF_BALANCER_H_

#define BL_UPDATE_TIME (1.0f / 30.0f)
#define ERR_ANGLE_RANGE (0.5f)

#define FRONT_LED 9
#define BACK_LED 10

extern float BL_CENTER_ANGLE;

/*
�⺻����

��/�� ���� PID��� �����Ѵ�.

�ʴ� 30�� ������ ���� ��ü�� ������ �����ؼ� �ش� ���� �̿��� PID���� ���� ���� �ϰ� ���� ��� �ݿ��Ѵ�.
*/

//�뷱�� ����
class CSelfBlancer
{
protected:
	bool m_bEnabled;			//�뷱�� ���� Ȱ��ȭ ��������
	float m_fLeftCenterAngle;	//���� ���� �뷱�� ����
	float m_fRightCenterAngle;	//���� ���� �뷱�� ����
	CMpu6050Manager m_Gyro;		//mpu6550 ���̷�/���ӵ� ���� 
	CPIDMng m_Left;				//���� ���͸� ���� pid��ƿ
	CPIDMng m_Right;			//���� ���͸� ���� pid��ƿ
	bool m_bLeftBlancing;		//���� ���� �뷱���� �ʿ��Ѱ�
	bool m_bRightBlancing;		//���� ���� �뷱���� �ʿ��Ѱ�
	CMyTime m_UpdateTime;		//�ð����� ��ƿ
public:
	CSelfBlancer()
	{
		m_bEnabled = false;
		m_bLeftBlancing = false;
		m_bRightBlancing = false;
		m_fLeftCenterAngle = BL_CENTER_ANGLE;
		m_fRightCenterAngle = BL_CENTER_ANGLE;
	}
	
	//�뷱�� ���� ����
	void Enable(bool bEnable)
	{
		m_bEnabled = bEnable;

		if (bEnable)
		{
			m_bLeftBlancing = false;
			m_bRightBlancing = false;
			m_fLeftCenterAngle = BL_CENTER_ANGLE;
			m_fRightCenterAngle = BL_CENTER_ANGLE;
			m_UpdateTime.Capture();
		}
		
		g_Motor.StopAll();
	}

	//���� �ʱ�ȭ ����
	void Init()
	{
		m_Gyro.Init();
		m_UpdateTime.Capture();

		//������������ LED�� PWM�����ϱ����� PWM�ɿ� �����ص���
		pinMode(FRONT_LED, OUTPUT);
		pinMode(BACK_LED, OUTPUT);

		//LED��β���
		TurnFrontLed(0);
		TurnBackLed(0);
	}

	//���� LED����
	void TurnFrontLed(int pwm)
	{
		if (pwm == 0)
			digitalWrite(FRONT_LED, LOW);
		else
			analogWrite(FRONT_LED, pwm);
	}

	//�ĸ� LED����
	void TurnBackLed(int pwm)
	{
		if (pwm == 0)
			digitalWrite(BACK_LED, LOW);
		else
			analogWrite(BACK_LED, pwm);
	}

	//��ü�� �߽� ������ �����Ѵ�. �⺻������ 0���̸�, �̵���� ���ؼ� �̰��� �����ϱ����ؼ� ������
	void SetCenter(float fAngle)
	{
		m_fLeftCenterAngle = fAngle;
		m_fRightCenterAngle = fAngle;

		m_Left.SetDestAngle(fAngle);
		m_Right.SetDestAngle(fAngle);
	}

	//��ü�� �� ���� ���� ������ �����Ѵ�. ��ü�� ȸ�� ��� ����
	void SetCenter(float fLeft, float fRight)
	{
		m_fLeftCenterAngle = fLeft;
		m_fRightCenterAngle = fRight;

		m_Left.SetDestAngle(fLeft);
		m_Right.SetDestAngle(fRight);
	}

	//��ü�� ���� ���� �߽ɰ� ����
	void SetLeftCenter(float fAngle)
	{
		m_fLeftCenterAngle = fAngle;
		m_Left.SetDestAngle(fAngle);
	}

	//��ü�� ���� ���� �߽ɰ� ����
	void SetRightAngle(float fAngle)
	{
		m_fRightCenterAngle = fAngle;
		m_Right.SetDestAngle(fAngle);
	}

	//�������� ȣ��Ǿ� ������ ����ȴ�.
	void Update()
	{
		//�뷫 �ʴ� 30�� ���ŷ�ƾ�� ����ǵ��� �ð��� �˻��Ѵ�.
		if (m_UpdateTime.GetPassedTime() < BL_UPDATE_TIME)
			return;

		//���� �ð��� ĸ��
		m_UpdateTime.Capture();

		//���̷� �������Ž�Ŵ
		m_Gyro.Update();

		//�뷱�����̶��
		if (m_bEnabled)
		{
			UpdateLeft(m_Gyro.GetKalAngleY());//���� ���� �뷱��
			UpdateRight(m_Gyro.GetKalAngleY()); //�������� �뷱��
		}		
	}

	//pid����� �ʱ�ȭ ��Ű�� �¿��� �뷱���� �ٽ� �ǵ��� ������
	void ResetBlancing()
	{
		LeftBlancing();
		RightBlancing();
	}
private:

	//���� ���� �뷱�� �����ϵ��� ����
	void LeftBlancing()
	{
		m_bLeftBlancing = true;
		m_Left.Init(m_fLeftCenterAngle);	//PID���� �ʱ�ȭ����
	}

	//���� ���� �뷱�� �����ϵ��� ����
	void RightBlancing()
	{
		m_bRightBlancing = true;
		m_Right.Init(m_fRightCenterAngle);
	}

	//���� ���� �뷱�� ����
	void UpdateLeft(float fCurAngle)
	{
		if (!m_bLeftBlancing)
		{
			LeftBlancing();
		}		

		float fControl = m_Left.PID(fCurAngle);

		if (fControl < 0.0f) //��ü�� �ڷ� ��������
		{
			g_Motor.Backward(true, (uint8_t)(-fControl)); //����

			//���� LED�ѱ�
			TurnFrontLed(0);
			TurnBackLed((uint8_t)(-fControl));
		}
		else //��ü�� ������ ��������
		{
			g_Motor.Forward(true, (uint8_t)fControl);	//����

			//���� LED�ѱ�
			TurnFrontLed((uint8_t)fControl);
			TurnBackLed(0);
		}
	}

	//���� ���� �뷱������
	void UpdateRight(float fCurAngle)
	{
		if (!m_bRightBlancing)
		{
			RightBlancing();
		}		

		float fControl = m_Right.PID(fCurAngle);

		if (fControl < 0.0f)
			g_Motor.Backward(false, (uint8_t)(-fControl));
		else
			g_Motor.Forward(false, (uint8_t)fControl);
	}
};
#endif