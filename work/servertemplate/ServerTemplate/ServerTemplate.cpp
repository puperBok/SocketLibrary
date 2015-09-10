// ServerTemplate.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ServerIOCP.h"

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);

	CServerIOCP *server = new CServerIOCP();

	server->Begin();
	server->End();

	return 0;
}
