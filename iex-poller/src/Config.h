#pragma once

#include <chrono>
#include <string>

class Config {
public:
    explicit Config();
    Config(Config const& other) = delete;
    Config& operator=(Config const& other) = delete;
    Config(Config&& other) noexcept = delete;
    Config& operator=(Config&& other) noexcept = delete;
    ~Config() noexcept;

    std::string const& url() const;
    std::string const& token() const;
    std::string const& symbols() const;
    std::chrono::seconds interval() const;

private:
    std::string const          url_;
    std::string const          token_;
    std::string const          symbols_;
    std::chrono::seconds const interval_;
};
