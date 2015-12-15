
// Copyright 2005-2009 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  Based on Peter Dimov's proposal
//  http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2005/n1756.pdf
//  issue 6.18. 

// This implements the extensions to the standard.
// It's undocumented, so you shouldn't use it....

#if !defined(BOOST_FUNCTIONAL_HASH_EXTENSIONS_HPP)
#define BOOST_FUNCTIONAL_HASH_EXTENSIONS_HPP

#include <cutl/details/boost/functional/hash/hash.hpp>
#include <cutl/details/boost/detail/container_fwd.hpp>
#include <cutl/details/boost/utility/enable_if.hpp>
#include <cutl/details/boost/static_assert.hpp>
#include <cutl/details/boost/preprocessor/repetition/repeat_from_to.hpp>
#include <cutl/details/boost/preprocessor/repetition/enum_params.hpp>

#if !defined(BOOST_NO_CXX11_HDR_ARRAY)
#   include <array>
#endif

#if !defined(BOOST_NO_CXX11_HDR_TUPLE)
#   include <tuple>
#endif

#if !defined(BOOST_NO_CXX11_HDR_MEMORY)
#   include <memory>
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#if defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
#include <cutl/details/boost/type_traits/is_array.hpp>
#endif

#if BOOST_WORKAROUND(BOOST_MSVC, < 1300)
#include <cutl/details/boost/type_traits/is_const.hpp>
#endif

namespace cutl_details_boost
{
    template <class A, class B>
    std::size_t hash_value(std::pair<A, B> const&);
    template <class T, class A>
    std::size_t hash_value(std::vector<T, A> const&);
    template <class T, class A>
    std::size_t hash_value(std::list<T, A> const& v);
    template <class T, class A>
    std::size_t hash_value(std::deque<T, A> const& v);
    template <class K, class C, class A>
    std::size_t hash_value(std::set<K, C, A> const& v);
    template <class K, class C, class A>
    std::size_t hash_value(std::multiset<K, C, A> const& v);
    template <class K, class T, class C, class A>
    std::size_t hash_value(std::map<K, T, C, A> const& v);
    template <class K, class T, class C, class A>
    std::size_t hash_value(std::multimap<K, T, C, A> const& v);

    template <class T>
    std::size_t hash_value(std::complex<T> const&);

    template <class A, class B>
    std::size_t hash_value(std::pair<A, B> const& v)
    {
        std::size_t seed = 0;
        cutl_details_boost::hash_combine(seed, v.first);
        cutl_details_boost::hash_combine(seed, v.second);
        return seed;
    }

    template <class T, class A>
    std::size_t hash_value(std::vector<T, A> const& v)
    {
        return cutl_details_boost::hash_range(v.begin(), v.end());
    }

    template <class T, class A>
    std::size_t hash_value(std::list<T, A> const& v)
    {
        return cutl_details_boost::hash_range(v.begin(), v.end());
    }

    template <class T, class A>
    std::size_t hash_value(std::deque<T, A> const& v)
    {
        return cutl_details_boost::hash_range(v.begin(), v.end());
    }

    template <class K, class C, class A>
    std::size_t hash_value(std::set<K, C, A> const& v)
    {
        return cutl_details_boost::hash_range(v.begin(), v.end());
    }

    template <class K, class C, class A>
    std::size_t hash_value(std::multiset<K, C, A> const& v)
    {
        return cutl_details_boost::hash_range(v.begin(), v.end());
    }

    template <class K, class T, class C, class A>
    std::size_t hash_value(std::map<K, T, C, A> const& v)
    {
        return cutl_details_boost::hash_range(v.begin(), v.end());
    }

    template <class K, class T, class C, class A>
    std::size_t hash_value(std::multimap<K, T, C, A> const& v)
    {
        return cutl_details_boost::hash_range(v.begin(), v.end());
    }

    template <class T>
    std::size_t hash_value(std::complex<T> const& v)
    {
        cutl_details_boost::hash<T> hasher;
        std::size_t seed = hasher(v.imag());
        seed ^= hasher(v.real()) + (seed<<6) + (seed>>2);
        return seed;
    }

#if !defined(BOOST_NO_CXX11_HDR_ARRAY)
    template <class T, std::size_t N>
    std::size_t hash_value(std::array<T, N> const& v)
    {
        return cutl_details_boost::hash_range(v.begin(), v.end());
    }
#endif

#if !defined(BOOST_NO_CXX11_HDR_TUPLE)
    namespace hash_detail {
        template <std::size_t I, typename T>
        inline typename cutl_details_boost::enable_if_c<(I == std::tuple_size<T>::value),
                void>::type
            hash_combine_tuple(std::size_t&, T const&)
        {
        }

        template <std::size_t I, typename T>
        inline typename cutl_details_boost::enable_if_c<(I < std::tuple_size<T>::value),
                void>::type
            hash_combine_tuple(std::size_t& seed, T const& v)
        {
            cutl_details_boost::hash_combine(seed, std::get<I>(v));
            cutl_details_boost::hash_detail::hash_combine_tuple<I + 1>(seed, v);
        }

