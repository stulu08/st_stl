#include "tests/vector_tests.h"
#include "tests/array_tests.h"
#include "tests/unique_ptr_tests.h"
#include "tests/string_tests.h"
#include "tests/socket_tests.h"
#include "tests/http_tests.h"

#include <stulu/stulu.h>

#include <iostream>

#include <list>

int main(int argc, char** argv) {

	//return socket_tests(argc);
	//return http_parse_test();
	//return simple_tcp_echo_test();

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
