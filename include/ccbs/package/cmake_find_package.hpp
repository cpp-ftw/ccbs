#ifndef PROJECT_CMAKE_FIND_PACKAGE_HPP
#define PROJECT_CMAKE_FIND_PACKAGE_HPP

#include <ccbs/package/installed_package.hpp>

#include <ccbs/util/find_utils.hpp>

namespace ccbs {

class cmake_find_package : public ccbs::package_flags
{
    std::vector<std::pair<std::string, std::string>> options_;
    ccsh::fs::path cmake_module_path_;
    std::string name_;
    std::vector<std::string> components_;

public:

    explicit cmake_find_package(std::string name, std::vector<std::string> components_ = {})
        : cmake_module_path_(CCBS_DOT)
        , name_(std::move(name))
        , components_(std::move(components_))
    {}

    int prepare(options&, compiler_ptr&) override;

    void add_option(std::string key, std::string value)
    {
        options_.emplace_back(std::move(key), std::move(value));
    }
    void add_option(std::string key, bool value)
    {
        options_.emplace_back(std::move(key), value ? "ON" : "OFF");
    }

    timestamp last_modified() const override
    {
        // TODO: find latest library
        return to_timestamp(0);
    }
};

}

#endif //PROJECT_CMAKE_FIND_PACKAGE_HPP
