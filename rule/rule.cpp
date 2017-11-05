#include "rule.hpp"

namespace ccbs
{

void dump_command(ccsh::command_builder<ccsh::gcc> const& rule, std::ostream& os)
{
    auto& args = rule.args();
    os << rule.binary().string() << " ";
    for (const auto& arg : args)
    {
        os << arg << " ";
    }
    os << std::endl;
}

rule_cmd make_rule_cmd(ccsh::command_builder<ccsh::gcc> const& rule)
{
    return [rule](std::set<ccsh::fs::path> const& inputs,
                  ccsh::fs::path const& output,
                  std::set<package*> const& pkgs) -> int {

        auto rule_copy = rule;

        auto& args = rule_copy.args();
        for (const auto& p : inputs)
            args.push_back(p.string());

        rule_copy.o(output);

        for (const auto& pkg : pkgs)
        {
            for (const auto& def : pkg->definitions())
                rule_copy.D(def.first, def.second);

            for (const auto& include_dir : pkg->include_directories())
                rule_copy.I(include_dir);

            for (const auto& link_dir : pkg->link_directories())
                rule_copy.L(link_dir);

            for (const auto& link_lib : pkg->link_libraries())
                rule_copy.l(link_lib);
        }

        ccsh::fs::path output_dir = output.parent_path();

        if (!output_dir.empty())
        {
            int mkdir_result = ccsh::shell("mkdir", {"-p", output.parent_path().string()}).run();
            if (mkdir_result != 0)
                return mkdir_result;
        }

        dump_command(rule_copy, std::cout);

        return rule_copy.run();
    };
}

std::set<rule_ptr> make_rules(rule_cmd const& cmd, std::set<ccsh::fs::path> const& inputs, path_transformer path_rule,
                              std::experimental::filesystem::path const& temp_dir)
{
    std::set<rule_ptr> result;
    for (const auto& input : inputs)
    {
        ccsh::fs::path output = path_rule(input);
        if (!temp_dir.empty())
        {
            output = temp_dir / output;
        }
        result.emplace(new rule{{input}, output, cmd});
    }
    return result;
}

rule_ptr make_rule(rule_cmd const& cmd, std::set<rule_ptr> const& inputs, ccsh::fs::path const& output)
{
    std::set<ccsh::fs::path> input_files;
    for (const auto& input : inputs)
    {
        input_files.insert(input->output());
    }
    return std::make_shared<rule>(std::move(input_files), output, cmd);
}



}