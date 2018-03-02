
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

	//가변 매크로 참고
};

#pragma pack(pop)

////////////////////////////////////////////////////////////


#define MAX_QUEUE_LENGTH 1024


//이차원배열이 되지않도록 조심하자!
//데이터만 직렬화가능. 파일은 부스트의 도움을 받아서 확장해야함

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

