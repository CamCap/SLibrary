#include "stdafx.h"
#include "Packet.h"


SCircleQueue::SCircleQueue()
	:m_front(0), m_tail(0)
{
	m_data = new char[MAX_QUEUE_LENGTH];
}

SCircleQueue::~SCircleQueue()
{
	SAFE_DELETE_ARR(m_data);
}

bool SCircleQueue::Push(IN char * data, IN short size)
{
	if (size == 0)
	{
		CircleQueueStateToString(ERROR_TEMP_PACKET);
		return false;
	}
	if (size > MAX_QUEUE_LENGTH)
	{
		CircleQueueStateToString(ERROR_OVER_PACKETSIZE);
		return false;
	}

	if ((m_front + size) > MAX_QUEUE_LENGTH)
	{
		InitQueue();
	}

	memcpy(&m_data[m_front], data, size);
	m_front += size;

	return true;
}


void SCircleQueue::Pop(OUT BTZPacket * data)
{
	BTZPacket* packet = NULL;
	int packetsize = 0;
	int size = m_front - m_tail;

	packet = ((BTZPacket*)&m_data[m_front]);
	packetsize = packet->packet_size;

	if (packetsize < sizeof(BTZPacket)) return;
	if (size < packetsize) return;

	//	m_tail += packetsize;

	int pre = m_tail;
	m_tail += packetsize;

	data = packet;
}

//front가 max_queue_length를 넘으려고 할 때, 큐를 초기화
void SCircleQueue::InitQueue()
{
	int size = m_front - m_tail;
	if (size >= MAX_QUEUE_LENGTH)
	{
		m_front = m_tail = 0;
		return;
	}

	memcpy(&m_data[0], &m_data[m_front], size);

	m_tail = 0;
	m_front = size;
}


//////////////////////////////////////////////////////


//////////////////////////////////////////////////

SPacketContainer::SPacketContainer()
	: m_VecSize(BTZ_PACKET_LENGTH)
{

}

SPacketContainer::SPacketContainer(int size)
	: m_VecSize(size)
{
	BTZPacket* packet = NULL;

	for (int i = 0; i < m_VecSize; i++)
	{
		packet = new BTZPacket;
		m_VecPacket.push_back(packet);
	}
}

SPacketContainer::~SPacketContainer()
{
	BTZPacket* packet = NULL;

	for (int i = 0; i < m_VecPacket.size(); i)
	{
		packet = m_VecPacket.back();
		m_VecPacket.pop_back();
		SAFE_DELETE(packet);
	}
}

bool SPacketContainer::Push(IN BTZPacket* data)
{
	if (data == NULL) return false;

	m_VecPacket.reserve(m_VecSize);
	m_VecPacket.push_back(data);

	return true;
}

void SPacketContainer::Pop(OUT BTZPacket* data)
{
	//data = m_VecPacket.front();
	if (m_VecPacket.size() == 0) return;

	std::vector<BTZPacket*>::iterator it = m_VecPacket.begin();
	m_VecPacket.reserve(m_VecSize);
	m_VecPacket.erase(it);

	data = *it;
}
