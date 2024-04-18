#pragma once
#include "stulu/stl/cstddef.h"
#include "stulu/stl/cassert.h"
#include "stulu/stl/type_traits.h"
#include "stulu/stl/memory.h"

ST_STL_BEGIN

namespace internal {
	template<class T>
	struct VectorStorage {
		using value_type = T;

		// first element of the vector
		value_type* first = nullptr;
		// last element of the vector + 1 (this - first = size)
		value_type* last = nullptr;
		// the last allocated but unused element + 1 (this - first = capacity)
		value_type* end = nullptr;
	};

	struct EmptyType {
	public:
		explicit EmptyType() = default;
	};
}


template<class T, typename A = ST_STL allocator<T>>
class vector {
public:
	using value_type = T;
	using allocator_type = A;
	using storage_type = ST_STL internal::VectorStorage<T>;
	using size_type = size_t;

	ST_INLINE ST_CONSTEXPR vector() ST_NOEXCEPT {}
	ST_INLINE vector(const vector& copy) {
		copy_from_other(copy);
	}
	ST_INLINE vector(vector&& src) ST_NOEXCEPT
		: m_data(storage_type()) {
		ST_STL swap(m_data, src.m_data);
	};
	ST_INLINE vector(const size_type count) {
		resize(count);
	}
	
	ST_INLINE ~vector() {
		if (m_data.first && capacity() > 0) {
			get_allocator().deallocate(m_data.first, capacity());
		}
	}
	
	ST_INLINE vector& operator=(const vector& right) {
		if (this == ST_STL addressof(right)) {
			return *this;
		}
		copy_from_other(right);
		return *this;
	}
	ST_INLINE vector& operator=(vector&& right) ST_NOEXCEPT {
		if (this == ST_STL addressof(right)) {
			return *this;
		}
		ST_STL swap(m_data, src.m_data);
		return *this;
	}

	ST_INLINE void reserve(const size_type count) {
		const size_type newCap = count;
		const size_type oldCap = capacity();

#if ST_DEBUG_LEVEL > 0
		if (newCap < oldCap) {
			ST_REPORT_ERROR("Can't reserve with a capacity lower then the old capacity");
		}
		else 
#endif
		if(newCap > oldCap) {
			reallocate(newCap);
		}
		// oldCap == newCap no reserve needed
	}

	template<class fill_type>
	ST_INLINE void resize(const size_type count, const fill_type& fill) {
		const size_type newSize = count;
		const size_type oldSize = size();
		const size_type oldCap = capacity();

		// erease element
		if (newSize < oldSize) {
			value_type* newLast = m_data.first + newSize;
			destroy_range(newLast, m_data.last);
			m_data.last = newLast;
			return;
		}
		// append elements
		if (newSize > oldSize) {
			value_type* oldFirst = m_data.first;
			if (newSize > oldCap) {
				resize_reallocate(newSize);
			}

			if constexpr (is_same<fill_type, value_type>) {
				m_data.last = uninitlize_fill_range(m_data.last, newSize - oldSize, fill);
			}
			else {
				m_data.last = construct_fill_range(m_data.last, newSize - oldSize);
			}
			if (oldFirst != nullptr && oldCap > 0) {
				get_allocator().deallocate(oldFirst, oldCap);
			}
			return;
		}
		// oldSize == newSize no resize needed
	}
	ST_INLINE void resize(const size_type count) {
		resize(count, internal::EmptyType());
	}

	ST_INLINE void clear() {
		resize(0);
	}

	ST_INLINE void shrink_to_fit() {
		const size_type oldCap = capacity();
		const size_type newCap = size();

		if (newCap > oldCap) {
			// no shrinking needed
			return;
		}
		// free all memory
		if (newCap == 0) {
			get_allocator().deallocate(m_data.first, oldCap);
			m_data.last = nullptr;
			m_data.end = nullptr;
			m_data.first = nullptr;
			return;
		}
		
		reallocate(newCap);
	}

