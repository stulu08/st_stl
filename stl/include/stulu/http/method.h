#pragma once
#include "stulu/stl/cstddef.h"
#include "stulu/stl/cstdint.h"
#include "stulu/stl/cctype.h"
#include "stulu/stl/string.h"

ST_STL_BEGIN
namespace http {
	enum class request_method : uint8_t {
		NONE,
		GET,
		HEAD,
		POST,
		PUT,
		// DELETE
		DEL,
		CONNECT,
		OPTIONS,
		TRACE,
	};

	ST_NODISCARD ST_INLINE stulu::string method_to_string(request_method method) ST_NOEXCEPT {
		switch (method)
		{
		case request_method::GET:
			return "GET";
		case request_method::HEAD:
			return "HEAD";
		case request_method::POST:
			return "POST";
		case request_method::PUT:
			return "PUT";
		case request_method::DEL:
			return "DELETE";
		case request_method::CONNECT:
			return "CONNECT";
		case request_method::OPTIONS:
			return "OPTIONS";
		case request_method::TRACE:
			return "TRACE";
		}
		return "INVALID";
	}

	ST_NODISCARD ST_INLINE request_method method_from_str(const stulu::string& str) ST_NOEXCEPT {
		// to upper
		stulu::string method(str.length());
		for (size_t i = 0; i < str.length(); i++)
			method[i] = static_cast<char>(stulu::toupper(static_cast<int>(str[i])));

		if (method == "GET")
			return request_method::GET;
		if (method == "HEAD")
			return request_method::HEAD;
		if (method == "POST")
			return request_method::POST;
		if (method == "PUT")
			return request_method::PUT;
		if (method == "DELETE")
			return request_method::DEL;
		if (method == "CONNECT")
			return request_method::CONNECT;
		if (method == "OPTIONS")
			return request_method::OPTIONS;
		if (method == "TRACE")
			return request_method::TRACE;

		return request_method::NONE;
	}
}
ST_STL_END