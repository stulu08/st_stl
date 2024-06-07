#pragma once
#include "stulu/stl/platform.h"
#include "stulu/stl/array.h"
#include "stulu/stl/vector.h"
#include "stulu/networking/address_info.h"
#include "stulu/networking/SSA.h"

#include <string>

ST_STL_BEGIN
namespace networking {
	using buffer = stulu::vector<char>;
	template<size_t N>
	using array = stulu::array<char, N>;

	struct receive_flags {
		static ST_INLINE ST_CONSTEXPR int32_t None = 0;
		/* process out-of-band data */
		static ST_INLINE ST_CONSTEXPR int32_t OOB = MSG_OOB;
		/* peek at incoming message */
		static ST_INLINE ST_CONSTEXPR int32_t Peek = MSG_PEEK;
		/* do not complete until packet is completely filled */
		static ST_INLINE ST_CONSTEXPR int32_t WaitAll = MSG_WAITALL;

		ST_INLINE ST_CONSTEXPR receive_flags() ST_NOEXCEPT
			: m_value(None) {}
		ST_INLINE ST_CONSTEXPR receive_flags(int32_t value) ST_NOEXCEPT
			: m_value(value) {}
		ST_INLINE ST_CONSTEXPR operator int32_t() const ST_NOEXCEPT {
			return m_value;
		}
		ST_INLINE ST_CONSTEXPR receive_flags operator=(int32_t val) ST_NOEXCEPT {
			this->m_value = val;
			return *this;
		}
	private:
		int32_t m_value = None;
	};
	struct send_flags {
		static ST_INLINE ST_CONSTEXPR int32_t None = 0;
		/* process out-of-band data */
		static ST_INLINE ST_CONSTEXPR int32_t OOB = MSG_OOB;
		/* send without using routing tables */
		static ST_INLINE ST_CONSTEXPR int32_t DontRoute = MSG_DONTROUTE;

		ST_INLINE ST_CONSTEXPR send_flags() ST_NOEXCEPT
			: m_value(None) {}
		ST_INLINE ST_CONSTEXPR send_flags(int32_t value) ST_NOEXCEPT
			: m_value(value) {}
		ST_INLINE ST_CONSTEXPR operator int32_t() const ST_NOEXCEPT {
			return m_value;
		}
		ST_INLINE ST_CONSTEXPR send_flags operator=(int32_t val) ST_NOEXCEPT {
			this->m_value = val;
			return *this;
		}
	private:
		int32_t m_value = None;
	};

	class socket {
	public:
		ST_CONSTEXPR socket() ST_NOEXCEPT
			: m_socket(INVALID_SOCKET), m_address(nullptr) {}
		ST_INLINE socket(const address& addr) ST_NOEXCEPT
			: m_socket(INVALID_SOCKET), m_address(addr) { 
			const struct addrinfo* info = m_address.info();
			m_socket = ::socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		}
		ST_INLINE socket(SOCKET sock, const address& addr) ST_NOEXCEPT
			: m_socket(sock), m_address(addr) {}

		ST_INLINE ~socket() ST_NOEXCEPT {
			if (m_socket != INVALID_SOCKET) {
				ST_ASSERT(close() == 0, "Socket could not be closed");
			}
		}

		ST_NODISCARD ST_INLINE int bind() const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			return ::bind(m_socket, m_address.info()->ai_addr, (int)m_address.info()->ai_addrlen);
		}
		ST_NODISCARD ST_INLINE int listen(int32_t queue_size = SOMAXCONN) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			return ::listen(m_socket, queue_size);
		}
		ST_NODISCARD ST_INLINE int close() ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
#ifdef ST_WINDOWS
			int status = closesocket(m_socket);
#else
			int status = ::close(m_socket);
