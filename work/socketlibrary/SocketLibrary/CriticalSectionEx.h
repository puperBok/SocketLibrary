#pragma once

class CCriticalSectionEx
{
public:
	CCriticalSectionEx();
	~CCriticalSectionEx();

private:
	CRITICAL_SECTION mSync;

public:
	VOID Enter(VOID);
	VOID Leave(VOID);
};

