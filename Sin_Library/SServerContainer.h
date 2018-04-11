#pragma once
#include "ssocket.h"
#include "Container.h"
#include <functional>

#define SERVER_SIZE 10

template <class T, typename type = T::GetType()>
class SServerContainer
{
private:
	VecContainer<T> m_vecServer; // �̻�� ����
	MapContainer<T> m_mapServer; // ����� ����
};

