#pragma once

#include <assert.h>

template <typename T>
class SSingleton
{
public:
	SSingleton(void)
	{
		assert(!ms_singleton);
		int offset = (int)(T*)1 - (int)(SSingleton <T>*)(T*)1;
		ms_singleton = (T*)((int)this + offset);
	}
	~SSingleton(void)
	{
		assert(ms_singleton);
		ms_singleton = 0;
	}
	static T* GetInstance(void)
	{
		assert(ms_singleton);
		return (ms_singleton);
	}
	static T* GetSingletonPtr(void)
	{
		return (ms_singleton);
	}

protected:
	static T* ms_singleton;
};


