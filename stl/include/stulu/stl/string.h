#pragma once
#include "stulu/stl/cstddef.h"
#include "stulu/stl/allocator.h"
#include "stulu/stl/mem.h"

ST_STL_BEGIN

namespace Internal {
    template <class C, class I>
    struct t_char_traits {
        using char_type = C;
        using int_type = I;
        //using off_type = streamoff;
        //using pos_type = streampos;
        using state_type = mbstate_t;

		ST_NODISCARD ST_INLINE static int compare(const char_type* const left, const char_type* const right, size_t count) ST_NOEXCEPT {
			auto* s1 = left;
			auto* s2 = right;

			while (count-- > 0) {
				if (*s1++ != *s2++)
					return s1[-1] < s2[-1] ? -1 : 1;
			}
			return 0;
		}
		ST_NODISCARD ST_INLINE static size_t length(const char_type* ptr) ST_NOEXCEPT {
			const char_type* s;
			for (s = ptr; *s; ++s);
			return(s - ptr);
		}
		ST_NODISCARD ST_INLINE static char_type* copy(char_type* dest, const char_type* const src, size_t count) ST_NOEXCEPT {
			for (size_t i = 0; i != count; i++) {
				dest[i] = src[i];
			}
			return dest;
		}
    };

	template <class Traits>
	using traits_ptr = typename Traits::char_type const *;
}

template <class C>
struct char_traits : Internal::t_char_traits<C, int> {};



template <class Traits>
ST_CONSTEXPR bool traits_equal(
	const Internal::traits_ptr<Traits> left, const size_t lSize,
	const Internal::traits_ptr<Traits> right, const size_t rSize) ST_NOEXCEPT {
	return lSize == rSize && Traits::compare(left, right, lSize) == 0;
}

template <class C, class T = char_traits<C>, class A = allocator<C>>
class basic_string {
public:
    using value_type = C;
    using traits_type = T;
	using allocator_type = A;
	using size_type = typename allocator_type::size_type;
    using refrence = value_type&;
    using const_refrence = const value_type&;
    using pointer = typename allocator_type::pointer;
    using const_pointer = typename allocator_type::const_pointer;

    ST_INLINE ST_CONSTEXPR basic_string() ST_NOEXCEPT 
		: m_pointer(nullptr), m_size(0) {
		construct("", 0);
	}
	ST_INLINE ST_CONSTEXPR basic_string(ST_STL nullptr_t) ST_NOEXCEPT
		: m_pointer(nullptr), m_size(0) {
		construct("", 0);
	}
    ST_INLINE ST_CONSTEXPR basic_string(const basic_string& str) ST_NOEXCEPT {
		construct(str.m_pointer, str.m_size);
	}
	ST_INLINE ST_CONSTEXPR basic_string(basic_string&& other) ST_NOEXCEPT
		: m_pointer(nullptr), m_size(0) {
		ST_STL swap(m_pointer, other.m_pointer);
		ST_STL swap(m_size, other.m_size);
	}
	ST_INLINE ST_CONSTEXPR basic_string(const value_type* ptr) ST_NOEXCEPT
		: m_pointer(nullptr), m_size(0) {
		construct(ptr, traits_type::length(ptr));
	}
	ST_INLINE ST_CONSTEXPR basic_string(const value_type* ptr, size_type count) ST_NOEXCEPT
		: m_pointer(nullptr), m_size(count) {
		construct(ptr, count);
		null_termiate();
	}
	ST_INLINE ST_CONSTEXPR basic_string(size_type count) ST_NOEXCEPT
		: m_pointer(nullptr), m_size(count) {
		allocate(count);
		null_termiate();
	}

	ST_INLINE ~basic_string() ST_NOEXCEPT {
		deallocate();
	}

	ST_INLINE basic_string& operator=(const basic_string& other) {
		if (this == ST_STL addressof(other)) {
			return *this;
		}
		return assign(other.m_pointer, other.m_size);
	}
	ST_INLINE basic_string& operator=(basic_string&& other) ST_NOEXCEPT {
		if (this == ST_STL addressof(other)) {
			return *this;
		}
		ST_STL swap(m_pointer, other.m_pointer);
		ST_STL swap(m_size, other.m_size);
		return *this;
	}
	ST_INLINE basic_string& operator=(const value_type* const ptr) {
		return assign(ptr);
	}
	