#endif
			m_socket = INVALID_SOCKET;
			return status;

		}
		ST_NODISCARD ST_INLINE int accept(socket* out_sock) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");

			sockaddr_in addInfo = sockaddr_in();
			SOCKET client = INVALID_SOCKET;
			socklen_t addrelen = sizeof(addInfo);

			client = ::accept(m_socket, (sockaddr*)&addInfo, &addrelen);

			if (client == INVALID_SOCKET) {
				return SOCKET_ERROR;
			}

			out_sock->m_socket = client;
			out_sock->m_address = stulu::networking::address(
				inet_ntoa(addInfo.sin_addr), 
				std::to_string(addInfo.sin_port).c_str(),
				m_address.info());

			return out_sock->m_address.status();
		}
		ST_NODISCARD ST_INLINE int connect(const address& addr) ST_NOEXCEPT {
			struct addrinfo* ptr = NULL;
			if (m_socket != INVALID_SOCKET)
				(void)close();

			for (ptr = addr.info(); ptr != NULL; ptr = ptr->ai_next) {
				// Create a SOCKET for connecting to server
				m_socket = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (m_socket == INVALID_SOCKET) {
					return SOCKET_ERROR;
				}
				// Connect to server.
				int result = ::connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
				if (result == SOCKET_ERROR) {
					(void)close();
					continue;
				}
				m_address = addr;
				break;
			}
			if (m_socket == INVALID_SOCKET) {
				return SOCKET_ERROR;
			}
			return 0;
		}
		ST_NODISCARD ST_INLINE int connect(const string_type& node, const string_type& port) ST_NOEXCEPT {
			return connect(address(node, port));
		}
		// returns the amount of bytes received
		ST_NODISCARD ST_INLINE int receive(buffer& buff, int* out_size = nullptr, receive_flags flags = receive_flags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::recv(m_socket, buff.data(), (int)buff.size(), (int)flags);
			if (out_size) {
				*out_size = res;
			}
			return res;

		}
		// returns the amount of bytes received
		template<size_t N>
		ST_NODISCARD ST_INLINE int receive(array<N>& buff, int* out_size = nullptr, receive_flags flags = receive_flags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::recv(m_socket, buff.data(), (int)buff.size(), (int)flags);
			if (out_size) {
				*out_size = res;
			}
			return res;

		}
		// returns the amount of bytes received
		ST_NODISCARD ST_INLINE int receive(char* buff, int bufSize, int* out_size = nullptr, receive_flags flags = receive_flags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::recv(m_socket, buff, bufSize, (int)flags);
			if (out_size) {
				*out_size = res;
			}
			return res;
		}
		// returns the msg as string_type
		ST_NODISCARD ST_INLINE string_type receive(int preAllocSize = 4096, receive_flags flags = receive_flags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			char* buff = new char[preAllocSize];
			int size = ::recv(m_socket, buff, preAllocSize, (int)flags);
			string_type msg(buff, size);
			delete[] buff;
			return msg;
		}
		// returns the amount of bytes sent
		ST_NODISCARD ST_INLINE int send(const buffer& buff, int* bytes_sent = nullptr, send_flags flags = send_flags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::send(m_socket, buff.data(), (int)buff.size(), (int)flags);
			if (bytes_sent) {
				*bytes_sent = res;
			}
			return res;
		}
		// returns the amount of bytes sent
		template<size_t N>
		ST_NODISCARD ST_INLINE int send(const array<N>& buff, int* bytes_sent = nullptr, send_flags flags = send_flags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::send(m_socket, buff.data(), (int)buff.size(), (int)flags);
			if (bytes_sent) {
				*bytes_sent = res;
			}
			return res;
		}
		// returns the amount of bytes sent
		ST_NODISCARD ST_INLINE int send(const char* buff, int buffSize, int* bytes_sent = nullptr, send_flags flags = send_flags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::send(m_socket, buff, buffSize, (int)flags);
			if (bytes_sent) {
				*bytes_sent = res;
			}
			return res;
		}
		// returns the amount of bytes sent
		ST_NODISCARD ST_INLINE int send(const string_type msg, int* bytes_sent = nullptr, send_flags flags = send_flags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::send(m_socket, msg.c_str(), (int)msg.size(), (int)flags);
			if (bytes_sent) {
				*bytes_sent = res;
			}
			return res;
		}

		ST_NODISCARD ST_INLINE operator bool() const ST_NOEXCEPT {
			return m_socket != INVALID_SOCKET;
		}
		ST_NODISCARD ST_INLINE const address& get_address() const ST_NOEXCEPT {
			return m_address;
		}

	private:
		SOCKET m_socket;
		address m_address;
	};
}
ST_STL_END