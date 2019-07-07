#ifndef PROJECT_DEPENDENCY_RULE_HPP
#define PROJECT_DEPENDENCY_RULE_HPP

#include <ccbs/rule/rule.hpp>

#include <mutex>

namespace ccbs
{

class dependency_rule : public rule
{
public:
    dependency_rule(ccsh::fs::path input, ccsh::fs::path output, rule_cmd cmd)
        : rule({input}, std::move(output), std::move(cmd))
    { }

    int explore() override;
};

}

#endif //PROJECT_DEPENDENCY_RULE_HPP
