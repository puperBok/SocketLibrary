#pragma once

#include "../Library/include/stdafx.h"
#include "../Library/include/Iocp.h"
#include "../Library/include/PacketSession.h"
#include "../Library/include/Stream.h"

#include "DatabaseSession.h"
#include "Protocol.h"
#include "ProtocolStruct.h"
#include "ProtocolRead.h"
#include "ProtocolWrite.h"

#include <random>
#include <ctime>
#include <functional>
#include <comdef.h>
#include <crtdbg.h>
#include <atlconv.h>

#define MAX_USER 100
#define DEFAULT_PORT 9000