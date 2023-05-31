#pragma once
#include <stulu/allocator.h>

ST_STL_BEGIN

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

template<class T, typename A = ST_STL allocator<T>>
class vector {
public:
	using value_type = T;
	using allocator_type = A;
	using storage_type = ST_STL VectorStorage<T>;
	using size_type = ST_DEFAULT_SIZE_TYPE;

	ST_INLINE constexpr vector() ST_NOEXCEPT {}
	ST_INLINE vector(const vector& copy) {
		copy_from_other(copy);
	}
	ST_INLINE vector(vector&& src) ST_NOEXCEPT {
		m_data = src.m_data;
		src.m_data = storage_type();
	};
	ST_INLINE vector(const size_type count) {
		reserve(count);
	}
	
	ST_INLINE ~vector() {
		if (m_data.first && capacity() > 0) {
			get_allocator().deallocate(m_data.first, capacity());
		}
	}
	
	ST_INLINE vector& operator=(const vector& right) {
		if (this == __builtin_addressof(right)) {
			return *this;
		}
		copy_from_other(right);
		return *this;
	}
	ST_INLINE vector& operator=(vector&& right) ST_NOEXCEPT {
		if (this == __builtin_addressof(right)) {
			return *this;
		}
		m_data = right.m_data;
		right.m_data = storage_type();
		return *this;
	}

	ST_INLINE void reserve(const size_type count) {
		const size_type newCap = count;
		const size_type oldCap = capacity();

		if (newCap < oldCap) {
#if ST_DEBUG_LEVEL > 0
			ST_REPORT_ERROR("Can't reserve with a capacity lower then the old capacity");
#endif
		}
		else if(newCap > oldCap) {
			value_type* newFirst = get_allocator().allocate(newCap);

			if (m_data.first != nullptr && oldCap > 0) {
				get_allocator().copy(newFirst, m_data.first, oldCap);
				get_allocator().deallocate(m_data.first, oldCap);
			}
			m_data.last = newFirst + size();

			m_data.end = newFirst + newCap;
			m_data.first = newFirst;
		}
		// oldCap == newCap no reserve needed
	}

	template<class fill_type>
	ST_INLINE void resize(const size_type count, const fill_type& fill) {
		const size_type newSize = count;
		const size_type oldSize = size();

		// erease element
		if (newSize < oldSize) {
			value_type* newLast = m_data.first + newSize;
			destroy_range(newLast, m_data.last);
			m_data.last = newLast;
			return;
		}
		// append elements
		if (newSize > oldSize) {
			if (newSize > capacity()) {
				reserve(newSize);
			}

			if constexpr (is_same<fill_type, value_type>) {
				m_data.last = uninitlize_fill_range(m_data.last, newSize - oldSize, fill);
			}
			else {
				m_data.last = construct_fill_range(m_data.last, newSize - oldSize);
			}
			return;
		}
		// oldSize == newSize no resize needed
	}
	ST_INLINE void resize(const size_type count) {
		resize(count, EmptyType());
	}

	ST_INLINE void clear() {
		resize(0);
	}
	// clear and then free the memory
	ST_INLINE void m_clear() {
		resize(0);
		shrink_to_fit();
	}

	ST_INLINE void shrink_to_fit() {
		const size_type _size = size();
		const size_type oldCap = capacity();
		const size_type newCap = _size;

		if (newCap > oldCap) {
			// no shrinking needed
			return;
		}
		// free all memory
		if (_size == 0) {
			get_allocator().deallocate(m_data.first, oldCap);
			m_data.last = nullptr;
			m_data.end = nullptr;
			m_data.first = nullptr;
			return;
		}
		value_type* newFirst = get_allocator().allocate(newCap);

		// TODO: May switch to moving each object using copy or move constructor
		if (m_data.first != nullptr) {
			get_allocator().copy(newFirst, m_data.first, newCap);
			get_allocator().deallocate(m_data.first, oldCap);
		}

		m_data.last = newFirst + _size;
		m_data.end = newFirst + newCap;
		m_data.first = newFirst;
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
	ST_INLINE value_type& at(const size_type pos) {
		if (static_cast<size_type>(m_data.last - m_data.first) <= pos) {
#if ST_DEBUG_LEVEL > 0
			ST_ASSERT(false, "Requested index out of vector capacity");
#endif
			return;
		}
		return m_data.first[pos];
	}
	ST_INLINE const value_type& at(const size_type _Pos) const {
		if (static_cast<size_type>(m_data.last - m_data.first) <= pos) {
#if ST_DEBUG_LEVEL > 0
			ST_ASSERT(false, "Requested index out of vector capacity");
#endif
			return;
		}
		return m_data.first[pos];
	}

	ST_INLINE constexpr size_type max_size() const ST_NOEXCEPT {
		return get_allocator().max_size();
	}
	ST_INLINE size_type capacity() const ST_NOEXCEPT {
		return static_cast<size_type>((m_data.end - m_data.first));
	}
	ST_INLINE size_type size() const ST_NOEXCEPT {
		return static_cast<size_type>((m_data.last - m_data.first));
	}
	ST_INLINE value_type* data() ST_NOEXCEPT {
		return m_data.first;
	}
	ST_INLINE const value_type* data() const ST_NOEXCEPT {
		return m_data.first;
	}
	ST_INLINE allocator_type get_allocator() const ST_NOEXCEPT {
		static allocator_type alloc;
		return alloc;
	}
private:
	// same way as msvc (geometric growth)
	ST_INLINE size_type calculate_growth(const size_type newSize) const {
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

	ST_INLINE void copy_from_other(const vector& copy) {
		resize(copy.size());
		if (copy.capacity() > copy.size())
			reserve(copy.capacity());
		get_allocator().copy(m_data.first, copy.m_data.first, copy.capacity());
	}

	template <class... Types>
	ST_INLINE void unused_capacity_emplace_back(Types&&... Args) {
		const size_type _size = m_data.last - m_data.first;
		const size_type _cap = m_data.end - m_data.first;
#if ST_DEBUG_LEVEL > 0
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
		reserve(newCap);

		unused_capacity_emplace_back(static_cast<Types&&>(Args)...);
	}

	template<class Val>
	ST_INLINE value_type* uninitlize_fill_range(value_type* first, const size_type count, const Val& filler) {
		for (size_type i = 0; i < count; i++) {
			ST_C_CALL memcpy_s(first, sizeof(value_type), &filler, sizeof(Val));
			first++;
		}
		return first;
	}
	ST_INLINE value_type* construct_fill_range(value_type* first, const size_type count) {
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