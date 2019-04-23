#include "Config.h"

#include <cstdlib>
#include <cstring>
#include "Errors.h"

static std::string getEnv(char const* const name) {
    auto const val = getenv(name);
    ASSERT(val != nullptr, name << " is not set")
    ASSERT(strlen(val) != 0, name << " is empty")
    return val;
}

Config::Config()
    : url_{getEnv("IEX_URL")},
      token_{getEnv("IEX_TOKEN")},
      symbols_{getEnv("IEX_SYMBOLS")},
      interval_{std::stoi(getEnv("IEX_POLL_INTERVAL"))} {
}

Config::~Config() noexcept = default;

std::string const& Config::url() const {
    return url_;
}

std::string const& Config::token() const {
    return token_;
}

std::string const& Config::symbols() const {
    return symbols_;
}

std::chrono::seconds Config::interval() const {
    return interval_;
}
