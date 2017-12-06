#ifndef CCBS_PACKAGE_HPP
#define CCBS_PACKAGE_HPP

#include <ccbs/dirs/options.hpp>

#include <ccbs/rule/timestamp.hpp>

#include <ccbs/compiler/compiler.hpp>

#include <ccsh/ccsh.hpp>

#include <memory>
#include <set>

namespace ccbs {

class package
{
public:
    virtual int prepare(options&) = 0;

    virtual timestamp last_modified() const = 0;

    virtual std::set<package*> dependencies() const { return {}; }

    virtual void add_arguments(compiler&) const = 0;

    virtual ~package() = default;
};

}

#endif // CCBS_PACKAGE_HPP
