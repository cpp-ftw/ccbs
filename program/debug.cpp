#include <../ccbs_pkg.ccbs.hpp>
#define _CCBS_CLASSNAME_AUTOGEN ccbs_pkg

int main(int argc, const char** argv)
{
    auto opts = ccbs::parse_options(argc, argv);
    return _CCBS_CLASSNAME_AUTOGEN{}.build(opts);
}

