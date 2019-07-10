#ifndef PROJECT_COMPILER_HPP
#define PROJECT_COMPILER_HPP

#include "compiler_ptr.hpp"

#include <ccsh/ccsh.hpp>
#include <ccbs/util/polymorphic_value.hpp>
#include <ccbs/util/category.hpp>

#include <set>

namespace ccbs
{

class compiler
{
public:
    virtual ~compiler() = default;

    void add_arg(const std::string& arg) { add_arg(arg, category_all{}); }
    virtual void add_arg(const std::string& arg, category_spec c) = 0;

    virtual ccsh::command build(std::set<ccsh::fs::path> const& inputs, ccsh::fs::path const& output, category_spec c) const = 0;

    enum std_
    {
        cpp98,
        cpp03,
        cpp11,
        cpp14,
        cpp17,
    };

    virtual void std_version(std_ std1) = 0;

    virtual void include_directory(ccsh::fs::path const&) = 0;
    virtual void link_directory(ccsh::fs::path const&) = 0;
    virtual void link_library(std::string const&) = 0;

    virtual void definition(std::string const&, std::string const&) = 0;

    struct dep_cmd final : category_of<dep_cmd> {};
    struct object_cmd final : category_of<object_cmd> {};
    struct object_so_cmd final : category_of<object_so_cmd> {};
    struct exe_cmd final : category_of<exe_cmd> {};
    struct shared_cmd final : category_of<shared_cmd> {};
    struct static_cmd final : category_of<static_cmd> {};

    struct cpp_flag final : category_of<cpp_flag, dep_cmd, object_cmd, object_so_cmd> {};
    struct c_flag final : category_of<c_flag, cpp_flag> {};
    struct cxx_flag final : category_of<cxx_flag, cpp_flag> {};
    struct linker_flag final : category_of<linker_flag, exe_cmd, shared_cmd> {};

    struct compiler_flag final : category_of<compiler_flag, c_flag, cxx_flag, linker_flag> {};
};

template<typename Impl>
class compiler_t : public compiler
{
public:
    using builder = ccsh::command_builder<Impl>;
private:
    builder impl_;
    std::vector<std::pair<ccsh::tstring_t, category_spec>> args_;

public:

    explicit compiler_t(builder impl)
        : impl_(std::move(impl))
    { }

    ccsh::command build(std::set<ccsh::fs::path> const& inputs, ccsh::fs::path const& output, category_spec c) const override
    {
        builder impl = impl_;
        for (const auto& p : inputs)
            impl.input(p);
        impl.output(output);
        for (const auto& arg_pair : args_)
            if (arg_pair.second.satisfies(c))
                impl.add_arg(arg_pair.first);
        return impl;
    }

    template<typename Func>
    void categorized(Func&& func, category_spec c = category_all{})
    {
        Impl impl;
        func(impl);
        for (auto& arg : impl.args())
            args_.emplace_back(std::move(arg), c);
    }

    using compiler::add_arg;
    void add_arg(const std::string& arg, category_spec c) override { args_.emplace_back(ccsh::from_utf8(arg), c); }
};

}

#endif //PROJECT_COMPILER_HPP
