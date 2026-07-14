#pragma once

#include <vector>
#include <string>
#include <toml++/toml.hpp>

// I need to make this a namespace instead, terrible decision making it a struct. - discarded
// It makes no sense to create objects of this. - discarded
// It might actually make sense as the file could be handled multiple times, but it really doesn't unless each value is retrieved by an independent function. - done
class ConfigHandler {
private:
    static std::string file;
    static toml::v3::ex::parse_result parsedconfigfile;
public:
    std::vector<std::vector<std::string>> env();
    std::vector<std::string> rc();
    int maxcommands();
    // this method should be called before manipulating data - done
    // definitely should change it to not retrieve the file at each of the methods from avobe executions - done
    static void handleConfigFile();
};
