#pragma once
#include "stulu/stl/cstddef.h"

ST_STL_BEGIN

template<class T>
ST_CONSTEXPR const bool is_memset_fill_safe = sizeof(T) == sizeof(char);

ST_INLINE_VAR ST_CONSTEXPR bool always_false = false;

template <class T>
struct remove_reference {
    using type = T;
    using _Const_thru_ref_type = const T;
};
template <class T>
struct remove_reference<T&> {
    using type = T;
    using _Const_thru_ref_type = const T&;
};
template <class T>
struct remove_reference<T&&> {
    using type = T;
    using _Const_thru_ref_type = const T&&;
};
template <class T>
using remove_reference_t = typename remove_reference<T>::type;

template <class T, class = void>
struct add_reference {
    using lvalue = T;
    using rvalue = T;
};
template <class T>
struct add_reference<T, void_t<T&>> {
    using lvalue = T&;
    using rvalue = T&&;
};
template <class T>
struct add_lvalue_reference {
    using type = typename add_reference<T>::lvalue;
};
template <class T>
using add_lvalue_reference_t = typename add_reference<T>::lvalue;

template <class T>
struct add_rvalue_reference {
    using type = typename add_reference<T>::rvalue;
};
template <class T>
using add_rvalue_reference_t = typename add_reference<T>::rvalue;

template <class T>
add_rvalue_reference_t<T> declval() noexcept {
    static_assert(always_false<_Ty>, "Calling declval is ill-formed, see N4950 [declval]/2.");
}

template <class, class>
ST_INLINE_VAR constexpr bool is_same = false;
template <class _Ty>
ST_INLINE_VAR constexpr bool is_same<_Ty, _Ty> = true;

ST_STL_END