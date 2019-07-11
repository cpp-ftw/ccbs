#include <ccbs/target/shared_library.hpp>
#include <ccbs/util/containers.hpp>

using namespace ccsh::literals;

namespace ccbs
{

int shared_library::prepare(options& options_, compiler_ptr& compiler_)
{
    auto serialized_deps = serialize_set(
            dependencies(),
            [](package* dep) { return dep; },
            [](package* dep) { return dep->dependencies(); }
    );
    return build_rules(serialized_deps, options_, compiler_);
}

void shared_library::use_flags(compiler& cc, options&) const
{
//    const auto& public_flags = command_public_->native().args();
//    auto& flags = cc.native().args();
//    flags.insert(flags.end(), public_flags.begin(), public_flags.end());
    cc.link_directory(output().parent_path());
    cc.link_library(extract_library_name(output()));
}


}

