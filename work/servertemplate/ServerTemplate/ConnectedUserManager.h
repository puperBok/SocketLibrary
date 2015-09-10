#pragma once
#include "ConnectedUser.h"

class CConnectedUserManager : public CMultiThreadSync<CConnectedUserManager>
{
public:
	CConnectedUserManager(VOID);
	~CConnectedUserManager(VOID);

private:
	std::vector<CConnectedUser*> mConnectedUserVector;
	std::vector<CConnectedUser*> mLoginedUserVector;

	DWORD mMaxUserCount;
	DWORD mCurrentUserCount;

	SOCKET mListenSocket;

public:
	BOOL Begin(DWORD maxUser, SOCKET listenSocket);
	BOOL End(VOID);
	BOOL AcceptAll(VOID);
	BOOL WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength);

public:
	BOOL IsAlreadyLogined(CHAR* userID);
	VOID IsLogin(CHAR* userID, CHAR* sessionID, CHAR* ipAddress);
	VOID isLogoutID(CHAR* userID);
	VOID isLogoutIP(CHAR* ipAddress);

public:
	inline VOID IncreaseConnectedUserCount(VOID)
	{
		InterlockedIncrement((LONG*)&mCurrentUserCount);
	}
	inline VOID DecreaseConnectedUserCount(VOID)
	{
		InterlockedDecrement((LONG*)&mCurrentUserCount);
	}
};
