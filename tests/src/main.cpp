#include "tests/vector_tests.h"
#include "tests/array_tests.h"
#include "tests/unique_ptr_tests.h"
#include "tests/string_tests.h"
#include "tests/socket_tests.h"

#include <stulu/stulu.h>

#include <vector>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
	
	//socket_tests();
	//return 0;

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}