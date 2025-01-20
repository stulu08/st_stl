#pragma once
#include "stulu/stl/platform.h"
#include "stulu/stl/array.h"
#include "stulu/stl/vector.h"
#include "stulu/networking/address_info.h"
#include "stulu/networking/SSA.h"

#include <string>

ST_STL_BEGIN
namespace Networking {
	using buffer =  ST_STL vector<char>;
	template<size_t N>
	using array = ST_STL array<char, N>;

	struct ReceiveFlags {
		static ST_INLINE ST_CONSTEXPR int32_t None = 0;
		/* process out-of-band data */
		static ST_INLINE ST_CONSTEXPR int32_t OOB = MSG_OOB;
		/* peek at incoming message */
		static ST_INLINE ST_CONSTEXPR int32_t Peek = MSG_PEEK;
		/* do not complete until packet is completely filled */
		static ST_INLINE ST_CONSTEXPR int32_t WaitAll = MSG_WAITALL;

		ST_INLINE ST_CONSTEXPR ReceiveFlags() ST_NOEXCEPT
			: m_value(None) {}
		ST_INLINE ST_CONSTEXPR ReceiveFlags(int32_t value) ST_NOEXCEPT
			: m_value(value) {}
		ST_INLINE ST_CONSTEXPR operator int32_t() const ST_NOEXCEPT {
			return m_value;
		}
		ST_INLINE ST_CONSTEXPR ReceiveFlags operator=(int32_t val) ST_NOEXCEPT {
			this->m_value = val;
			return *this;
		}
	private:
		int32_t m_value = None;
	};
	struct SendFlags {
		static ST_INLINE ST_CONSTEXPR int32_t None = 0;
		/* process out-of-band data */
		static ST_INLINE ST_CONSTEXPR int32_t OOB = MSG_OOB;
		/* send without using routing tables */
		static ST_INLINE ST_CONSTEXPR int32_t DontRoute = MSG_DONTROUTE;

		ST_INLINE ST_CONSTEXPR SendFlags() ST_NOEXCEPT
			: m_value(None) {}
		ST_INLINE ST_CONSTEXPR SendFlags(int32_t value) ST_NOEXCEPT
			: m_value(value) {}
		ST_INLINE ST_CONSTEXPR operator int32_t() const ST_NOEXCEPT {
			return m_value;
		}
		ST_INLINE ST_CONSTEXPR SendFlags operator=(int32_t val) ST_NOEXCEPT {
			this->m_value = val;
			return *this;
		}
	private:
		int32_t m_value = None;
	};

	class Socket {
	public:
		ST_CONSTEXPR Socket() ST_NOEXCEPT
			: m_socket(INVALID_SOCKET), m_address(nullptr) {}
		ST_INLINE Socket(const Address& addr) ST_NOEXCEPT
			: m_socket(INVALID_SOCKET), m_address(addr) { 
			const struct addrinfo* info = m_address.Info();
			m_socket = ::socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		}
		ST_INLINE Socket(SOCKET sock, const Address& addr) ST_NOEXCEPT
			: m_socket(sock), m_address(addr) {}

		ST_INLINE ~Socket() ST_NOEXCEPT {
			if (m_socket != INVALID_SOCKET) {
				ST_ASSERT(this->Close() == 0, "Socket could not be closed");
			}
		}

		ST_NODISCARD ST_INLINE int Bind() const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			return ::bind(m_socket, m_address.Info()->ai_addr, (int)m_address.Info()->ai_addrlen);
		}
		ST_NODISCARD ST_INLINE int Listen(int32_t queue_size = SOMAXCONN) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			return ::listen(m_socket, queue_size);
		}
		ST_NODISCARD ST_INLINE int Close() ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
#ifdef ST_WINDOWS
			int Status = closesocket(m_socket);
#else
			int status = ::close(m_socket);
