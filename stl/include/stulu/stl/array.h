#pragma once
#include "stulu/stl/cstddef.h"
#include "stulu/stl/cassert.h"
#include "stulu/stl/memory.h"

ST_STL_BEGIN

template<class T, size_t Size>
class array {
public:
	using element_type = T;
    using size_type = size_t;

    ST_CONSTEXPR void fill(const element_type& value) ST_NOEXCEPT {
        if constexpr (Size < 0)
            return;

        for (size_type i = 0; i < Size; i++) {
            *(m_elements + i) = value;
        }
    }

    ST_CONSTEXPR void swap(array& other) ST_NOEXCEPT {
        if constexpr (Size < 0)
            return;

        for (size_type i = 0; i < Size; i++) {
            ST_STL swap(this->m_elements[i], other.m_elements[i]);
        }
    }

    ST_NODISCARD ST_CONSTEXPR element_type& at(const size_type pos) {
#if ST_DEBUG_LEVEL > 0
        ST_ASSERT(pos < Size, "Requested index out of array capacity");
#endif
        return m_elements[pos];
    }
    ST_NODISCARD ST_CONSTEXPR const element_type& at(const size_type pos) const {
#if ST_DEBUG_LEVEL > 0
        ST_ASSERT(pos < Size, "Requested index out of array capacity");
#endif
        return m_elements[pos];
    }

    ST_NODISCARD ST_CONSTEXPR element_type& operator[](size_type pos) ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
        ST_ASSERT(pos < Size, "Requested index out of array capacity");
#endif
        return m_elements[pos];
    }
    ST_NODISCARD ST_CONSTEXPR const element_type& operator[](size_type pos) const ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
        ST_ASSERT(pos < Size, "Requested index out of array capacity");
#endif
        return m_elements[pos];
    }

    ST_NODISCARD ST_CONSTEXPR element_type& front() ST_NOEXCEPT {
        return m_elements[0];
    }
    ST_NODISCARD ST_CONSTEXPR const element_type& front() const ST_NOEXCEPT {
        return m_elements[0];
    }

    ST_NODISCARD ST_CONSTEXPR element_type& back() ST_NOEXCEPT {
        return m_elements[Size - 1];
    }
    ST_NODISCARD ST_CONSTEXPR const element_type& back() const ST_NOEXCEPT {
        return m_elements[Size - 1];
    }

    ST_NODISCARD ST_CONSTEXPR element_type* data() ST_NOEXCEPT {
        return m_elements;
    }
    ST_NODISCARD ST_CONSTEXPR const element_type* data() const ST_NOEXCEPT {
        return m_elements;
    }

    ST_NODISCARD ST_CONSTEXPR size_type size() const ST_NOEXCEPT {
        return Size;
    }

    ST_NODISCARD ST_CONSTEXPR size_type capacity() const ST_NOEXCEPT {
        return Size;
    }

    ST_NODISCARD ST_CONSTEXPR size_type max_size() const ST_NOEXCEPT {
        return Size;
    }

    ST_NODISCARD ST_CONSTEXPR bool empty() const ST_NOEXCEPT {
        return false;
    }

    element_type m_elements[Size];
};
ST_STL_END