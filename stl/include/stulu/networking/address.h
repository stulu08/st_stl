#pragma once
#include "stulu/core/core.h"
#include "stulu/networking/address_info.h"
#include "stulu/networking/SSA.h"

ST_STL_BEGIN
namespace networking {
	class address {
	public:
		ST_CONSTEXPR address() ST_NOEXCEPT
			: m_node(""), m_port(""), m_addr(nullptr) { }
		ST_INLINE address(const string_type& node, const string_type& port, const address_info& hintInfo = address_info()) ST_NOEXCEPT
			: m_node(node), m_port(port), m_addr(nullptr) {
			struct addrinfo hint = addrinfo();
			hint.ai_flags = hintInfo.flags;
			hint.ai_family = (int32_t)hintInfo.family;
			hint.ai_socktype = (int32_t)hintInfo.socketType;
			hint.ai_protocol = (int32_t)hintInfo.protocol;
			create_info(hint);
		}
		ST_INLINE address(const string_type& node, const string_type& port, const struct addrinfo* hint) ST_NOEXCEPT
			: m_node(node), m_port(port), m_addr(nullptr) {
			struct addrinfo _hint = addrinfo();
			_hint.ai_flags = hint->ai_flags;
			_hint.ai_family = hint->ai_family;
			_hint.ai_socktype = hint->ai_socktype;
			_hint.ai_protocol = hint->ai_protocol;
			create_info(_hint);
		}
		
		ST_INLINE address(const address& other) ST_NOEXCEPT
			: m_addr(nullptr) {
			this->m_node = other.m_node;
			this->m_port = other.m_port;

			struct addrinfo hint = addrinfo();
			hint.ai_flags = other.info()->ai_flags;
			hint.ai_family = other.info()->ai_family;
			hint.ai_socktype = other.info()->ai_socktype;
			hint.ai_protocol = other.info()->ai_protocol;
			create_info(hint);
		}

		ST_INLINE address(address&& other) ST_NOEXCEPT
			: m_node(""), m_port(""), m_addr(nullptr) {
			swap(this->m_node, other.m_node);
			swap(this->m_port, other.m_port);
			swap(this->m_addr, other.m_addr);
		}

		ST_INLINE ~address() {
			if (m_addr)
				freeaddrinfo(m_addr);
		}

		ST_INLINE address& operator=(const address& other) ST_NOEXCEPT {
			if (this == ST_STL addressof(other)) {
				return *this;
			}
			this->m_node = other.m_node;
			this->m_port = other.m_port;
			this->m_addr = nullptr;

			struct addrinfo hint = addrinfo();
			hint.ai_flags = other.info()->ai_flags;
			hint.ai_family = other.info()->ai_family;
			hint.ai_socktype = other.info()->ai_socktype;
			hint.ai_protocol = other.info()->ai_protocol;
			create_info(hint);

			return *this;
		}
		ST_INLINE address& operator=(address&& other) ST_NOEXCEPT {
			if (this == ST_STL addressof(other)) {
				return *this;
			}
			swap(this->m_node, other.m_node);
			swap(this->m_port, other.m_port);
			swap(this->m_addr, other.m_addr);
			return *this;
		}

		ST_NODISCARD ST_INLINE int status() const ST_NOEXCEPT {
			return m_status;
		}
		ST_NODISCARD ST_INLINE int last_error() const ST_NOEXCEPT {
			return m_status;
		}
		ST_NODISCARD ST_INLINE string_type node() const ST_NOEXCEPT {
			return m_node;
		}
		ST_NODISCARD ST_INLINE string_type ip() const ST_NOEXCEPT {
			return m_node;
		}
		ST_NODISCARD ST_INLINE string_type hostname() const ST_NOEXCEPT {
			return m_node;
		}
		ST_NODISCARD ST_INLINE string_type port() const ST_NOEXCEPT {
			return m_port;
		}
		ST_NODISCARD ST_INLINE struct addrinfo* info() const ST_NOEXCEPT {
			return m_addr;
		}

		ST_NODISCARD ST_INLINE static int GetHostName(string_type& outName) ST_NOEXCEPT  {
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
		string_type m_node, m_port;
		int m_status = 0;
	};
}
ST_STL_END