#include "utils.h"

#include <memory>

#define ST_ADDITIONAL_ALLOCATOR_FEATURES 0
#include <stulu/vector.h>

namespace VectorTestsImpl {
	template<class vec, class T = int>
	static inline int64_t TestFunc(const size_t itCount) {
		Timer timer = Timer();
		vec v;
		for (size_t i = 0; i < itCount; i++) {
			v.push_back(T());
		}
		v.clear();
		return timer.Stop();
	}
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
	class DataTestClass {
	public:
		DataTestClass() {
			this->isOrig = true;
			printf("DataTestClass()\n");
		}
		DataTestClass(const DataTestClass&) {
			this->isOrig = true;
			printf("DataTestClass(const DataTestClass&)\n");
		}
		DataTestClass(DataTestClass&& other) ST_NOEXCEPT {
			other.isOrig = false;
			this->isOrig = true;
			printf("DataTestClass(DataTestClass&&)\n");
		}
		~DataTestClass() {
			if (isOrig)
				printf("~DataTestClass()\n");
		}

		bool isOrig;
		int storage[100] = {};
	};
}

void TestVector() {
	

	std::vector<VectorTestsImpl::TestClass> std_vec;
	stulu::vector<VectorTestsImpl::TestClass> stl_vec;

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
	std_vec.push_back(VectorTestsImpl::TestClass());
	printf("--------------------\n");
	stl_vec.push_back(VectorTestsImpl::TestClass());
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

	using DataType = std::unique_ptr<VectorTestsImpl::DataTestClass>;

	constexpr size_t itCount = 200000;
	constexpr size_t testCount = 200;
	printf("Additional stulu::allocator features: %d\n\n", stulu::allocator<int>::AdditionalFeaturesEnable());

	printf("Testing with %d iterations a %d times (%llu total) for each vector\n", (int)itCount, (int)testCount, itCount * testCount);
	for (size_t i = 0; i < testCount; i++) {
		_stulu += VectorTestsImpl::TestFunc<stulu::vector<DataType>, DataType>(itCount);
		_std += VectorTestsImpl::TestFunc<std::vector<DataType>, DataType>(itCount);
	}

	printf("stulu		: %lldmq\n", (int64_t)(_stulu / testCount));
	printf("std		: %lldmq\n", (int64_t)(_std / testCount));

}