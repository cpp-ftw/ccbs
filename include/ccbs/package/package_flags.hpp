#ifndef CCBS_PACKAGE_FLAGS_HPP
#define CCBS_PACKAGE_FLAGS_HPP

#include <ccbs/package/package.hpp>

namespace ccbs {

class package_flags : public package
{
protected:
    std::vector<ccsh::fs::path> I;
    std::vector<ccsh::fs::path> L;
    std::vector<std::string> l;
    std::vector<std::pair<std::string, std::string>> D;

public:

    timestamp last_modified() const override
    {
        return to_timestamp(0);
    }
    int prepare(options&, compiler_ptr&) override { return 0; }
    void use_flags(compiler&, options&) const override;

    void include_directories(ccsh::fs::path dir)
    {
        I.push_back(ccsh::fs::weakly_canonical(dir));
    }
    void link_directories(ccsh::fs::path dir)
    {
        L.push_back(ccsh::fs::weakly_canonical(dir));
    }
    void link_libraries(std::string lib)
    {
        l.push_back(std::move(lib));
    }
    void definitions(std::string name, std::string value)
    {
        D.emplace_back(std::move(name), std::move(value));
    }
};

}

#endif // CCBS_PACKAGE_FLAGS_HPP
