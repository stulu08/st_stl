#pragma once
#include "stulu/stl/cassert.h"
#include "stulu/stl/cstdint.h"
#include "stulu/stl/ctime.h"
#include "stulu/stl/string.h"

#include "stulu/http/status.h"
#include "stulu/http/method.h"

#include <stdio.h>
ST_STL_BEGIN
namespace HTTP {
#define MAX_URI_LENGTH 1024
#define MAX_URI_LENGTH_S "1024"
#define VERSION_HTTP_1_1 "HTTP/1.1"

	class HttpBase {
	public:
		struct LineData {
			string name;
			string value;
			LineData* next = nullptr;
		};

		ST_CONSTEXPR HttpBase() ST_NOEXCEPT
			: m_version(VERSION_HTTP_1_1), m_contentLength(0), m_content(), m_data(nullptr) { }

		ST_INLINE ~HttpBase() ST_NOEXCEPT {
			ClearLines();
		}

		ST_INLINE void ClearLines() ST_NOEXCEPT {
			LineData* line = m_data;
			while (line != nullptr) {
				LineData* next_line = line->next;
				delete line;
				line = next_line;
			}
			m_data = nullptr;
		}
		ST_INLINE void AddLineFront(const string& line_name, const string& line_value) ST_NOEXCEPT {
			LineData* newLine = new LineData();
			newLine->name = line_name;
			newLine->value = line_value;
			newLine->next = m_data;

			m_data = newLine;
		}
		ST_INLINE void AddLine(const string& line_name, const string& line_value) ST_NOEXCEPT {
			return AddLineBack(line_name, line_value);
		}
		ST_INLINE void AddLineBack(const string& line_name, const string& line_value) ST_NOEXCEPT {
			if (m_data == nullptr) {
				AddLineFront(line_name, line_value);
				return;
			}
			LineData* newLine = new LineData();
			newLine->name = line_name;
			newLine->value = line_value;
			newLine->next = nullptr;


			for (LineData* line = m_data; line != nullptr; line = line->next) {
				if (line->next == nullptr) {
					line->next = newLine;
					return;
				}
			}
		}
		ST_NODISCARD ST_INLINE bool HasLine(const string& line_name) const ST_NOEXCEPT {
			for (LineData* line = m_data; line != nullptr; line = line->next) {
				if (line->name == line_name) {
					return true;
				}
			}
			return false;
		}
		ST_NODISCARD ST_INLINE string GetLine(const string& line_name) const ST_NOEXCEPT {
			for (LineData* line = m_data; line != nullptr; line = line->next) {
				if (line->name == line_name) {
					return line->value;
				}
			}
			ST_REPORT_ERROR("Line not found!");
			return "";
		}
		ST_INLINE void SetLine(const string& line_name, const string& line_value) const ST_NOEXCEPT {
			for (LineData* line = m_data; line != nullptr; line = line->next) {
				if (line->name == line_name) {
					line->value = line_value;
					return;
				}
			}
			ST_REPORT_ERROR("Line not found!");
		}

		ST_INLINE void SetVersion(const string& version) ST_NOEXCEPT {
			m_version = version;
		}
		ST_INLINE void SetContent(const string& content) ST_NOEXCEPT {
			m_content = content;
			m_contentLength = m_content.length();
		}

		ST_NODISCARD ST_INLINE string GetVersion() const ST_NOEXCEPT {
			return m_version;
		}
		ST_NODISCARD ST_INLINE string GetContent() const ST_NOEXCEPT {
			return m_content;
		}
		ST_NODISCARD ST_INLINE size_t GetContentLength() const ST_NOEXCEPT {
			return m_contentLength;
		}
		ST_NODISCARD ST_INLINE const LineData* GetData() const ST_NOEXCEPT {
			return m_data;
		}

