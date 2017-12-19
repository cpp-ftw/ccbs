#include <ccbs/rule/rule.hpp>

#include <mutex>

namespace ccbs
{

void dump_command(compiler_ptr const& rule, std::ostream& os)
{
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock{mtx};
    auto& args = rule->native().args();
    os << rule->native().binary().string() << " ";
    for (const auto& arg : args)
    {
        os << arg << " ";
    }
    os << std::endl;
}

rule_cmd make_rule_cmd(compiler_ptr const& rule)
{
    return [rule](std::set<ccsh::fs::path> const& inputs,
                  ccsh::fs::path const& output,
                  std::set<package*> const& pkgs) -> int {

        auto rule_copy = rule;

        for (const auto& p : inputs)
            rule_copy->input(p);

        rule_copy->output(output);

        for (const auto& pkg : pkgs)
            pkg->add_arguments(*rule_copy);

        ccsh::fs::path output_dir = output.parent_path();

        if (!output_dir.empty())
        {
            int mkdir_result = ccsh::shell("mkdir", {"-p", output.parent_path().string()}).run();
            if (mkdir_result != 0)
                return mkdir_result;
        }

        dump_command(rule_copy, std::cout);

        return rule_copy->run();
    };
}

std::set<ccsh::fs::path> rule_outputs(std::set<rule_ptr> const& rules)
{
    std::set<ccsh::fs::path> output_files;
    for (const auto& input : rules)
    {
        output_files.insert(input->output());
    }
    return output_files;
}

bool rule::needs_rebuild() const
{
    if (!ccsh::fs::exists(output_))
        return true;

    auto outstamp = ccsh::fs::last_write_time(output_);

    auto is_fresher = [outstamp](ccsh::fs::path const& p) {
        return !ccsh::fs::exists(p) || outstamp < ccsh::fs::last_write_time(p);
    };

    return std::any_of(inputs_.begin(), inputs_.end(), is_fresher) ||
        std::any_of(dependencies_.begin(), dependencies_.end(), is_fresher);
}

}
