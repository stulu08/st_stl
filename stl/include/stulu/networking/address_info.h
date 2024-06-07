#pragma once
#include "stulu/core/core.h"
#include "stulu/stl/cstdint.h"
#include "stulu/stl/string.h"
#include "stulu/networking/SSA.h"

ST_STL_BEGIN
namespace networking {
	using string_type = stulu::string;

	struct address_flags {
		static ST_INLINE ST_CONSTEXPR int32_t None = 0;
		/// <summary>
		/// The socket address will be used in a call to the bind function.
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t Passive = AI_PASSIVE;
		/// <summary>
		/// The canonical name is returned in the first ai_canonname member.
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t CanonicalName = AI_CANONNAME;
		/// <summary>
		/// The nodename parameter passed to the getaddrinfo function must be a numeric string.
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t NumericHost = AI_NUMERICHOST;
		/// <summary>
		/// <para> If this bit is set, a request is made for IPv6 addresses and IPv4 addresses with IPv4Mapped. </para>
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t All = AI_ALL;
		/// <summary>
		/// The getaddrinfo will resolve only if a global address is configured. The IPv6 and IPv4 loopback address is not considered a valid global address.
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t AddressConfig = AI_ADDRCONFIG;
		/// <summary>
		/// If the getaddrinfo request for IPv6 addresses fails, a name service request is made for IPv4 addresses and these addresses are converted to IPv4-mapped IPv6 address format.
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t IPv4Mapped = AI_V4MAPPED;
		/// <summary>
		/// The address information can be from a non-authoritative namespace provider.
		/// <para> This option is only supported on Windows Vista and later for the NS_EMAIL namespace.</para>
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t Nt_NonAuthoritative
#ifdef ST_WINDOWS
			= AI_NON_AUTHORITATIVE;
#elif defined(ST_LINUX)
			= None;
#endif
		/// <summary>
		/// The address information is from a secure channel.
		/// <para> This option is only supported on Windows Vistaand later for the NS_EMAIL namespace. </para>
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t Nt_Secure
#ifdef ST_WINDOWS
		= AI_SECURE;
#elif defined(ST_LINUX)
		= None;
#endif
		/// <summary>
		/// The address information is for a preferred name for a user.
		/// <para >This option is only supported on Windows Vista and later for the NS_EMAIL namespace. </para>
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t ReturnPreferedNames
#ifdef ST_WINDOWS
		= AI_RETURN_PREFERRED_NAMES;
#elif defined(ST_LINUX)
		= None;
#endif
		/// <summary>
		/// <para> 
		/// If a flat name (single label) is specified, getaddrinfo will return the fully 
		/// qualified domain name that the name eventually resolved to. The fully qualified 
		/// domain name is returned in the ai_canonname member.
		/// </para>
		/// <para> 
		/// This is different than AI_CANONNAME bit flag that returns the canonical 
		/// name registered in DNS which may be different than the fully qualified domain 
		/// name that the flat name resolved to. 
		/// </para>
		/// <para> 
		/// Only one of the AI_FQDNand AI_CANONNAME bits can be set.The getaddrinfo function will fail if both flags are present with EAI_BADFLAGS.
		/// </para>
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t FQDN
#ifdef ST_WINDOWS
		= AI_FQDN;
#elif defined(ST_LINUX)
		= AI_IDN;
#endif
		/// <summary>
		/// A hint to the namespace provider that the hostname being queried is being used in a file share scenario. The namespace provider may ignore this hint.
		/// <para> This option is supported on Windows 7, Windows Server 2008 R2, and later. </para>
		/// </summary>
		static ST_INLINE ST_CONSTEXPR int32_t Nt_FileServer
#ifdef ST_WINDOWS
		= AI_FILESERVER;
#elif defined(ST_LINUX)
		= None;
#endif


		ST_INLINE ST_CONSTEXPR address_flags() ST_NOEXCEPT
			: m_value(None){}
		ST_INLINE ST_CONSTEXPR address_flags(int32_t value) ST_NOEXCEPT
			: m_value(value) {}