	protected:
		ST_INLINE void ReadLines(char const* lines_begin) ST_NOEXCEPT {
			char* value_scan_str = new char[MAX_URI_LENGTH + 1];
			char* name_scan_str = new char[MAX_URI_LENGTH + 1];
			for (uint16_t faile_safe = 0; faile_safe < 1024; faile_safe++) {
				char newLineCheck, colonCheck;

				// "%1024[^:]%c %1024[^\n]%c"
				const char* line_str_input_format = "%" MAX_URI_LENGTH_S "[^:]%c %" MAX_URI_LENGTH_S "[^\n]%c";

				if (sscanf(lines_begin, line_str_input_format, name_scan_str, &colonCheck, value_scan_str, &newLineCheck) != 4) {
					break;
				}

				if (colonCheck != ':' || newLineCheck != '\n') {
					break;
				}

				const size_t nameLength = strlen(name_scan_str);
				const size_t valueLength = strlen(value_scan_str);
				lines_begin += nameLength + valueLength + 3;

				const string name = string(name_scan_str, nameLength);

				if (m_contentLength == 0 && name == "Content-Length") {
					int length = 0;
					if (sscanf(value_scan_str, "%d", &length) != 1) {
						ST_REPORT_ERROR("Could not parse Content-Length");
					}
					else {
						m_contentLength = length;
					}
				}

				AddLineBack(name, string(value_scan_str));

			}
			delete[] name_scan_str;
			delete[] value_scan_str;
		}

		string m_version;

		size_t m_contentLength;
		string m_content;

		struct LineData* m_data;
	};

	class Request : public HttpBase {
	public:
		ST_CONSTEXPR Request() ST_NOEXCEPT 
			: HttpBase(), m_method(), m_uri() { }

		ST_INLINE Request(const char* msg) ST_NOEXCEPT 
			: HttpBase(), m_method(), m_uri() {
			if (!Parse(msg))
				ST_REPORT_ERROR("Error parsing request!");
		}
		ST_INLINE Request(const string msg) ST_NOEXCEPT
			: HttpBase(), m_method(), m_uri() {
			if (!Parse(msg.c_str()))
				ST_REPORT_ERROR("Error parsing request!");
		}
		ST_INLINE ~Request() ST_NOEXCEPT {
			ClearLines();
		}

		ST_NODISCARD ST_INLINE bool Parse(char const* msg) {
			const char* originalMsg = msg;

			// buffer with an max length of an http uri
			char* scan_str = new char[MAX_URI_LENGTH + 1];

			// "%1024s"
			const char* str_input_format = "%" MAX_URI_LENGTH_S "s";

			// read request method
			if (sscanf(msg, str_input_format, scan_str) != 1) {
				delete[] scan_str;
				return false;
			}
			m_method = MethodFromString(scan_str);
			msg += strlen(scan_str) + 1;

			// read uri
			if (sscanf(msg, str_input_format, scan_str) != 1) {
				delete[] scan_str;
				return false;
			}
			m_uri = string(scan_str);
			msg += strlen(scan_str) + 1;

			// read http version
			if (sscanf(msg, str_input_format, scan_str) != 1) {
				delete[] scan_str;
				return false;
			}
			m_version = string(scan_str);
			msg += strlen(scan_str) + 2;

			ReadLines(msg);

			if (m_contentLength != 0) {
				const size_t originalLength = strlen(originalMsg);
				if (originalLength < m_contentLength) {
					ST_REPORT_ERROR("Content-Length does not match");
				}
				else {
					const char* begin = originalMsg + (originalLength - m_contentLength);
					m_content = string(begin, m_contentLength);
				}
			}

			delete[] scan_str;
			return true;
		}
		ST_NODISCARD ST_INLINE string Build() const {
			string str;
			str += MethodToString(m_method) + ' ' + m_uri + ' ' + m_version + '\n';

			for (LineData * line = m_data; line != nullptr; line = line->next) {
				str += line->name + ": " + line->value + '\n';
			}

			str += '\n';
			str += m_content;
			return str;
		}

		ST_INLINE void SetURI(const string& uri) ST_NOEXCEPT {
			m_uri = uri;
		}
		ST_INLINE void SetMethod(request_method method) ST_NOEXCEPT {
			m_method = method;
		}

