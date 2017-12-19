#include <ccbs/rule/ruleset.hpp>

#include <ccbs/util/containers.hpp>
#include <ccbs/util/parallel.hpp>

namespace ccbs
{

int ruleset::build_dependencies(std::set<package*> const& dependencies, options& options_)
{
    auto serialized_deps = serialize_set(
        dependencies,
        [](package* dep) { return dep; },
        [](package* dep) { return dep->dependencies(); }
    );

    for (const auto& dep : serialized_deps)
    {
        int result = dep->prepare(options_);
        if (result != 0)
            return result;
    }
    return 0;
}

int ruleset::build(std::set<package*> const& dependencies, options& options_)
{
    // TODO: options may contain information about parallelization

    auto serialized_rules = serialize_set(
        this->rules,
        [](rule_ptr const& rule) { return rule->output(); },
        [](rule_ptr const& rule) { return rule->inputs(); }
    );

    bool rebuild = options_[action::key] == action::rebuild;

    auto worker_func = [&](rule_ptr& ptr) -> int
    {
        int res = 0;
        if (rebuild || ptr->needs_rebuild())
            res = ptr->make(dependencies);
        return res;
    };

    auto conflicts_func = [&](rule_ptr const& a, rule_ptr const& b) -> bool
    {
        const auto& file = a->output();
        return b->inputs().count(file) || b->dependencies().count(file);
    };

    return parallel_process(5, serialized_rules.begin(), serialized_rules.end(), worker_func, conflicts_func);
}

}