        template <typename T>
        inline std::size_t hash_tuple(T const& v)
        {
            std::size_t seed = 0;
            cutl_details_boost::hash_detail::hash_combine_tuple<0>(seed, v);
            return seed;
        }
    }

#if !defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
    template <typename... T>
    inline std::size_t hash_value(std::tuple<T...> const& v)
    {
        return cutl_details_boost::hash_detail::hash_tuple(v);
    }
#else

    inline std::size_t hash_value(std::tuple<> const& v)
    {
        return cutl_details_boost::hash_detail::hash_tuple(v);
    }

#   define BOOST_HASH_TUPLE_F(z, n, _)                                      \
    template<                                                               \
        BOOST_PP_ENUM_PARAMS_Z(z, n, typename A)                            \
    >                                                                       \
    inline std::size_t hash_value(std::tuple<                               \
        BOOST_PP_ENUM_PARAMS_Z(z, n, A)                                     \
    > const& v)                                                             \
    {                                                                       \
        return cutl_details_boost::hash_detail::hash_tuple(v);                           \
    }

    BOOST_PP_REPEAT_FROM_TO(1, 11, BOOST_HASH_TUPLE_F, _)
#   undef BOOST_HASH_TUPLE_F
#endif

#endif

#if !defined(BOOST_NO_CXX11_SMART_PTR)
    template <typename T>
    inline std::size_t hash_value(std::shared_ptr<T> const& x) {
        return cutl_details_boost::hash_value(x.get());
    }

    template <typename T, typename Deleter>
    inline std::size_t hash_value(std::unique_ptr<T, Deleter> const& x) {
        return cutl_details_boost::hash_value(x.get());
    }
#endif

    //
    // call_hash_impl
    //

    // On compilers without function template ordering, this deals with arrays.

#if defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
    namespace hash_detail
    {
        template <bool IsArray>
        struct call_hash_impl
        {
            template <class T>
            struct inner
            {
                static std::size_t call(T const& v)
                {
                    using namespace cutl_details_boost;
                    return hash_value(v);
                }
            };
        };

        template <>
        struct call_hash_impl<true>
        {
            template <class Array>
            struct inner
            {
#if !BOOST_WORKAROUND(BOOST_MSVC, < 1300)
                static std::size_t call(Array const& v)
#else
                static std::size_t call(Array& v)
#endif
                {
                    const int size = sizeof(v) / sizeof(*v);
                    return cutl_details_boost::hash_range(v, v + size);
                }
            };
        };

        template <class T>
        struct call_hash
            : public call_hash_impl<cutl_details_boost::is_array<T>::value>
                ::BOOST_NESTED_TEMPLATE inner<T>
        {
        };
    }
#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

    //
    // cutl_details_boost::hash
    //


#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

    template <class T> struct hash
        : std::unary_function<T, std::size_t>
    {
#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
        std::size_t operator()(T const& val) const
        {
            return hash_value(val);
        }
#else
        std::size_t operator()(T const& val) const
        {
            return hash_detail::call_hash<T>::call(val);
        }
#endif
    };

#if BOOST_WORKAROUND(__DMC__, <= 0x848)
    template <class T, unsigned int n> struct hash<T[n]>
        : std::unary_function<T[n], std::size_t>
    {
        std::size_t operator()(const T* val) const
        {
            return cutl_details_boost::hash_range(val, val+n);
        }
    };
#endif

#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    // On compilers without partial specialization, cutl_details_boost::hash<T>
    // has already been declared to deal with pointers, so just
    // need to supply the non-pointer version of hash_impl.

    namespace hash_detail
    {
        template <bool IsPointer>
        struct hash_impl;

#if !BOOST_WORKAROUND(BOOST_MSVC, < 1300)

        template <>
        struct hash_impl<false>
        {
            template <class T>
            struct inner
                : std::unary_function<T, std::size_t>
            {
#if !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)
                std::size_t operator()(T const& val) const
                {
                    return hash_value(val);
                }
#else
                std::size_t operator()(T const& val) const
                {
                    return hash_detail::call_hash<T>::call(val);
                }
#endif
            };
        };

#else // Visual C++ 6.5

        // Visual C++ 6.5 has problems with nested member functions and
        // applying const to const types in templates. So we get this:

        template <bool IsConst>
        struct hash_impl_msvc
        {
            template <class T>
            struct inner
                : public std::unary_function<T, std::size_t>
            {
                std::size_t operator()(T const& val) const
                {
                    return hash_detail::call_hash<T const>::call(val);
                }

                std::size_t operator()(T& val) const
                {
                    return hash_detail::call_hash<T>::call(val);
                }
            };
        };

        template <>
        struct hash_impl_msvc<true>
        {
            template <class T>
            struct inner
                : public std::unary_function<T, std::size_t>
            {
                std::size_t operator()(T& val) const
                {
                    return hash_detail::call_hash<T>::call(val);
                }
            };
        };
        
        template <class T>
        struct hash_impl_msvc2
            : public hash_impl_msvc<cutl_details_boost::is_const<T>::value>
                    ::BOOST_NESTED_TEMPLATE inner<T> {};
        
        template <>
        struct hash_impl<false>
        {
            template <class T>
            struct inner : public hash_impl_msvc2<T> {};
        };

#endif // Visual C++ 6.5
    }
#endif  // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
}

#endif
