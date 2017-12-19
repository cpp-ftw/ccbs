#include <ccbs/target/build_target.hpp>
#include <ccbs/rule/dependency_rule.hpp>
#include <ccbs/rule/link_rule.hpp>

using namespace ccsh::literals;

namespace ccbs
{

int build_target::build_dependencies(options& options_)
{
    return ruleset::build_dependencies(dependencies(), options_);
}

int build_target::build_rules(options& options_)
{
    if (options_[action::key] == action::clean)
    {
        ccsh::fs::error_code ec;
        for (const auto& file : files)
        {
            ccsh::fs::remove(ccbs::prefix_dir(tempdir, ccbs::add_extension(".d"_p))(file), ec);
            ccsh::fs::remove(ccbs::prefix_dir(tempdir, ccbs::add_extension(".o"_p))(file), ec);
        }
        ccsh::fs::remove(outfile, ec);
        return 0;
    }

    auto objects_cmd = object_command();
    auto dependency_cmd = dependency_command();
    auto so_cmd = target_command();

    std::set<rule_ptr> dep_rules;
    for (const auto& file : files)
    {
        dep_rules.emplace(new dependency_rule{
            file,
            ccbs::prefix_dir(tempdir, ccbs::add_extension(".d"_p))(file),
            dependency_cmd
        });
    }

    std::set<rule_ptr> object_rules;
    for (const auto& file : files)
    {
        object_rules.emplace(new rule{
            {file},
            ccbs::prefix_dir(tempdir, ccbs::add_extension(".o"_p))(file),
            objects_cmd,
            {ccbs::prefix_dir(tempdir, ccbs::add_extension(".d"_p))(file)}
        });
    }

    auto so_rule = std::make_shared<link_rule>(rule_outputs(object_rules), outfile, so_cmd, std::set<ccsh::fs::path>{}, dependencies());

    ruleset target;
    target.add_rules(dep_rules);
    target.add_rules(object_rules);
    target.add_rule(so_rule);
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
