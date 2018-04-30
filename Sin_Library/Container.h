#pragma once
#include <functional>
#include <map>
#include <vector>
#include <list>
#include "CriticalSection.h"
#include <algorithm>

template <class _Ty>
class Container
{
public:
	virtual void push(_Ty*) = 0;
	virtual _Ty* pop() = 0;
	virtual void Erase(_Ty*) = 0;

};

template <class _Ty>
class VecContainer
	:public Container<_Ty>
{
public:
	VecContainer(int size);
	virtual ~VecContainer();

	virtual void push(_Ty* pElement);
	virtual _Ty* pop();
	virtual void Erase(_Ty* pElement);

	template <typename find_function>
	constexpr _Ty* find(find_function ty);

	template <typename process_function, typename... Args>
	void process(process_function pf, Args... arg);

	int size() { return m_pVec.size(); }

//	template <typename process_function>
//	void process(process_function pf);

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
		p = new _Ty;
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
		p = m_pVec.back();
		SAFE_DELETE(p);

		m_pVec.pop_back();
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
			pElement = m_pVec.back();
			if (pElement != NULL)
				m_pVec.pop_back();
			else
				pElement = new _Ty;
		}
		else
		{
			pElement = new _Ty;
		}
	}

	return pElement;
}

////////////////////////////////////////////////////////////////////

template <class _Ty>
void VecContainer<_Ty>::Erase(_Ty* pElement)
{
	CSLOCK(this->m_cs) {
		typename std::vector<_Ty*>::iterator it = std::find(this->m_pVec.begin(), this->m_pVec.end(), pElement);
		this->m_pVec.erase(it);
	}
}

////////////////////////////////////////////////////////////////////

template<class _Ty>
template<typename find_function>
inline constexpr _Ty * VecContainer<_Ty>::find(find_function ty)
{
	typename std::vector<_Ty *>::iterator it;
	
	CSLOCK(this->m_cs)
	{
		it = std::find_if(this->m_pVec.begin(), this->m_pVec.end(), ty);
	}
	if (it == this->m_pVec.end()) return NULL;

	return *it;
}

////////////////////////////////////////////////////////////////////

template<class _Ty>
template<typename process_function, typename... Args>
inline void VecContainer<_Ty>::process(process_function pf, Args... arg)
{
	typename std::vector<_Ty*>::iterator it = this->m_pVec.begin();

	for (it; it != this->m_pVec.end(); it++)
	{
		pf(*it,  arg...);
	}
}


//////////////////////////////////////////////////


template <class _Ty>
class ListContainer
	:public Container<_Ty>
{
public:
	ListContainer(int size);
	virtual ~ListContainer();

	virtual void push(_Ty* pElement);
	virtual _Ty* pop();
	virtual void Erase(_Ty* pElement);

	template <typename find_function>
	constexpr _Ty* find(find_function ty);

	template <typename process_function, typename... Args>
	void process(process_function pf, Args... arg);

	//	template <typename process_function>
	//	void process(process_function pf);

protected:
	int		m_count; //size

	std::list<_Ty *> m_pList;

	SCriticalSection m_cs;
};

//------------------------------------------------------------------------------
//
template <class _Ty>
ListContainer<_Ty>::ListContainer(int size)
{
	m_count = size;

	_Ty *p;
	for (int i = 0; i < size; ++i)
	{
		p = new _Ty;
		m_pList.push_back(p);
	}
}

//------------------------------------------------------------------------------
//
template <class _Ty>
ListContainer<_Ty>::~ListContainer()
{
	_Ty *p;
	while (m_pList.size() > 0)
	{
		p = m_pList.back();
		SAFE_DELETE(p);

		m_pList.pop_back();
	}
}


//------------------------------------------------------------------------------
//
template <class _Ty>
void ListContainer<_Ty>::push(_Ty *pElement)
{
	if (!pElement)
		return;

	CSLOCK(m_cs)
	{
		m_pList.push_front(pElement);
	}
}


//------------------------------------------------------------------------------
// push 보다 pop 가 먼저 일어남
template <class _Ty>
_Ty* ListContainer<_Ty>::pop()
{
	_Ty *pElement = NULL;

	CSLOCK(m_cs)
	{
		if (this->m_pList.size() > 0)
		{
			pElement = m_pList.front();
			if (pElement != NULL)
				m_pList.pop_front();
			else
				pElement = new _Ty;
		}
		else
		{
			pElement = new _Ty;
		}
	}

	return pElement;
}

