#pragma once
#include "stulu/stl/cassert.h"
#include "stulu/stl/cstdint.h"
#include "stulu/stl/string.h"

#include "stulu/http/status.h"
#include "stulu/http/method.h"

#include <stdio.h>
ST_STL_BEGIN
namespace http {
#define MAX_URI_LENGTH 1024
#define MAX_URI_LENGTH_S "1024"

	class http_base {
	public:
		struct line_data {
			stulu::string name;
			stulu::string value;
			line_data* next = nullptr;
		};

		ST_CONSTEXPR http_base() ST_NOEXCEPT
			: m_version("HTTP/1.1"), m_content_length(0), m_content(), m_data(nullptr) { }

		ST_INLINE ~http_base() ST_NOEXCEPT {
			clear_lines();
		}

		ST_INLINE void clear_lines() ST_NOEXCEPT {
			line_data* line = m_data;
			while (line != nullptr) {
				line_data* next_line = line->next;
				delete line;
				line = next_line;
			}
			m_data = nullptr;
		}
		ST_INLINE void add_line_front(const stulu::string& line_name, const stulu::string& line_value) ST_NOEXCEPT {
			line_data* newLine = new line_data();
			newLine->name = line_name;
			newLine->value = line_value;
			newLine->next = m_data;

			m_data = newLine;
		}
		ST_INLINE void add_line(const stulu::string& line_name, const stulu::string& line_value) ST_NOEXCEPT {
			return add_line_back(line_name, line_value);
		}
		ST_INLINE void add_line_back(const stulu::string& line_name, const stulu::string& line_value) ST_NOEXCEPT {
			if (m_data == nullptr) {
				add_line_front(line_name, line_value);
				return;
			}
			line_data* newLine = new line_data();
			newLine->name = line_name;
			newLine->value = line_value;
			newLine->next = nullptr;


			for (line_data* line = m_data; line != nullptr; line = line->next) {
				if (line->next == nullptr) {
					line->next = newLine;
					return;
				}
			}
		}
		ST_NODISCARD ST_INLINE bool has_line(const stulu::string& line_name) const ST_NOEXCEPT {
			for (line_data* line = m_data; line != nullptr; line = line->next) {
				if (line->name == line_name) {
					return true;
				}
			}
			return false;
		}
		ST_NODISCARD ST_INLINE stulu::string get_line(const stulu::string& line_name) const ST_NOEXCEPT {
			for (line_data* line = m_data; line != nullptr; line = line->next) {
				if (line->name == line_name) {
					return line->value;
				}
			}
			ST_REPORT_ERROR("Line not found!");
			return "";
		}
		ST_INLINE void set_line(const stulu::string& line_name, const stulu::string& line_value) const ST_NOEXCEPT {
			for (line_data* line = m_data; line != nullptr; line = line->next) {
				if (line->name == line_name) {
					line->value = line_value;
					return;
				}
			}
			ST_REPORT_ERROR("Line not found!");
		}

		ST_INLINE void set_version(const stulu::string& version) ST_NOEXCEPT {
			m_version = version;
		}
		ST_INLINE void set_content(const stulu::string& content) ST_NOEXCEPT {
			m_content = content;
			m_content_length = m_content.length();
		}

		ST_NODISCARD ST_INLINE stulu::string get_version() const ST_NOEXCEPT {
			return m_version;
		}
		ST_NODISCARD ST_INLINE stulu::string get_content() const ST_NOEXCEPT {
			return m_content;
		}
		ST_NODISCARD ST_INLINE size_t get_content_length() const ST_NOEXCEPT {
			return m_content_length;
		}
		ST_NODISCARD ST_INLINE const line_data* get_data() const ST_NOEXCEPT {
			return m_data;
		}

	protected:
		ST_INLINE void read_lines(char const* lines_begin) ST_NOEXCEPT {
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

				const size_t nameLength = stulu::strlen(name_scan_str);
				const size_t valueLength = stulu::strlen(value_scan_str);
				lines_begin += nameLength + valueLength + 3;

				const stulu::string name = stulu::string(name_scan_str, nameLength);

				if (m_content_length == 0 && name == "Content-Length") {
					int length = 0;
					if (sscanf(value_scan_str, "%d", &length) != 1) {
						ST_REPORT_ERROR("Could not parse Content-Length");
					}
					else {
						m_content_length = length;
					}
				}

				add_line_back(name, stulu::string(value_scan_str));

			}
			delete[] name_scan_str;
			delete[] value_scan_str;
		}

		stulu::string m_version;

		size_t m_content_length;
		stulu::string m_content;

