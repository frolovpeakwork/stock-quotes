#pragma once

#include <curl/curl.h>
#include <string>

class HttpClient {
public:
    explicit HttpClient();
    HttpClient(HttpClient const& other) = delete;
    HttpClient& operator=(HttpClient const& other) = delete;
    HttpClient(HttpClient&& other) noexcept = delete;
    HttpClient& operator=(HttpClient&& other) noexcept = delete;
    ~HttpClient() noexcept;

    std::string get(std::string const& url) const;

private:
    CURL* handle_;
};
