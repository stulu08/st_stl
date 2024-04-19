#pragma once
#include "stulu/stl/cstddef.h"
#include "stulu/stl/type_traits.h"

ST_STL_BEGIN

template <class T>
struct default_delete {
	ST_CONSTEXPR default_delete() ST_NOEXCEPT = default;

	template<class U>
	ST_CONSTEXPR default_delete(const default_delete<U>& d) ST_NOEXCEPT {}

	ST_INLINE void operator()(T* ptr) const ST_NOEXCEPT {
		static_assert(0 < sizeof(T), "can't delete an incomplete type");
		delete ptr;
	}
};
template <class T>
struct default_delete<T[]> {
	ST_CONSTEXPR default_delete() ST_NOEXCEPT = default;

	template<class U>
	ST_CONSTEXPR default_delete(const default_delete<U[]>& d) ST_NOEXCEPT {}

	ST_INLINE void operator()(T* arr) const ST_NOEXCEPT {
		static_assert(0 < sizeof(T), "can't delete an incomplete type");
		delete[] arr;
	}
};

template<class T, class NoRefD, class = void>
struct get_pointer_type {
	using type = T*;
};

template<class T, class NoRefD>
struct get_pointer_type<T, NoRefD, void_t<typename NoRefD::pointer>> {
	using type = typename NoRefD::pointer;
};

template<class T, class D = default_delete<T>>
class unique_ptr {
public:
	// std::remove_reference<Deleter>::type::pointer if that type exists, otherwise T*. Must satisfy NullablePointer
	using pointer = typename get_pointer_type<T, remove_reference_t<D>>::type;
	// T, the type of the object managed by this unique_ptr
	using element_type = T;
	// Deleter, the function object or lvalue reference to function or to function object, to be called from the destructor
	using deleter_type = D;

	ST_CONSTEXPR unique_ptr() ST_NOEXCEPT
		: m_ptr(nullptr) {}
	ST_CONSTEXPR unique_ptr(nullptr_t) ST_NOEXCEPT
		: m_ptr(nullptr) {}
	ST_INLINE explicit unique_ptr(pointer ptr) ST_NOEXCEPT
		: m_ptr(ptr) {}
	ST_INLINE unique_ptr(unique_ptr&& other) ST_NOEXCEPT 
		: m_ptr(other.release()) { }

	ST_INLINE ~unique_ptr() {
		if (m_ptr)
			get_deleter()(m_ptr);
	}

	ST_INLINE unique_ptr& operator=(unique_ptr&& other) ST_NOEXCEPT {
		if (this == ST_STL addressof(other)) {
			return *this;
		}
		reset(other.release());
		return *this;
	}
	ST_INLINE unique_ptr& operator=(nullptr_t) ST_NOEXCEPT {
		reset();
		return *this;
	}
	
	ST_INLINE unique_ptr(const unique_ptr&) ST_NOEXCEPT = delete;
	ST_INLINE unique_ptr& operator=(const unique_ptr&) ST_NOEXCEPT = delete;

	ST_INLINE pointer release() ST_NOEXCEPT {
		return ST_STL exchange(m_ptr, nullptr);
	}

	ST_INLINE void reset(pointer ptr = nullptr) ST_NOEXCEPT {
		pointer old = ST_STL exchange(m_ptr, ptr);
		if (old) {
			get_deleter()(old);
		}
	}

	ST_INLINE void swap(unique_ptr& other) ST_NOEXCEPT {
		ST_STL swap(this->m_ptr, other->m_ptr);
	}

	ST_NODISCARD ST_INLINE deleter_type& get_deleter() const ST_NOEXCEPT {
		return *(deleter_type*)m_ptr;
	}

	ST_NODISCARD ST_INLINE pointer get() ST_NOEXCEPT {
		return m_ptr;
	}
	ST_NODISCARD ST_INLINE const pointer get() const ST_NOEXCEPT {
		return m_ptr;
	}

	ST_NODISCARD ST_INLINE add_lvalue_reference_t<element_type> operator*() const ST_NOEXCEPT(ST_NOEXCEPT(*ST_STL declval<pointer>())) {
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
	pointer m_ptr = nullptr;
};

template<class T, class D>
class unique_ptr<T[], D> {
public:
	// std::remove_reference<Deleter>::type::pointer if that type exists, otherwise T*. Must satisfy NullablePointer
	using pointer = typename get_pointer_type<T, remove_reference_t<D>>::type;
	// T, the type of the object managed by this unique_ptr
	using element_type = T;
	// Deleter, the function object or lvalue reference to function or to function object, to be called from the destructor
	using deleter_type = D;
	using size_type = size_t;

	ST_CONSTEXPR unique_ptr() ST_NOEXCEPT
		: m_ptr(nullptr) {}
	ST_CONSTEXPR unique_ptr(nullptr_t) ST_NOEXCEPT
		: m_ptr(nullptr) {}
	ST_INLINE explicit unique_ptr(pointer ptr) ST_NOEXCEPT
		: m_ptr(ptr) {}
	ST_INLINE unique_ptr(unique_ptr&& other) ST_NOEXCEPT
		: m_ptr(other.release()) { }

	ST_INLINE ~unique_ptr() {
		if (m_ptr)
			get_deleter()(m_ptr);
	}

	ST_INLINE unique_ptr& operator=(unique_ptr&& other) ST_NOEXCEPT {
		if (this == ST_STL addressof(other)) {
			return *this;
		}
		reset(other.release())
			return *this;
	}
	ST_INLINE unique_ptr& operator=(nullptr_t) ST_NOEXCEPT {
		reset();
		return *this;
	}

