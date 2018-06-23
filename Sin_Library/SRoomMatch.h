#pragma once
#include "ssocket.h"
#include <array>
#include "header.h"
#include "SSingleton.h"
#include "SPeerContainer.h"
#include <thread>

struct Room
{
	friend class MatchRequestBehavior;
	friend class MatchCompleteBehavior;
	friend class MatchBehavior;
	friend class VecContainer<Room>;

private:
	std::array<SPeer*, MAX_MATCH_USER> Users;
	GUID id;
public:
	int m_matchnum = MAX_MATCH_USER;
	
	void CreateID();
	GUID& GetID();
	void RoomSend(BTZPacket* pack);
	int Size();
	
private:
	void EnterRoom(SPeer& user, int matchnum);
	void ReleaseRoom();
private:
	Room() {}
	Room(const Room& room) {}
};

class MatchBehavior
{
public:
	virtual	Room* IsMatching() = 0;
	virtual void MatchBehavir(Room* room) = 0;
	virtual void MatchComplete(Room* room) = 0;
	virtual void ReleaseRoom(Room* room);

	virtual Room* MatchingRequest(SPeer& user) { return NULL; }
	virtual Room* MatchingRequest(SPeer& user, GUID& id, int matchnum) { return NULL; }
	void SetMatchNum(int num) { m_matchnum = num; }

protected:
typedef SPeerContainer<Room> RoomContainer;

	MatchBehavior();
	~MatchBehavior();
	
	void WaitEvent();
	void SetEvent();
	void ResetEvent();

	RoomContainer m_roomContainer;

	int m_matchnum;

private:
	HANDLE m_event; // 이벤트 핸들
};

class MatchRequestBehavior
	:public MatchBehavior
{
public:
	MatchRequestBehavior() : m_waitUsers(10) {}
	~MatchRequestBehavior() {}

	Room* IsMatching();
	Room* MatchingRequest(SPeer & user);
	void MatchBehavir(Room* room);

	void ReleaseRoom(Room* room); 
	void MatchComplete(Room* room);

private:
	ListContainer<SPeer> m_waitUsers;
};

class MatchCompleteBehavior
	:public MatchBehavior
{
public:
	Room * IsMatching();
	Room* MatchingRequest(SPeer* user);
	void MatchBehavir(Room* room);

	void ReleaseRoom(Room* room); 
	void MatchComplete(Room* room);


	void RoomRequest(Room* _room) {
		Room* room = m_roomContainer.Find([&](Room* room) {return room->GetID() == _room->GetID(); });

		if (room == NULL) {
			room->id = _room->GetID();
			room->m_matchnum = _room->m_matchnum;
		}
	}
	virtual Room* MatchingRequest(SPeer& user, GUID& id, int matchnum);
};

//template<class Behavior>
class SMatchingManager
{
public:
	SMatchingManager();
	~SMatchingManager();

	void SetMatchNum(int num);

	DWORD WINAPI MatchingLoop(LPVOID pArg);
	void CreateMatchingManager(MatchBehavior * type);

	Room* MatchingRequest(SPeer& user);
	Room* MatchingRequest(SPeer& user, GUID& id, int& matchnum);
	void MatchingCompelete(Room* room);
	void MatchingCancel(Room* room);

private:
	MatchBehavior* m_match;

	std::thread m_threadHandle;
};

