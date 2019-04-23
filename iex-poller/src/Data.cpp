#include "Data.h"
#include <nlohmann/json.hpp>

static std::string processSymbol(std::string symbol) {
    for (auto& c : symbol) {
        c = std::toupper(c);
    }
    return symbol;
}

void Company::deserialize(nlohmann::json const& parser) {
    logo   = parser["logo"]["url"];
    name   = parser["company"]["companyName"];
    symbol = processSymbol(parser["company"]["symbol"]);
}

void Quote::deserialize(nlohmann::json const& parser) {
    auto const quote = parser["quote"];
    symbol  = processSymbol(quote["symbol"]);
    price   = quote["latestPrice"];
    updated = std::chrono::system_clock::time_point{std::chrono::milliseconds{quote["latestUpdate"]}};
}
