#pragma once
#include "ssocket.h"
#include "SPeerContainer.h"
//인증과 비인증 서버를 어떻게 나눌까..
//일단 암호패킷(인코딩)을 보내서 인증을 하기보다는, 이미 연결된 서버인지를 따지자
class SServerContainer
	:public SPeerContainer<SServer>
{
public:
	static SServerContainer* GetInstance();

	void ServerCheckPing(DWORD tickcount);
	//bool IsCurrentServer(SServer::SERVERTYPE type);
	void AuthServer(SServer* server);

	SServer* FindServer(int id);

	void RemoveAuthServer(int key) { this->MapPopBack(key); }
	void PushServer(SServer* server) { this->VecPushBack(server); }
	void PushRogueServer(SServer* server);
	void AddAuthServer(int id, SServer* server) { this->MapPushBack(id, server); }
	SServer* PopServer() { return (SServer*)this->Pop_EmptyPeer(); }
	void PopRogueServer();

private:


public:
	SServerContainer();
	~SServerContainer();

private:
	VEC_CONTANINER m_vecRogueServer; // 비인증 서버 목록

	DWORD m_tickPing;

	static SServerContainer* m_instance;
};

