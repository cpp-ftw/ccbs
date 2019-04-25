#ifndef PROJECT_CONTAINERS_HPP
#define PROJECT_CONTAINERS_HPP

#include <vector>
#include <set>
#include <map>

namespace ccbs
{

template<typename T>
std::vector<T>& merge_vectors(std::vector<T>& target_, std::vector<T> items_)
{
    target_.reserve(target_.size() + items_.size());
    while (!items_.empty())
    {
        target_.push_back(std::move(items_.back()));
        items_.pop_back();
    }
    return target_;
}

template<typename ELEMENT, typename KEY_FUNC, typename VALUE_FUNC>
std::vector<ELEMENT> serialize_set(std::set<ELEMENT> const& rules_, KEY_FUNC&& key_func, VALUE_FUNC&& value_func)
{
    using KEY = decltype(key_func(*rules_.begin()));
    std::map<KEY, ELEMENT> rules;
    for (auto& rule : rules_)
        rules.insert(std::make_pair(key_func(rule), rule));

    std::vector<ELEMENT> result;
    auto num = rules.size();
    result.reserve(num);

    while (result.size() != num)
    {
        size_t rules_size = rules.size();
        for (auto it = rules.begin(); it != rules.end();)
        {
            const auto& ruleInputs = value_func(it->second);
            bool hasUnresolvedInput = std::any_of(
                ruleInputs.begin(),
                ruleInputs.end(),
                [&] (const KEY& input) {
                    return bool(rules.count(input));
                });

            if (!hasUnresolvedInput)
            {
                result.push_back(it->second);
                it = rules.erase(it);
            }
            else
            {
                ++it;
            }
        }
        if (rules_size == rules.size())
            throw "circular dependency";
    }

    return result;
}

}

#endif //PROJECT_CONTAINERS_HPP
