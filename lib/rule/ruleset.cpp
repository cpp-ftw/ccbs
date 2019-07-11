#include <ccbs/rule/ruleset.hpp>

#include <ccbs/util/containers.hpp>
#include <ccbs/util/parallel.hpp>

namespace ccbs
{

int ruleset::build_dependencies(std::set<package*> const& dependencies, options& options_, compiler_ptr& compiler_)
{
    auto serialized_deps = serialize_set(
        dependencies,
        [](package* dep) { return dep; },
        [](package* dep) { return dep->dependencies(); }
    );

    for (const auto& dep : serialized_deps)
    {
        int result = dep->prepare(options_, compiler_);
        if (result != 0)
            return result;
    }
    return 0;
}

int ruleset::build(std::vector<package*> const& dependencies, options& options_)
{
    // TODO: options may contain information about parallelization
    int no_threads = 5;

    auto serialized_rules = serialize_set(
        this->rules_,
        [](rule_ptr const& rule) { return rule->output(); },
        [](rule_ptr const& rule) { return rule->inputs(); }
    );

    bool rebuild = options_[action::key] == action::rebuild;

    auto explore_func = [&](rule_ptr& ptr) -> int {
        return ptr->explore();
    };
    auto explore_conflicts = [&](rule_ptr const&, rule_ptr const&) { return false; };

    int explore_result = parallel_process(no_threads, serialized_rules.begin(), serialized_rules.end(),
                                          explore_func, explore_conflicts);
    if (explore_result != 0)
        return explore_result;

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

    return parallel_process(no_threads, serialized_rules.begin(), serialized_rules.end(), worker_func, conflicts_func);
}

}
