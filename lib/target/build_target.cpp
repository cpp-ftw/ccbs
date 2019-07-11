#include <ccbs/target/build_target.hpp>
#include <ccbs/rule/ruleset.hpp>
#include <ccbs/rule/dependency_rule.hpp>
#include <ccbs/rule/link_rule.hpp>
#include <ccbs/util/containers.hpp>

using namespace ccsh::literals;

namespace ccbs
{

int build_target::build_dependencies(std::vector<package*> deps, options& options_, compiler_ptr& compiler_)
{
    for (const auto& dep : deps)
    {
        int result = dep->prepare(options_, compiler_);
        if (result != 0)
            return result;
    }
    return 0;
}

void build_target::add_rules(ruleset& rules, compiler_ptr& compiler_)
{
    ccsh::fs::path tempdir = this->tempdir / this->project_;

    auto objects_cmd = object_command(compiler_);
    auto dependency_cmd = dependency_command(compiler_);
    auto so_cmd = target_command(compiler_);

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


int build_target::build_rules(std::vector<package*> deps, options& options_, compiler_ptr& compiler_)
{
    ruleset target;
    build_flags(options_, compiler_);
    add_rules(target, compiler_);

    if (options_[action::key] == action::clean)
    {
        ccsh::fs::error_code ec;
        for (const auto& rule : target.rules())
            ccsh::fs::remove(rule->output(), ec);
        return ec ? 1 : 0;
    }

    return target.build(deps, options_);
}

int build_target::build(options& options_, compiler_ptr& compiler_)
{
    auto serialized_deps = serialize_set(
            dependencies(),
            [](package* dep) { return dep; },
            [](package* dep) { return dep->dependencies(); }
    );

    int result1 = build_dependencies(serialized_deps, options_, compiler_);
    if (result1 != 0)
        return result1;

    return build_rules(serialized_deps, options_, compiler_);
}

}
