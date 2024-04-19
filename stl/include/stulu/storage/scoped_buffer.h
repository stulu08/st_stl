#pragma once
#include "stulu/core/core.h"
#include "stulu/stl/memory.h"
ST_STL_BEGIN

template<class T, class A = allocator<T>>
class scoped_buffer {
public:
	using value_type = T;
	using allocator_type = A;
	using size_type = typename allocator_type::size_type;
	using pointer = typename allocator_type::pointer;
	using const_pointer = typename allocator_type::const_pointer;
	using refrence = typename allocator_type::refrence;
	using const_refrence = typename allocator_type::const_refrence;

	ST_CONSTEXPR scoped_buffer() ST_NOEXCEPT
		: m_ptr(nullptr) { }
	ST_CONSTEXPR scoped_buffer(nullptr_t) ST_NOEXCEPT
		: m_ptr(nullptr) { }
	ST_CONSTEXPR scoped_buffer(pointer ptr) ST_NOEXCEPT
		: m_ptr(ptr) {}
	ST_CONSTEXPR scoped_buffer(scoped_buffer&& other) ST_NOEXCEPT
		: m_ptr(other.release()) { }

	ST_INLINE ~scoped_buffer() ST_NOEXCEPT {
		if (m_ptr) {
			get_allocator().deallocate(m_ptr, size());
		}
	}

	ST_INLINE scoped_buffer& operator=(scoped_buffer&& other) ST_NOEXCEPT {
		if (this == ST_STL addressof(other)) {
			return *this;
		}
		reset(other.release());
		return *this;
	}
	ST_INLINE scoped_buffer& operator=(nullptr_t) ST_NOEXCEPT {
		reset();
		return *this;
	}

	ST_INLINE pointer release() ST_NOEXCEPT {
		return ST_STL exchange(m_ptr, nullptr);
	}

	ST_INLINE void reset(pointer ptr = nullptr) ST_NOEXCEPT {
		pointer old = ST_STL exchange(m_ptr, ptr);
		if (old) {
			get_allocator().deallocate(m_ptr, size());
		}
	}

	ST_INLINE scoped_buffer(const scoped_buffer&) ST_NOEXCEPT = delete;
	ST_INLINE scoped_buffer& operator=(const scoped_buffer&) ST_NOEXCEPT = delete;

	ST_NODISCARD ST_INLINE bool empty() const ST_NOEXCEPT {
		return m_ptr == nullptr;
	}
	ST_NODISCARD ST_CONSTEXPR size_type max_size() const ST_NOEXCEPT {
		return get_allocator().max_size();
	}
	ST_NODISCARD ST_CONSTEXPR size_type size() const ST_NOEXCEPT {
		return sizeof(value_type);
	}
	ST_NODISCARD ST_INLINE allocator_type get_allocator() const ST_NOEXCEPT {
		return *(allocator_type*)m_ptr;
	}
	ST_NODISCARD ST_INLINE pointer get() ST_NOEXCEPT {
		return m_ptr;
	}
	ST_NODISCARD ST_INLINE const pointer get() const ST_NOEXCEPT {
		return m_ptr;
	}
	ST_NODISCARD ST_INLINE add_lvalue_reference_t<value_type> operator*() const ST_NOEXCEPT(ST_NOEXCEPT(*ST_STL declval<pointer>())) {
		return *m_ptr;
	}
	ST_NODISCARD ST_INLINE pointer operator->() ST_NOEXCEPT {
		return m_ptr;
	}
	ST_NODISCARD ST_INLINE const pointer operator->() const ST_NOEXCEPT {
		return m_ptr;
	}

	ST_INLINE explicit operator bool() const ST_NOEXCEPT {
		return static_cast<bool>(m_ptr);
	}

private:
	pointer m_ptr;
};

ST_STL_END