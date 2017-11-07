#ifndef PROJECT_DEPENDENCY_RULE_HPP
#define PROJECT_DEPENDENCY_RULE_HPP

#include "rule.hpp"

namespace ccbs
{

class dependency_rule : public rule
{
    mutable ccsh::fs::file_time_type last_read = ccsh::fs::file_time_type::clock::from_time_t(0);
public:

    dependency_rule(ccsh::fs::path input, ccsh::fs::path output, rule_cmd cmd)
        : rule({input}, std::move(output), std::move(cmd))
    { }

    bool needs_rebuild() const override;
};

}

#endif //PROJECT_DEPENDENCY_RULE_HPP
