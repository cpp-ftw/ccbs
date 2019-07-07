#include <ccbs/rule/dependency_rule.hpp>

#include <ccsh/core.hpp>

namespace
{

void dependency_parser(std::string line, std::set<ccsh::fs::path>& headers)
{
    auto npos = std::string::npos;

    auto colon = line.find(':');
    if (colon != npos)
        line = line.substr(colon + 1);

    if (line[line.size()-1] == '\\' && line[line.size()-2] == ' ')
        line = line.substr(0, line.length() - 2);

    while (!line.empty())
    {
        size_t last_space = 0;
        while (true)
        {
            last_space = line.find(' ', last_space);
            if (last_space != 0 && line[last_space - 1] == '\\')
            {
                line.erase(last_space - 1, 1);
            }
            else
            {
                break;
            }
        }

        if (last_space == npos)
        {
            headers.emplace(std::move(line));
            line.clear(); // std::move leaves line in an unspecified state, need to make sure it's clear
        }
        else if (last_space == 0)
        {
            line = line.substr(1);
        }
        else
        {
            headers.emplace(std::string{&line[0], &line[last_space]});
            line = line.substr(last_space + 1);
        }
    }
}

}

namespace ccbs
{

int dependency_rule::explore()
{
    if (!ccsh::fs::exists(output()))
        return 0;

    auto parser = [&](std::string line) {
        return dependency_parser(std::move(line), dependencies());
    };

    return (ccsh::cat{this->output()} > parser).run();
}

}