	ST_INLINE void push_back(const value_type& val) {
		const size_type _size = m_data.last - m_data.first;
		const size_type _cap = m_data.end - m_data.first;
		if (_size < _cap) {
			unused_capacity_emplace_back(val);
		}
		else {
			used_capacity_emplace_back(val);
		}
	}
	ST_INLINE void push_back(value_type&& val) {
		const size_type _size = m_data.last - m_data.first;
		const size_type _cap = m_data.end - m_data.first;
		if (_size < _cap) {
			unused_capacity_emplace_back(static_cast<value_type&&>(val));
		}
		else {
			used_capacity_emplace_back(static_cast<value_type&&>(val));
		}
	}

	ST_INLINE void pop_back() {
#if ST_DEBUG_LEVEL > 0
		ST_ASSERT(m_data.first != m_data.last, "Vector is empty, pop no possible");
#endif
		destroy_range(m_data.last - 1, m_data.last);
		m_data.last--;
	}

	ST_NODISCARD ST_INLINE value_type& at(const size_type pos) {
#if ST_DEBUG_LEVEL > 0
		if (static_cast<size_type>(m_data.last - m_data.first) <= pos) {
			ST_ASSERT(false, "Requested index out of vector capacity");
		}
#endif
		return m_data.first[pos];
	}
	ST_NODISCARD ST_INLINE const value_type& at(const size_type pos) const {
#if ST_DEBUG_LEVEL > 0
		if (static_cast<size_type>(m_data.last - m_data.first) <= pos) {
			ST_ASSERT(false, "Requested index out of vector capacity");
		}
#endif
		return m_data.first[pos];
	}

	ST_NODISCARD ST_INLINE value_type& operator[](const size_type pos) {
#if ST_DEBUG_LEVEL > 1
		if (static_cast<size_type>(m_data.last - m_data.first) <= pos) {
			ST_ASSERT(false, "Requested index out of vector capacity");
		}
#endif
		return m_data.first[pos];
	}
	ST_NODISCARD ST_INLINE const value_type& operator[](const size_type pos) const {
#if ST_DEBUG_LEVEL > 1
		if (static_cast<size_type>(m_data.last - m_data.first) <= pos) {
			ST_ASSERT(false, "Requested index out of vector capacity");
		}
#endif
		return m_data.first[pos];
	}

	ST_NODISCARD ST_INLINE value_type& front() ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
		ST_ASSERT(m_data.first != m_data.last, "Vector is empty");
#endif
		return *m_data.first;
	}
	ST_NODISCARD ST_INLINE const value_type& front() const ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
		ST_ASSERT(m_data.first != m_data.last, "Vector is empty");
#endif
		return *m_data.first;
	}

	ST_NODISCARD ST_INLINE value_type& back() ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
		ST_ASSERT(m_data.first != m_data.last, "Vector is empty");
#endif
		return *(m_data.last - 1);
	}
	ST_NODISCARD ST_INLINE const value_type& back() const ST_NOEXCEPT {
#if ST_DEBUG_LEVEL > 1
		ST_ASSERT(m_data.first != m_data.last, "Vector is empty");
#endif
		return *(m_data.last - 1);
	}

	ST_NODISCARD ST_INLINE bool empty() const ST_NOEXCEPT {
		return m_data.first == m_data.last;
	}
	ST_NODISCARD ST_CONSTEXPR size_type max_size() const ST_NOEXCEPT {
		return get_allocator().max_size();
	}
	ST_NODISCARD ST_INLINE size_type capacity() const ST_NOEXCEPT {
		return static_cast<size_type>((m_data.end - m_data.first));
	}
	ST_NODISCARD ST_INLINE size_type size() const ST_NOEXCEPT {
		return static_cast<size_type>((m_data.last - m_data.first));
	}
	ST_NODISCARD ST_INLINE value_type* data() ST_NOEXCEPT {
		return m_data.first;
	}
	ST_NODISCARD ST_INLINE const value_type* data() const ST_NOEXCEPT {
		return m_data.first;
	}
	ST_NODISCARD ST_INLINE allocator_type get_allocator() const ST_NOEXCEPT {
		return *(allocator_type*)m_data.first;
	}
