#include "stdafx.h"
#include "ConnectedUser.h"


CConnectedUser::CConnectedUser(VOID)
{
	ZeroMemory(mUserID, sizeof(mUserID));
	ZeroMemory(mIPAddress, sizeof(mIPAddress));
	mPort = 0;
	mIsConnected = FALSE;
}

BOOL CConnectedUser::Begin(VOID)
{
	CThreadSync Sync;

	if (!CPacketSession::Begin())
		return FALSE;

	ZeroMemory(mUserID, sizeof(mUserID));

	ZeroMemory(mIPAddress, sizeof(mIPAddress));
	mPort = 0;

	mIsConnected = FALSE;

	return TRUE;
}

BOOL CConnectedUser::End(VOID)
{
	CThreadSync Sync;

	CPacketSession::End();

	return TRUE;
}

BOOL CConnectedUser::Reload(SOCKET listenSocket)
{
	CThreadSync Sync;

	if (!listenSocket)
		return TRUE;

	CConnectedUser::End();

	return (CConnectedUser::Begin() && CPacketSession::Accept(listenSocket));
}