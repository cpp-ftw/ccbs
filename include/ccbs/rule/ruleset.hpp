#ifndef CCBS_BUILD_PACKAGE_HPP
#define CCBS_BUILD_PACKAGE_HPP

#include <ccbs/rule/rule.hpp>
#include <ccbs/compiler/compiler_ptr.hpp>

namespace ccbs {

class ruleset
{
    std::set<rule_ptr> rules_;

public:

    void add_rules(std::set<rule_ptr> const& inputs)
    {
        rules_.insert(inputs.begin(), inputs.end());
    }
    void add_rule(rule_ptr const& input)
    {
        rules_.insert(input);
    }

    const std::set<rule_ptr>& rules() const { return rules_; }

    int build(std::set<package*> const& dependencies, options&);
    static int build_dependencies(std::set<package*> const& dependencies, options&, compiler_ptr&);
};

}

#endif // CCBS_BUILD_PACKAGE_HPP
