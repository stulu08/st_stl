#pragma once
#include "stulu/core/core.h"
#include "stulu/networking/address_info.h"
#include "stulu/networking/SSA.h"

ST_STL_BEGIN
namespace networking {
	class address {
	public:
		ST_INLINE address(const string_type& node, const string_type& port, const address_info& hintInfo = address_info())
			: m_node(node), m_port(port), m_addr(nullptr) {
			struct addrinfo hint = addrinfo();
			hint.ai_flags = hintInfo.flags;
			hint.ai_family = (int32_t)hintInfo.family;
			hint.ai_socktype = (int32_t)hintInfo.socketType;
			hint.ai_protocol = (int32_t)hintInfo.protocol;
			create_info(hint);
		}
		
		ST_INLINE address(const address& other)
			: m_addr(nullptr) {
			this->m_node = other.m_node;
			this->m_port = other.m_port;

			if(other.m_addr)
				create_info(*other.m_addr);
		}

		ST_INLINE address(address&& other) 
			: m_node(""), m_port(""), m_addr(nullptr) {
			swap(this->m_node, other.m_node);
			swap(this->m_port, other.m_port);
			swap(this->m_addr, other.m_addr);
		}

		ST_INLINE ~address() {
			if (m_addr)
				freeaddrinfo(m_addr);
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
		ST_NODISCARD ST_INLINE const struct addrinfo* info() const ST_NOEXCEPT {
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
		ST_INLINE void create_info(struct addrinfo& hint) {
			m_status = getaddrinfo(m_node.c_str(), m_port.c_str(), &hint, &m_addr);
		}

		struct addrinfo* m_addr;
		string_type m_node, m_port;
		int m_status = 0;
	};
}
ST_STL_END