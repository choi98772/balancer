//����������� �����ܿ� �ȵ���̵������ ���� ���� �����ؾ� �Ǳ� ������ �ش�κ� �ҽ��� ���� �Ƶ��̳� �ʿ����� ���� �ҽ��� ��ó���� ó�� �߽��ϴ�.
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

#define STAND_BY 6	//TB6612FNG���� ����̹��� READY��, arduino pin 6

//----------------------------------------------------------------------------------------------------------------------------------------
#ifdef USING_BLUETOOTH
void AnalyzeBTPacket(char* pPacket, int len);

#ifdef SOFTBT //����Ʈ ���� �ø���v���� �������(��������, ����Ʈ���� ��������� ���ͷ�Ʈ�� �������̺귯���� ���ͷ�Ʈ�� �����ؼ�(����Ʈ�ø��� ���̺귯������ cli()���� �Լ���), �������͸� Ƣ�� ���� �����ڴ�)
CBlueToothMng g_BM(2, 3, 57600, AnalyzeBTPacket);  //software serial bluetooth
//CBlueToothMng g_BM(0, 1, 57600, AnalyzeBTPacket);  //software serial bluetooth, ADU RX, TX => blueTooth TX, RX
#else
CSEBlueToothMng g_BM(57600, AnalyzeBTPacket);  //serial bluetooth
#endif
#endif

/*
���͵���̹� ���� Ŭ����

pin1, pin2, pwm1 : (arduino 5, 4, 3) ����1����
pnn3, pin4, pwm2 : (arduino 7, 8, 11)����2����

bEnablePWM : true�ϰ�� pwm��� ����
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

  //���͵���̹��� STAND_BY�ɿ� HIGH�� ����ؼ� ���͵���̹��� Ȱ��ȭ ��Ų��.
  pinMode(STAND_BY, OUTPUT);
  digitalWrite(STAND_BY, HIGH);

#ifdef USING_BLUETOOTH
  //�ҷ����� �Ŵ��� �ʱ�ȭ
  g_BM.Start();
#endif

  //���͵���̹� �ʱ�ȭ
  g_Motor.Init();

  //�뷱�� ��Ʈ�ѷ� �ʱ�ȭ
  g_Blancer.Init();

#ifndef USING_BLUETOOTH
  //������� ���� �ٷ� �����ϴ°�� �ٷ� �뷱�� �����ƾ ����
  g_Blancer.Enable(true);
#endif
}

void loop()
{
#ifdef USING_BLUETOOTH
	g_BM.Update();		//������� �Ŵ��� ������Ʈ
#endif

	g_Blancer.Update();	//�뷱�� ������Ʈ
}

/*���ŵ� ������� ��Ŷ ó�� ������

�ȵ���̵� ���� ���÷� ���� ���� ��ȣ�� ���� �޾Ƽ� ó���Ѵ�., 

�뷱�̷κ� ����/����
����/����/��ȸ��/��ȸ��
PID���� ��� ����(P, I, D, PWM���Ѱ�)
��ü �߽ɰ�
---------------------------------------------------------------------
�Ƶ��̳�� �ȵ���̵尣�� ������� ����� ���ؼ� �ְ� �޴� ��Ŷ��
������ ���� ������ �̷�����ִ�.

��������(1����Ʈ), ��Ŷ����(2����Ʈ), ��ŶŸ��(1����Ʈ)

���� ������ ���� ª�� ��Ŷ�� �����̴�.

��Ŷ�� Ÿ�Կ� ���� �ڿ� ������ ������ �����Ͱ� �� �߰��ȴ�.

�� �ۼ��� ��Ŷ�� ���Ź��ۿ� �۽� ���ۿ� �������� ��ϵǸ�,
�۽Ž� �۽� ���۷� ���� �Ѳ����� ������ �۽��� �ϰ�, ���ŵǴ� �����ʹ�
���Ź��ۿ� �������� �׾Ҵٰ�, 
�ϳ��� ��Ŷ�� �ϼ��ɶ����� �ϳ��� ������ ó���ϰԵȴ�.
�������� ����Ʈ�� ����, �߰� ��Ŷ�սǿ� ����ϴ�.
��������� ��Ŷ�ս��� ����������� �������ִ��� �𸣰ڴ�.
�ƹ�ư �ٰŸ����� ��������� �ƹ��� ���� ������, ������� ����� �Ÿ��� �־�����
��ȣ�� �������ϴ� ��Ȳ�� �������� ��� ���� �𸣰ڴ�.
�޸𸮰� ���簡 ����Ͽ� �����ս��� ������ ���� �� �ִ�.
*/

#ifdef USING_BLUETOOTH
void AnalyzeBTPacket(char* pPacket, int len)
{
  switch(pPacket[3])
  {
    case PK_BT_PING: //���� �߻�� �ִ��� Ȯ���ϱ� ������Ŷ
      {
        g_BM.SendPacket(PK_BT_PING);
      }
      break;

    case PK_BT_DC_FOWARD: //����
      {
        g_Blancer.SetCenter(BL_CENTER_ANGLE - 6.0f);
      }
      break;    
    case PK_BT_DC_BACKWARD: //����
      {
        g_Blancer.SetCenter(BL_CENTER_ANGLE + 6.0f);
      }
      break;    
    case PK_BT_DC_STOP: //����
      {
        g_Blancer.SetCenter(BL_CENTER_ANGLE);
		g_Blancer.ResetBlancing();
      }
      break;    

    case PK_BT_DC_TURN_LEFT: //��ȸ��
      {
        g_Blancer.SetCenter(BL_CENTER_ANGLE + 0.8f, BL_CENTER_ANGLE - 0.8f);
      }
      break;    
    case PK_BT_DC_TURN_RIGHT: //��ȸ��
      {
        g_Blancer.SetCenter(BL_CENTER_ANGLE - 0.8f, BL_CENTER_ANGLE + 0.8f);
      }      
      break;

    case PK_BT_SERVO_GET_ANGLE: //�������� ���� �б�, �뷱�� �κ������� �Ⱦ��� �Ķ������
      {
        g_BM.SendPacket(PK_BT_SERVO_GET_ANGLE, (char)0);
      }
      break;    
      
    case PK_BT_ENABLE_AI: //�ڵ��������
      {
        g_Blancer.Enable(true);
      }
      break;
    case PK_BT_DISABLE_AI: //�ڵ���������
      {
        g_Blancer.Enable(false);
      }
      break;
	case PK_BT_PID_KP:	//PID,��ʻ�� ����
		{
			CPIDMng::SetKP(*((float *)&pPacket[4]));
		}
		break;
	case PK_BT_PID_KI:	//PID, ���� ��� ����
		{
			CPIDMng::SetKI(*((float *)&pPacket[4]));
		}
		break;
	case PK_BT_PID_KD:	//PID, �̺� ��� ����
		{
			CPIDMng::SetKD(*((float *)&pPacket[4]));
		}
		break;
	case PK_BT_CENTER:	//��ü�� �߽� ������ ����, �⺻ ������ 0��
		{
			gSetCenter(*((float *)&pPacket[4]));
		}
		break;
	case PK_BT_MAX_PWM://PWM���ļ��� �ִ�ġ�� ���� (0 ~ 255)
		{
			CPIDMng::SetPWMLimit(*((float *)&pPacket[4]));
		}
		break;
  }
}
#endif