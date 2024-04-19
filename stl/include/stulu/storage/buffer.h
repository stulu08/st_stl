#pragma once
#include "stulu/core/core.h"
#include "stulu/stl/memory.h"

ST_STL_BEGIN

template<class T, class A = allocator<T>>
class buffer {
public:
	using value_type = T;
	using allocator_type = A;
	using size_type = typename allocator_type::size_type;
	using pointer = typename allocator_type::pointer;
	using const_pointer = typename allocator_type::const_pointer;
	using refrence = typename allocator_type::refrence;
	using const_refrence = typename allocator_type::const_refrence;

	ST_CONSTEXPR buffer() ST_NOEXCEPT
		: m_ptr(nullptr), m_count(0) {}
	ST_INLINE buffer(const size_type count) ST_NOEXCEPT
		: m_ptr(nullptr), m_count(0) {
		reallocate(count);
	}
	ST_CONSTEXPR buffer(buffer&& other) ST_NOEXCEPT
		: m_ptr(nullptr), m_count(0) { 
		ST_STL swap(this->m_ptr, other->m_ptr);
		ST_STL swap(this->m_count, other->m_count);
	}

	ST_INLINE buffer& operator=(buffer&& other) ST_NOEXCEPT {
		if (this == ST_STL addressof(other)) {
			return *this;
		}
		ST_STL swap(this->m_ptr, other->m_ptr);
		ST_STL swap(this->m_count, other->m_count);
		return *this;
	}
	
	ST_INLINE void reallocate(const size_type count) ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(count > m_count, "Cant shrink a buffer");
#endif
		if (count == m_count)
			return;

		pointer newPtr = get_allocator().allocate(count);
		if (m_ptr) {
			MoveConstructTo(newPtr, m_count);
			get_allocator().deallocate(m_ptr, m_count);
		}
		
		m_count = count;
		m_ptr = newPtr;
	}

	ST_NODISCARD ST_INLINE bool empty() const ST_NOEXCEPT {
		return m_ptr == nullptr;
	}
	ST_NODISCARD ST_CONSTEXPR size_type max_size() const ST_NOEXCEPT {
		return get_allocator().max_size();
	}
	ST_NODISCARD ST_INLINE size_type size() const ST_NOEXCEPT {
		return m_count * sizeof(value_type);
	}
	ST_NODISCARD ST_INLINE size_type count() const ST_NOEXCEPT {
		return m_count;
	}
	ST_NODISCARD ST_INLINE allocator_type get_allocator() const ST_NOEXCEPT {
		return *(allocator_type*)m_ptr;
	}
	ST_NODISCARD ST_INLINE refrence operator[](size_type pos) ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
		ST_ASSERT(pos < m_count, "Requested index out of buffer size");
#endif
		return m_ptr[pos];
	}
	ST_NODISCARD ST_INLINE const const_refrence operator[](size_type pos) const ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
		ST_ASSERT(pos < m_count, "Requested index out of buffer size");
#endif
		return m_ptr[pos];
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
	ST_INLINE void CopyConstructTo(pointer* dest, const size_type count) const ST_NOEXCEPT {
		if (dest && m_count >= count) {
			for (size_type i = 0; i < count; i++) {
				get_allocator().construct(dest + i, ST_STL move(*(m_ptr + i)));
			}
		}
	}
	ST_INLINE void MoveConstructTo(pointer* dest, const size_type count) const ST_NOEXCEPT {
		if (dest && m_count >= count) {
			for (size_type i = 0; i < count; i++) {
				get_allocator().construct(dest + i, *(m_ptr + i));
			}
		}
	}


	pointer m_ptr;
	size_type m_count;
};

template<class T, class A = ST_STL allocator<T>>
using buffer = basic_buffer<T, A>;

using byte_buffer = buffer<byte>;
using char_buffer = buffer<char>;

using int_buffer = buffer<int>;
using int8_buffer = buffer<int8_t>;
using int16_buffer = buffer<int16_t>;
using int32_buffer = buffer<int32_t>;
using int64_buffer = buffer<int64_t>;

using uint_buffer = buffer<unsigned int>;
using uint8_buffer = buffer<uint8_t>;
using uint16_buffer = buffer<uint16_t>;
using uint32_buffer = buffer<uint32_t>;
using iunt64_buffer = buffer<uint64_t>;

using float_buffer = buffer<float>;
using double_buffer = buffer<double>;

ST_STL_END