		struct line_data* m_data;
	};

	class request : public http_base {
	public:
		ST_CONSTEXPR request() ST_NOEXCEPT 
			: http_base(), m_method(), m_uri() { }

		ST_INLINE request(const char* msg) ST_NOEXCEPT 
			: http_base(), m_method(), m_uri() {
			if (!parse(msg))
				ST_REPORT_ERROR("Error parsing request!");
		}
		ST_INLINE request(const stulu::string msg) ST_NOEXCEPT
			: http_base(), m_method(), m_uri() {
			if (!parse(msg.c_str()))
				ST_REPORT_ERROR("Error parsing request!");
		}
		ST_INLINE ~request() ST_NOEXCEPT {
			clear_lines();
		}

		ST_NODISCARD ST_INLINE bool parse(char const* msg) {
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
			m_method = method_from_str(scan_str);
			msg += stulu::strlen(scan_str) + 1;

			// read uri
			if (sscanf(msg, str_input_format, scan_str) != 1) {
				delete[] scan_str;
				return false;
			}
			m_uri = stulu::string(scan_str);
			msg += stulu::strlen(scan_str) + 1;

			// read http version
			if (sscanf(msg, str_input_format, scan_str) != 1) {
				delete[] scan_str;
				return false;
			}
			m_version = stulu::string(scan_str);
			msg += stulu::strlen(scan_str) + 2;

			read_lines(msg);

			if (m_content_length != 0) {
				const size_t originalLength = stulu::strlen(originalMsg);
				if (originalLength < m_content_length) {
					ST_REPORT_ERROR("Content-Length does not match");
				}
				else {
					const char* begin = originalMsg + (originalLength - m_content_length);
					m_content = stulu::string(begin, m_content_length);
				}
			}

			delete[] scan_str;
			return true;
		}
		ST_NODISCARD ST_INLINE stulu::string build() const {
			stulu::string str;
			str += method_to_string(m_method) + ' ' + m_uri + ' ' + m_version + '\n';

			for (line_data * line = m_data; line != nullptr; line = line->next) {
				str += line->name + ": " + line->value + '\n';
			}

			str += '\n';
			str += m_content;
			return str;
		}

		ST_INLINE void set_uri(const stulu::string& uri) ST_NOEXCEPT {
			m_uri = uri;
		}
		ST_INLINE void set_method(request_method method) ST_NOEXCEPT {
			m_method = method;
		}

		ST_NODISCARD ST_INLINE request_method get_method() const ST_NOEXCEPT {
			return m_method;
		}
		ST_NODISCARD ST_INLINE stulu::string get_uri() const ST_NOEXCEPT {
			return m_uri;
		}
	private:
		request_method m_method;
		stulu::string m_uri;
	};

	class response : public http_base {
	public:
		ST_CONSTEXPR response() ST_NOEXCEPT
			: http_base(), m_status(status::Invalid) { }

		ST_INLINE response(const char* msg) ST_NOEXCEPT
			: http_base(), m_status(status::Invalid) {
			if (!parse(msg))
				ST_REPORT_ERROR("Error parsing request!");
		}
		ST_INLINE response(const stulu::string msg) ST_NOEXCEPT
			: http_base(), m_status(status::Invalid) {
			if (!parse(msg.c_str()))
				ST_REPORT_ERROR("Error parsing request!");
		}
		ST_INLINE ~response() ST_NOEXCEPT {
			clear_lines();
		}

		ST_NODISCARD ST_INLINE bool parse(char const* msg) {
			const char* originalMsg = msg;

			// buffer with an max length of an http uri
			char* scan_str = new char[MAX_URI_LENGTH + 1];

			// read http version
			if (sscanf(msg, "%" MAX_URI_LENGTH_S "s", scan_str) != 1) {
				delete[] scan_str;
				return false;
			}
			m_version = stulu::string(scan_str);
			msg += stulu::strlen(scan_str) + 2;

			// read status
			int status_code = 0;
			if (sscanf(msg, "%" MAX_URI_LENGTH_S "d", &status_code) != 1) {
				delete[] scan_str;
				return false;
			}
			m_status = (status)status_code;
			msg += 4;

			// check status msg
			if (sscanf(msg, "%" MAX_URI_LENGTH_S "[^\n]", scan_str) != 1) {
				delete[] scan_str;
				return false;
			}
			if (status_to_string(m_status) != stulu::string(scan_str)) {
				ST_REPORT_ERROR("Status code and message do not match");
			}
			msg += stulu::strlen(scan_str) + 1;

			read_lines(msg);

			if (m_content_length != 0) {
				const size_t originalLength = stulu::strlen(originalMsg);
				if (originalLength < m_content_length) {
					ST_REPORT_ERROR("Content-Length does not match");
				}
				else {
					const char* begin = originalMsg + (originalLength - m_content_length);
					m_content = stulu::string(begin, m_content_length);
				}

			}

			delete[] scan_str;
			return true;
		}
		ST_NODISCARD ST_INLINE stulu::string build() const {
			stulu::string str;
			char statusNum[4] = "999";
			if ((uint16_t)m_status <= 999 && (uint16_t)m_status >= 100) {
				sprintf(statusNum, "%u", (uint32_t)m_status);
			}

			str += m_version + ' ' + statusNum + ' ' + status_to_string(m_status) + '\n';

			for (line_data* line = m_data; line != nullptr; line = line->next) {
				str += line->name + ": " + line->value + '\n';
			}

			str += '\n';
			str += m_content;
			return str;
		}
		ST_INLINE void set_status(status _status) ST_NOEXCEPT {
			m_status = _status;
		}

		ST_NODISCARD ST_INLINE status get_status() const ST_NOEXCEPT {
			return m_status;
		}
	private:
		status m_status;
	};
}
ST_STL_END