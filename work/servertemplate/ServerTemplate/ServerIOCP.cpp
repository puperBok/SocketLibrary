#include "stdafx.h"
#include "ServerIOCP.h"

DWORD WINAPI KeepAliveCallback(VOID *parameter)
{
	CServerIOCP *Owner = (CServerIOCP*)parameter;
	Owner->KeepAliveThreadCallback();

	return 0;
}

CServerIOCP::CServerIOCP(VOID)
{
	mKeepAliveThreadHandle = NULL;
	mKeepAliveThreadDestroyEvent = NULL;
}

CServerIOCP::~CServerIOCP(VOID)
{
}

BOOL CServerIOCP::Begin(VOID)
{
	if (!CIocp::Begin())
		return FALSE;

	if (!mListenSession.Begin())
	{
		CServerIOCP::End();

		return FALSE;
	}

	if (!mListenSession.TcpBind())
	{
		CServerIOCP::End();

		return FALSE;
	}

	if (!mListenSession.Listen(DEFAULT_PORT, MAX_USER))
	{
		CServerIOCP::End();

		return FALSE;
	}

	if (!CIocp::RegisterSocketToIocp(mListenSession.GetSocket(), reinterpret_cast<ULONG_PTR>(&mListenSession)))
	{
		CServerIOCP::End();

		return FALSE;
	}

	if (!mConnectedUserManager.Begin(MAX_USER, mListenSession.GetSocket()))
	{
		CServerIOCP::End();

		return FALSE;
	}

	if (!mConnectedUserManager.AcceptAll())
	{
		CServerIOCP::End();

		return FALSE;
	}

	mKeepAliveThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!mKeepAliveThreadDestroyEvent)
	{
		CServerIOCP::End();

		return FALSE;
	}

	mKeepAliveThreadHandle = CreateThread(NULL, 0, ::KeepAliveCallback, this, 0, NULL);
	if (!mKeepAliveThreadHandle)
	{
		CServerIOCP::End();

		return FALSE;
	}

	return TRUE;
}

BOOL CServerIOCP::End(VOID)
{
	if (mKeepAliveThreadHandle && mKeepAliveThreadDestroyEvent)
	{
		SetEvent(mKeepAliveThreadDestroyEvent);

		WaitForSingleObject(mKeepAliveThreadHandle, INFINITE);

		CloseHandle(mKeepAliveThreadHandle);
		CloseHandle(mKeepAliveThreadDestroyEvent);
	}

	CIocp::End();

	mConnectedUserManager.End();

	mListenSession.End();

	return TRUE;
}


VOID CServerIOCP::KeepAliveThreadCallback(VOID)
{
	DWORD KeepAlive = 0xFFFF;

	while (TRUE)
	{
		DWORD Result = WaitForSingleObject(mKeepAliveThreadDestroyEvent, 30000);

		if (Result == WAIT_OBJECT_0)
			return;

		mConnectedUserManager.WriteAll(0x3000000, (BYTE*)&KeepAlive, sizeof(DWORD));
	}
}

VOID CServerIOCP::OnIoConnected(VOID *object)
{
	CConnectedUser *ConnectedUser = reinterpret_cast<CConnectedUser*>(object);

	TCHAR RemoteAddress[32] = { 0, };
	USHORT RemotePort = 0;

	ConnectedUser->GetRemoteAddressAfterAccept(RemoteAddress, RemotePort);

	CHAR  cRemoteAddress[32] = { 0, };
	WideCharToMultiByte(CP_ACP, 0, RemoteAddress, 32, cRemoteAddress, 32, NULL, NULL);

	ConnectedUser->SetConnectedInfo(cRemoteAddress, RemotePort);

	_tprintf(_T("Connected - %s:%d\n"), RemoteAddress, RemotePort);

	//_tprintf(_T("Connected...\n"));

	if (!CIocp::RegisterSocketToIocp(ConnectedUser->GetSocket(), reinterpret_cast<ULONG_PTR>(ConnectedUser)))
	{
		_tprintf(_T("RegisterSocketToIocp FAIL - %s:%d\n"), RemoteAddress, RemotePort);

		ConnectedUser->Reload(mListenSession.GetSocket());

		return;
	}

	if (!ConnectedUser->InitializeReadForIocp())
	{
		_tprintf(_T("InitializeReadForIocp FAIL - %s:%d\n"), RemoteAddress, RemotePort);

		ConnectedUser->Reload(mListenSession.GetSocket());

		return;
	}

	ConnectedUser->SetIsConnected(TRUE);

	mConnectedUserManager.IncreaseConnectedUserCount();
}


VOID CServerIOCP::OnIoDisconnected(VOID *object)
{
	CConnectedUser *ConnectedUser = reinterpret_cast<CConnectedUser*>(object);

	_tprintf(_T("Disconnected - %s:%d\n"), ConnectedUser->GetIPAddress(), ConnectedUser->GetPortNumber());

	mConnectedUserManager.isLogoutIP(ConnectedUser->GetIPAddress());

	if (!ConnectedUser->Reload(mListenSession.GetSocket()))
	{
		CServerIOCP::End();

		return;
	}

	ConnectedUser->SetIsConnected(FALSE);

	mConnectedUserManager.DecreaseConnectedUserCount();
}


VOID CServerIOCP::OnIoRead(VOID *object, DWORD dataLength)
{
	CConnectedUser *ConnectedUser = reinterpret_cast<CConnectedUser*>(object);

	DWORD Protocol = 0;
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	DWORD PacketLength = 0;

	if (ConnectedUser->ReadPacketForIocp(dataLength))
	{
		if (ConnectedUser->GetPacket(Protocol, Packet, PacketLength))
		{
			_tprintf(_T("Read - %s:%d - [Protocol]%d\n"), ConnectedUser->GetIPAddress(), ConnectedUser->GetPortNumber(), Protocol);

			/*if (Protocol == PT_OVERLAP_USER_ID)
				onPT_OVERLAP_USER_ID(ConnectedUser, Packet);

			else if (Protocol == PT_CREATE_USER_ID)
				onPT_CREATE_USER_ID(ConnectedUser, Packet);

			else if (Protocol == PT_LOGIN)
				onPT_LOGIN(ConnectedUser, Packet);

			else if (Protocol == PT_LOGOUT)
				onPT_LOGOUT(ConnectedUser, Packet);*/
		}
	}

	if (!ConnectedUser->InitializeReadForIocp())
	{
		if (!ConnectedUser->Reload(mListenSession.GetSocket()))
		{
			CServerIOCP::End();

			return;
		}
	}
}

VOID CServerIOCP::OnIoWrote(VOID *object, DWORD dataLength)
{
	CConnectedUser *ConnectedUser = reinterpret_cast<CConnectedUser*>(object);

	_tprintf(_T("Wrote - %s:%d \n"), ConnectedUser->GetIPAddress(), ConnectedUser->GetPortNumber());

	ConnectedUser->WriteComplete();
}