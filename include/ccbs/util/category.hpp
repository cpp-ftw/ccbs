#ifndef CCBS_UTIL_CATEGORY_HPP
#define CCBS_UTIL_CATEGORY_HPP

#include <unordered_set>
#include <typeindex>
#include <type_traits>
#include <initializer_list>

namespace ccbs {

class category_base
{
    friend class category_spec;
    virtual bool satisfies(const std::type_info&) const = 0;
public:
    virtual ~category_base() = default;
};

class category_all {};

template<typename Derived, typename... Bases>
class category_of : public category_base
{
    static std::unordered_set<std::type_index> make_satisfied()
    {
        std::unordered_set<std::type_index> result = {typeid(category_of)};
        std::initializer_list<const std::unordered_set<std::type_index>*> derived_sets = { &Bases::satisfied()... };
        for (const auto& item : derived_sets)
            result.insert(item->begin(), item->end());
        return result;
    }

    static const std::unordered_set<std::type_index>& satisfied()
    {
        static const std::unordered_set<std::type_index> result = make_satisfied();
        return result;
    }

    bool satisfies(const std::type_info& ti) const override
    {
        return satisfied().count(ti);
    }

    static category_of& instance()
    {
        static_assert(std::is_base_of<category_of, Derived>::value, "Bad Derived argument");
        static category_of i;
        return i;
    }
    friend class category_spec;

    template<typename, typename...>
    friend class category_of;
};

class category_spec
{
    category_base* impl;
public:
    // ReSharper disable once CppNonExplicitConvertingConstructor
    template<typename Derived, typename... Bases>
    category_spec(const category_of<Derived, Bases... >&)
        : impl(&category_of<Derived, Bases... >::instance())
    { }

    // ReSharper disable once CppNonExplicitConvertingConstructor
    category_spec(const category_all&)
        : impl(nullptr)
    { }

    template<typename T>
    bool satisfies()
    {
        static_assert(std::is_base_of<category_base, T>::value, "Bad category");
        return impl == nullptr || impl->satisfies(typeid(T));
    }

    bool satisfies(const category_spec& other) const
    {
        return impl == nullptr || other.impl == nullptr || impl->satisfies(typeid(*other.impl));
    }
};

} // namespace ccbs


#endif // CCBS_UTIL_CATEGORY_HPP
