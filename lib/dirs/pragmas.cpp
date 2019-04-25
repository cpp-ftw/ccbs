#include <ccbs/dirs/pragmas.hpp>
#include <ccbs/compiler/gcc.hpp>

using namespace ccsh::literals;

namespace {

template<typename FUNC>
void tokenize_string(std::string line, std::string const& delimiters, FUNC&& func)
{
    char* saveptr = nullptr;
    char* token = ccsh::internal::strtok_compat(&line[0], delimiters.c_str(), &saveptr);

    while (token != nullptr)
    {
        func(token);
        token = ccsh::internal::strtok_compat(nullptr, delimiters.c_str(), &saveptr);
    }
}

}

namespace ccbs {

std::vector<ccsh::fs::path> get_all_pragmas(ccsh::command_builder<ccsh::gcc>& command, int& result)
{
    std::vector<ccsh::fs::path> includes;

    auto pragma_processor = [&](std::string line) {
        std::vector<std::string> parts;
        tokenize_string(line, " ", [&](char * part){
            parts.emplace_back(part);
        });

        if (parts.size() == 3 &&
            parts[0] == "#pragma" &&
            parts[1] == "ccbs")
        {
            if (std::find(includes.begin(), includes.end(), parts[2]) == includes.end())
            {
                // TODO: handle relative paths
                // "# 1 filename" lines of output helps
                includes.emplace_back(parts[2]);
                command.add_arg("-I" + parts[2]);
            }
        }
    };

    size_t includes_count;
    do
    {
        includes_count = includes.size();
        result = (command >= "/dev/null"_p > pragma_processor).run();
    } while(includes_count != includes.size());

    return includes;
}

ccsh::fs::path get_compiler_path()
{
    std::string cxx = ccsh::dollar("CXX");
    if (cxx.empty())
        cxx = "/usr/bin/c++";

    return ccsh::fs::path{cxx};
}

ccsh::command_builder<ccsh::gcc> get_gcc_command(int argc, const char** argv)
{
    ccsh::command_builder<ccsh::gcc> gcc = ccsh::gcc();

    gcc.args().reserve(std::size_t(argc));
    for (int i = 1; i < argc; ++i)
    {
        gcc.add_arg(argv[i]);
    }

    gcc.add_arg("-E");

    return gcc;
}

}
