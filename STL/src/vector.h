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
	ST_INLINE constexpr vector(const vector& copy) {
		this->copy_from(copy);
	}
	ST_INLINE constexpr vector(vector&& src) ST_NOEXCEPT {
		m_data = src.m_data;
		src.m_data = storage_type();
	};
	ST_INLINE constexpr vector(const size_type count) {
		this->reserve(count);
	}
	
	ST_INLINE ~vector() {
		if (m_data.first && this->capacity() > 0) {
			allocator_type{}.deallocate(m_data.first, this->capacity());
		}
	}
	
	ST_INLINE vector& operator=(const vector& right) {
		if (this == __builtin_addressof(right)) {
			return *this;
		}
		this->copy_from(right);
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
		size_type newCap = count;
		size_type oldCap = this->capacity();

		if (newCap < oldCap) {
#if ST_DEBUG_LEVEL > 0
			ST_REPORT_ERROR("Can't reserve with a capacity lower then the old capacity");
#endif
		}
		else if(newCap > oldCap) {
			value_type* newFirst = allocator_type{}.allocate(newCap);

			if (m_data.first != nullptr && oldCap > 0) {
				allocator_type{}.copy(newFirst, m_data.first, newCap);
				allocator_type{}.deallocate(m_data.first, oldCap);
			}
			m_data.last = newFirst + this->size();

			m_data.end = newFirst + newCap;
			m_data.first = newFirst;
		}
		// oldCap == newCap no reserve needed
	}

	template<class fill_type>
	ST_INLINE void resize(const size_type count, const fill_type& fill) {
		size_type newSize = count;
		size_type oldSize = this->size();

		// erease elements, capacity will not change, objects will be deleted
		if (newSize < oldSize) {
			value_type* newLast = m_data.first + newSize;
			this->destroy_range(newLast, m_data.last);
			m_data.last = newLast;
			return;
		}
		// append elements
		if (newSize > oldSize) {
			if (newSize > this->capacity()) {
				this->reserve(newSize);
			}

			if constexpr (is_same<fill_type, value_type>) {
				m_data.last = this->uninitlize_fill_range(m_data.last, newSize - oldSize, fill);
			}
			else {
				m_data.last = this->construct_fill_range(m_data.last, newSize - oldSize);
			}
		}
		// oldSize == newSize no resize needed
	}

	ST_INLINE void resize(const size_type count) {
		this->resize(count, EmptyType());
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
private:
	ST_INLINE void copy_from(const vector& copy) {
		resize(copy.size());
		if (copy.capacity() > copy.size())
			this->reserve(copy.capacity());
		allocator_type{}.copy(m_data.first, copy.m_data.first, copy.capacity());
	}
	ST_INLINE void destroy_range(value_type* first, value_type* last) {
		for (; first != last; first++) {
			allocator_type{}.destroy(first);
		}
	}

	template<class Val>
	ST_INLINE value_type* uninitlize_fill_range(value_type* first, size_type count, const Val& filler) {
		for (size_type i = 0; i < count; i++) {
			ST_C_CALL memcpy_s(first, sizeof(value_type), &filler, sizeof(Val));
			first++;
		}
		return first;
	}
	ST_INLINE value_type* construct_fill_range(value_type* first, size_type count) {
		for (size_type i = 0; i < count; i++) {
			allocator_type{}.construct(first);
			first++;
		}
		return first;
	}
private:
	storage_type m_data;
};

ST_STL_END