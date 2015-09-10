#pragma once
#include "CriticalSectionEx.h"

template <class T>
class CMultiThreadSync
{
	friend class CThreadSync;
public:
	CMultiThreadSync(){};
	~CMultiThreadSync(){};

private:
	static CCriticalSectionEx mSync;

public:
	class CThreadSync
	{
	public:
		CThreadSync()
		{
			T::mSync.Enter();
		}
		~CThreadSync()
		{
			T::mSync.Leave();
		}
	};
};

template <class T>
CCriticalSectionEx CMultiThreadSync<T>::mSync;