#endif
			m_socket = INVALID_SOCKET;
			return Status;

		}
		ST_NODISCARD ST_INLINE int Accept(Socket* out_sock) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");

			sockaddr_in addInfo = sockaddr_in();
			SOCKET client = INVALID_SOCKET;
			socklen_t addrelen = sizeof(addInfo);

			client = ::accept(m_socket, (sockaddr*)&addInfo, &addrelen);

			if (client == INVALID_SOCKET) {
				return SOCKET_ERROR;
			}

			out_sock->m_socket = client;
			out_sock->m_address = Address(
				inet_ntoa(addInfo.sin_addr), 
				std::to_string(addInfo.sin_port).c_str(),
				m_address.Info());

			return out_sock->m_address.Status();
		}
		ST_NODISCARD ST_INLINE int Connect(const Address& addr) ST_NOEXCEPT {
			struct addrinfo* ptr = NULL;
			if (m_socket != INVALID_SOCKET)
				(void)this->Close();

			for (ptr = addr.Info(); ptr != NULL; ptr = ptr->ai_next) {
				// Create a SOCKET for connecting to server
				m_socket = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (m_socket == INVALID_SOCKET) {
					return SOCKET_ERROR;
				}
				// Connect to server.
				int result = ::connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
				if (result == SOCKET_ERROR) {
					(void)this->Close();
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
		ST_NODISCARD ST_INLINE int Connect(const string& node, const string& port) ST_NOEXCEPT {
			return Connect(Address(node, port));
		}
		// returns the amount of bytes received
		ST_NODISCARD ST_INLINE int Receive(buffer& buff, int* out_size = nullptr, ReceiveFlags flags = ReceiveFlags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::recv(m_socket, buff.data(), (int)buff.size(), (int)flags);
			if (out_size) {
				*out_size = res;
			}
			return res;

		}
		// returns the amount of bytes received
		template<size_t N>
		ST_NODISCARD ST_INLINE int Receive(array<N>& buff, int* out_size = nullptr, ReceiveFlags flags = ReceiveFlags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::recv(m_socket, buff.data(), (int)buff.size(), (int)flags);
			if (out_size) {
				*out_size = res;
			}
			return res;

		}
		// returns the amount of bytes received
		ST_NODISCARD ST_INLINE int Receive(char* buff, int bufSize, int* out_size = nullptr, ReceiveFlags flags = ReceiveFlags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::recv(m_socket, buff, bufSize, (int)flags);
			if (out_size) {
				*out_size = res;
			}
			return res;
		}
		// returns the msg as string
		ST_NODISCARD ST_INLINE string Receive(int preAllocSize = 4096, ReceiveFlags flags = ReceiveFlags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			char* buff = new char[preAllocSize];
			int size = ::recv(m_socket, buff, preAllocSize, (int)flags);
			if (size > 0) {
				string msg(buff, size);
				return msg;
			}
			else {
				delete[] buff;
				return "";
			}
		}
		// returns the amount of bytes sent
		ST_NODISCARD ST_INLINE int Send(const buffer& buff, int* bytes_sent = nullptr, SendFlags flags = SendFlags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::send(m_socket, buff.data(), (int)buff.size(), (int)flags);
			if (bytes_sent) {
				*bytes_sent = res;
			}
			return res;
		}
		// returns the amount of bytes sent
		template<size_t N>
		ST_NODISCARD ST_INLINE int Send(const array<N>& buff, int* bytes_sent = nullptr, SendFlags flags = SendFlags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::send(m_socket, buff.data(), (int)buff.size(), (int)flags);
			if (bytes_sent) {
				*bytes_sent = res;
			}
			return res;
		}
		// returns the amount of bytes sent
		ST_NODISCARD ST_INLINE int Send(const char* buff, int buffSize, int* bytes_sent = nullptr, SendFlags flags = SendFlags::None) const ST_NOEXCEPT {
			ST_ASSERT(m_socket != INVALID_SOCKET, "Socket is invalid or empty");
			int res = ::send(m_socket, buff, buffSize, (int)flags);
			if (bytes_sent) {
				*bytes_sent = res;
			}
			return res;
		}
		// returns the amount of bytes sent
		ST_NODISCARD ST_INLINE int Send(const string msg, int* bytes_sent = nullptr, SendFlags flags = SendFlags::None) const ST_NOEXCEPT {
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
		ST_NODISCARD ST_INLINE const Address& GetAddress() const ST_NOEXCEPT {
			return m_address;
		}

	private:
		SOCKET m_socket;
		Address m_address;
	};
}
ST_STL_END