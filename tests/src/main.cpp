#include "tests/vector_tests.h"
#include "tests/array_tests.h"
#include "tests/unique_ptr_tests.h"
#include "tests/string_tests.h"
#include "tests/socket_tests.h"

#include <stulu/stulu.h>

#include <vector>
#include <string>
#include <iostream>

namespace net = stulu::networking;

int main(int argc, char** argv) {
	stulu::http::request request = R"(
POST / HTTP/1.1
User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
Host: www.tutorialspoint.com
Content-Type: application/x-www-form-urlencoded
Content-Length: 50
Accept-Language: en-us
Accept-Encoding: gzip, deflate
Connection: Keep-Alive

licenseID=string&content=string&/paramsXML=string
)";


	stulu::http::response response;
	response.set_version("HTTP/1.1");
	response.set_status(stulu::http::status::NotFound);
	response.add_line("Date", "Sun, 18 Oct 2012 10:36:20 GMT");
	response.add_line("Server", "Apache/2.2.14 (Win32)");
	response.add_line("Connection", "Closed");
	response.add_line("Content-Type", "text/html; charset=iso-8859-1");
	response.set_content(R"(
<html>
	<head>
		<title>404 Not Found</title>
	</head>
	<body>
		<div>
			<center>
				<p> 404 - Not Found </p>
			<center>
		</div>
	</body>
</html>
)");

	printf("Request: {\n%s}\nResponse: {\n%s}\n", request.build().c_str(), response.build().c_str());

	//return simple_tcp_echo_test();
	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}