#ifndef PROJECT_CMAKE_PACKAGE_HPP
#define PROJECT_CMAKE_PACKAGE_HPP

#include <ccbs/package/installed_package.hpp>

namespace ccbs {

class cmake_package : public ccbs::installed_package
{
    std::vector<std::pair<std::string, std::string>> options_;

public:
    using installed_package::installed_package;

    int prepare(options&, compiler_ptr&) override;

    void add_option(std::string key, std::string value)
    {
        options_.emplace_back(std::move(key), std::move(value));
    }
    void add_option(std::string key, bool value)
    {
        options_.emplace_back(std::move(key), value ? "ON" : "OFF");
    }
};

}

#endif //PROJECT_CMAKE_PACKAGE_HPP
