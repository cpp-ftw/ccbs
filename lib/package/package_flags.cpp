#include <ccbs/package/package_flags.hpp>

namespace ccbs
{

void package_flags::use_flags(compiler& cc) const
{
    for (const auto& def : D)
        cc.definition(def.first, def.second);

    for (const auto& include_dir : I)
        cc.include_directory(include_dir);

    for (const auto& link_dir : L)
        cc.link_directory(link_dir);

    for (const auto& link_lib : l)
        cc.link_library(link_lib);
}


}

