#pragma once
#include <map>
#include <vector>


template <typename T>
class Container
{
typedef std::map<int, T*> MAP_CONTANINER;
typedef std::vector<T*> VEC_CONTANINER;

public:
	T* Pop_

public:
	Container();
	virtual ~Container();

private:
	MAP_CONTANINER m_mapCon;
	VEC_CONTANINER m_vecCon;
};

