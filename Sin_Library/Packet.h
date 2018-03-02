
#ifndef _SDATA_H_
#define _SDATA_H_ 

#include "header.h"
#include <vector>

#define IN
#define OUT

///////////////
#pragma pack(push, 1)

typedef unsigned long DWORD;

struct BTZPacket
{
public:
	DWORD packet_id;
	short packet_size;

	//���� ��ũ�� ����
};

#pragma pack(pop)

////////////////////////////////////////////////////////////


#define MAX_QUEUE_LENGTH 1024


//�������迭�� �����ʵ��� ��������!
//�����͸� ����ȭ����. ������ �ν�Ʈ�� ������ �޾Ƽ� Ȯ���ؾ���

template<typename T>
class SCircleQueue
{
public:
	DECLARE_ENUM(
	CircleQueueState,

		INVALID,
		SUCCESS,
		ERROR_OVER_PACKETSIZE,
		ERROR_PARAMETER,
		ERROR_TEMP_PACKET
		);

public:
	SCircleQueue();
	~SCircleQueue();

public:
	bool Push(IN T* data, IN short size);
	void Pop(OUT T* data);

private:
	void InitQueue();

private:
	T * m_data;
	int m_front;
	int m_tail;
};


///////////////

#define MAX_PACKET_LENGTH 10

typedef class SPacketContainer
{
public:
	explicit SPacketContainer();
	explicit SPacketContainer(int size);
	~SPacketContainer();

	bool Push(IN BTZPacket* data);
	void Pop(OUT BTZPacket* data);

private:
	std::vector<BTZPacket*> m_VecPacket;
	const int m_VecSize;
}PacketCon;


#endif