	ST_INLINE basic_string& operator+=(const basic_string& right) {
		return this->append(right);
	}
	ST_INLINE basic_string& operator+=(const value_type* right) {
		return this->append(right);
	}
	ST_INLINE basic_string& operator+=(value_type right) {
		return this->append(right);
	}

	ST_INLINE basic_string& assign(const value_type* const ptr) {
		return assign(ptr, traits_type::length(ptr));
	}
	ST_INLINE basic_string& assign(const value_type* const ptr, size_type count) {
		if (count != m_size) {
			deallocate();
			allocate(count);
		}
		m_pointer = traits_type::copy(m_pointer, ptr, count + 1);
		return *this;
	}

	ST_INLINE basic_string& append(const basic_string& str) {
		return append(str.c_str(), str.length());
	}
	ST_INLINE basic_string& append(const value_type* str) {
		return append(str, traits_type::length(str));
	}
	ST_INLINE basic_string& append(const value_type* str, size_t subpos, size_t sublen) {
		return append(str + subpos, sublen);
	}
	ST_INLINE basic_string& append(const value_type* appendPtr, size_t appendSize) {
		if (appendSize == 0) {
			return *this;
		}

		const size_t oldSize = length();
		const size_t newSize = oldSize + appendSize;

		pointer oldPtr = m_pointer;
		allocate(newSize);

		(void)traits_type::copy(m_pointer, oldPtr, oldSize);
		(void)traits_type::copy(m_pointer + oldSize, appendPtr, appendSize);
		null_termiate();

		get_allocator().deallocate(oldPtr, oldSize + 1);

		return *this;
	}
	ST_INLINE basic_string& append(value_type c) {
		const size_t oldSize = length();

		pointer oldPtr = m_pointer;
		allocate(oldSize + 1);

		m_pointer = traits_type::copy(m_pointer, oldPtr, oldSize);
		m_pointer[oldSize] = c;
		null_termiate();

		get_allocator().deallocate(oldPtr, oldSize + 1);

		return *this;
	}
	ST_INLINE basic_string& append(size_t num, value_type c) {
		const size_t oldSize = length();

		pointer oldPtr = m_pointer;
		allocate(oldSize + num);

		m_pointer = traits_type::copy(m_pointer, oldPtr, oldSize);
		for(size_t i = 0; i < num; i++)
			m_pointer[oldSize + i] = c;
		null_termiate();

		get_allocator().deallocate(oldPtr, oldSize + 1);

		return *this;
	}

	ST_INLINE bool _Equal(const basic_string& right) const ST_NOEXCEPT {
		return traits_equal<traits_type>(m_pointer, m_size, right.m_pointer, right.m_size);
	}
	ST_INLINE bool _Equal(const value_type* const right) const ST_NOEXCEPT {
		return traits_equal<traits_type>(m_pointer, m_size, right, traits_type::length(right));
	}

	ST_NODISCARD ST_INLINE value_type& at(const size_type off) {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(off < m_size, "Requested offset out of string range");
#endif
		return m_pointer[off];
	}
	ST_NODISCARD ST_INLINE const value_type at(const size_type off) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(off < m_size, "Requested offset out of string range");
#endif
		return m_pointer[off];
	}
	ST_NODISCARD ST_INLINE value_type& operator[](const size_type off) {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(off < m_size, "Requested offset out of string range");
#endif
		return m_pointer[off];
	}
	ST_NODISCARD ST_INLINE const value_type operator[](const size_type off) const {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(off < m_size, "Requested offset out of string range");
#endif
		return m_pointer[off];
	}

	ST_NODISCARD ST_INLINE value_type& front() ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
		ST_ASSERT(m_size != 0, "String is empty");
#endif
		return *m_pointer;
	}
	ST_NODISCARD ST_INLINE const value_type front() const ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
		ST_ASSERT(m_size != 0, "String is empty");
#endif
		return *m_pointer;
	}

	ST_NODISCARD ST_INLINE value_type& back() ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
		ST_ASSERT(m_size != 0, "String is empty");
#endif
		return *(m_pointer + (m_size - 1));
	}
	ST_NODISCARD ST_INLINE const value_type back() const ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
		ST_ASSERT(m_size != 0, "String is empty");
