#ifndef PROJECT_GCC_HPP
#define PROJECT_GCC_HPP

#include "compiler.hpp"

#include <ccsh/cc/gcc.hpp>

namespace ccbs
{

class gcc : public compiler_t<ccsh::gcc_t>
{
    static constexpr std::array<const char*, 5> std_mapping()
    {
        return {
            "c++98",
            "c++98",
            "c++11",
            "c++14",
            "c++17",
        };
    };
    using base = compiler_t<ccsh::gcc_t>;
public:
    explicit gcc(builder impl) : base(std::move(impl))
    {
        categorized([&](ccsh::gcc_t &cc) { cc.MM(); }, dep_cmd{});
        categorized([&](ccsh::gcc_t &cc) { cc.c(); }, object_cmd{});
        categorized([&](ccsh::gcc_t &cc) { cc.c().PIC(); }, object_so_cmd{});
        categorized([&](ccsh::gcc_t &cc) { cc.shared(); }, shared_cmd{});
    }

    void std_version(std_ std1) override
    {
        categorized([&](ccsh::gcc_t& cc) { cc.std(ccsh::enum_to_string(std1, std_mapping())); }, cpp_flag{});
    }
    void include_directory(ccsh::fs::path const& path) override
    {
        categorized([&](ccsh::gcc_t& cc) { cc.I(ccsh::fs::weakly_canonical(path)); }, cpp_flag{});
    }
    void link_directory(ccsh::fs::path const& path) override
    {
        categorized([&](ccsh::gcc_t& cc) { cc.L(ccsh::fs::weakly_canonical(path)); }, linker_flag{});
    }
    void add_rpath(ccsh::fs::path const& path)
    {
        add_arg("-Wl,-rpath," + ccsh::fs::weakly_canonical(path).string(), linker_flag{});
    }
    void link_library(std::string const& lib) override
    {
        categorized([&](ccsh::gcc_t& cc) { cc.l(lib); }, linker_flag{});
    }
    void definition(std::string const& string, std::string const& string1) override
    {
        categorized([&](ccsh::gcc_t& cc) { cc.D(string, string1); }, cpp_flag{});
    }

    ccsh::command build(std::set<ccsh::fs::path> const& inputs, ccsh::fs::path const& output,
        category_spec c) const override
    {
        if (c.satisfies<static_cmd>())
        {
            std::vector<std::string> args = {"rcs", output.string()};
            for (const auto& p : inputs)
                args.push_back(p.string());
            return ccsh::shell("ar", args);
        }
        return base::build(inputs, output, c);
    }
};

}

#endif //PROJECT_GCC_HPP
