#pragma once

#include <assert.h>

template <typename T>
class SSingleton
{
protected:
	SSingleton()
	{
		assert(!ms_singleton);
		long long offset = (long long)(T *)1 - (long long)(SSingleton<T> *)(T *)1;
		ms_singleton = (T *)((long long)this + offset);
	}
	~SSingleton()
	{
		assert(ms_singleton);
		ms_singleton = 0;
	}

public:
	static T * GetInstance()
	{
		if (ms_singleton == NULL)
			ms_singleton = new T;
		return ms_singleton;
	};
	static void destroyInstance()
	{
		if (ms_singleton) {
			delete ms_singleton;
			ms_singleton = NULL;
		}
	};

private:
	static T * ms_singleton;
};

