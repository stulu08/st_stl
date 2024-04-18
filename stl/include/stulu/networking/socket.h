#pragma once
#include "stulu/core/core.h"
#include "stulu/networking/address_info.h"

ST_STL_BEGIN
namespace networking {
	enum class receive_flags : int32_t {
		None = 0,
		/* process out-of-band data */
		OOB = MSG_OOB,
		/* peek at incoming message */
		Peek = MSG_PEEK,
		/* do not complete until packet is completely filled */
		WaitAll = MSG_WAITALL,
	};
	enum class send_flags : int32_t {
		None = 0,
		/* process out-of-band data */
		OOB = MSG_OOB,
		/* send without using routing tables */
		DontRoute = MSG_DONTROUTE,
	};

	class socket {
	public:
	private:
		SOCKET m_socket;
		address m_address;
	};
}
ST_STL_END