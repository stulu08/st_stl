#pragma once
#include "stulu/core/platform.h"
#ifdef ST_WINDOWS
	#undef UNICODE
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
	#include <netdb.h>
	typedef int SOCKET;
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1

	#define WSADESCRIPTION_LEN      256
	#define WSASYS_STATUS_LEN       128
	struct WSAData {
		unsigned short          wVersion;
		unsigned short          wHighVersion;
		unsigned short          iMaxSockets;
		unsigned short          iMaxUdpDg;
		char* lpVendorInfo;
		char                    szDescription[WSADESCRIPTION_LEN + 1];
		char                    szSystemStatus[WSASYS_STATUS_LEN + 1];
	};
#endif

#include "stulu/stl/memory.h"
#include "stulu/stl/cstdint.h"

#define MAKE_SSA_VERSION(a, b) ((uint16_t)(((uint8_t)(((uint64_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint64_t)(b)) & 0xff))) << 8))

ST_STL_BEGIN
namespace networking {

	using SSAData = WSAData;

	// Stulu Socket Application
	class SSA {
	public:
	private:
		ST_INLINE SSA() ST_NOEXCEPT
			: m_data(SSAData{}) {
			const unsigned short Version = MAKE_SSA_VERSION(2, 2);

#ifdef ST_WINDOWS
			ST_ASSERT(WSAStartup((WORD)Version, &m_data) == 0, "SSA startup failed");
#else
			s_instance->m_data.wVersion = Version;
			s_instance->m_data.wHighVersion = Version;
			s_instance->m_data.iMaxSockets = UINT16_MAX;
			s_instance->m_data.iMaxUdpDg = UINT16_MAX;
#endif
		}

	public:
		ST_INLINE ~SSA() {
#ifdef ST_WINDOWS
			ST_ASSERT(WSACleanup == 0, "SSA cleanup failed");
#endif
		}

		static ST_INLINE void Startup() ST_NOEXCEPT {
			ST_ASSERT(s_instance == nullptr, "SSA has already been started!");
			s_instance = stulu::unique_ptr<SSA>(new SSA());
		}

		static ST_INLINE int Cleanup() ST_NOEXCEPT {
			ST_ASSERT(s_instance != nullptr, "SSA has not been started!");
			s_instance.reset();
		}

		static ST_NODISCARD ST_INLINE int GetLastError() ST_NOEXCEPT {
#ifdef ST_WINDOWS
			return WSAGetLastError();
#else
			return errno;
#endif
		}

		static ST_NODISCARD ST_INLINE bool Started() ST_NOEXCEPT {
			return s_instance.get() != nullptr;
		}

	private:
		SSAData m_data;

		ST_STL_API static stulu::unique_ptr<SSA> s_instance;
	};
}
ST_STL_END
