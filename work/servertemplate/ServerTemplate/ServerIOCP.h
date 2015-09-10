#pragma once
#include "ConnectedUser.h"
#include "ConnectedUserManager.h"

class CServerIOCP : public CIocp
{
public:
	CServerIOCP(VOID);
	virtual ~CServerIOCP(VOID);

private:
	CNetworkSession mListenSession;
	CConnectedUserManager mConnectedUserManager;

	HANDLE mKeepAliveThreadHandle;
	HANDLE mKeepAliveThreadDestroyEvent;

public:
	BOOL Begin(VOID);
	BOOL End(VOID);
	VOID KeepAliveThreadCallback(VOID);

protected:
	virtual VOID OnIoConnected(VOID *object);
	virtual VOID OnIoDisconnected(VOID *object);
	virtual VOID OnIoRead(VOID *object, DWORD dataLength);
	virtual VOID OnIoWrote(VOID *object, DWORD dataLength);

private:
	VOID onPT_OVERLAP_USER_ID(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_CREATE_USER_ID(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_LOGIN(CConnectedUser *connectedUser, BYTE *packet);
	VOID onPT_LOGOUT(CConnectedUser *connectedUser, BYTE *packet);
};



//UNICODE -> UTF-8
//WideCharToMultiByte(CP_UTF8, 0, unicode /* WCHAR* 버퍼*/, -1, utf8 /* char* 버퍼*/, sizeof(utf8), NULL, NULL);
//
//UNICODE <- UTF-8
//MultiByteToWideChar(CP_UTF8, 0, utf8 /* char* 버퍼*/, -1, unicode /* WCHAR* 버퍼*/, strlen(utf8) * 2);
