#ifndef PROJECT_INSTALLED_PACKAGE_HPP
#define PROJECT_INSTALLED_PACKAGE_HPP

#include <ccbs/package/package_flags.hpp>

namespace ccbs
{

class installed_package : public package_flags
{
    ccsh::fs::path basedir_;
public:
    explicit installed_package(ccsh::fs::path basedir_ = ccsh::fs::path("/usr"), std::vector<std::string> libraries = {});

    ccsh::fs::path& basedir() { return basedir_; }
    ccsh::fs::path const& basedir() const { return basedir_; }

    int prepare(options&, compiler_ptr&) override { return 0; }
    timestamp last_modified() const override
    {
        // TODO: find latest library
        return to_timestamp(0);
    }
};

}

#endif //PROJECT_INSTALLED_PACKAGE_HPP
