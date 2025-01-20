#pragma once
#include "stulu/stl/cstddef.h"
#include "stulu/stl/cstdint.h"
#include "stulu/stl/string.h"

ST_STL_BEGIN
namespace HTTP {
	enum class Status : uint16_t
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

	ST_NODISCARD ST_INLINE string StatusToString(Status code) ST_NOEXCEPT {
		switch (code) {
		case Status::Continue:
			return "Continue";
		case Status::SwitchingProtocols:
			return "Switching Protocols";
		case Status::Processing:
			return "Processing";
		case Status::EarlyHints:
			return "Early Hints";
		case Status::OK:
			return "OK";
		case Status::Created:
			return "Created";
		case Status::Accepted:
			return "Accepted";
		case Status::NonAuthoritativeInformation:
			return "Non Authoritative Information";
		case Status::NoContent:
			return "No Content";
		case Status::ResetContent:
			return "Reset Content";
		case Status::PartialContent:
			return "Partial Content";
		case Status::MultiStatus:
			return "Multi Status";
		case Status::AlreadyReported:
			return "Already Reported";
		case Status::IMUsed:
			return "IM Used";
		case Status::MultipleChoices:
			return "Multiple Choices";
		case Status::MovedPermanently:
			return "Moved Permanently";
		case Status::Found:
			return "Found";
		case Status::SeeOther:
			return "See Other";
		case Status::NotModified:
			return "Not Modified";
		case Status::UseProxy:
			return "Use Proxy";
		case Status::TemporaryRedirect:
			return "Temporary Redirect";
		case Status::PermanentRedirect:
			return "Permanent Redirect";
		case Status::BadRequest:
			return "Bad Request";
		case Status::Unauthorized:
			return "Unauthorized";
		case Status::PaymentRequired:
			return "Payment Required";
		case Status::Forbidden:
			return "Forbidden";
		case Status::NotFound:
			return "Not Found";
		case Status::MethodNotAllowed:
			return "Method Not Allowed";
		case Status::NotAcceptable:
			return "Not Acceptable";
		case Status::ProxyAuthenticationRequired:
			return "Proxy Authentication Required";
		case Status::RequestTimeout:
			return "Request Timeout";
		case Status::Conflict:
			return "Conflict";
		case Status::Gone:
			return "Gone";
		case Status::LengthRequired:
			return "Length Required";
		case Status::PreconditionFailed:
			return "Precondition Failed";
		case Status::ContentTooLarge:
			return "Content Too Large";
		//case status::PayloadTooLarge:
			//	return "Payload Too Large";
		case Status::URITooLong:
			return "URI Too Long";
		case Status::UnsupportedMediaType:
			return "Unsupported Media Type";
		case Status::RangeNotSatisfiable:
			return "Range Not Satisfiable";
		case Status::ExpectationFailed:
			return "Expectation Failed";
		case Status::ImATeapot:
			return "ImA Teapot";
		case Status::MisdirectedRequest:
			return "Misdirected Request";
		case Status::UnprocessableContent:
			return "Unprocessable Content";
		//case status::UnprocessableEntity:
			//	return "Unprocessable Entity";
		case Status::Locked:
			return "Locked";
		case Status::FailedDependency:
			return "Failed Dependency";
		case Status::TooEarly:
			return "Too Early";
		case Status::UpgradeRequired:
			return "Upgrade Required";
		case Status::PreconditionRequired:
			return "Precondition Required";
		case Status::TooManyRequests:
			return "Too Many Requests";
		case Status::RequestHeaderFieldsTooLarge:
			return "Request Header Fields Too Large";
		case Status::UnavailableForLegalReasons:
			return "Unavailable For Legal Reasons";
		case Status::InternalServerError:
			return "Internal Server Error";
		case Status::NotImplemented:
			return "Not Implemented";
		case Status::BadGateway:
			return "Bad Gateway";
		case Status::ServiceUnavailable:
			return "Service Unavailable";
		case Status::GatewayTimeout:
			return "Gateway Timeout";
		case Status::HTTPVersionNotSupported:
			return "HTTP Version Not Supported";
		case Status::VariantAlsoNegotiates:
			return "Variant Also Negotiates";
		case Status::InsufficientStorage:
			return "Insufficient Storage";
		case Status::LoopDetected:
			return "Loop Detected";
		case Status::NotExtended:
			return "Not Extended";
		case Status::NetworkAuthenticationRequired:
			return "Network Authentication Required";
		}
		return "Invalid";
	}
}
ST_STL_END