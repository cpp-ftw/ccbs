#ifndef PROJECT_BUILD_TARGET_HPP
#define PROJECT_BUILD_TARGET_HPP

#include <ccbs/rule/ruleset.hpp>
#include <ccbs/rule/rule.hpp>
#include <ccbs/package/repository.hpp>
#include <ccbs/util/polymorphic_value.hpp>
#include <ccbs/compiler/gcc.hpp>

namespace ccbs {

class build_target
{
    std::set<package*> dependencies_;
    std::set<ccsh::fs::path> files;
    ccsh::fs::path tempdir;
    ccsh::fs::path outfile;
    ccsh::fs::path project_root_;
    std::string project_;
    compiler_ptr cmd;

public:

    explicit build_target(ccsh::fs::path output, compiler_ptr cmd = jbcoe::make_polymorphic_value<gcc>(ccsh::gcc{}))
        : outfile(std::move(output))
        , cmd(std::move(cmd))
    {}

    virtual int build_dependencies(options&);
    virtual int build_rules(options&);
    virtual int build(options&);

    void sources(std::set<ccsh::fs::path> const& source_files)
    {
        for (const auto& file : source_files)
            files.insert(ccsh::fs::weakly_canonical(file));
    }

    void sources(ccsh::fs::path const& source_file)
    {
        files.insert(source_file);
    }

    std::set<package*> const& dependencies() const { return dependencies_; }

    void add_dependency(package& package)
    {
        dependencies_.insert(&package);
        for (auto depPtr : package.dependencies())
            dependencies_.insert(depPtr);
    }

    std::string const& project() const { return project_; }
    void project(std::string name) { project_ = std::move(name); }

    ccsh::fs::path const& project_root() const { return project_root_; }
    void project_root(ccsh::fs::path dir) { project_root_ = std::move(dir); }

    template<typename PACKAGE>
    void depends() { add_dependency(repository::get<PACKAGE>()); }

    void temp_dir(ccsh::fs::path dir) { tempdir = std::move(dir); }

    compiler_ptr& command() { return cmd; }
    compiler_ptr command_copy() const { return cmd; }

    ccsh::fs::path const& output() const { return outfile; }

    virtual rule_cmd dependency_command() = 0;
    virtual rule_cmd object_command() = 0;
    virtual rule_cmd target_command() = 0;

    virtual ~build_target() = default;
};

}

#endif //PROJECT_BUILD_TARGET_HPP
