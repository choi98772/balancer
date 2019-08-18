#ifndef _WIRE_BUUFER_
#define _WIRE_BUUFER_

#define WIRE_BUFFER 50
#define WIRE_PACKET_HEAD 11

class CPacketSerializer
{
protected:
	char* m_pBuffer;
	int m_iPos;
	int m_iBufsize;
	bool m_bRecv;
public:
	//pBuf : 직렬화 작업에 사용할 버퍼
	//buflen : 위 버퍼의 길이
	//bRecvPacket : 수신된 패킷인경우 true, 패킷송신을 위한 직렬화 작업인경우 false
	CPacketSerializer(char* pBuf, int buflen, bool bRecvPacket)
	{
		m_pBuffer = pBuf;
		m_iBufsize = buflen;

		m_bRecv = bRecvPacket;

		if (bRecvPacket)
			m_iPos = 4;
		else
			m_iPos = 0;
	}

	//현재 위치의 버퍼 포인터를 얻음
	char* GetCurrent()
	{
		return &m_pBuffer[m_iPos];
	}

	//버퍼포지션을 시작위치로 돌림
	void Rewind()
	{
		if (m_bRecv)
			m_iPos = 4;
		else
			m_iPos = 0;
	}

	//버퍼 포지션을 len바이트 길이 만큼 전진시킴
	void MovePoint(int len)
	{
		m_iPos += len;
		if (m_iPos >= m_iBufsize)
			m_iPos = m_iBufsize;
	}

	//현재 버퍼 포지션을 얻음
	int GetCurPos()
	{
		return m_iPos;
	}

	//버퍼를 얻음
	char* GetBuffer()
	{
		return m_pBuffer;
	}

	//수신패킷의 ID를 얻음
	unsigned char GetPacketID()
	{
		return m_pBuffer[3];
	}

