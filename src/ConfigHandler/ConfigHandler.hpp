#pragma once

#include <vector>
#include <string>
#include <toml++/toml.hpp>

// I need to make this a namespace instead, terrible decision making it a struct.
// It makes no sense to create objects of this.
// It might actually make sense as the file could be handled multiple times, but it really doesn't unless each value is retrieved by an independent function.
class ConfigHandler {
private:
    std::string file = ".ishellconfig.toml";
    // here I need to add the parsed toml file so I can then get the tables separately
    toml::v3::ex::parse_result parsedconfigfile;
public:
    std::vector<std::vector<std::string>> env();
    std::vector<std::string> rc();
    int maxcommands();
    // this method should be called before manipulating data
    // definitely should change it to not retrieve the file at each of the methods from avobe executions
    void handleConfigFile();
};
