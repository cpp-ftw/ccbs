#include <ccbs/dirs/pragmas.hpp>
#include <ccbs/util/containers.hpp>
#include <ccbs/util/find_utils.hpp>
#include <ccbs/rule/rule.hpp>

using namespace ccsh::literals;

int main(int argc, const char ** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: ccbs <package.ccbs.hpp> <ARGS=values...>" << std::endl;
        return 1;
    }

    ccsh::fs::path self_include_dir = ccsh::fs::weakly_canonical(CCBS_DOT / "../include"_p);

    ccsh::command_builder<ccsh::gcc> pragma_command = ccsh::gcc();
    pragma_command.I(self_include_dir);
    pragma_command.add_arg(self_include_dir);
    pragma_command.add_arg(argv[1]);
    pragma_command.add_arg("-E");

    int result;
    auto includes = ccbs::get_all_pragmas(pragma_command, result);
    if (result != 0)
    {
        return (pragma_command > "/dev/null"_p).run();
    }

    // TODO: detect duplicates

    ccsh::fs::path header{argv[1]};

    std::string classname = header.filename().string();
    auto dot = classname.find('.');
    if (dot != std::string::npos)
        classname = classname.substr(0, dot);


    ccsh::fs::path output = "/tmp/"_p / ccsh::generate_filename();

    ccsh::command_builder<ccsh::gcc> gcc = ccsh::gcc("-");

    gcc.D("_CCBS_CLASSNAME_AUTOGEN", classname);
    gcc.I(self_include_dir);
    gcc.I(ccsh::fs::absolute(header).parent_path());

    // TODO: make these configurable
    gcc.L(CCBS_DOT / "../lib"_p);
    gcc.rpath(CCBS_DOT / "../lib"_p);
    gcc.l("ccbs_lib");

    gcc.L(CCBS_DOT / "../../ccsh/lib"_p);
    gcc.rpath(CCBS_DOT / "../../ccsh/lib"_p);
    gcc.l("ccsh_lib");

    gcc.o(output);

    gcc.add_arg("-x");
    gcc.add_arg("c++");

    for (const auto& dir : includes)
        gcc.I(dir);

    std::string program =
    "#include <" + header.string() + ">\n" +
    R"PROGRAM(
int main(int argc, const char** argv)
{
    auto opts = ccbs::parse_options(argc, argv);
    return (_CCBS_CLASSNAME_AUTOGEN){}.build(opts);
}
    )PROGRAM";

    result = (gcc < program).run();
    if (result != 0)
        return result;

    std::vector<std::string> args;
    for (int i = 2; i < argc; ++i)
        args.emplace_back(argv[i]);

    return ccsh::shell(output, args).run();
}
