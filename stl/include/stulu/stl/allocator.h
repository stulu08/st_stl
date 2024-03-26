#pragma once
#include "cstddef.h"
#include "assert.h"
#include "cstring.h"

ST_STL_BEGIN

// for implementation
#if ST_ADDITIONAL_FEATURES && !defined(ST_ADDITIONAL_ALLOCATOR_FEATURES)
#define ST_ADDITIONAL_ALLOCATOR_FEATURES 1
#endif // ST_ADDITIONAL_FEATURES

template<class T>
class allocator {
public:
	using value_type = T;
	using size_type = ST_DEFAULT_SIZE_TYPE;

	ST_CONSTEXPR allocator() ST_NOEXCEPT {}
	ST_CONSTEXPR allocator(const allocator&) ST_NOEXCEPT = default;
	template <class Other>
	ST_CONSTEXPR allocator(const allocator<Other>&) ST_NOEXCEPT {}
	ST_INLINE ~allocator() = default;
	ST_INLINE allocator& operator=(const allocator&) ST_NOEXCEPT = default;


	ST_NODISCARD_MEMLEAK ST_INLINE value_type* allocate(const size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(count > 0, "Can't allocate with a count of zero");
		ST_ASSERT(sizeof(value_type) > 0, "Can't allocate with an incomplete type");
#endif
		return static_cast<value_type*>(::operator new(static_cast<size_t>(count * sizeof(value_type))));
	}
	ST_NODISCARD_MEMLEAK ST_INLINE value_type* allocate_aligned(const size_type count, const size_type align) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(count > 0, "Can't allocate with a count of zero");
		ST_ASSERT(align > 0, "Can't allocate with an align of zero");
		ST_ASSERT(sizeof(value_type) > 0, "Can't allocate with an incomplete type");
#endif
		return static_cast<value_type*>(::operator new(static_cast<size_t>(count * sizeof(value_type)), ST_CPP_STD align_val_t{ align }));
	}
	ST_INLINE void deallocate(value_type* ptr, const size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(ptr, "Can't deallocate a nullptr");
		ST_ASSERT(count > 0, "Can't deallocate a zero sized memory block");
		ST_ASSERT(sizeof(value_type) > 0, "Can't deallocate with an incomplete type");
#endif
		::operator delete(ptr, static_cast<size_t>(count * sizeof(value_type)));
	}
	ST_INLINE void destroy(value_type* ptr) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(ptr, "Can't destroy a nullptr");
#endif
		ptr->~value_type();
	}
	template <class... Types>
	ST_INLINE void construct(value_type* ptr, Types&&... Args) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(ptr, "Can't construct a nullptr");
#endif
		// call constructor without allocating memory
		// new (addres) type(constructor params);
		::new (ptr) value_type(static_cast<Types&&>(Args)...);
	}
	ST_NODISCARD ST_CONSTEXPR size_type max_size() const ST_NOEXCEPT {
		return static_cast<size_type>(static_cast<size_t>(-1) / sizeof(value_type));
	}
#if ST_ADDITIONAL_ALLOCATOR_FEATURES
	ST_INLINE void copy(value_type* dest, const value_type* src, size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(src, "Can't copy a nullptr");
		ST_ASSERT(dest, "Can't copy to a nullptr");
		ST_ASSERT(count > 0, "Can't copy a zero sized memory block");
#endif
		ST_STL memcpy(dest, src, static_cast<size_t>(count * sizeof(value_type)));
	}

	ST_INLINE void move(value_type* dest, const value_type* src, size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(src, "Can't copy a nullptr");
		ST_ASSERT(dest, "Can't copy to a nullptr");
		ST_ASSERT(count > 0, "Can't copy a zero sized memory block");
#endif
		ST_STL memmove(dest, src, static_cast<size_t>(count * sizeof(value_type)));
	}
	ST_INLINE void safe_copy(value_type* dest, const value_type* src, size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(src, "Can't copy a nullptr");
		ST_ASSERT(dest, "Can't copy to a nullptr");
		ST_ASSERT(count > 0, "Can't copy a zero sized memory block");
#endif
		ST_STL memcpy_s(dest, count * sizeof(value_type), src, count * sizeof(value_type));
	}
	ST_INLINE void safe_move(value_type* dest, const value_type* src, size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(src, "Can't copy a nullptr");
		ST_ASSERT(dest, "Can't copy to a nullptr");
		ST_ASSERT(count > 0, "Can't copy a zero sized memory block");
#endif
		ST_STL memmove_s(dest, count * sizeof(value_type), src, count * sizeof(value_type));
	}
