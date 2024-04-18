#pragma once
#include "stulu/core/core.h"
#include "stulu/stl/cstdint.h"
#include "stulu/stl/string.h"

ST_STL_BEGIN
namespace networking {
	using string_type = stulu::string;

	enum class address_flags : int32_t {
		/// <summary>
		/// The socket address will be used in a call to the bind function.
		/// </summary>
		Passive = 0x01,
		/// <summary>
		/// The canonical name is returned in the first ai_canonname member.
		/// </summary>
		CanonicalName = 0x02,
		/// <summary>
		/// The nodename parameter passed to the getaddrinfo function must be a numeric string.
		/// </summary>
		NumericHost = 0x04,
		/// <summary>
		/// <para> If this bit is set, a request is made for IPv6 addresses and IPv4 addresses with AI_V4MAPPED. </para>
		/// <para> This option is supported on Windows Vistaand later. </para>
		/// </summary>
		All = 0x0100,
		/// <summary>
		/// The getaddrinfo will resolve only if a global address is configured. The IPv6 and IPv4 loopback address is not considered a valid global address.
		/// <para> This option is supported on Windows Vista and later. </para>
		/// </summary>
		AddressConfig = 0x0400,
		/// <summary>
		/// If the getaddrinfo request for IPv6 addresses fails, a name service request is made for IPv4 addresses and these addresses are converted to IPv4-mapped IPv6 address format.
		/// <para> This option is supported on Windows Vistaand later. </para>
		/// </summary>
		IPv4Mapped = 0x0800,
		/// <summary>
		/// The address information can be from a non-authoritative namespace provider.
		/// <para> This option is only supported on Windows Vista and later for the NS_EMAIL namespace.</para>
		/// </summary>
		NonAuthoritative = 0x04000,
		/// <summary>
		/// The address information is from a secure channel.
		/// <para> This option is only supported on Windows Vistaand later for the NS_EMAIL namespace. </para>
		/// </summary>
		Secure = 0x08000,
		/// <summary>
		/// The address information is for a preferred name for a user.
		/// <para >This option is only supported on Windows Vistaand later for the NS_EMAIL namespace. </para>
		/// </summary>
		ReturnPreferedNames = 0x010000,
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
		/// <para> 
		/// This option is supported on Windows 7, Windows Server 2008 R2, and later.
		/// </para>
		/// </summary>
		FQDN = 0x00020000,
		/// <summary>
		/// A hint to the namespace provider that the hostname being queried is being used in a file share scenario. The namespace provider may ignore this hint.
		/// <para> This option is supported on Windows 7, Windows Server 2008 R2, and later. </para>
		/// </summary>
		FileServer = 0x00040000,
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
		Stream = 1,
		/// <summary>
		/// Supports datagrams, which are connectionless, 
		/// unreliable buffers of a fixed (typically small) maximum length. 
		/// Uses the User Datagram Protocol (UDP) for the Internet 
		/// address family (AF_INET or AF_INET6).
		/// <para> DataGram is an alias for packets </para>
		/// </summary>
		DataGram = 2,
		/// <summary>
		/// Provides a raw socket that allows an application to manipulate 
		/// the next upper-layer protocol header. To manipulate the IPv4 header,
		/// the IP_HDRINCL socket option must be set on the socket. To manipulate 
		/// the IPv6 header, the IPV6_HDRINCL socket option must be set on the socket.
		/// </summary>
		Raw = 3,
		/// <summary>
		/// Provides a reliable datagram layer that does not guarantee ordering.
		/// </summary>
		RDM = 4,
		/// <summary>
		/// Provides a pseudo-stream packet based on datagrams.
		/// </summary>
		SequencedPacket = 5

	};
	enum class address_family : int32_t {
		// The address family is unspecified.
		Unspecified = 0,
		/// The Internet Protocol version 4 (IPv4) address family.
		IPv4 = 2,
		// The NetBIOS address family. This address family is only supported if a Windows Sockets provider for NetBIOS is installed.
		NetBIOS = 17,
		// The Internet Protocol version 6 (IPv6) address family.
		IPv6 = 23,
		// The Infrared Data Association (IrDA) address family. This address family is only supported if the computer has an infrared port and driver installed.
		IrDA = 26,
		// The Bluetooth address family. This address family is only supported if a Bluetooth adapter is installed on Windows Server 2003 or later.
		Bluetooth = 32
	};
	enum class protocol : int32_t {
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
		int32_t flags = 0;
		address_family family = address_family::IPv4;
		socket_type socketType = socket_type::Stream;
		protocol protocol = protocol::TCP;
		string_type canonicalName = "";
	};
}
ST_STL_END