		ST_INLINE ST_CONSTEXPR operator int32_t() const ST_NOEXCEPT {
			return m_value;
		}
		ST_INLINE ST_CONSTEXPR address_flags operator=(int32_t val) ST_NOEXCEPT {
			this->m_value = val;
			return *this;
		}

	private:
		int32_t m_value = None;
	};
	enum class socket_type : int32_t {
		/// <summary>
		/// Provides sequenced, reliable, two-way, connection-based 
		/// byte streams with an OOB data transmission mechanism. 
		/// Uses the Transmission Control Protocol (TCP) for the 
		/// Internet address family (AF_INET or AF_INET6). 
		/// If the ai_family member is AF_IRDA, then SOCK_STREAM is 
		/// the only supported socket type.
		/// </summary>
		Stream = SOCK_STREAM,
		/// <summary>
		/// Supports datagrams, which are connectionless, 
		/// unreliable buffers of a fixed (typically small) maximum length. 
		/// Uses the User Datagram Protocol (UDP) for the Internet 
		/// address family (AF_INET or AF_INET6).
		/// <para> DataGram is an alias for packets </para>
		/// </summary>
		DataGram = SOCK_DGRAM,
		/// <summary>
		/// Provides a raw socket that allows an application to manipulate 
		/// the next upper-layer protocol header. To manipulate the IPv4 header,
		/// the IP_HDRINCL socket option must be set on the socket. To manipulate 
		/// the IPv6 header, the IPV6_HDRINCL socket option must be set on the socket.
		/// </summary>
		Raw = SOCK_RAW,
		/// <summary>
		/// Provides a reliable datagram layer that does not guarantee ordering.
		/// </summary>
		RDM = SOCK_RDM,
		/// <summary>
		/// Provides a pseudo-stream packet based on datagrams.
		/// </summary>
		SequencedPacket = SOCK_SEQPACKET

	};
	enum class address_family : int32_t {
		// The address family is unspecified.
		Unspecified = AF_UNSPEC,
		/// The Internet Protocol version 4 (IPv4) address family.
		IPv4 = AF_INET,
		// The Internet Protocol version 6 (IPv6) address family.
		IPv6 = AF_INET6,
		// The Infrared Data Association (IrDA) address family. This address family is only supported if the computer has an infrared port and driver installed.
		IrDA = AF_IRDA,
#ifdef ST_WINDOWS
		// The NetBIOS address family. This address family is only supported if a Windows Sockets provider for NetBIOS is installed.
		NetBIOS = AF_NETBIOS,
		// The Bluetooth address family. This address family is only supported if a Bluetooth adapter is installed on Windows Server 2003 or later.
		Bluetooth = AF_BTH
#endif
	};
	enum class address_protocol : int32_t {
		Any = 0,
		/// <summary>
		/// The Transmission Control Protocol (TCP). 
		/// This is a possible value when the ai_family 
		/// member is AF_INET or AF_INET6 and the 
		/// ai_socktype member is SOCK_STREAM.
		/// </summary>
		TCP = 6,
		/// <summary>
		/// The User Datagram Protocol (UDP). 
		/// This is a possible value when the 
		/// ai_family member is AF_INET or AF_INET6 
		/// and the type parameter is SOCK_DGRAM.
		/// </summary>
		UDP = 17,
		/// <summary>
		/// The PGM protocol for reliable multicast.
		/// This is a possible value when the ai_family 
		/// member is AF_INET and the ai_socktype member 
		/// is SOCK_RDM. On the Windows SDK released for 
		/// Windows Vista and later, this value is also 
		/// called IPPROTO_PGM.
		/// </summary>
		RM = 113
	};

	struct address_info {
		address_flags flags = address_flags::Passive;
		address_family family = address_family::IPv4;
		socket_type socketType = socket_type::Stream;
		address_protocol protocol = address_protocol::Any;
		string_type canonicalName = "";
	};
}
ST_STL_END
