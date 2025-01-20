#pragma once
#include "stulu/stl/cassert.h"
#include "stulu/stl/cstdint.h"
#include "stulu/stl/platform.h"
#include "stulu/stl/memory.h"

#define MAKE_SSA_VERSION(a, b) ((uint16_t)(((uint8_t)(((uint64_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint64_t)(b)) & 0xff))) << 8))

#ifdef ST_WINDOWS
	#undef UNICODE
	#pragma comment (lib, "Ws2_32.lib")
#elif defined(ST_LINUX)
	typedef int SOCKET;
#endif

#ifndef INVALID_SOCKET
	#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
	#define SOCKET_ERROR -1
#endif

ST_STL_BEGIN
namespace Networking {
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

	constexpr int Success = 0;

	template<class T>
	using Scope = ST_STL unique_ptr<T>;

	// Stulu Socket Application
	class SSA {
	private:
		ST_CONSTEXPR SSA() ST_NOEXCEPT { }

	public:
		ST_NODISCARD ST_INLINE static Scope<SSAData> Startup() {
			SSAData* data = new SSAData();
			const unsigned short Version = MAKE_SSA_VERSION(2, 2);
#ifdef ST_WINDOWS
			ST_ASSERT(WSAStartup((WORD)Version, data) == 0, "SSA startup failed");
#else
			data->wVersion = Version;
			data->wHighVersion = Version;
			memset(data->szDescription, '\0', SSA_DESCRIPTION_LENGTH + 1);
			memset(data->szDescription, '\0', SSA_STATUS_LENGTH + 1);
			strcpy(data->szDescription, "WinSock 2.0 Stulu Linux Wrapper");
			strcpy(data->szSystemStatus, "Running");
#endif
			return move(Scope<SSAData>(data));
		}

		ST_INLINE static void Cleanup() ST_NOEXCEPT {
#ifdef ST_WINDOWS
			ST_ASSERT(WSACleanup() == 0, "SSA cleanup failed");
#endif
		}

		ST_INLINE static int GetLastError() ST_NOEXCEPT {
#ifdef ST_WINDOWS
			return WSAGetLastError();
#else
			return errno;
#endif
		}
	};
}
#undef MAKE_SSA_VERSION
ST_STL_END