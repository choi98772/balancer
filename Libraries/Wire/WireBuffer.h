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
	//pBuf : ����ȭ �۾��� ����� ����
	//buflen : �� ������ ����
	//bRecvPacket : ���ŵ� ��Ŷ�ΰ�� true, ��Ŷ�۽��� ���� ����ȭ �۾��ΰ�� false
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

	//���� ��ġ�� ���� �����͸� ����
	char* GetCurrent()
	{
		return &m_pBuffer[m_iPos];
	}

	//������������ ������ġ�� ����
	void Rewind()
	{
		if (m_bRecv)
			m_iPos = 4;
		else
			m_iPos = 0;
	}

	//���� �������� len����Ʈ ���� ��ŭ ������Ŵ
	void MovePoint(int len)
	{
		m_iPos += len;
		if (m_iPos >= m_iBufsize)
			m_iPos = m_iBufsize;
	}

	//���� ���� �������� ����
	int GetCurPos()
	{
		return m_iPos;
	}

	//���۸� ����
	char* GetBuffer()
	{
		return m_pBuffer;
	}

	//������Ŷ�� ID�� ����
	unsigned char GetPacketID()
	{
		return m_pBuffer[3];
	}

	//float�� �� ����
	float GetFloat()
	{
		if (m_iPos >= m_iBufsize)
			return 0.0f;

		float f = *((float*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(float);

		return f;
	}

	//double �� ����
	double GetDouble()
	{
		if (m_iPos >= m_iBufsize)
			return 0.0;

		double f = *((double*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(double);

		return f;
	}

	//int�� ����, ���� : �÷����� ��Ű���Ŀ� ���� int�� ����Ʈ ���̰� �޶��� �� �����Ƿ�, int8_t, int16_t��� ���� ��Ȯ�� ���̸� ���� ������ ������ ����ϱ⸦ ������
	int GetInt()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		int i = *((int*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(int);

		return i;
	}

	//unsigned int�� ����, ���� : �÷����� ��Ű���Ŀ� ���� int�� ����Ʈ ���̰� �޶��� �� �����Ƿ�, int8_t, int16_t��� ���� ��Ȯ�� ���̸� ���� ������ ������ ����ϱ⸦ ������
	unsigned int GetUInt()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		unsigned int i = *((unsigned int*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(unsigned int);

		return i;
	}

	//�� ����Ʈ�� ����
	char GetChar()
	{
		if (m_iPos >= m_iBufsize)
			return 0;

		char c = *((char*)&m_pBuffer[m_iPos]);
		m_iPos += sizeof(char);

		return c;
	}

	//�� ����Ʈ�� ����
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

	//���ۿ� char�� �����͸� ����ȭ ����
	bool PutChar(char c)
	{
		if ((m_iPos + 1) > m_iBufsize)
			return false;

		m_pBuffer[m_iPos++] = c;

		return true;
	}

	//���ۿ� short�� �����͸� ����ȭ ����
	bool PutShort(short s)
	{
		if ((m_iPos + sizeof(short)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &s, sizeof(short));
		m_iPos += sizeof(short);
		return true;
	}

	//���ۿ� int�� �����͸� ����ȭ ����, 
	bool PutInt(int i)
	{
		if ((m_iPos + sizeof(int)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &i, sizeof(int));
		m_iPos += sizeof(int);
		return true;
	}

	//���ۿ� float�� �����͸� ����ȭ ����, 
	bool PutFloat(float f)
	{
		if ((m_iPos + sizeof(float)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &f, sizeof(float));
		m_iPos += sizeof(float);
		return true;
	}

	//���ۿ� double�� �����͸� ����ȭ ����, 
	bool PutDouble(double f)
	{
		if ((m_iPos + sizeof(double)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &f, sizeof(double));
		m_iPos += sizeof(double);
		return true;
	}

	//���ۿ� int8_t�� �����͸� ����ȭ ����, 
	bool PutInt8(int8_t i)
	{
		if ((m_iPos + sizeof(int8_t)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &i, sizeof(int8_t));
		m_iPos += sizeof(int8_t);
		return true;
	}

	//���ۿ� int16_t�� �����͸� ����ȭ ����, 
	bool PutInt16(int16_t i)
	{
		if ((m_iPos + sizeof(int16_t)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &i, sizeof(int16_t));
		m_iPos += sizeof(int16_t);
		return true;
	}

	//���ۿ� int32_t�� �����͸� ����ȭ ����, 
	bool PutInt32(int32_t i)
	{
		if ((m_iPos + sizeof(int32_t)) > m_iBufsize)
			return false;

		memcpy(&m_pBuffer[m_iPos], &i, sizeof(int32_t));
		m_iPos += sizeof(int32_t);
		return true;
	}

	//���ۿ� int64_t�� �����͸� ����ȭ ����, 
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
		// �뷮 �ʰ���.
		if(m_iPos+n >= WIRE_BUFFER)
		{
			return 0;
		}

		//�ƴϸ�,, �ڿ� �߰�
		memcpy(m_arrBuffer+m_iPos,_Data,n);
		m_iPos += n;

		return 1;
	}

	// �� ��Ŷ���� �̾� ����.
	int Output(int &n, char *msg)
	{

		if(m_iPos < 4)
		{
			return 0;
		}

		// �ϴ� ';'��� ã�´�.
		while(m_arrBuffer[0]!=WIRE_PACKET_HEAD)
		{
			memcpy(m_arrBuffer,m_arrBuffer+1,m_iPos-1);
			--m_iPos;
			if(m_iPos < 4)
			{
				return 0;
			}
		}

		// ���̸� ���Ѵ�.
		short Length = 0;
		memcpy(&Length, m_arrBuffer+1, 2);
		n = Length;

		if (Length > WIRE_BUFFER || !Length) //�̷��� ū �Ǵ� ���� ��Ŷ�� ������ �� ����
		{
			m_iPos = 0;
			return -1;
		}

		// ť�� ����� ���̰� �������� �ʴٸ�, 
		if(Length > m_iPos)
		{
			return 0;
		}

		memcpy(msg, m_arrBuffer, Length);	
		memcpy(m_arrBuffer, m_arrBuffer+Length, m_iPos-Length);
		m_iPos-=Length;

		return 1;
	}

	// �� ��Ŷ���� �̾� ����.
	int Output(int &n, char *msg, int buflen)
	{

		if(m_iPos < 4)
		{
			return 0;
		}

		// �ϴ� ';'��� ã�´�.
		while(m_arrBuffer[0]!=WIRE_PACKET_HEAD)
		{
			memcpy(m_arrBuffer,m_arrBuffer+1,m_iPos-1);
			--m_iPos;
			if(m_iPos < 4)
			{
				return 0;
			}
		}

		// ���̸� ���Ѵ�.
		short Length = 0;
		memcpy(&Length, m_arrBuffer+1, 2);
		n = Length;

		if (Length > WIRE_BUFFER || !Length || Length > buflen) //�̷��� ū �Ǵ� ���� ��Ŷ�� ������ �� ����
		{
			m_iPos = 0;

			return -1;
		}

		// ť�� ����� ���̰� �������� �ʴٸ�, 
		if(Length > m_iPos)
		{
			return 0;
		}

		memcpy(msg, m_arrBuffer, Length);	
		memcpy(m_arrBuffer, m_arrBuffer+Length, m_iPos-Length);
		m_iPos-=Length;

		return 1;
	}

	// ��� ��Ŷ�� �̾� ����.
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

	void Remove(int size) //size ��ŭ ����
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
		m_arrBuffer[m_iPos++]=(char)(len+4);	// ���� �ʱ�ȭ.
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
		m_arrBuffer[m_iPos++]=(char)4;	// ���� �ʱ�ȭ.
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
		m_arrBuffer[m_iPos++]=(char)5;	// ���� �ʱ�ȭ.
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
		m_arrBuffer[m_iPos++]=(char)(4 + sizeof(float));	// ���� �ʱ�ȭ.
		m_arrBuffer[m_iPos++]=0;
		m_arrBuffer[m_iPos++]=btPacketID;

		memcpy(&m_arrBuffer[m_iPos], &fData, sizeof(float));

		m_iPos += sizeof(float);

		return true;
	}
};
#endif