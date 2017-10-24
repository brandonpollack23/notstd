#ifndef __NOTSTD_TUPLEEXTENSIONS__
#define __NOTSTD_TUPLEEXTENSIONS__

#include <tuple>

namespace notstd
{
    /*tuple has type*/
    template<class T, class... Types>
    struct param_pack_has_type;

    // Base Case: first element is type we seek
    template<class T, class... Types>
    struct param_pack_has_type<T, T, Types...>
    {
        static constexpr bool value = true;
    };

    // Base Case: tuple list is empty :(
    template<class T, class... Types>
    struct param_pack_has_type<T, std::tuple<>>
    {
        static constexpr bool value = false;
    };

    // Recurse, first element not it
    template<class T, class U, class... Types>
    struct param_pack_has_type<T, U, Types...>
    {
        static constexpr bool value = param_pack_has_type<T, Types...>::value;
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
        static_assert(param_pack_has_type<T, Types...>::value, "The tuple does not have the given type")
        static constexpr std::size_t value = 1 + type_to_index<T, std::tuple<Types...>>::value;
    };

    /*are all tuple elements unique*/
    template<class... Types>
    struct ensure_parameter_pack_unique;

    // Base case, only one element
    template<class T>
    struct ensure_parameter_pack_unique<T>
    {
        static constexpr bool value = true;
    };

    //TODO fuck this doesnt work right
    template<class T, class... Types>
    struct ensure_parameter_pack_unique<T, Types...>
    {
        static constexpr bool value = 
            // The type at the head can't be in the rest of the pack
            !param_pack_has_type<T, Types...>::value &&
            // The same is true for the tail of the pack recursively
            ensure_parameter_pack_unique<Types...>::value;
    };
}

#endif