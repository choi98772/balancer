//블루투스제어 같은겨웅 안드로이드어플쪽 까지 같이 포함해야 되기 때문에 해당부분 소스는 빼고 아두이노 쪽에서도 관련 소스를 전처리기 처리 했습니다.
//#define USING_BLUETOOTH

#include <Wire.h>
#include <dcmotor2.h>

#include <inttypes.h>
#include <Kalman.h>
#include <CMPU6050.h>

#ifdef USING_BLUETOOTH
#ifdef SOFTBT
	#include <SoftwareSerial.h>
	#include <CBlueToothMng.h>
#else
	#include <CSEBlueToothMng.h>
#endif
#include <CPacketDefine.h>
#endif



#include "mytime.h"
#include "CPIDMng.h"

#define STAND_BY 6	//TB6612FNG모터 드라이버의 READY핀, arduino pin 6

//----------------------------------------------------------------------------------------------------------------------------------------
#ifdef USING_BLUETOOTH
void AnalyzeBTPacket(char* pPacket, int len);

#ifdef SOFTBT //소프트 웨어 시리얼틍통한 블루투스(편리하지만, 소프트웨어 블루투스의 인터럽트가 서보라이브러리의 인터럽트를 방해해서(소프트시리얼 라이브러리내의 cli()같은 함수들), 서보모터를 튀게 만들어서 못쓰겠다)
CBlueToothMng g_BM(2, 3, 57600, AnalyzeBTPacket);  //software serial bluetooth
//CBlueToothMng g_BM(0, 1, 57600, AnalyzeBTPacket);  //software serial bluetooth, ADU RX, TX => blueTooth TX, RX
#else
CSEBlueToothMng g_BM(57600, AnalyzeBTPacket);  //serial bluetooth
#endif
#endif

/*
모터드라이버 제어 클래스

pin1, pin2, pwm1 : (arduino 5, 4, 3) 모터1제어
pnn3, pin4, pwm2 : (arduino 7, 8, 11)모터2제어

bEnablePWM : true일경우 pwm제어를 수행
(int pin1, int pin2, int pin3, int pin4, int pwm1, int pwm2, bool bEnablePWM)
*/
CDCMotorDriver g_Motor(5, 4, 7, 8, 3, 11, true);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "CSelfBlancer.h"
CSelfBlancer g_Blancer;

void setup()
{
#ifdef MYDEBUG   
  Serial.begin(9600);
#endif

  //모터드라이버의 STAND_BY핀에 HIGH를 출력해서 모터드라이버를 활성화 시킨다.
  pinMode(STAND_BY, OUTPUT);
  digitalWrite(STAND_BY, HIGH);

#ifdef USING_BLUETOOTH
  //불루투스 매니저 초기화
  g_BM.Start();
#endif

  //모터드라이버 초기화
  g_Motor.Init();

  //밸런싱 컨트롤러 초기화
  g_Blancer.Init();

#ifndef USING_BLUETOOTH
  //블루투스 없이 바로 제어하는경우 바로 밸런싱 제어루틴 시작
  g_Blancer.Enable(true);
#endif
}

void loop()
{
#ifdef USING_BLUETOOTH
	g_BM.Update();		//블루투스 매니저 업데이트
#endif

	g_Blancer.Update();	//밸런서 업데이트
}

/*수신된 블루투스 패킷 처리 수행함

안드로이드 제어 어플로 부터 다음 신호를 수신 받아서 처리한다., 

밸런싱로봇 시작/종료
전진/후진/좌회전/우회전
PID관련 상수 설정(P, I, D, PWM제한값)
동체 중심각
---------------------------------------------------------------------
아두이노와 안드로이드간에 블루투스 통신을 통해서 주고 받는 패킷은
다음과 같은 구조로 이루어져있다.

시작지시(1바이트), 패킷길이(2바이트), 패킷타입(1바이트)

위의 구조가 가장 짧은 패킷의 구조이다.

패킷의 타입에 따라서 뒤에 임의의 길이의 데이터가 더 추가된다.

각 송수신 패킷은 수신버퍼와 송신 버퍼에 차곡차곡 기록되며,
송신시 송신 버퍼로 부터 한꺼번에 꺼내서 송신을 하고, 수신되는 데이터는
수신버퍼에 차곡차곡 쌓았다가, 
하나의 패킷이 완성될때마다 하나씩 꺼내서 처리하게된다.
종료지시 바이트가 없어, 중간 패킷손실에 취약하다.
블루투스가 패킷손실을 어느정도까지 보정해주는지 모르겠다.
아무튼 근거리에서 현재까지는 아무런 문제 없지만, 블루투스 장비간의 거리가 멀어져서
신호가 까딱까딱하는 상황이 생겼을때 어떻게 될지 모르겠다.
메모리가 복사가 빈번하여 퍼포먼스에 문제가 있을 수 있다.
*/

#ifdef USING_BLUETOOTH
void AnalyzeBTPacket(char* pPacket, int len)
{
  switch(pPacket[3])
  {
    case PK_BT_PING: //서로 잘살아 있는지 확인하기 위한패킷
      {
        g_BM.SendPacket(PK_BT_PING);
      }
      break;

    case PK_BT_DC_FOWARD: //전진
      {
        g_Blancer.SetCenter(BL_CENTER_ANGLE - 6.0f);
      }
      break;    
    case PK_BT_DC_BACKWARD: //후진
      {
        g_Blancer.SetCenter(BL_CENTER_ANGLE + 6.0f);
      }
      break;    
    case PK_BT_DC_STOP: //정지
      {
        g_Blancer.SetCenter(BL_CENTER_ANGLE);
		g_Blancer.ResetBlancing();
      }
      break;    

    case PK_BT_DC_TURN_LEFT: //좌회전
      {
        g_Blancer.SetCenter(BL_CENTER_ANGLE + 0.8f, BL_CENTER_ANGLE - 0.8f);
      }
      break;    
    case PK_BT_DC_TURN_RIGHT: //우회전
      {
        g_Blancer.SetCenter(BL_CENTER_ANGLE - 0.8f, BL_CENTER_ANGLE + 0.8f);
      }      
      break;

    case PK_BT_SERVO_GET_ANGLE: //서보모터 각도 읽기, 밸런싱 로봇에서는 안쓰는 파라메터임
      {
        g_BM.SendPacket(PK_BT_SERVO_GET_ANGLE, (char)0);
      }
      break;    
      
    case PK_BT_ENABLE_AI: //자동주행시작
      {
        g_Blancer.Enable(true);
      }
      break;
    case PK_BT_DISABLE_AI: //자동주행종료
      {
        g_Blancer.Enable(false);
      }
      break;
	case PK_BT_PID_KP:	//PID,비례상수 제어
		{
			CPIDMng::SetKP(*((float *)&pPacket[4]));
		}
		break;
	case PK_BT_PID_KI:	//PID, 적분 상수 제어
		{
			CPIDMng::SetKI(*((float *)&pPacket[4]));
		}
		break;
	case PK_BT_PID_KD:	//PID, 미분 상수 제어
		{
			CPIDMng::SetKD(*((float *)&pPacket[4]));
		}
		break;
	case PK_BT_CENTER:	//동체의 중심 각도를 지정, 기본 각도는 0도
		{
			gSetCenter(*((float *)&pPacket[4]));
		}
		break;
	case PK_BT_MAX_PWM://PWM주파수의 최대치를 설정 (0 ~ 255)
		{
			CPIDMng::SetPWMLimit(*((float *)&pPacket[4]));
		}
		break;
  }
}
#endif