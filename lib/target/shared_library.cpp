#include <ccbs/target/shared_library.hpp>

using namespace ccsh::literals;

namespace ccbs
{

void shared_library::add_arguments(compiler& cc) const
{
//    const auto& public_flags = command_public_->native().args();
//    auto& flags = cc.native().args();
//    flags.insert(flags.end(), public_flags.begin(), public_flags.end());
    cc.link_directory(output().parent_path());
    cc.link_library(extract_library_name(output()));
}

}

