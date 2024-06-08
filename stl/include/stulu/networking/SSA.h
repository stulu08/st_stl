#pragma once
#include "stulu/stl/cassert.h"
#include "stulu/stl/cstdint.h"
#include "stulu/stl/platform.h"
#include "stulu/stl/memory.h"

#define MAKE_SSA_VERSION(a, b) ((uint16_t)(((uint8_t)(((uint64_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint64_t)(b)) & 0xff))) << 8))

#ifdef ST_WINDOWS

#undef UNICODE

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#elif defined(ST_LINUX)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

typedef int SOCKET;
#endif

#ifndef INVALID_SOCKET
	#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
	#define SOCKET_ERROR -1
#endif

ST_STL_BEGIN
namespace networking {
#ifdef ST_WINDOWS
	ST_INLINE_VAR const size_t SSA_DESCRIPTION_LENGTH = WSADESCRIPTION_LEN;
	ST_INLINE_VAR const size_t SSA_STATUS_LENGTH = WSASYS_STATUS_LEN;
	using SSAData = WSAData;
#elif defined(ST_LINUX)
	ST_INLINE_VAR const size_t SSA_DESCRIPTION_LENGTH = 256;
	ST_INLINE_VAR const size_t SSA_STATUS_LENGTH = 128;
	struct SSAData {
		unsigned short          wVersion;
		unsigned short          wHighVersion;
		unsigned short          iMaxSockets = 0;
		unsigned short          iMaxUdpDg = 0;
		const char*				lpVendorInfo = "";
		char                    szDescription[SSA_DESCRIPTION_LENGTH + 1];
		char                    szSystemStatus[SSA_STATUS_LENGTH + 1];
}	;
#endif

	// Stulu Socket Application
	class SSA {
	public:
		ST_INLINE SSA() ST_NOEXCEPT
			: m_data(SSAData{}) {
		}

	public:
		ST_INLINE static stulu::unique_ptr<SSA> Create() {
			SSA* ssa = new SSA();
			ssa->startup();
			return std::move(stulu::unique_ptr<SSA>(ssa));
		}

		ST_INLINE ~SSA() {
			cleanup();
		}

		ST_INLINE void startup() ST_NOEXCEPT {
			const unsigned short Version = MAKE_SSA_VERSION(2, 2);
#ifdef ST_WINDOWS
			ST_ASSERT(WSAStartup((WORD)Version, &m_data) == 0, "SSA startup failed");
#else
			m_data.wVersion = Version;
			m_data.wHighVersion = Version;
			memset(m_data.szDescription, '\0', SSA_DESCRIPTION_LENGTH + 1);
			memset(m_data.szDescription, '\0', SSA_STATUS_LENGTH + 1);
			strcpy(m_data.szDescription, "WinSock 2.0 Stulu Linux Wrapper");
			strcpy(m_data.szSystemStatus, "Running");
#endif
		}

		ST_INLINE void cleanup() ST_NOEXCEPT {
#ifdef ST_WINDOWS
			ST_ASSERT(WSACleanup() == 0, "SSA cleanup failed");
#endif
		}

		static ST_INLINE int getLastError() ST_NOEXCEPT {
#ifdef ST_WINDOWS
			return WSAGetLastError();
#else
			return errno;
#endif
		}

		ST_INLINE SSAData& get() ST_NOEXCEPT {
			return m_data;
		}
	private:
		SSAData m_data;
	};
	using ssa = SSA;
}
#undef MAKE_SSA_VERSION
ST_STL_END