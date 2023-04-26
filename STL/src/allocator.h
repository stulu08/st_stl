#pragma once
#include <Stulu.h>

ST_STL_BEGIN

template<class T>
class allocator {
public:
	using value_type = T;
	using size_type = ST_DEFAULT_SIZE_TYPE;

	constexpr allocator() ST_NOEXCEPT {}
	constexpr allocator(const allocator&) ST_NOEXCEPT = default;
	template <class Other>
	constexpr allocator(const allocator<Other>&) ST_NOEXCEPT {}
	ST_VIRTUAL ST_INLINE ~allocator() = default;
	ST_VIRTUAL ST_INLINE allocator& operator=(const allocator&) = default;


	ST_NODISCARD_MEMLEAK ST_VIRTUAL ST_INLINE value_type* allocate(const size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(count > 0, "Can't allocate with a count of zero");
		ST_ASSERT(sizeof(value_type) > 0, "Can't allocate with an incomplete type");
#endif
		return static_cast<value_type*>(::operator new(count * sizeof(value_type)));
	}
	ST_NODISCARD_MEMLEAK ST_VIRTUAL ST_INLINE value_type* allocate_aligned(const size_type count, const size_type align) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(count > 0, "Can't allocate with a count of zero");
		ST_ASSERT(align > 0, "Can't allocate with an align of zero");
		ST_ASSERT(sizeof(value_type) > 0, "Can't allocate with an incomplete type");
#endif
		return static_cast<value_type*>(::operator new(count * sizeof(value_type), std::align_val_t{align}));
	}
	ST_VIRTUAL ST_INLINE void deallocate(value_type* ptr, const size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(ptr, "Can't deallocate a nullptr");
		ST_ASSERT(count > 0, "Can't deallocate a zero sized memory block");
		ST_ASSERT(sizeof(value_type) > 0, "Can't deallocate with an incomplete type");
#endif
		::operator delete(ptr, count * sizeof(value_type));
	}
	ST_VIRTUAL ST_INLINE void destroy(value_type* ptr) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(ptr, "Can't destroy a nullptr");
#endif
		ptr->~value_type();
	}
	template <class... Types>
	ST_INLINE void construct(value_type* ptr, Types&&... Args) const {
		// call constructor without allocation memory
		// new (addres) type(constructor params);
		::new (ptr) value_type(static_cast<Types&&>(Args)...);
	}
	ST_INLINE void copy(value_type* dest, const value_type* src, size_type count) const {
		ST_C_CALL memcpy_s(dest, count * sizeof(value_type), src, count * sizeof(value_type));
	}
};

template<>
class allocator<void> {
public:
	using value_type = void;
	using size_type = ST_DEFAULT_SIZE_TYPE;

	constexpr allocator() ST_NOEXCEPT {}
	constexpr allocator(const allocator&) ST_NOEXCEPT = default;
	template <class Other>
	constexpr allocator(const allocator<Other>&) ST_NOEXCEPT {}
	ST_VIRTUAL ST_INLINE ~allocator() = default;
	ST_VIRTUAL ST_INLINE allocator& operator=(const allocator&) = default;


	ST_NODISCARD_MEMLEAK ST_VIRTUAL ST_INLINE value_type* allocate(const size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(count > 0, "Can't allocate with a count of zero");
#endif
		return static_cast<void*>(::operator new(count));
	}
	ST_NODISCARD_MEMLEAK ST_VIRTUAL ST_INLINE value_type* allocate_aligned(const size_type count, const size_type align) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(count > 0, "Can't allocate with a count of zero");
		ST_ASSERT(align > 0, "Can't allocate with an align of zero");
#endif
		return static_cast<void*>(::operator new(count, std::align_val_t{ align }));
	}
	ST_VIRTUAL ST_INLINE void deallocate(value_type* ptr, const size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(ptr, "Can't deallocate a nullptr");
		ST_ASSERT(count > 0, "Can't deallocate a zero sized memory block");
#endif
		::operator delete(ptr, count);
	}
};

ST_STL_END