  #ifndef SELF_BALANCER_H_
#define SELF_BALANCER_H_

#define BL_UPDATE_TIME (1.0f / 30.0f)
#define ERR_ANGLE_RANGE (0.5f)

#ifdef USING_LED
#define FRONT_LED 9
#define BACK_LED 10
#endif

extern float BL_CENTER_ANGLE;

/*
기본구조

좌/우 각각 PID제어를 수행한다.

초당 30번 센서로 현재 차체의 각도를 측정해서 해당 값을 이용해 PID제어 값을 산출 하고 모터 제어에 반영한다.
*/

//밸런스 제어
class CSelfBlancer
{
protected:
	bool m_bEnabled;			//밸런스 제어 활성화 상태인지
	float m_fLeftCenterAngle;	//좌측 모터 밸런스 제어
	float m_fRightCenterAngle;	//우측 모터 밸런스 제어
	CMpu6050Manager m_Gyro;		//mpu6550 자이로/가속도 센서 
	CPIDMng m_Left;				//좌측 모터를 위한 pid유틸
	CPIDMng m_Right;			//우측 모터를 위한 pid유틸
	bool m_bLeftBlancing;		//좌측 모터 밸런싱이 필요한가
	bool m_bRightBlancing;		//우측 모터 밸런싱이 필요한가
	CMyTime m_UpdateTime;		//시간측정 유틸
public:
	CSelfBlancer()
	{
		m_bEnabled = false;
		m_bLeftBlancing = false;
		m_bRightBlancing = false;
		m_fLeftCenterAngle = BL_CENTER_ANGLE;
		m_fRightCenterAngle = BL_CENTER_ANGLE;
	}
	
	//밸런싱 제어 시작
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

	//각종 초기화 수행
	void Init()
	{
		m_Gyro.Init();
		m_UpdateTime.Capture();

#ifdef USING_LED
		//기울어진방향의 LED를 PWM제어하기위해 PWM핀에 연결해뒀음
		pinMode(FRONT_LED, OUTPUT);
		pinMode(BACK_LED, OUTPUT);


		//LED모두끄기
		TurnFrontLed(0);
		TurnBackLed(0);
#endif
	}

#ifdef USING_LED
	//전면 LED제어
	void TurnFrontLed(int pwm)
	{
		if (pwm == 0)
			digitalWrite(FRONT_LED, LOW);
		else
			analogWrite(FRONT_LED, pwm);
	}

	//후면 LED제어
	void TurnBackLed(int pwm)
	{
		if (pwm == 0)
			digitalWrite(BACK_LED, LOW);
		else
			analogWrite(BACK_LED, pwm);
	}
#endif

	//차체의 중심 각도를 설정한다. 기본적으로 0값이며, 이동제어를 위해서 이값을 조절하기위해서 존재함
	void SetCenter(float fAngle)
	{
		m_fLeftCenterAngle = fAngle;
		m_fRightCenterAngle = fAngle;

		m_Left.SetDestAngle(fAngle);
		m_Right.SetDestAngle(fAngle);
	}

	//차체의 좌 우측 센터 각도를 설정한다. 차체의 회전 제어를 위함
	void SetCenter(float fLeft, float fRight)
	{
		m_fLeftCenterAngle = fLeft;
		m_fRightCenterAngle = fRight;

		m_Left.SetDestAngle(fLeft);
		m_Right.SetDestAngle(fRight);
	}

	//차체의 좌측 모터 중심각 설정
	void SetLeftCenter(float fAngle)
	{
		m_fLeftCenterAngle = fAngle;
		m_Left.SetDestAngle(fAngle);
	}

	//차체의 우측 모터 중심각 설정
	void SetRightAngle(float fAngle)
	{
		m_fRightCenterAngle = fAngle;
		m_Right.SetDestAngle(fAngle);
	}

	//매프레임 호출되어 갱신이 수행된다.
	void Update()
	{
		//대략 초당 30번 갱신루틴이 진행되도록 시간을 검사한다.
		if (m_UpdateTime.GetPassedTime() < BL_UPDATE_TIME)
			return;

		//현재 시간을 캡쳐
		m_UpdateTime.Capture();

		//자이로 센서갱신시킴
		m_Gyro.Update();

		//밸런싱중이라면
		if (m_bEnabled)
		{
			UpdateLeft(m_Gyro.GetKalAngleY());//좌측 모터 밸런싱
			UpdateRight(m_Gyro.GetKalAngleY()); //우측모터 밸런싱
		}		
	}

	//pid제어값을 초기화 시키고 좌우측 밸런싱이 다시 되도록 설정함
	void ResetBlancing()
	{
		LeftBlancing();
		RightBlancing();
	}
private:

	//좌측 모터 밸런싱 시작하도록 설정
	void LeftBlancing()
	{
		m_bLeftBlancing = true;
		m_Left.Init(m_fLeftCenterAngle);	//PID제어 초기화수행
	}

	//우측 모터 밸런싱 시작하도록 설정
	void RightBlancing()
	{
		m_bRightBlancing = true;
		m_Right.Init(m_fRightCenterAngle);
	}

	//좌측 모터 밸런싱 제어
	void UpdateLeft(float fCurAngle)
	{
		if (!m_bLeftBlancing)
		{
			LeftBlancing();
		}		

		float fControl = m_Left.PID(fCurAngle);

		if (fControl < 0.0f) //차체가 뒤로 기울었으면
		{
			g_Motor.Backward(true, (uint8_t)(-fControl)); //후진
                        //g_Motor.Forward(true, (uint8_t)(-fControl)); //후진

#ifdef USING_LED
			//후진 LED켜기
			TurnFrontLed(0);
			TurnBackLed((uint8_t)(-fControl));
#endif
		}
		else //차체가 앞으로 기울었으면
		{
			g_Motor.Forward(true, (uint8_t)fControl);	//전진
                        //g_Motor.Backward(true, (uint8_t)fControl);	//전진

#ifdef USING_LED
			//전진 LED켜기
			TurnFrontLed((uint8_t)fControl);
			TurnBackLed(0);
#endif
		}
	}

	//우측 모터 밸런싱제어
	void UpdateRight(float fCurAngle)
	{
		if (!m_bRightBlancing)
		{
			RightBlancing();
		}		

		float fControl = m_Right.PID(fCurAngle);

		if (fControl < 0.0f)
                {
			g_Motor.Backward(false, (uint8_t)(-fControl));
                        //g_Motor.Forward(false, (uint8_t)(-fControl));
                }
		else
                {
			g_Motor.Forward(false, (uint8_t)fControl);
                        //g_Motor.Backward(false, (uint8_t)fControl);
                }
	}
};
#endif
