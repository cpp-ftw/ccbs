#ifndef CCSH_SHARED_LIBRARY_HPP
#define CCSH_SHARED_LIBRARY_HPP

#include <ccbs/rule/ruleset.hpp>
#include <ccbs/rule/rule.hpp>
#include <ccbs/package/repository.hpp>
#include <ccbs/target/build_target.hpp>

namespace ccbs {


class shared_library : public build_target, public package
{
public:
    using build_target::build_target;

    std::set<package*> dependencies() const override
    {
        return build_target::dependencies();
    }
    int prepare(options& options_, compiler_ptr& compiler_) override;
    void use_flags(compiler& cc) const override;

    rule_cmd dependency_command(compiler_ptr& compiler_) override
    {
        return make_rule_cmd(compiler_, compiler::dep_cmd{});
    }
    rule_cmd object_command(compiler_ptr& compiler_) override
    {
        return make_rule_cmd(compiler_, compiler::object_so_cmd{});
    }
    rule_cmd target_command(compiler_ptr& compiler_) override
    {
        return make_rule_cmd(compiler_, compiler::shared_cmd{});
    }

    timestamp last_modified() const override
    {
        return ccsh::fs::last_write_time(build_target::output());
    }
};

}

#endif //CCSH_SHARED_LIBRARY_HPP
