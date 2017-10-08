#ifndef __NOTSTD_BIDIR_MAP__
#define __NOTSTD_BIDIR_MAP__

#include <type_traits>
#include <utility>
#include <memory>
#include <functional>

#include "notstdtypetraits.h"

#include <unordered_map>

namespace notstd
{
    static auto hasNeededMembers = is_valid([](const auto& t) -> decltype(
        t.size(),
        t.max_size(),
        t.begin(),
        t.cbegin(),
        t.end(),
        t.cend(),
        t.operator[],
        t.at(),
        t.find(),
        t.insert(),
        t.erase(),
        t.clear()) {});

    template<class MapType, class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>, class Alloc = allocator< std::pair<const Key, T> > >
    class bidir_map
    {
    public:
        static_assert(hasNeededMembers(std::declval(MapType)), "MapType backing bidir_map does not have all the needed member functions");

        //Constructors

        //TODO more than just two constructors
        bidir_map() : m_forwardMap(), m_reverseMap()
        {
        }

        bidir_map(size_t n,
                  const MapType::hasher& hf = MapType::hasher(),
                  const MapType::key_equal& eql = MapType::key_equal(),
                  const allocator_type& alloc = MapType::allocator_type()) :
            m_forwardMap(MapType(n, hf, eql, alloc)),
            m_reverseMap(MapType(n, hf, eql, alloc))
        {
        }

        // Capacity
        bool empty() const noexcept
        {
            return m_forwardMap.empty();
        }
        size_t size() const noexcept
        {
            return m_forwardMap.size();
        }
        size_t max_size() const noexcept
        {
            return m_forwardMap.max_size();
        }

        // Iterators
        auto begin() noexcept
        {
            return m_forwardMap.begin();
        }
        auto reverse_begin() noexcept
        {
            return m_reverseMap.begin();
        }
        auto end() noexcept
        {
            return m_forwardMap.end();
        }
        auto reverse_end() noexcept
        {
            return m_reverseMap.end();
        }
        auto cbegin() noexcept
        {
            return m_forwardMap.cbegin();
        }
        auto reverse_cbegin() noexcept
        {
            return m_reverseMap.cbegin();
        }
        auto cend() noexcept
        {
            return m_forwardMap.cend();
        }
        auto reverse_cend() noexcept
        {
            return m_reverseMap.cend();
        }

        // Access
        T& at(const Key& k)
        {
            return m_forwardMap.at(k);
        }
        const T& at(Key& k) const
        {
            return m_forwardMap.at(k);
        }
        Key& reverse_at(const T& v)
        {
            returm m_reverseMap.at(v);
        }
        const Key& reverse_at(T& v) const
        {
            return m_reverseMap.at(v);
        }
        auto find(const Key& k)
        {
            return m_forwardMap.find(k);
        }
        auto find(const Key& k) const
        {
            return m_forwardMap.find(k);
        }
        auto reverse_find(const T& v)
        {
            return m_reverseMap.find(v);
        }
        auto reverse_find(const T& v) const
        {
            return m_reverseMap.find(v);
        }

        // Modification
        void put(const Key& k, const T& v)
        {
            m_forwardMap[k] = v;
            m_reverseMap[v] = k;
        }
        void erase(const Key& k)
        {
            auto v = m_forwardMap[k];
            m_forwardMap.erase(k);
            m_reverseMap.erase(v);
        }
        void clear()
        {
            m_forwardMap.clear();
            m_reverseMap.clear();
        }

    private:
        MapType<Key, T> m_forwardMap;
        MapType<T, Key> m_reverseMap;
    };

    template<class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>, class Alloc = allocator< std::pair<const Key, T> > >
    using unordered_bidir_map = bidir_map<std::unordered_map<Key, T, Hash, Pred, Alloc>>;
} /*notstd*/

#endif
