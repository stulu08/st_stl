#pragma once
#include "stulu/core/core.h"
#include "stulu/networking/address_info.h"
#include "stulu/networking/SSA.h"

ST_STL_BEGIN
namespace Networking {
	class Address {
	public:
		ST_CONSTEXPR Address(nullptr_t) ST_NOEXCEPT
			: m_node(), m_port(), m_addr(nullptr), m_status(-1) {}

		ST_INLINE Address(const string& node, const string& port, const AddressInfo& hintInfo = AddressInfo()) ST_NOEXCEPT
			: m_node(node), m_port(port), m_addr(nullptr), m_status(-1) {
			struct addrinfo hint = addrinfo();
			hint.ai_flags = hintInfo.flags;
			hint.ai_family = (int32_t)hintInfo.family;
			hint.ai_socktype = (int32_t)hintInfo.socketType;
			hint.ai_protocol = (int32_t)hintInfo.protocol;
			create_info(hint);
		}
		ST_INLINE Address(const string& node, const string& port, const struct addrinfo* hint) ST_NOEXCEPT
			: m_node(node), m_port(port), m_addr(nullptr), m_status(-1) {
			struct addrinfo _hint = addrinfo();
			_hint.ai_flags = hint->ai_flags;
			_hint.ai_family = hint->ai_family;
			_hint.ai_socktype = hint->ai_socktype;
			_hint.ai_protocol = hint->ai_protocol;
			create_info(_hint);
		}
		ST_INLINE Address(const Address& other) ST_NOEXCEPT
			: m_addr(nullptr) {
			this->m_node = other.m_node;
			this->m_port = other.m_port;
			this->m_status = other.m_status;

			if (other.m_status == 0) {
				struct addrinfo hint = addrinfo();
				hint.ai_flags = other.Info()->ai_flags;
				hint.ai_family = other.Info()->ai_family;
				hint.ai_socktype = other.Info()->ai_socktype;
				hint.ai_protocol = other.Info()->ai_protocol;
				this->create_info(hint);
			}
		}

		ST_INLINE Address(Address&& other) ST_NOEXCEPT
			: m_node(""), m_port(""), m_addr(nullptr) {
			swap(this->m_node, other.m_node);
			swap(this->m_port, other.m_port);
			swap(this->m_addr, other.m_addr);
			swap(this->m_status, other.m_status);
		}

		ST_INLINE ~Address() {
			if (m_addr)
				freeaddrinfo(m_addr);
		}

		ST_INLINE Address& operator=(const Address& other) ST_NOEXCEPT {
			if (this == ST_STL addressof(other)) {
				return *this;
			}
			this->m_node = other.m_node;
			this->m_port = other.m_port;
			this->m_status = other.m_status;
			this->m_addr = nullptr;

			if (other.m_status == 0) {
				struct addrinfo hint = addrinfo();
				hint.ai_flags = other.Info()->ai_flags;
				hint.ai_family = other.Info()->ai_family;
				hint.ai_socktype = other.Info()->ai_socktype;
				hint.ai_protocol = other.Info()->ai_protocol;
				create_info(hint);
			}

			return *this;
		}
		ST_INLINE Address& operator=(Address&& other) ST_NOEXCEPT {
			if (this == ST_STL addressof(other)) {
				return *this;
			}
			swap(this->m_node, other.m_node);
			swap(this->m_port, other.m_port);
			swap(this->m_addr, other.m_addr);
			swap(this->m_status, other.m_status);
			return *this;
		}

		ST_NODISCARD ST_INLINE int Status() const ST_NOEXCEPT {
			return m_status;
		}
		ST_NODISCARD ST_INLINE int LastError() const ST_NOEXCEPT {
			return m_status;
		}
		ST_NODISCARD ST_INLINE string Node() const ST_NOEXCEPT {
			return m_node;
		}
		ST_NODISCARD ST_INLINE string IP() const ST_NOEXCEPT {
			return m_node;
		}
		ST_NODISCARD ST_INLINE string Hostname() const ST_NOEXCEPT {
			return m_node;
		}
		ST_NODISCARD ST_INLINE string Port() const ST_NOEXCEPT {
			return m_port;
		}
		ST_NODISCARD ST_INLINE struct addrinfo* Info() const ST_NOEXCEPT {
			return m_addr;
		}

		ST_NODISCARD ST_INLINE static int GetHostName(string& outName) ST_NOEXCEPT  {
			char hostname[50];
			int re = gethostname(hostname, sizeof(hostname));
			if (re == 0) {
				outName = hostname;
			}
			return re;
		}
	private:
		ST_INLINE void create_info(const struct addrinfo& hint) ST_NOEXCEPT {
			m_status = getaddrinfo(m_node.c_str(), m_port.c_str(), &hint, &m_addr);
		}

		struct addrinfo* m_addr;
		string m_node, m_port;
		int m_status = 0;
	};
}
ST_STL_END