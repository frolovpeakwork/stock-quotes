#include "IexClient.h"

#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include "Config.h"
#include "Data.h"
#include "HttpClient.h"

using nlohmann::json;

IexClient::IexClient(std::shared_ptr<Config> cfg, std::shared_ptr<HttpClient> http)
    : cfg_{std::move(cfg)}, http_{std::move(http)} {
}

IexClient::~IexClient() noexcept = default;

std::vector<Company> IexClient::getCompanies() {
    std::vector<Company> companies{};

    auto const parser = json::parse(http_->get(fmt::format("{}/stock/market/batch"
                                                           "?token={}"
                                                           "&symbols={}"
                                                           "&types=company,logo"
                                                           "&filter=symbol,companyName,logo",
                                                           cfg_->url(),
                                                           cfg_->token(),
                                                           cfg_->symbols())));

    for (auto it = parser.begin(); it != parser.end(); ++it) {
        companies.emplace_back();
        companies.back().deserialize(it.value());
    }
    return companies;
}

std::vector<Quote> IexClient::getQuotes() {
    std::vector<Quote> quotes{};

    auto const parser = json::parse(http_->get(fmt::format("{}/stock/market/batch"
                                                           "?token={}"
                                                           "&symbols={}"
                                                           "&types=quote"
                                                           "&filter=symbol,latestPrice,latestUpdate",
                                                           cfg_->url(),
                                                           cfg_->token(),
                                                           cfg_->symbols())));

    for (auto it = parser.begin(); it != parser.end(); ++it) {
        quotes.emplace_back();
        quotes.back().deserialize(it.value());
    }
    return quotes;
}
