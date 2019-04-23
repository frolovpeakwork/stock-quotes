#pragma once

#include <memory>
#include <vector>

class Config;
class HttpClient;
struct Company;
struct Quote;

class IexClient {
public:
    explicit IexClient(std::shared_ptr<Config> cfg, std::shared_ptr<HttpClient> http);
    IexClient(IexClient const& other) = delete;
    IexClient& operator=(IexClient const& other) = delete;
    IexClient(IexClient&& other) noexcept = delete;
    IexClient& operator=(IexClient&& other) noexcept = delete;
    ~IexClient() noexcept;

    std::vector<Company> getCompanies();
    std::vector<Quote> getQuotes();

private:
    std::shared_ptr<Config> const     cfg_;
    std::shared_ptr<HttpClient> const http_;
};
