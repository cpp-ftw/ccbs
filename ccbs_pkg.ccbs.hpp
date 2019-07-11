#pragma ccbs /mnt/game/repos/ccbs/ccsh/include
#pragma ccbs /mnt/game/repos/ccbs/ccsh/wrappers

#include <ccbs/ccbs.hpp>

using namespace ccsh::literals;

struct pthread_pkg : public ccbs::package_flags
{
    void use_flags(ccbs::compiler& compiler, ccbs::options&) const override
    {
        ccbs::visit_one(compiler, [](ccbs::gcc& cc) {
            cc.add_arg("-pthread");
        });
    }
};

struct ccsh_pkg : public ccbs::cmake_package
{
    ccsh_pkg() : ccbs::cmake_package{CCBS_DOT / "ccsh"_p, {"ccsh_lib"}}
    {
        include_directories(basedir() / "wrappers"_p);
    }
    std::set<package*> dependencies() const override
    {
        return { &ccbs::repository::get<pthread_pkg>() };
    }
};

struct ccbs_pkg : public ccbs::shared_library
{
    ccbs_pkg() : ccbs::shared_library(CCBS_DOT / "libccbs2.so"_p)
    {
        project("ccbs");
        project_root(CCBS_DOT);
        sources(ccbs::find_matching(CCBS_DOT / "lib"_p, "*.cpp"));
        temp_dir(CCBS_DOT / "tmp"_p);
        depends<pthread_pkg>();
        depends<ccsh_pkg>();
    }

    void build_flags(ccbs::options &, ccbs::compiler_ptr &compiler_) override
    {
#ifdef _WIN32
        compiler_->definition("_UNICODE", "1");
#endif
        compiler_->include_directory(CCBS_DOT / "include"_p);
        compiler_->std_version(ccbs::compiler::cpp11);
    }

};
