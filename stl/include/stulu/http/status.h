#pragma once
#include "stulu/stl/cstddef.h"
#include "stulu/stl/cstdint.h"
#include "stulu/stl/string.h"

ST_STL_BEGIN
namespace http {
	enum class status : uint16_t
	{
		Invalid = 999,
		Continue = 100,
		SwitchingProtocols = 101,
		Processing = 102,
		EarlyHints = 103,
		OK = 200,
		Created = 201,
		Accepted = 202,
		NonAuthoritativeInformation = 203,
		NoContent = 204,
		ResetContent = 205,
		PartialContent = 206,
		MultiStatus = 207,
		AlreadyReported = 208,
		IMUsed = 226,
		MultipleChoices = 300,
		MovedPermanently = 301,
		Found = 302,
		SeeOther = 303,
		NotModified = 304,
		UseProxy = 305,
		TemporaryRedirect = 307,
		PermanentRedirect = 308,
		BadRequest = 400,
		Unauthorized = 401,
		PaymentRequired = 402,
		Forbidden = 403,
		NotFound = 404,
		MethodNotAllowed = 405,
		NotAcceptable = 406,
		ProxyAuthenticationRequired = 407,
		RequestTimeout = 408,
		Conflict = 409,
		Gone = 410,
		LengthRequired = 411,
		PreconditionFailed = 412,
		ContentTooLarge = 413,
		PayloadTooLarge = 413,
		URITooLong = 414,
		UnsupportedMediaType = 415,
		RangeNotSatisfiable = 416,
		ExpectationFailed = 417,
		ImATeapot = 418,
		MisdirectedRequest = 421,
		UnprocessableContent = 422,
		UnprocessableEntity = 422,
		Locked = 423,
		FailedDependency = 424,
		TooEarly = 425,
		UpgradeRequired = 426,
		PreconditionRequired = 428,
		TooManyRequests = 429,
		RequestHeaderFieldsTooLarge = 431,
		UnavailableForLegalReasons = 451,
		InternalServerError = 500,
		NotImplemented = 501,
		BadGateway = 502,
		ServiceUnavailable = 503,
		GatewayTimeout = 504,
		HTTPVersionNotSupported = 505,
		VariantAlsoNegotiates = 506,
		InsufficientStorage = 507,
		LoopDetected = 508,
		NotExtended = 510,
		NetworkAuthenticationRequired = 511,
	};

	ST_NODISCARD ST_INLINE stulu::string status_to_string(status code) ST_NOEXCEPT {
		switch (code) {
		case status::Continue:
			return "Continue";
		case status::SwitchingProtocols:
			return "Switching Protocols";
		case status::Processing:
			return "Processing";
		case status::EarlyHints:
			return "Early Hints";
		case status::OK:
			return "OK";
		case status::Created:
			return "Created";
		case status::Accepted:
			return "Accepted";
		case status::NonAuthoritativeInformation:
			return "Non Authoritative Information";
		case status::NoContent:
			return "No Content";
		case status::ResetContent:
			return "Reset Content";
		case status::PartialContent:
			return "Partial Content";
		case status::MultiStatus:
			return "Multi Status";
		case status::AlreadyReported:
			return "Already Reported";
		case status::IMUsed:
			return "IM Used";
		case status::MultipleChoices:
			return "Multiple Choices";
		case status::MovedPermanently:
			return "Moved Permanently";
		case status::Found:
			return "Found";
		case status::SeeOther:
			return "See Other";
		case status::NotModified:
			return "Not Modified";
		case status::UseProxy:
			return "Use Proxy";
		case status::TemporaryRedirect:
			return "Temporary Redirect";
		case status::PermanentRedirect:
			return "Permanent Redirect";
		case status::BadRequest:
			return "Bad Request";
		case status::Unauthorized:
			return "Unauthorized";
		case status::PaymentRequired:
			return "Payment Required";
		case status::Forbidden:
			return "Forbidden";
		case status::NotFound:
			return "Not Found";
		case status::MethodNotAllowed:
			return "Method Not Allowed";
		case status::NotAcceptable:
			return "Not Acceptable";
		case status::ProxyAuthenticationRequired:
			return "Proxy Authentication Required";
		case status::RequestTimeout:
			return "Request Timeout";
		case status::Conflict:
			return "Conflict";
		case status::Gone:
			return "Gone";
		case status::LengthRequired:
			return "Length Required";
		case status::PreconditionFailed:
			return "Precondition Failed";
		case status::ContentTooLarge:
			return "Content Too Large";
		//case status::PayloadTooLarge:
			//	return "Payload Too Large";
		case status::URITooLong:
			return "URI Too Long";
		case status::UnsupportedMediaType:
			return "Unsupported Media Type";
		case status::RangeNotSatisfiable:
			return "Range Not Satisfiable";
		case status::ExpectationFailed:
			return "Expectation Failed";
		case status::ImATeapot:
			return "ImA Teapot";
		case status::MisdirectedRequest:
			return "Misdirected Request";
		case status::UnprocessableContent:
			return "Unprocessable Content";
		//case status::UnprocessableEntity:
			//	return "Unprocessable Entity";
		case status::Locked:
			return "Locked";
		case status::FailedDependency:
			return "Failed Dependency";
		case status::TooEarly:
			return "Too Early";
		case status::UpgradeRequired:
			return "Upgrade Required";
		case status::PreconditionRequired:
			return "Precondition Required";
		case status::TooManyRequests:
			return "Too Many Requests";
		case status::RequestHeaderFieldsTooLarge:
			return "Request Header Fields Too Large";
		case status::UnavailableForLegalReasons:
			return "Unavailable For Legal Reasons";
		case status::InternalServerError:
			return "Internal Server Error";
		case status::NotImplemented:
			return "Not Implemented";
		case status::BadGateway:
			return "Bad Gateway";
		case status::ServiceUnavailable:
			return "Service Unavailable";
		case status::GatewayTimeout:
			return "Gateway Timeout";
		case status::HTTPVersionNotSupported:
			return "HTTP Version Not Supported";
		case status::VariantAlsoNegotiates:
			return "Variant Also Negotiates";
		case status::InsufficientStorage:
			return "Insufficient Storage";
		case status::LoopDetected:
			return "Loop Detected";
		case status::NotExtended:
			return "Not Extended";
		case status::NetworkAuthenticationRequired:
			return "Network Authentication Required";
		}
		return "Invalid";
	}
}
ST_STL_END