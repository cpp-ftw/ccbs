#include <ccbs/target/build_target.hpp>
#include <ccbs/rule/ruleset.hpp>
#include <ccbs/rule/dependency_rule.hpp>
#include <ccbs/rule/link_rule.hpp>

using namespace ccsh::literals;

namespace ccbs
{

int build_target::build_dependencies(options& options_)
{
    return ruleset::build_dependencies(dependencies(), options_);
}

void build_target::add_rules(ruleset& rules)
{
    ccsh::fs::path tempdir = this->tempdir / this->project_;

    auto objects_cmd = object_command();
    auto dependency_cmd = dependency_command();
    auto so_cmd = target_command();

    std::set<rule_ptr> dep_rules;
    for (const auto& file : files)
    {
        dep_rules.emplace(new dependency_rule{
            file,
            ccbs::prefix_dir(tempdir, ccbs::add_extension(".d"_p))(ccsh::fs::safe_relative(file, project_root_)),
            dependency_cmd
        });
    }

    std::set<rule_ptr> object_rules;
    for (const auto& file : files)
    {
        const auto& rel = ccsh::fs::safe_relative(file, project_root_);
        object_rules.emplace(new rule{
            {file},
            ccbs::prefix_dir(tempdir, ccbs::add_extension(".o"_p))(rel),
            objects_cmd,
            {ccbs::prefix_dir(tempdir, ccbs::add_extension(".d"_p))(rel)}
        });
    }

    auto so_rule = std::make_shared<link_rule>(rule_outputs(object_rules), outfile, so_cmd, std::set<ccsh::fs::path>{}, dependencies());
    rules.add_rules(dep_rules);
    rules.add_rules(object_rules);
    rules.add_rule(so_rule);
}


int build_target::build_rules(options& options_)
{
    ruleset target;
    add_rules(target);

    if (options_[action::key] == action::clean)
    {
        ccsh::fs::error_code ec;
        for (const auto& rule : target.rules())
            ccsh::fs::remove(rule->output(), ec);
        return ec ? 1 : 0;
    }

    return target.build(dependencies(), options_);
}

int build_target::build(options& options_)
{
    int result1 = build_dependencies(options_);
    if (result1 != 0)
        return result1;

    return build_rules(options_);
}

}
