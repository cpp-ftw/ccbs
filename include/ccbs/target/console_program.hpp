#ifndef PROJECT_CONSOLE_PROGRAM_HPP
#define PROJECT_CONSOLE_PROGRAM_HPP

#include <ccbs/target/build_target.hpp>
#include <ccbs/util/visitors.hpp>

namespace ccbs
{

class console_program : public build_target
{
public:
    using build_target::build_target;

    rule_cmd dependency_command(compiler_ptr& compiler_) override
    {
        return make_rule_cmd(compiler_, compiler::dep_cmd{});
    }
    rule_cmd object_command(compiler_ptr& compiler_) override
    {
        return make_rule_cmd(compiler_, compiler::object_cmd{});
    }
    rule_cmd target_command(compiler_ptr& compiler_) override
    {
        return make_rule_cmd(compiler_, compiler::exe_cmd{});
    }
};

}

#endif //PROJECT_CONSOLE_PROGRAM_HPP
