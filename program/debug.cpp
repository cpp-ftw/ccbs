#include <../ccbs_pkg.ccbs.hpp>
#define _CCBS_CLASSNAME_AUTOGEN ccbs_pkg

int main(int argc, const char** argv)
{
    auto opts = ccbs::parse_options(argc, argv);
    jbcoe::polymorphic_value<ccbs::compiler> comp = jbcoe::make_polymorphic_value<ccbs::gcc>(ccsh::gcc{});
    return _CCBS_CLASSNAME_AUTOGEN{}.build(opts, comp);
}
