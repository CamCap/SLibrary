#pragma once
#include <map>
#include <vector>
#include "CriticalSection.h"

template <class _Ty>
class VecContainer
{
public:
	VecContainer(int size);
	virtual ~VecContainer();

	void push(_Ty *pElement);
	_Ty* pop();

protected:

protected:
	int		m_count; //size

	std::vector<_Ty *> m_pVec;

	SCriticalSection m_cs;
};


//------------------------------------------------------------------------------
//
template <class _Ty>
VecContainer<_Ty>::VecContainer(int size)
{
	m_count = size;

	_Ty *p;
	for (int i = 0; i < size; ++i)
	{
		p = new type;
		m_pVec.push_back(p);
	}
}

//------------------------------------------------------------------------------
//
template <class _Ty>
VecContainer<_Ty>::~VecContainer()
{
	_Ty *p;
	while (m_pVec.size() > 0)
	{
		p = m_pVec.front();
		SAFE_DELETE(p);

		m_pVec.pop_front();
	}
}


//------------------------------------------------------------------------------
//
template <class _Ty>
void VecContainer<_Ty>::push(_Ty *pElement)
{
	if (!pElement)
		return;

	CSLOCK(m_cs)
	{
		m_pVec.push_back(pElement);
	}
}


//------------------------------------------------------------------------------
// push 보다 pop 가 먼저 일어남
template <class _Ty>
_Ty* VecContainer<_Ty>::pop()
{
	_Ty *pElement = NULL;

	CSLOCK(m_cs)
	{
		if (m_pVec.size() > 0)
		{
			pElement = m_pVec.front();
			if (pElement != NULL)
				m_pVec.pop_front();
			else
				pElement = new type;
		}
		else
		{
#ifdef _DEBUG
			OutputDebugString("[ERROR]다써서 다시 할당한다.[CArrayListContainer]\n");
#endif
			pElement = new type;
		}
	}

	return pElement;
}


template <class _Ty>
class MapContainer
{
public:
	MapContainer(int size);
	virtual ~MapContainer();

	void push(_Ty *pElement);
	_Ty* pop();

protected:

protected:
	int		m_count; //size

	std::map<int, _Ty *> m_pMap;

	SCriticalSection m_cs;
};


//------------------------------------------------------------------------------
//
template <class _Ty>
MapContainer<_Ty>::MapContainer(int size)
{
	m_count = size;

	_Ty *p;
	for (int i = 0; i < size; ++i)
	{
		p = new type;
		m_pMap.insert(p);
	}
}

//------------------------------------------------------------------------------
//
template <class _Ty>
MapContainer<_Ty>::~MapContainer()
{
	_Ty *p;
	while (m_pVec.size() > 0)
	{
		p = m_pMap.begin();
		SAFE_DELETE(p);

		m_pMap.erase(p);
	}
}


//------------------------------------------------------------------------------
//
template <class _Ty>
void MapContainer<_Ty>::push(_Ty *pElement)
{
	if (!pElement)
		return;

	CSLOCK(m_cs)
	{
		m_pMap.insert(pElement);
	}
}


//------------------------------------------------------------------------------
// push 보다 pop 가 먼저 일어남
template <class _Ty>
_Ty* MapContainer<_Ty>::pop()
{
	_Ty *pElement = NULL;

	CSLOCK(m_cs)
	{
		if (m_pVec.size() > 0)
		{
			pElement = m_pMap.begin();

			if (pElement != NULL)
				m_pMap.erase(pElement);
			else
				pElement = new type;
		}
		else
		{
#ifdef _DEBUG
			OutputDebugString("[ERROR]다써서 다시 할당한다.[CArrayListContainer]\n");
#endif
			pElement = new type;
		}
	}

	return pElement;
}

