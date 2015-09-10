#include "stdafx.h"
#include "ConnectedUserManager.h"


CConnectedUserManager::CConnectedUserManager(VOID)
{
	mMaxUserCount = 0;
	mCurrentUserCount = 0;
}


CConnectedUserManager::~CConnectedUserManager(VOID)
{
}

BOOL CConnectedUserManager::Begin(DWORD maxUser, SOCKET listenSocket)
{
	CThreadSync Sync;

	if (maxUser <= 0 || !listenSocket)
		return FALSE;

	mMaxUserCount = maxUser;
	mCurrentUserCount = 0;
	mListenSocket = listenSocket;

	for (DWORD i = 0; i < maxUser; i++)
	{
		CConnectedUser *ConnectedUser = new CConnectedUser();

		if (ConnectedUser->Begin())
		{
			mConnectedUserVector.push_back(ConnectedUser);
		}
		else
		{
			CConnectedUserManager::End();

			return FALSE;
		}
	}

	return TRUE;
}

BOOL CConnectedUserManager::End(VOID)
{
	CThreadSync Sync;

	for (DWORD i = 0; i < mConnectedUserVector.size(); i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		ConnectedUser->End();

		delete ConnectedUser;
	}

	mConnectedUserVector.clear();

	return TRUE;
}

BOOL CConnectedUserManager::AcceptAll(VOID)
{
	CThreadSync Sync;

	for (DWORD i = 0; i < mConnectedUserVector.size(); i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		if (!ConnectedUser->Accept(mListenSocket))
		{
			CConnectedUserManager::End();

			return FALSE;
		}
	}

	return TRUE;
}

BOOL CConnectedUserManager::WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength)
{
	CThreadSync Sync;

	for (DWORD i = 0; i < mConnectedUserVector.size(); i++)
	{
		CConnectedUser *ConnectedUser = mConnectedUserVector[i];

		if (ConnectedUser->GetIsConnected())
		{
			if (!ConnectedUser->WritePacket(protocol, packet, packetLength))
			{
				ConnectedUser->Reload(mListenSocket);
			}
		}
	}

	return TRUE;
}

BOOL CConnectedUserManager::IsAlreadyLogined(CHAR* userID)
{
	CThreadSync Sync;

	if (!userID)
		return FALSE;

	for (DWORD i = 0; i < mLoginedUserVector.size(); i++)
	{
		CConnectedUser *ConnectedUser = mLoginedUserVector[i];

		if (!strcmp(userID, ConnectedUser->GetUserID()))
			return TRUE;
	}

	return FALSE;
}

VOID CConnectedUserManager::IsLogin(CHAR* userID, CHAR* sessionID, CHAR* ipAddress)
{
	CThreadSync Sync;

	CConnectedUser *ConnectedUser = new CConnectedUser();
	ConnectedUser->SetUserID(userID);
	ConnectedUser->SetIPAddress(ipAddress);

	mLoginedUserVector.push_back(ConnectedUser);

	return;
}

VOID CConnectedUserManager::isLogoutID(CHAR* userID)
{
	CThreadSync Sync;

	for (DWORD i = 0; i < mLoginedUserVector.size(); i++)
	{
		CConnectedUser *ConnectedUser = mLoginedUserVector[i];

		if (!strcmp(userID, ConnectedUser->GetUserID()))
		{
			mLoginedUserVector.erase(mLoginedUserVector.begin() + i);
			return;
		}
	}

	return;
}

VOID CConnectedUserManager::isLogoutIP(CHAR* ipAddress)
{
	CThreadSync Sync;

	for (DWORD i = 0; i < mLoginedUserVector.size(); i++)
	{
		CConnectedUser *ConnectedUser = mLoginedUserVector[i];

		if (!strcmp(ipAddress, ConnectedUser->GetIPAddress()))
		{
			mLoginedUserVector.erase(mLoginedUserVector.begin() + i);
			return;
		}
	}

	return;
}