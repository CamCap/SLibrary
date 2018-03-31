#pragma once
#include "ssocket.h"
#include "SPeerContainer.h"
//������ ������ ������ ��� ������..
//�ϴ� ��ȣ��Ŷ(���ڵ�)�� ������ ������ �ϱ⺸�ٴ�, �̹� ����� ���������� ������
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
	VEC_CONTANINER m_vecRogueServer; // ������ ���� ���

	DWORD m_tickPing;

	static SServerContainer* m_instance;
};