private:
	// same way as msvc (geometric growth)
	ST_NODISCARD ST_INLINE size_type calculate_growth(const size_type newSize) const ST_NOEXCEPT {
		const size_type oldCap = capacity();
		const size_type max = max_size();
		if (oldCap > max - oldCap / 2) {
			return max;
		}
		const size_type geometric = oldCap + oldCap / 2;
		if (geometric < newSize) {
			return newSize;
		}
		return geometric;
	}

	ST_INLINE void reallocate(const size_type newCap) {
		const size_type oldCap = capacity();
		const size_type _size = size();

		value_type* oldFirst = m_data.first;
		value_type* newFirst = get_allocator().allocate(newCap);

		m_data.last = newFirst;
		m_data.end = newFirst + newCap;
		m_data.first = newFirst;

		if (oldFirst != nullptr && oldCap > 0) {
			for (value_type* _first = oldFirst; _first != oldFirst + _size; _first++) {
				unused_capacity_emplace_back(static_cast<value_type&&>(*_first));
			}

			get_allocator().deallocate(oldFirst, oldCap);
		}
	}
	ST_INLINE void resize_reallocate(const size_type newSize) {
		const size_type oldSize = size();
		const size_type oldCap = capacity();
		const size_type newCap = calculate_growth(newSize);

		value_type* oldFirst = m_data.first;
		value_type* newFirst = get_allocator().allocate(newCap);

		m_data.last = newFirst;
		m_data.end = newFirst + newCap;
		m_data.first = newFirst;

		if (oldFirst != nullptr && oldCap > 0) {
			for (value_type* _first = oldFirst; _first != oldFirst + oldSize; _first++) {
				unused_capacity_emplace_back(static_cast<value_type&&>(*_first));
			}
		}
	}

	ST_INLINE void copy_from_other(const vector& copy) {
		const size_type cap = copy.capacity();
		const size_type size = copy.size();
		resize(size);
		if (cap > size)
			reserve(cap);

#if ST_ADDITIONAL_ALLOCATOR_FEATURES
		get_allocator().move(m_data.first, copy.m_data.first, cap);
#else
		ST_STL memmove(m_data.first, copy.m_data.first, sizeof(value_type) * cap);
#endif
	}

	template <class... Types>
	ST_INLINE void unused_capacity_emplace_back(Types&&... Args) {
#if ST_DEBUG_LEVEL > 1
		const size_type _size = m_data.last - m_data.first;
		const size_type _cap = m_data.end - m_data.first;
		if (_size >= _cap) {
			ST_ASSERT(false, "No capacity for unused_capacity_emplace_back");
			return;
		}
#endif
		get_allocator().construct(m_data.last, static_cast<Types&&>(Args)...);
		m_data.last++;
	}
	template <class... Types>
	ST_INLINE void used_capacity_emplace_back(Types&&... Args) {
		const size_type oldSize = size();
		const size_type newSize = oldSize + 1;
		const size_type newCap = calculate_growth(newSize);
		reallocate(newCap);

		get_allocator().construct(m_data.last, static_cast<Types&&>(Args)...);
		m_data.last++;
	}

	template<class Val>
	ST_NODISCARD ST_INLINE value_type* uninitlize_fill_range(value_type* first, const size_type count, const Val& filler) {
		for (size_type i = 0; i < count; i++) {
#if ST_ADDITIONAL_ALLOCATOR_FEATURES
			get_allocator().move(first, &filler, 1);
#else
			ST_STL memmove(first, &filler, sizeof(value_type));
#endif
			first++;
		}
		return first;
	}
	ST_NODISCARD ST_INLINE value_type* construct_fill_range(value_type* first, const size_type count) {
		for (size_type i = 0; i < count; i++) {
			get_allocator().construct(first);
			first++;
		}
		return first;
	}
	ST_INLINE void destroy_range(value_type* first, value_type* last) {
		for (; first != last; first++) {
			get_allocator().destroy(first);
		}
	}

private:
	storage_type m_data;
};

ST_STL_END