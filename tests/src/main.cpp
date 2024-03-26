#define ST_ADDITIONAL_ALLOCATOR_FEATURES 0


#include "tests/vector_tests.h"
#include "tests/array_tests.h"
#include "tests/unique_ptr_tests.h"


int main(int argc, char** argv) {
	std::weak_ptr<int> e;

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}