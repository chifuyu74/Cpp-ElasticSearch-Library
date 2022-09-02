#pragma once

#include "Standard.h"

#if _WIN64
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#undef DELETE
#undef ERROR
#endif


#if _WIN64
// Windows
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")
#endif

// Lib
#include <cpr/cpr.h>
#include <cpprest/json.h>

// Common Types
#include "MacroCore.h"
#include "Types.h"
#include "Templates.h"
#include "HttpMethod.h"

// Custom Library
#include "ElasticClient.h"
