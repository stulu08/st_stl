#include "vector.h"

#include "vector"
using namespace Stulu;


class STATS {
public:
	static inline uint32_t Allocations = 0;
	static inline uint32_t Deallocations = 0;
};

template<class T>
class custom_allocater : public Stulu::allocator<T> {
public:
	ST_NODISCARD_MEMLEAK virtual inline value_type* allocate(const size_type count) const override {
		STATS::Allocations++;
		return Stulu::allocator<T>::allocate(count);
	}
	virtual inline void deallocate(value_type* ptr, const size_type count) const override {
		STATS::Deallocations++;
		Stulu::allocator<T>::deallocate(ptr, count);
	}
};

#if 1
template<class T>
using st_vec = Stulu::vector<T, custom_allocater<T>>;
#else
template<class T>
using st_vec = std::vector<T>;
#endif

template<class T>
void dump_vector(const st_vec<T>& vec) {
	printf("capacity:	%d\n", (int)vec.capacity());
	printf("size:		%d\n", (int)vec.size());
	printf("data:		%p\n", (void*)vec.data());
	for (size_t i = 0; i < vec.size(); i++) {
		printf("%d: %d\n", (int)i, vec.data()[i]);
	}
	printf("--------------------------------------\n");
}

int main() {
	{
		st_vec<int> vec(10);



		dump_vector(vec);

		printf("Allocations/Deallocations: %d/%d\n", STATS::Allocations, STATS::Deallocations);
	}
	printf("Allocations/Deallocations: %d/%d\n", STATS::Allocations, STATS::Deallocations);

	return 0;
}
