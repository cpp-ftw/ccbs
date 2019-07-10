#include <ccbs/package/cmake_find_package.hpp>
#include <ccbs/util/containers.hpp>

using namespace ccsh::literals;

namespace ccbs {

int cmake_find_package::prepare(options&, compiler_ptr&)
{
    std::vector<std::string> default_arguments = {
        "--find-package",
        "-DCMAKE_MODULE_PATH=" + cmake_module_path_.string(),
        "-DCOMPILER_ID=GNU",
        "-DLANGUAGE=CXX",
        "-DNAME=" + name_,
    };

    if (!components_.empty())
    {
        std::string arg = "-DARGUMENTS=COMPONENTS";
        for (const auto& component : components_)
        {
            arg += ";";
            arg += component;
        }
        default_arguments.push_back(arg);
    }

    for (const auto& option : options_)
        default_arguments.push_back("-D" + option.first + "=" + option.second);

    auto link_arguments = default_arguments;
    auto compile_arguments = default_arguments;

    merge_vectors(link_arguments, {"-DMODE=LINK"});
    merge_vectors(compile_arguments, {"-DMODE=COMPILE"});

    auto process_compile = [this] (std::string line) {
        if (line.rfind("-I") != std::string::npos)
            include_directories(line.substr(2));
        else if (line.rfind("-D") != std::string::npos)
        {
            auto equals = line.find_first_of('=');
            if (equals == std::string::npos)
                definitions(line.substr(2), "");
            else
                definitions(line.substr(2, equals-2), line.substr(equals + 1));
        }
    };

    auto process_link = [this] (std::string line) {
        if (line.rfind("-l") != std::string::npos)
            link_libraries(line.substr(2));
        else if (line.rfind("-L") != std::string::npos)
            link_directories(line.substr(2));
        else
        {
            ccsh::fs::path p{line};
            link_directories(p.parent_path());
            link_libraries(extract_library_name(p));
        }
    };

    ccsh::command compile_options = ccsh::shell("cmake"_p, compile_arguments) > ccsh::internal::line_splitter_make(process_compile, ' ');
    ccsh::command link_options = ccsh::shell("cmake"_p, link_arguments) > ccsh::internal::line_splitter_make(process_link, ' ');

    return (compile_options && link_options).run();
}

}