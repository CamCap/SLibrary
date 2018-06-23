#pragma once

#include <WinSock2.h>
#include "header.h"
#include <Windows.h>

class SCriticalSection
    :public CRITICAL_SECTION
{
public:
    SCriticalSection()
    {
        InitializeCriticalSection(this);    
    }
    ~SCriticalSection()
    {
        DeleteCriticalSection(this);
    }
};
class SCriticalSectionLock
{
public:
    SCriticalSectionLock(SCriticalSection& cs)
        :m_pcs(&cs)
    {
        EnterCriticalSection(m_pcs);
    }
    ~SCriticalSectionLock()
    {
        if(m_pcs != NULL)
            LeaveCriticalSection(m_pcs);
    }

    explicit operator bool() { return true;}
private:
    SCriticalSectionLock() {}

    SCriticalSection* m_pcs;    
};
#define CSLOCK( cs_ )   if( SCriticalSectionLock STRING_CAT( lock_, __LINE__ ) = cs_ )
