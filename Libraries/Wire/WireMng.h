#ifndef WIRE_MNG_H_
#define WIRE_MNG_H_

#include "WireBuffer.h"

class CWireManager : public CWireEvent
{
protected:
	char m_cMyAddress;
	char m_cTargetAddress;
	CWireBuffer m_SendBuf;
	CWireBuffer m_RecvBuf;
	void(*AnalyzeFunc)(char* pBuf, int len);

	bool m_bOnRequest;
public:
	CWireManager(char myaddress, char targetaddress, void(*_AnalyzeFunc)(char* pBuf, int len))
	{
		m_cMyAddress = myaddress;
		m_cTargetAddress = targetaddress;
		AnalyzeFunc = _AnalyzeFunc;
		m_bOnRequest = false;
	}

	~CWireManager()
	{
		Wire.SetEventReceiver(NULL);
	}

	bool IsRequested() {return m_bOnRequest;}
	
	virtual void OnRecv(int size)
	{
		int num = Wire.available();
		if (num < 1)
			return;

		for (int i = 0;i < num; ++i)
		{
			m_RecvBuf.PutByte((char)Wire.read());
		}
	}

	virtual void OnRequest()
	{
		m_bOnRequest = true;
	}

	void Init()
	{
		Wire.begin(m_cMyAddress);
		Wire.SetEventReceiver(this);
	}

	void Update()
	{
		char buf[50];
		
		while(true)
		{
			int len = 0;
			if (m_RecvBuf.Output(len, buf, 50) <= 0)
				break;

			if (AnalyzeFunc != NULL)
				AnalyzeFunc(buf, len);
		}

		if (m_SendBuf.GetPos() > 0)
		{
			char* pArr = m_SendBuf.GetArray();

			Wire.beginTransmission(m_cTargetAddress);
			int size = Wire.write(pArr, m_SendBuf.GetPos());
			Wire.endTransmission();

			if (size == m_SendBuf.GetPos())
			{
				m_SendBuf.Clear();
			}
			else if (size > 0)
			{
				m_SendBuf.Remove(size);
			}
		}
	}

	bool CheckSendBuffer()
	{
		return (m_SendBuf.GetPos() > 0);
	}

	void SendPacket(char btPacketID)
	{
		m_SendBuf.InsertPacket(btPacketID);
	}

	void SendPacket(char btPacketID, char btData)
	{
		m_SendBuf.InsertPacket(btPacketID, btData);
	}

	void SendPacket(char btPacketID, char* pData, int len)
	{
		m_SendBuf.InsertPacket(btPacketID, pData, len);
	}

	void SendPacket(char btPacketID, float fData)
	{
		m_SendBuf.InsertPacket(btPacketID, fData);
	}
};
#endif