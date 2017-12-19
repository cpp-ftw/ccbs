#ifndef PROJECT_OPTIONS_HPP
#define PROJECT_OPTIONS_HPP

#include <unordered_map>
#include <string>

namespace ccbs {

using options = std::unordered_map<std::string, std::string>;

options parse_options(int argc, const char** argv);

namespace config {

static constexpr const char* key = "CONFIG";

static constexpr const char* debug = "debug";
static constexpr const char* release = "release";

}

namespace action {

static constexpr const char* key = "ACTION";

static constexpr const char* clean = "clean";
static constexpr const char* rebuild = "rebuild";


}

}

#endif //PROJECT_OPTIONS_HPP
