#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>
#include <tchar.h>
#include <assert.h>
#include <vector>
#pragma comment(lib, "ws2_32.lib" )
#pragma comment(lib, "mswsock.lib")

enum IO_TYPE
{
	IO_ACCEPT,
	IO_READ,
	IO_WRITE
};

typedef struct _OVERLAPPED_EX
{
	OVERLAPPED      Overlapped;
	IO_TYPE         IoType;
	VOID            *Object;
} OVERLAPPED_EX;

#define MAX_BUFFER_LENGTH       4096

#ifdef _SERVER_SIDE
#define MAX_QUEUE_LENGTH        50
#else
#define MAX_QUEUE_LENGTH        500
#endif

#include "Log.h"