#endif
		return *(m_pointer + (m_size - 1));
	}

	ST_NODISCARD ST_INLINE bool empty() const ST_NOEXCEPT {
		return m_size == 0;
	}
	ST_NODISCARD ST_CONSTEXPR size_type max_size() const ST_NOEXCEPT {
		return get_allocator().max_size();
	}
	ST_NODISCARD ST_INLINE size_type capacity() const ST_NOEXCEPT {
		return m_size;
	}
	ST_NODISCARD ST_INLINE size_type size() const ST_NOEXCEPT {
		return m_size;
	}
	ST_NODISCARD ST_INLINE size_type length() const ST_NOEXCEPT {
		return m_size;
	}
	ST_NODISCARD ST_INLINE value_type* data() ST_NOEXCEPT {
		return m_pointer;
	}
	ST_NODISCARD ST_INLINE const value_type* data() const ST_NOEXCEPT {
		return m_pointer;
	}
	ST_NODISCARD ST_INLINE const value_type* c_str() const noexcept {
		return m_pointer;
	}
	ST_NODISCARD ST_INLINE allocator_type get_allocator() const ST_NOEXCEPT {
		return *(allocator_type*)m_pointer;
	}

private:
	ST_INLINE void allocate(size_type count) {
		m_pointer = get_allocator().allocate(count + 1);
		m_size = count;
	}
	ST_INLINE void deallocate() {
		if (m_pointer) {
			get_allocator().deallocate(m_pointer, m_size + 1);
		}
		m_pointer = nullptr;
		m_size = 0;
	}

	ST_INLINE void null_termiate() {
		m_pointer[m_size] = '\0';
	}

	ST_INLINE void construct(const value_type* const ptr, size_type count) {
		m_pointer = traits_type::copy(get_allocator().allocate(count + 1), ptr, count + 1);
		m_size = count;
	}

	pointer m_pointer;
	size_type m_size;
};

template <class C, class T = char_traits<C>, class A = allocator<C>>
ST_NODISCARD ST_CONSTEXPR bool operator==(const basic_string<C, T, A>& left, const basic_string<C, T, A>& right) ST_NOEXCEPT {
	return left._Equal(right);
}
template <class C, class T = char_traits<C>, class A = allocator<C>>
ST_NODISCARD ST_CONSTEXPR bool operator==(const basic_string<C, T, A>& left, const C* const right) ST_NOEXCEPT {
	return left._Equal(right);
}
template <class C, class T = char_traits<C>, class A = allocator<C>>
ST_NODISCARD ST_CONSTEXPR bool operator==(const C* const left, const basic_string<C, T, A>& right) ST_NOEXCEPT {
	return right._Equal(left);
}

template <class C, class T = char_traits<C>, class A = allocator<C>>
ST_NODISCARD ST_CONSTEXPR bool operator!=(const basic_string<C, T, A>& left, const basic_string<C, T, A>& right) ST_NOEXCEPT {
	return !(left == right);
}
template <class C, class T = char_traits<C>, class A = allocator<C>>
ST_NODISCARD ST_CONSTEXPR bool operator!=(const basic_string<C, T, A>& left, const C* const right) ST_NOEXCEPT {
	return !(left == right);
}
template <class C, class T = char_traits<C>, class A = allocator<C>>
ST_NODISCARD ST_CONSTEXPR bool operator!=(const C* const left, const basic_string<C, T, A>& right) ST_NOEXCEPT {
	return !(left == right);
}

template <class C, class T = char_traits<C>, class A = allocator<C>>
ST_NODISCARD ST_CONSTEXPR basic_string<C, T, A> operator+(const basic_string<C, T, A>& left, const basic_string<C, T, A>& right) {
	ST_STL string str(left);
	return str.append(right);
}
template <class C, class T = char_traits<C>, class A = allocator<C>>
ST_NODISCARD ST_CONSTEXPR basic_string<C, T, A> operator+(const basic_string<C, T, A>& left, const C* const right) {
	ST_STL string str(left);
	return str.append(right);
}
template <class C, class T = char_traits<C>, class A = allocator<C>>
ST_NODISCARD ST_CONSTEXPR basic_string<C, T, A> operator+(const basic_string<C, T, A>& left, C right) {
	ST_STL string str(left);
	return str.append(right);
}

using string = basic_string<char, char_traits<char>, allocator<char>>;

namespace literals {
	namespace string_literals {
		ST_NODISCARD ST_INLINE string operator""_s(const char* ptr, size_t count) {
			return string(ptr, count);
		}
	}
}



ST_STL_END