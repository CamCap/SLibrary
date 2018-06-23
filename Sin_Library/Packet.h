
#ifndef _SDATA_H_
#define _SDATA_H_ 

#include "header.h"
#include "CriticalSection.h"
#include <vector>

#define IN
#define OUT

///////////////

typedef unsigned long DWORD;

#pragma pack(push, 1)

#define PKT_BASIC_SIZE sizeof(BTZPacket)

struct BTZPacket
{
public:
	unsigned int packet_id;
	short packet_size;

	//가변 매크로 참고
};

struct ERR_PACKET
	:public BTZPacket
{
	DWORD errcode;
};

#define PACKET_ID_ERR 0x00000001

struct MOVE_SERVER_PACKET
	:public BTZPacket
{
	DWORD server;
	GUID id;
};
#define PACKET_ID_MOVE_SERVER 0x00000003


struct MOVE_SERVER_REQ_PACKET
	:public BTZPacket
{
	DWORD server;
};
#define PACKET_ID_MOVE_SERVER_REQ 0x00000004

struct MOVE_SERVER_RES_PACKET
	:public MOVE_SERVER_REQ_PACKET
{
	char server_ip[8];
	int server_port;
};

#define PACKET_ID_MOVE_SERVER_RES 0x00000005

struct MATCHING_PACKET
	:public BTZPacket
{
	GUID room_id;
	int matchnum;
};

#define PACKET_ID_MATCHING_REQ 0x00000006
#define PACKET_ID_MATCHING_RES 0x00000007

struct MATCHING_COMPLETE
	:public MATCHING_PACKET
{
	bool IsComplete;
};

#define PACKET_ID_MATCHING_COMPLETE 0x00000008

#pragma pack(pop)

///////////////

#define ERROR_ID 0x000000000
#define ERROR_CONNECT_USER 0x01111111 // 이미 접속한 ID
#define ERROR_DISCONNECT_USER 0x02222222 // 접속이 끊어진 상태
#define ERROR_SERVER_MOVE_ERROR 0x03333333

////////////////////////////////////////////////////////////


#define MAX_QUEUE_LENGTH 1024


//이차원배열이 되지않도록 조심하자!
//데이터만 직렬화가능. 파일은 부스트의 도움을 받아서 확장해야함

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
	bool Push(IN char* data, IN short size);
	BTZPacket* Pop();

private:
	void InitQueue();

private:
	char * m_data;
	int m_front;
	int m_tail;
};

#define BTZ_PACKET_LENGTH 10


class SPacketContainer
{
public:
	explicit SPacketContainer();
//	explicit SPacketContainer(int size);
	~SPacketContainer();

	bool Push(IN BTZPacket* data);

	BTZPacket* Pop();

private:
	std::vector<BTZPacket*> m_VecPacket;
	int m_VecSize;
	SCriticalSection m_cs;
};

#endif

