#include "HostHandler.hpp"

#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <string>

std::string HostHandler::getHost() {
    char host[_POSIX_HOST_NAME_MAX + 1];
    gethostname(host, sizeof(host));

    return static_cast<std::string>(host);
}

std::string HostHandler::getUser() {
    uid_t uid = getuid();
    struct passwd* pw = getpwuid(uid);

    return static_cast<std::string>(pw->pw_name);
}
