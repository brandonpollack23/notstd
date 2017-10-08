#ifndef __NOTSTD_TYPETRAITS__
#define __NOTSTD_TYPETRAITS__

#include <type_traits>
#include <utility>

namespace notstd
{
    template<class TemplatedFunctor>
    struct MemberChecker
    {
    private:
        //sfinae will cause this specialization to fail and cascade to the next function def
        //if there is not a valid type returning from ClassUnderTest being passed to the templated functor
        //this happens if there is no such method specified by the TemplatedFunctor inside of the ClassUnderTest
        template<class ClassUnderTest>
        constexpr auto testValidity(int /*unused*/) -> decltype(std::declval<TemplatedFunctor>()(std::declval<ClassUnderTest>()), std::true_type())
        {
            return std::true_type();
        }

        // ... is always lowest priority for overloads
        template<class Param>
        constexpr auto testValidity(...) -> std::false_type
        {
            return std::false_type();
        }
    public:
        template<class ClassUnderTest>
        constexpr auto operator()(const ClassUnderTest& p)
        {
            return testValidity<ClassUnderTest>(int());
        }
    };

    template<class TemplatedFunctor>
    constexpr auto is_valid(const TemplatedFunctor& t)
    {
        //inject the TemplatedFunctor into the member checker
        //the MemberChecker (a meta Functor on it's own) () operator will then attempt to call 
        //the TemplatedFunctor's application on the ClassUnderTest (template param for the application of MemberChecker)
        //if sfinae occurs, then there is no member!  It will fall back to the ... parameter taking version, returning false_type
        return MemberChecker<TemplatedFunctor>();
    };
}

#endif