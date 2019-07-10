#ifndef CCBS_PKG_CONFIG_PACKAGE_HPP
#define CCBS_PKG_CONFIG_PACKAGE_HPP

#include <ccbs/package/package_flags.hpp>

namespace ccbs {

class pkg_config_package : public package_flags
{
    std::string pkg_name;

public:
    explicit pkg_config_package(std::string pkg_name)
        : package_flags({})
        , pkg_name(std::move(pkg_name))
    { }

    int prepare(options&, compiler_ptr&) override
    {
        std::string output;
        return (ccsh::shell("pkg-config", {"--libs-only-l", pkg_name}) > output).run();
    }
    timestamp last_modified() const override
    {
        return to_timestamp(0);
    }
};

}

#endif // CCBS_PKG_CONFIG_PACKAGE_HPP
