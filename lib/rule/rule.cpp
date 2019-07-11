#include <ccbs/rule/rule.hpp>

#include <ccbs/compiler/compiler.hpp>

#include <mutex>

namespace ccbs
{

void dump_command(ccsh::internal::command_native const& cmd, std::ostream& os)
{
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock{mtx};
    auto& args = cmd.args();
    os << cmd.binary().string() << " ";
    for (const auto& arg : args)
    {
        os << ccsh::to_utf8(arg) << " ";
    }
    os << std::endl;
}

rule_cmd make_rule_cmd(compiler_ptr const& rule, category_spec c)
{
    return [rule, c](std::set<ccsh::fs::path> const& inputs,
                  ccsh::fs::path const& output,
                  std::vector<package*> const& pkgs) -> int {

        auto rule_copy = rule;

        for (auto rit = pkgs.rbegin(); rit != pkgs.rend(); ++rit)
            (*rit)->use_flags(*rule_copy);

        ccsh::fs::path output_dir = output.parent_path();

        if (!output_dir.empty())
        {
            ccsh::fs::create_directories(output_dir);
        }

        ccsh::command cmd = rule_copy->build(inputs, output, c);
        if (auto* native = dynamic_cast<ccsh::internal::command_native*>(cmd.base()))
            dump_command(*native, std::cout);
        else
            std::cout << "MAKE " << output.string() << std::endl;

        return rule_copy->build(inputs, output, c).run();
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
