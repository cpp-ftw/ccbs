#include <ccbs/package/cmake_package.hpp>

using namespace ccsh::literals;

namespace ccbs
{

int cmake_package::prepare(options&, compiler_ptr&)
{
    ccsh::command generate_command = ccsh::shell("cmake"_p, {basedir().string()});
    ccsh::command build_command = ccsh::shell("cmake"_p, {"--build", basedir().string()});

    return (build_command || (generate_command && build_command)).run();
}

}
