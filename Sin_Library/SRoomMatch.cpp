#include "stdafx.h"
#include "SRoomMatch.h"
#include "Shlwapi.h"


void Room::CreateID()
{
	CoCreateGuid(&id);
}

GUID & Room::GetID()
{
	return id;
}

void Room::RoomSend(BTZPacket * pack)
{
	for (int i = 0; i < m_matchnum ; i++)
	{
		Users[i]->Send(pack);
	}
}

int Room::Size()
{
	int num = 0;

	auto it = Users.begin();

	for (it; it != Users.end(); it++)
	{
		if (*it == NULL) break;
		num++;
	}

	return num;
}

void Room::EnterRoom(SPeer & user, int m_matchnum)
{
	auto it = Users.begin();

	this->m_matchnum = m_matchnum;

	int num = 0;

	while (it != Users.end() && *it == NULL && num++ < m_matchnum)
	{
		if (*it != NULL) *it = &user;

		it++;
	}
}

void Room::ReleaseRoom()
{
	for (int i = 0; i < MAX_MATCH_USER; i++)
		Users[i] = NULL;

	ZeroMemory(&id, sizeof(id));
}

//////////////////////////////////////////////////////////


SMatchingManager::SMatchingManager()
{
}


SMatchingManager::~SMatchingManager()
{
	m_threadHandle.detach();
}

void SMatchingManager::SetMatchNum(int num)
{ 
	if (m_match != NULL) m_match->SetMatchNum(num);
}

void SMatchingManager::CreateMatchingManager(MatchBehavior * type)
{
	m_match = type;

	m_threadHandle = std::thread([&]() { MatchingLoop(NULL); });
}

Room* SMatchingManager::MatchingRequest(SPeer& user)
{
	if (m_match == NULL) return NULL;

	return m_match->MatchingRequest(user);
}

Room* SMatchingManager::MatchingRequest(SPeer& user, GUID& id, int& matchnum)
{
	if (m_match == NULL) return NULL;

	return m_match->MatchingRequest(user, id, matchnum);
}

void SMatchingManager::MatchingCompelete(Room* room)
{
	m_match->MatchComplete(room);
}

void SMatchingManager::MatchingCancel(Room * room)
{
	//room->ReleaseRoom();
	m_match->ReleaseRoom(room);
}

///////////////////////////////////////////////

DWORD SMatchingManager::MatchingLoop(LPVOID pArg)
{
	Room* room = NULL;

	while (1)
	{
		if ((room = m_match->IsMatching()) != NULL)
		{
			m_match->MatchBehavir(room);
		}
	}

	ExitThread(0);

	return 0;
}


////////////////////////////////////////////



MatchBehavior::MatchBehavior()
{
	m_event = CreateEvent(NULL, TRUE, FALSE, NULL);
}

MatchBehavior::~MatchBehavior()
{
	CloseHandle(m_event); m_event = NULL;
}

void MatchBehavior::ReleaseRoom(Room * room)
{
	room->ReleaseRoom();

	this->m_roomContainer.PopActivePeer(room);
	this->m_roomContainer.PushWaitPeer(room);
}

void MatchBehavior::WaitEvent()
{
	if (m_event != NULL)
		WaitForSingleObject(m_event, 100); // 1/1000초만큼 대기
}

void MatchBehavior::SetEvent()
{
	if (m_event != NULL)
		::SetEvent(m_event); // signal 상태로 변경
}

void MatchBehavior::ResetEvent()
{
	if (m_event != NULL)
		::ResetEvent(m_event); // non-signal 상태로 변경
}


////////////////////////////////////////////////////////

Room* MatchRequestBehavior::MatchingRequest(SPeer & user)
{
	this->m_waitUsers.push(&user);

	return NULL;
}

Room * MatchRequestBehavior::IsMatching()
{
	while (1)
	{
		if (m_waitUsers.size() > m_matchnum)
			break;
		else
		{
			ResetEvent();
			WaitEvent();
		}
	}

	Room* room = m_roomContainer.PopWaitPeer();

	return room;
}


void MatchRequestBehavior::MatchBehavir(Room* room)
{
	for (int i = 0; i < m_matchnum; i++)
		room->EnterRoom(*m_waitUsers.pop(), m_matchnum);

	room->CreateID();

	MATCHING_PACKET packet;
	packet.packet_id = PACKET_ID_MATCHING_REQ;
	packet.packet_size = sizeof(packet);
	memcpy(&packet.room_id, &room->GetID(), sizeof(packet.room_id));

	room->RoomSend(&packet);

	m_roomContainer.PushActive(room);
}

void MatchRequestBehavior::ReleaseRoom(Room * _room)
{
	if (!m_roomContainer.Find([&](Room* room) {
		return room == _room;
	}))
	{
		return;
	}

	for (int i = 0; i < _room->m_matchnum; i++)
		m_waitUsers.push(_room->Users[i]);

	__super::ReleaseRoom(_room);
}

void MatchRequestBehavior::MatchComplete(Room* room)
{
	__super::ReleaseRoom(room);
}

///////////////////////////////////////////////////////////////////////////////////////////////


Room * MatchCompleteBehavior::IsMatching()
{
	Room* room = NULL;
	while (1)
	{
		//if (m_roomContainer.ActivePeerNum() > matchnum)
		room = m_roomContainer.Find([&](Room* room) {
			return room->Size() == room->m_matchnum;
		});

		if(room != NULL)
			break;
		else
		{
			ResetEvent();
			WaitEvent();
		}
	}

	return room;
}

Room* MatchCompleteBehavior::MatchingRequest(SPeer* user)
{
	return NULL;
}

void MatchCompleteBehavior::MatchBehavir(Room * room)
{
	MATCHING_COMPLETE packet;
	packet.packet_id = PACKET_ID_MATCHING_COMPLETE;
	packet.packet_size = sizeof(packet);
	packet.room_id = room->id;
	packet.IsComplete = true;

	room->RoomSend(&packet);
}

void MatchCompleteBehavior::ReleaseRoom(Room * _room)
{
	__super::ReleaseRoom(_room);
}

void MatchCompleteBehavior::MatchComplete(Room* room)
{
	__super::ReleaseRoom(room);
}

Room * MatchCompleteBehavior::MatchingRequest(SPeer& user, GUID & id, int matchnum)
{
	Room* room = m_roomContainer.Find([&](Room* room) {
		return room->GetID() == id;
	});

	if (room == NULL) {
		room = m_roomContainer.PopWaitPeer();
		m_roomContainer.PushActive(room);
	}
	
	room->EnterRoom(user, matchnum);

	return room;
}