////////////////////////////////////////////////////////////////////

template <class _Ty>
void ListContainer<_Ty>::Erase(_Ty* pElement)
{
	CSLOCK(this->m_cs) {
		typename std::list<_Ty*>::iterator it = std::find(this->m_pList.begin(), this->m_pList.end(), pElement);
		this->m_pList.erase(it);
	}
}

////////////////////////////////////////////////////////////////////

template<class _Ty>
template<typename find_function>
inline constexpr _Ty * ListContainer<_Ty>::find(find_function ty)
{
	typename std::list<_Ty *>::iterator it;

	CSLOCK(this->m_cs)
	{
		it = std::find_if(this->m_pList.begin(), this->m_pList.end(), ty);
	}
	if (it == this->m_pList.end()) return NULL;

	return *it;
}

////////////////////////////////////////////////////////////////////

template<class _Ty>
template<typename process_function, typename... Args>
inline void ListContainer<_Ty>::process(process_function pf, Args... arg)
{
	typename std::list<_Ty*>::iterator it = this->m_pList.begin();

	for (it; it != this->m_pList.end(); it++)
	{
		pf(*it, arg...);
	}
}



//////////////////////////////////////////////////

//template <class _Ty>
//class MapContainer
//{
//public:
//	MapContainer(int size);
//	virtual ~MapContainer();
//
//	void push(_Ty *pElement);
//	_Ty* pop();
//
//	template <typename find_function>
//	constexpr _Ty* find(find_function ff);
//
//	template <typename process_function, typename... Args>
//	void process(process_function pf, Args... arg);
//
//protected:
//	int		m_count; //size
//
//	std::map<int, _Ty *> m_pMap;
//
//	SCriticalSection m_cs;
//};
//
//
////------------------------------------------------------------------------------
////
//template <class _Ty>
//MapContainer<_Ty>::MapContainer(int size)
//{
//	m_count = size;
//
//	_Ty *p;
//	for (int i = 0; i < size; ++i)
//	{
//		p = new _Ty;
//		this->m_pMap.insert(p);
//	}
//}
//
////------------------------------------------------------------------------------
////
//template <class _Ty>
//MapContainer<_Ty>::~MapContainer()
//{
//	_Ty *p;
//	while (this->m_pMap.size() > 0)
//	{
//		p = (this->m_pMap.begin())->second;
//		SAFE_DELETE(p);
//
//		this->m_pMap.erase(p);
//	}
//}
//
//
////------------------------------------------------------------------------------
////
//template <class _Ty>
//void MapContainer<_Ty>::push(_Ty *pElement)
//{
//	if (!pElement)
//		return;
//
//	CSLOCK(m_cs)
//	{
//		m_pMap.insert(m_pMap.size()+1, pElement);
//	}
//}
//
//
////------------------------------------------------------------------------------
//// push 보다 pop 가 먼저 일어남
//template <class _Ty>
//_Ty* MapContainer<_Ty>::pop()
//{
//	_Ty *pElement = NULL;
//
//	CSLOCK(m_cs)
//	{
//		if (m_pVec.size() > 0)
//		{
//			pElement = m_pMap.begin();
//
//			if (pElement != NULL)
//				m_pMap.erase(pElement);
//			else
//				pElement = new type;
//		}
//		else
//		{
//#ifdef _DEBUG
//			OutputDebugString("[ERROR]다써서 다시 할당한다.[CArrayListContainer]\n");
//#endif
//			pElement = new type;
//		}
//	}
//
//	return pElement;
//}
//
///////////////////////////////////
// 
//template<class _Ty>
//template<typename find_function>
//inline constexpr _Ty * MapContainer<_Ty>::find(find_function ff)
//{
//	typename std::map<int, _Ty*>::iterator it;
//
//	CSLOCK(this->m_cs)
//	{
//		it = std::find_if(m_pMap.begin(), m_pMap.end(), ff);
//
//		if (it == this->m_pMap.end()) return NULL;
//	}
//
//	return (it->second);
//}
//
//template<class _Ty>
//template<typename process_function, typename ...Args>
//inline void MapContainer<_Ty>::process(process_function pf, Args ...arg)
//{
//	typename std::map<int, _Ty*>::iterator it = this->m_pMap.begin();
//
//	for (it; it != this->m_pMap.end(); it++)
//	{
//		//f = std::bind(&pf, *it, arg...);
//		pf(arg...);
//	}
//}
