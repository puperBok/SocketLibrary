#pragma once

class CConnectedUser : public CPacketSession
{
public:
	CConnectedUser();
	virtual ~CConnectedUser(VOID){};

private:
	CHAR mUserID[32];

	CHAR mIPAddress[32];
	USHORT mPort;

	BOOL mIsConnected;

public:
	BOOL Begin(VOID);
	BOOL End(VOID);
	BOOL Reload(SOCKET listenSocket);

public:
	inline VOID SetConnectedInfo(CHAR* ipAddress, USHORT port)
	{
		CThreadSync Sync;
		CopyMemory(mIPAddress, ipAddress, 32);
		mPort = port;
	}

	inline VOID SetIPAddress(CHAR* ipAddress)
	{
		CThreadSync Sync;
		CopyMemory(mIPAddress, ipAddress, 32);
	}

	inline CHAR* GetIPAddress(VOID)
	{
		CThreadSync Sync;
		return mIPAddress;
	}

	inline USHORT GetPortNumber(VOID)
	{
		CThreadSync Sync;
		return mPort;
	}

	inline VOID SetIsConnected(BOOL isConnected)
	{
		CThreadSync Sync;
		mIsConnected = isConnected;
	}

	inline BOOL GetIsConnected(VOID)
	{
		CThreadSync Sync;
		return mIsConnected;
	}

public:
	inline VOID SetUserID(CHAR* userID)
	{
		CThreadSync Sync;
		CopyMemory(mUserID, userID, 32);
	}

	inline CHAR* GetUserID(VOID)
	{
		CThreadSync Sync;
		return mUserID;
	}
};