	//float형 값 추출
	float GetFloat()
	{
		if (m_iPos >= m_iBufsize)
			return 0.0f;

		float f = *((float*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(float);

		return f;
	}

	//double 값 추출
	double GetDouble()
	{
		if (m_iPos >= m_iBufsize)
			return 0.0;

		double f = *((double*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(double);

		return f;
	}

	//int값 추출, 주의 : 플랫폼의 아키텍쳐에 따라서 int의 바이트 길이가 달라질 수 있으므로, int8_t, int16_t등과 같은 명확한 길이를 가진 데이터 형식을 사용하기를 권장함
	int GetInt()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		int i = *((int*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(int);

		return i;
	}

	//unsigned int값 추출, 주의 : 플랫폼의 아키텍쳐에 따라서 int의 바이트 길이가 달라질 수 있으므로, int8_t, int16_t등과 같은 명확한 길이를 가진 데이터 형식을 사용하기를 권장함
	unsigned int GetUInt()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		unsigned int i = *((unsigned int*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(unsigned int);

		return i;
	}

	//한 바이트를 얻음
	char GetChar()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		char c = *((char*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(char);

		return c;
	}

	//한 바이트를 얻음
	unsigned char GetUChar()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		unsigned char c = *((unsigned char*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(unsigned char);

		return c;
	}

	short GetShort()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		short c = *((short*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(short);

		return c;
	}

	unsigned short GetUShort()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		unsigned short c = *((unsigned short*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(unsigned short);

		return c;
	}

	int8_t GetInt8()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		int8_t c = *((int8_t*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(int8_t);

		return c;
	}

	uint8_t GetUInt8()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		uint8_t c = *((uint8_t*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(uint8_t);

		return c;
	}

	int16_t GetInt16()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		int16_t c = *((int16_t*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(int16_t);

		return c;
	}

	uint16_t GetUInt16()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		uint16_t c = *((uint16_t*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(uint16_t);

		return c;
	}

	int32_t GetInt32()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		int32_t c = *((int32_t*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(int32_t);

		return c;
	}

	uint32_t GetUInt32()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		uint32_t c = *((uint32_t*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(uint32_t);

		return c;
	}

	int64_t GetInt64()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		int64_t c = *((int64_t*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(int64_t);

		return c;
	}

	uint64_t GetUInt64()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		uint64_t c = *((uint64_t*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(uint64_t);

		return c;
	}

	//버퍼에 char형 데이터를 직렬화 삽입
	bool PutChar(char c)
	{
		if ((m_iPos + 1) > m_iBufsize)
			return false;

		m_pBuffer[m_iPos++] = c;

		return true;
	}

	//버퍼에 short형 데이터를 직렬화 삽입
	bool PutShort(short s)
	{
		if ((m_iPos + sizeof(short)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &s, sizeof(short));
		m_iPos += sizeof(short);
		return true;
	}

	//버퍼에 int형 데이터를 직렬화 삽입, 
	bool PutInt(int i)
	{
		if ((m_iPos + sizeof(int)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &i, sizeof(int));
		m_iPos += sizeof(int);
		return true;
	}

	//버퍼에 float형 데이터를 직렬화 삽입, 
	bool PutFloat(float f)
	{
		if ((m_iPos + sizeof(float)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &f, sizeof(float));
		m_iPos += sizeof(float);
		return true;
	}

	//버퍼에 double형 데이터를 직렬화 삽입, 
	bool PutDouble(double f)
	{
		if ((m_iPos + sizeof(double)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &f, sizeof(double));
		m_iPos += sizeof(double);
		return true;
	}

	//버퍼에 int8_t형 데이터를 직렬화 삽입, 
	bool PutInt8(int8_t i)
	{
		if ((m_iPos + sizeof(int8_t)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &i, sizeof(int8_t));
		m_iPos += sizeof(int8_t);
		return true;
	}

	//버퍼에 int16_t형 데이터를 직렬화 삽입, 
	bool PutInt16(int16_t i)
	{
		if ((m_iPos + sizeof(int16_t)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &i, sizeof(int16_t));
		m_iPos += sizeof(int16_t);
		return true;
	}

	//버퍼에 int32_t형 데이터를 직렬화 삽입, 
	bool PutInt32(int32_t i)
	{
		if ((m_iPos + sizeof(int32_t)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &i, sizeof(int32_t));
		m_iPos += sizeof(int32_t);
		return true;
	}

	//버퍼에 int64_t형 데이터를 직렬화 삽입, 
	bool PutInt64(int64_t i)
	{
		if ((m_iPos + sizeof(int64_t)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &i, sizeof(int64_t));
		m_iPos += sizeof(int64_t);
		return true;
	}
};

class CWireBuffer
{
protected:
	char m_arrBuffer[WIRE_BUFFER];
	int m_iPos;

public:

	void Clear()
	{
		m_iPos = 0;
	}

	
	int GetPos() {return m_iPos;}

	char* GetArray() {return m_arrBuffer;}
	
	CWireBuffer()
	{
		m_iPos = 0;
	}

	~CWireBuffer()
	{
	}

	int Input(char *_Data, int n)
	{
		// 용량 초과면.
		if(m_iPos+n >= WIRE_BUFFER)
		{
			return 0;
		}

		//아니면,, 뒤에 추가
		memcpy(m_arrBuffer+m_iPos,_Data,n);
		m_iPos += n;

		return 1;
	}

	// 한 패킷만을 뽑아 낸다.
	int Output(int &n, char *msg)
	{

		if(m_iPos < 4)
		{
			return 0;
		}

		// 일단 ';'요걸 찾는다.
		while(m_arrBuffer[0]!=WIRE_PACKET_HEAD)
		{
			memcpy(m_arrBuffer,m_arrBuffer+1,m_iPos-1);
			--m_iPos;
			if(m_iPos < 4)
			{
				return 0;
			}
		}

		// 길이를 구한다.
		short Length = 0;
		memcpy(&Length, m_arrBuffer+1, 2);
		n = Length;

		if (Length > WIRE_BUFFER || !Length) //이렇게 큰 또는 작은 패킷은 존재할 수 없다
		{
			m_iPos = 0;
			return -1;
		}

		// 큐에 충분한 길이가 남아있지 않다면, 
		if(Length > m_iPos)
		{
			return 0;
		}

		memcpy(msg, m_arrBuffer, Length);	
		memcpy(m_arrBuffer, m_arrBuffer+Length, m_iPos-Length);
		m_iPos-=Length;

		return 1;
	}

	// 한 패킷만을 뽑아 낸다.
	int Output(int &n, char *msg, int buflen)
	{

		if(m_iPos < 4)
		{
			return 0;
		}

		// 일단 ';'요걸 찾는다.
		while(m_arrBuffer[0]!=WIRE_PACKET_HEAD)
		{
			memcpy(m_arrBuffer,m_arrBuffer+1,m_iPos-1);
			--m_iPos;
			if(m_iPos < 4)
			{
				return 0;
			}
		}

		// 길이를 구한다.
		short Length = 0;
		memcpy(&Length, m_arrBuffer+1, 2);
		n = Length;

		if (Length > WIRE_BUFFER || !Length || Length > buflen) //이렇게 큰 또는 작은 패킷은 존재할 수 없다
		{
			m_iPos = 0;

			return -1;
		}

		// 큐에 충분한 길이가 남아있지 않다면, 
		if(Length > m_iPos)
		{
			return 0;
		}

		memcpy(msg, m_arrBuffer, Length);	
		memcpy(m_arrBuffer, m_arrBuffer+Length, m_iPos-Length);
		m_iPos-=Length;

		return 1;
	}

	// 모든 패킷을 뽑아 낸다.
	int OutputAll(int &n, char *msg)
	{
		if(!m_iPos)
		{
			return 0;
		}

		memcpy(msg, m_arrBuffer, m_iPos);
		n=m_iPos;

		m_iPos=0;

		return 1;
	}

	bool PutByte(char btData)
	{
		if (m_iPos >= WIRE_BUFFER)
		{
			return false;
		}

		m_arrBuffer[m_iPos++] = btData;

		return true;
	}

	void Remove(int size) //size 만큼 제거
	{
		memcpy(&m_arrBuffer[0], &m_arrBuffer[size], m_iPos - size);

		m_iPos -= size;
	}

	bool InsertPacket(char btPacketID, char* pData, int len)
	{
		if ((WIRE_BUFFER - m_iPos) < (len + 4))
		{
			return false;
		}
		m_arrBuffer[m_iPos++]=WIRE_PACKET_HEAD;
		m_arrBuffer[m_iPos++]=(char)(len+4);	// 길이 초기화.
		m_arrBuffer[m_iPos++]=0;
		m_arrBuffer[m_iPos++]=btPacketID;
		memcpy(&m_arrBuffer[m_iPos], pData, len);

		m_iPos += len;

		return true;
	}

	bool InsertPacket(char btPacketID)
	{
		if ((WIRE_BUFFER - m_iPos) < 4)
		{
			return false;
		}
		m_arrBuffer[m_iPos++]=WIRE_PACKET_HEAD;
		m_arrBuffer[m_iPos++]=(char)4;	// 길이 초기화.
		m_arrBuffer[m_iPos++]=0;
		m_arrBuffer[m_iPos++]=btPacketID;

		return true;
	}

	bool InsertPacket(char btPacketID, char btData)
	{
		if ((WIRE_BUFFER - m_iPos) < 5)
		{
			return false;
		}
		m_arrBuffer[m_iPos++]=WIRE_PACKET_HEAD;
		m_arrBuffer[m_iPos++]=(char)5;	// 길이 초기화.
		m_arrBuffer[m_iPos++]=0;
		m_arrBuffer[m_iPos++]=btPacketID;
		m_arrBuffer[m_iPos++]=btData;

		return true;
	}

	bool InsertPacket(char btPacketID, float fData)
	{
		if ((WIRE_BUFFER - m_iPos) < (4 + sizeof(float)))
		{
			return false;
		}
		m_arrBuffer[m_iPos++]=WIRE_PACKET_HEAD;
		m_arrBuffer[m_iPos++]=(char)(4 + sizeof(float));	// 길이 초기화.
		m_arrBuffer[m_iPos++]=0;
		m_arrBuffer[m_iPos++]=btPacketID;

		memcpy(&m_arrBuffer[m_iPos], &fData, sizeof(float));

		m_iPos += sizeof(float);

		return true;
	}
};
#endif