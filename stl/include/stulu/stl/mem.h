#pragma once
#include "stulu/stl/cstddef.h"
#include "stulu/stl/cassert.h"
#include "stulu/stl/type_traits.h"

ST_STL_BEGIN

template <class T>
ST_NODISCARD ST_CONSTEXPR remove_reference_t<T>&& move(T&& moveable) ST_NOEXCEPT {
    return static_cast<remove_reference_t<T>&&>(moveable);
}

template <class T>
ST_NODISCARD ST_CONSTEXPR T* addressof(T& addr) ST_NOEXCEPT {
    return __builtin_addressof(addr);
}

template <class L, class R>
ST_INLINE void swap(L& left, R& right) ST_NOEXCEPT {
    L temp = ST_STL move(left);
    left = ST_STL move(right);
    right = ST_STL move(temp);
}

template <class L, class R = L>
ST_INLINE L exchange(L& value, R&& newValue) ST_NOEXCEPT {
    L old = static_cast<L&&>(value);
    value = static_cast<R&&>(newValue);
    return old;
}

ST_STL_END