#endif // ST_ADDITIONAL_ALLOCATOR_FEATURES
	ST_ADDITIONAL_FEATURES_ENABLED_IMPL_FUNC(ST_ADDITIONAL_ALLOCATOR_FEATURES)
};
// replaces every sizeof(value_type) with 1
template<>
class allocator<void> {
public:
	using value_type = void;
	using size_type = ST_DEFAULT_SIZE_TYPE;

	ST_CONSTEXPR allocator() ST_NOEXCEPT {}
	ST_CONSTEXPR allocator(const allocator&) ST_NOEXCEPT = default;
	template <class Other>
	ST_CONSTEXPR allocator(const allocator<Other>&) ST_NOEXCEPT {}
	ST_INLINE ~allocator() = default;
	ST_INLINE allocator& operator=(const allocator&) ST_NOEXCEPT = default;


	ST_NODISCARD_MEMLEAK ST_INLINE value_type* allocate(const size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(count > 0, "Can't allocate with a count of zero");
#endif
		return static_cast<value_type*>(::operator new(static_cast<size_t>(count)));
	}
	ST_NODISCARD_MEMLEAK ST_INLINE value_type* allocate_aligned(const size_type count, const size_type align) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(count > 0, "Can't allocate with a count of zero");
		ST_ASSERT(align > 0, "Can't allocate with an align of zero");
#endif
		return static_cast<value_type*>(::operator new(static_cast<size_t>(count), ST_CPP_STD align_val_t{ align }));
	}
	ST_INLINE void deallocate(value_type* ptr, const size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(ptr, "Can't deallocate a nullptr");
		ST_ASSERT(count > 0, "Can't deallocate a zero sized memory block");
#endif
		::operator delete(ptr, static_cast<size_t>(count));
}
	ST_INLINE void destroy(value_type* ptr) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(ptr, "Can't destroy a nullptr");
#endif
		ptr->~value_type();
	}
	template <class... Types>
	ST_INLINE void construct(value_type* ptr, Types&&... Args) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(ptr, "Can't construct a nullptr");
#endif
		// call constructor without allocating memory
		// new (addres) type(constructor params);
		::new (ptr) value_type(static_cast<Types&&>(Args)...);
	}
	ST_NODISCARD ST_INLINE ST_CONSTEXPR size_type max_size() const ST_NOEXCEPT {
		return static_cast<size_type>(static_cast<size_t>(-1) / 1);
	}
#if ST_ADDITIONAL_ALLOCATOR_FEATURES
	ST_INLINE void copy(value_type* dest, const value_type* src, size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(src, "Can't copy a nullptr");
		ST_ASSERT(dest, "Can't copy to a nullptr");
		ST_ASSERT(count > 0, "Can't copy a zero sized memory block");
#endif
		ST_STL memcpy(dest, src, static_cast<size_t>(count));
	}

	ST_INLINE void move(value_type* dest, const value_type* src, size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(src, "Can't copy a nullptr");
		ST_ASSERT(dest, "can't copy to a nullptr");
		ST_ASSERT(count > 0, "Can't copy a zero sized memory block");
#endif
		ST_STL memmove(dest, src, static_cast<size_t>(count));
	}
	ST_INLINE void safe_copy(value_type* dest, const value_type* src, size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(src, "Can't copy a nullptr");
		ST_ASSERT(dest, "Can't copy to a nullptr");
		ST_ASSERT(count > 0, "Can't copy a zero sized memory block");
#endif
		ST_STL memcpy_s(dest, count, src, count);
	}
	ST_INLINE void safe_move(value_type* dest, const value_type* src, size_type count) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(src, "Can't copy a nullptr");
		ST_ASSERT(dest, "Can't copy to a nullptr");
		ST_ASSERT(count > 0, "Can't copy a zero sized memory block");
#endif
		ST_STL memmove_s(dest, count, src, count);
	}
#endif // ST_ADDITIONAL_ALLOCATOR_FEATURES
	ST_ADDITIONAL_FEATURES_ENABLED_IMPL_FUNC(ST_ADDITIONAL_ALLOCATOR_FEATURES)
};

ST_STL_END