	ST_INLINE unique_ptr(const unique_ptr&) ST_NOEXCEPT = delete;
	ST_INLINE unique_ptr& operator=(const unique_ptr&) ST_NOEXCEPT = delete;

	ST_INLINE unique_ptr release() ST_NOEXCEPT {
		return ST_STL exchange(m_ptr, nullptr);
	}

	ST_INLINE void reset(pointer ptr = nullptr) ST_NOEXCEPT {
		pointer old = ST_STL exchange(m_ptr, ptr);
		if (old) {
			get_deleter()(old);
		}
	}

	ST_INLINE void swap(unique_ptr& other) ST_NOEXCEPT {
		ST_STL swap(this->m_ptr, other->m_ptr);
	}

	ST_NODISCARD ST_INLINE deleter_type& get_deleter() const ST_NOEXCEPT {
		return deleter_type{};
	}

	ST_NODISCARD ST_INLINE pointer get() ST_NOEXCEPT {
		return m_ptr;
	}
	ST_NODISCARD ST_INLINE const pointer get() const ST_NOEXCEPT {
		return m_ptr;
	}

	ST_NODISCARD ST_INLINE add_lvalue_reference_t<element_type> operator*() const ST_NOEXCEPT(ST_NOEXCEPT(*ST_STL declval<pointer>())) {
		return *m_ptr;
	}

	ST_NODISCARD ST_INLINE pointer operator->() ST_NOEXCEPT {
		return m_ptr;
	}
	ST_NODISCARD ST_INLINE const pointer operator->() const ST_NOEXCEPT {
		return m_ptr;
	}

	ST_NODISCARD ST_INLINE element_type& operator[](size_type index) const ST_NOEXCEPT {
		return m_ptr[index];
	}

	ST_INLINE explicit operator bool() const ST_NOEXCEPT {
		return static_cast<bool>(m_ptr);
	}

private:
	pointer m_ptr = nullptr;
};

template<class T, class D>
ST_INLINE void swap(unique_ptr<T, D> left, unique_ptr<T, D> right) ST_NOEXCEPT {
	left.swap(right);
}

template<class T1, class D1, class T2, class D2>
ST_NODISCARD ST_INLINE bool operator==(const unique_ptr<T1, D1>& left, const unique_ptr<T2, D2>& right) {
	return left.get() == right.get();
}
template<class T1, class D1, class T2, class D2>
ST_NODISCARD ST_INLINE bool operator!=(const unique_ptr<T1, D1>& left, const unique_ptr<T2, D2>& right) {
	return !(left.get() == right.get());
}
template<class T1, class D1, class T2, class D2>
ST_NODISCARD ST_INLINE bool operator<(const unique_ptr<T1, D1>& left, const unique_ptr<T2, D2>& right) {
	return left.get() < right.get();
}
template<class T1, class D1, class T2, class D2>
ST_NODISCARD ST_INLINE bool operator<=(const unique_ptr<T1, D1>& left, const unique_ptr<T2, D2>& right) {
	return !(right < left);
}
template<class T1, class D1, class T2, class D2>
ST_NODISCARD ST_INLINE bool operator>(const unique_ptr<T1, D1>& left, const unique_ptr<T2, D2>& right) {
	return right < left;
}
template<class T1, class D1, class T2, class D2>
ST_NODISCARD ST_INLINE bool operator>=(const unique_ptr<T1, D1>& left, const unique_ptr<T2, D2>& right) {
	return !(left < right);
}



template<class T, class D>
ST_NODISCARD ST_INLINE bool operator==(const unique_ptr<T, D>& left, const nullptr_t right) {
	return !left;
}
template<class T, class D>
ST_NODISCARD ST_INLINE bool operator==(const nullptr_t left, const unique_ptr<T, D>& right) {
	return !right;
}

template<class T, class D>
ST_NODISCARD ST_INLINE bool operator!=(const unique_ptr<T, D>& left, const nullptr_t right) {
	return !(left == right);
}
template<class T, class D>
ST_NODISCARD ST_INLINE bool operator!=(const nullptr_t left, const unique_ptr<T, D>& right) {
	return !(left == right);
}

template<class T, class D>
ST_NODISCARD ST_INLINE bool operator<(const unique_ptr<T, D>& left, const nullptr_t right) {
	using ptr = unique_ptr<T1, D>::pointer;
	return static_cast<ptr>(left.get()) < right;
}
template<class T, class D>
ST_NODISCARD ST_INLINE bool operator<(const nullptr_t left, const unique_ptr<T, D>& right) {
	using ptr = unique_ptr<T1, D>::pointer;
	return left < static_cast<ptr>(right.get());
}

template<class T, class D>
ST_NODISCARD ST_INLINE bool operator<=(const unique_ptr<T, D>& left, const nullptr_t right) {
	return !(right < left);
}
template<class T, class D>
ST_NODISCARD ST_INLINE bool operator<=(const nullptr_t left, const unique_ptr<T, D>& right) {
	return !(right < left);
}

template<class T, class D>
ST_NODISCARD ST_INLINE bool operator>(const unique_ptr<T, D>& left, const nullptr_t right) {
	return right < left;
}
template<class T, class D>
ST_NODISCARD ST_INLINE bool operator>(const nullptr_t left, const unique_ptr<T, D>& right) {
	return right < left;
}

template<class T, class D>
ST_NODISCARD ST_INLINE bool operator>=(const unique_ptr<T, D>& left, const nullptr_t right) {
	return !(left < right);
}
template<class T, class D>
ST_NODISCARD ST_INLINE bool operator>=(const nullptr_t left, const unique_ptr<T, D>& right) {
	return !(left < right);
}

ST_STL_END