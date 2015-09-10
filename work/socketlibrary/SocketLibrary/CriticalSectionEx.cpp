#include "stdafx.h"
#include "CriticalSectionEx.h"

CCriticalSectionEx::CCriticalSectionEx()
{
	InitializeCriticalSection(&mSync);
}


CCriticalSectionEx::~CCriticalSectionEx()
{
	DeleteCriticalSection(&mSync);
}

VOID CCriticalSectionEx::Enter(VOID)
{
	EnterCriticalSection(&mSync);
}

VOID CCriticalSectionEx::Leave(VOID)
{
	LeaveCriticalSection(&mSync);
}