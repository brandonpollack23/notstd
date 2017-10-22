#ifndef __NOTSTD_TUPLEEXTENSIONS__
#define __NOTSTD_TUPLEEXTENSIONS__

#include <tuple>

namespace notstd
{
    /*tuple has type*/
    template<class T, class Tuple>
    struct tuple_has_type;

    // Base Case: first element is type we seek
    template<class T, class... Types>
    struct tuple_has_type<T, std::tuple<T, Types...>>
    {
        static constexpr bool value = true;
    };

    // Base Case: tuple list is empty :(
    template<class T, class... Types>
    struct tuple_has_type<T, std::tuple<>>
    {
        static constexpr bool value = false;
    };

    // Recurse, first element not it
    template<class T, class U, class... Types>
    struct tuple_has_type<T, std::tuple<U, Types...>>
    {
        static constexpr bool value = tuple_has_type<Types...>::value;
    };

    /*tuple type to index*/
    template<class T, class Tuple>
    struct type_to_index;

    // base case: first element of tuple matches T, the type we seek
    template<class T, class... Types>
    struct type_to_index<T, std::tuple<T, Types...>>
    {
        static constexpr std::size_t value = 0;
    };

    //first element of tuple matches some other type
    template<class T, class U, class... Types>
    struct type_to_index<T, std::tuple<U, Types...>>
    {
        static_assert(tuple_has_type<T, std::tuple<Types...>, "The tuple does not have the given type")
        static constexpr std::size_t value = 1 + type_to_index<T, std::tuple<Types...>>::value;
    };

    /*are all tuple elements unique*/
    template<class... Types>
    struct ensure_parameterpack_unique;

    // Base case, only one element
    template<class T>
    struct ensure_parameterpack_unique<T>
    {
        static constexpr bool value = true;
    };

    // Base case, the head of the types list matches an element we already found
    template<class T, class... Types>
    struct ensure_parameterpack_unique<std::tuple<T, T, Types...>>
    {
        static constexpr bool value = false;
    };

    template<class T, class U, class... Types>
    struct ensure_parameterpack_unique<std::tuple<T, U, Types...>>
    {
        static constexpr bool value = 
            ensure_parameterpack_unique<std::tuple<T, Types...>>::value &&
            ensure_parameterpack_unique<std::tuple<U, Types...>>::value;
    };
}

#endif