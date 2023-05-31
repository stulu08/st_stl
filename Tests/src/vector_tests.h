#include "utils.h"

#include <stulu/vector.h>

void TestVector() {
	class TestClass {
	public:
		TestClass() {
			this->isOrig = true;
			printf("TestClass()\n");
		}
		TestClass(const TestClass&) {
			this->isOrig = true;
			printf("TestClass(const TestClass&)\n");
		}
		TestClass(TestClass&& other) ST_NOEXCEPT {
			other.isOrig = false;
			this->isOrig = true;
			printf("TestClass(TestClass&&)\n");
		}
		~TestClass() {
			if (isOrig)
				printf("~TestClass()\n");
		}

		bool isOrig;
	};

	std::vector<TestClass> std_vec;
	stulu::vector<TestClass> stl_vec;

	printf("std::vector<TestClass> (%d bytes) vs stulu::vector<TestClass> (%d bytes)\n\n", (uint32_t)sizeof(std_vec), (uint32_t)sizeof(stl_vec));

	printf("reserve 2\n\n");
	std_vec.reserve(2);
	printf("--------------------\n");
	stl_vec.reserve(2);
	printf("\n\n");

	printf("resize 5\n\n");
	std_vec.resize(5);
	printf("--------------------\n");
	stl_vec.resize(5);
	printf("\n\n");

	printf("resize 3\n\n");
	std_vec.resize(3);
	printf("--------------------\n");
	stl_vec.resize(3);
	printf("\n\n");

	printf("push_back\n\n");
	std_vec.push_back(TestClass());
	printf("--------------------\n");
	stl_vec.push_back(TestClass());
	printf("\n\n");

	printf("clear\n\n");
	std_vec.clear();
	printf("--------------------\n");
	stl_vec.clear();
	printf("\n\n");

}
void VectorSpeedComaprison() {
	int64_t _std = 0;
	int64_t _stulu = 0;

	constexpr size_t itCount = 200000;
	constexpr size_t testCount = 100;
	printf("Testing with %d iterations a %d times (%llu total) for each vector\n", (int)itCount, (int)testCount, itCount * testCount);
	for (size_t i = 0; i < testCount; i++) {
		{
			Timer timer = Timer();
			stulu::vector<int> std_vec;
			for (size_t i = 0; i < itCount; i++) {
				std_vec.push_back((int)i);
			}
			std_vec.clear();
			_stulu += timer.Stop();
		}

		{
			Timer timer = Timer();
			std::vector<int> std_vec;
			for (size_t i = 0; i < itCount; i++) {
				std_vec.push_back((int)i);
			}
			std_vec.clear();
			_std += timer.Stop();

		}
	}


	printf("stulu		: %" PRId64 "mq\n", (int64_t)(_stulu / testCount));
	printf("std		: %" PRId64 "mq\n", (int64_t)(_std / testCount));

}