		ST_NODISCARD ST_INLINE request_method GetMethod() const ST_NOEXCEPT {
			return m_method;
		}
		ST_NODISCARD ST_INLINE string GetURI() const ST_NOEXCEPT {
			return m_uri;
		}
	private:
		request_method m_method;
		string m_uri;
	};

	class Response : public HttpBase {
	public:
		ST_CONSTEXPR Response() ST_NOEXCEPT
			: HttpBase(), m_status(Status::Invalid) { }

		ST_INLINE Response(const char* msg) ST_NOEXCEPT
			: HttpBase(), m_status(Status::Invalid) {
			if (!Parse(msg))
				ST_REPORT_ERROR("Error parsing request!");
		}
		ST_INLINE Response(const string msg) ST_NOEXCEPT
			: HttpBase(), m_status(Status::Invalid) {
			if (!Parse(msg.c_str()))
				ST_REPORT_ERROR("Error parsing request!");
		}
		ST_INLINE ~Response() ST_NOEXCEPT {
			ClearLines();
		}

		ST_NODISCARD ST_INLINE bool Parse(char const* msg) {
			const char* originalMsg = msg;

			// buffer with an max length of an http uri
			char* scan_str = new char[MAX_URI_LENGTH + 1];

			// read http version
			if (sscanf(msg, "%" MAX_URI_LENGTH_S "s", scan_str) != 1) {
				delete[] scan_str;
				return false;
			}
			m_version = string(scan_str);
			msg += strlen(scan_str) + 2;

			// read status
			int status_code = 0;
			if (sscanf(msg, "%" MAX_URI_LENGTH_S "d", &status_code) != 1) {
				delete[] scan_str;
				return false;
			}
			m_status = (Status)status_code;
			msg += 4;

			// check status msg
			if (sscanf(msg, "%" MAX_URI_LENGTH_S "[^\n]", scan_str) != 1) {
				delete[] scan_str;
				return false;
			}
			if (StatusToString(m_status) != string(scan_str)) {
				ST_REPORT_ERROR("Status code and message do not match");
			}
			msg += strlen(scan_str) + 1;

			ReadLines(msg);

			if (m_contentLength != 0) {
				const size_t originalLength = strlen(originalMsg);
				if (originalLength < m_contentLength) {
					ST_REPORT_ERROR("Content-Length does not match");
				}
				else {
					const char* begin = originalMsg + (originalLength - m_contentLength);
					m_content = string(begin, m_contentLength);
				}

			}

			delete[] scan_str;
			return true;
		}
		ST_NODISCARD ST_INLINE string Build() const {
			string str;
			char statusNum[4] = "999";
			if ((uint16_t)m_status <= 999 && (uint16_t)m_status >= 100) {
				sprintf(statusNum, "%u", (uint32_t)m_status);
			}

			str += m_version + ' ' + statusNum + ' ' + StatusToString(m_status) + '\n';

			for (LineData* line = m_data; line != nullptr; line = line->next) {
				str += line->name + ": " + line->value + '\n';
			}

			str += '\n';
			str += m_content;
			return str;
		}
		ST_INLINE void SetStatus(Status _status) ST_NOEXCEPT {
			m_status = _status;
		}
		ST_INLINE void SetDate(__time64_t* timestamp = 0) {
			time_t local_time = _time64(timestamp);
			struct tm* gmt = _gmtime64(&local_time);

			char* time_str = new char[MAX_URI_LENGTH + 1];
			strftime(time_str, MAX_URI_LENGTH, "%a, %d %b %Y %X GMT", gmt);

			if (HasLine("Date")) {
				SetLine("Date", time_str);
			}
			else {
				AddLine("Date", time_str);
			}
		}

		ST_NODISCARD ST_INLINE Status GetStatus() const ST_NOEXCEPT {
			return m_status;
		}
	private:
		Status m_status;
	};
}
ST_STL_END