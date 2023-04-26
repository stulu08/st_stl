#pragma once
#include <allocator.h>


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
	using storage_type = VectorStorage<T>;
	using size_type = ST_DEFAULT_SIZE_TYPE;

	constexpr vector() ST_NOEXCEPT {}
	constexpr vector(const vector& copy) {
		copy_from(copy);
	}
	constexpr vector(vector&& src) ST_NOEXCEPT {
		m_data = src.m_data;
		src.m_data = storage_type();
	};

	ST_INLINE constexpr vector(const size_type count) {
		reserve(count);
	}
	
	ST_INLINE ~vector() {
		if (m_data.first && capacity() > 0) {
			allocator_type{}.deallocate(m_data.first, capacity());
		}
	}
	
	ST_INLINE vector& operator=(const vector& right) {
		if (this == __builtin_addressof(right)) {
			return *this;
		}
		copy_from(right);
		return *this;
	}
	ST_INLINE vector& operator=(vector&& right) {
		if (this == __builtin_addressof(right)) {
			return *this;
		}
		m_data = right.m_data;
		right.m_data = storage_type();
		return *this;
	}

	ST_INLINE void reserve(const size_type count) {
		size_type newCap = count;
		size_type oldCap = this->capacity();
		size_type oldSize = this->size();

		if (newCap < oldCap) {
#if ST_DEBUG_LEVEL > 0
			ST_REPORT_ERROR("Can't reserve with a capacity lower then the old capacity");
#endif
		}
		else if(newCap > oldCap) {
			value_type* newVec = allocator_type{}.allocate(count);
			if (oldCap != 0 && m_data.first) {
				ST_C_CALL memcpy_s(newVec, newCap * sizeof(value_type), m_data.first, oldCap * sizeof(value_type));
				allocator_type{}.deallocate(m_data.first, oldCap);
			}

			m_data.first = newVec;
			m_data.last = m_data.first + sizeof(value_type) * oldSize;
			m_data.end = m_data.first + sizeof(value_type) * newCap;
		}
		// oldCap == newCap no reserve needed
	}
	ST_INLINE void resize(const size_type count) {
		resize<EmptyType>(count, EmptyType());
	}
	template<class fill_type>
	ST_INLINE void resize(const size_type count, const fill_type& fill) {
		size_type newSize = count;
		size_type oldSize = this->size();

		// erease elements, capacity will not change, objects will be deleted
		if (newSize < oldSize) {
			value_type* newLast = m_data.first + sizeof(value_type) * newSize;
			destroy_range(newLast, m_data.last);
			m_data.last = newLast;
			return;
		}
		// append elements
		if (newSize > oldSize) {
			size_type oldCap = this->capacity();
			if (newSize > oldCap) {
				reserve(newSize);
			}

			if constexpr (is_same<fill_type, value_type>) {
				uninitlize_fill_range<fill_type>(m_data.first + sizeof(value_type) * oldSize, m_data.end, fill);
			}
			else {
				construct_fill_range(m_data.first + sizeof(value_type) * oldSize, m_data.end);
			}
			m_data.last = m_data.end;
		}
		// oldSize == newSize no resize needed
	}

	ST_INLINE constexpr size_type capacity() const ST_NOEXCEPT {
		return static_cast<size_type>((m_data.end - m_data.first) / sizeof(value_type));
	}
	ST_INLINE constexpr size_type size() const ST_NOEXCEPT {
		return static_cast<size_type>((m_data.last - m_data.first) / sizeof(value_type));
	}
	ST_INLINE constexpr value_type* data() const ST_NOEXCEPT {
		return m_data.first;
	}
private:
	ST_INLINE void copy_from(const vector& copy) {
		resize(copy.size());
		if (copy.capacity() > copy.size())
			reserve(copy.capacity());
		ST_C_CALL memcpy_s(m_data.first, m_data.end - m_data.first, copy.m_data.first, copy.m_data.end - copy.m_data.first);
	}
	ST_INLINE void destroy_range(value_type* first, value_type* last) {
		for (; first != last; first++) {
			allocator_type{}.destroy(first);
		}
	}
	template<class Val>
	ST_INLINE void uninitlize_fill_range(value_type* first, value_type* last, const Val& filler) {
		for (; first != last; first++) {
			ST_C_CALL memcpy_s(first, sizeof(value_type), &filler, sizeof(value_type));
		}
	}
	ST_INLINE void construct_fill_range(value_type* first, value_type* last) {
		for (; first != last; first++) {
			allocator_type{}.construct(first);
		}
	}
private:
	storage_type m_data;
};


ST_STL_END