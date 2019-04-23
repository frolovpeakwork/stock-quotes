#pragma once

#include <chrono>
#include <string>
#include <nlohmann/json_fwd.hpp>
#include <postgres/Visitable.h>

struct Company {
    std::string symbol;
    std::string name;
    std::string logo;

    void deserialize(nlohmann::json const& parser);

    POSTGRES_CXX_TABLE("companies", symbol, name, logo)
};

struct Quote {
    std::string                           symbol;
    double                                price = 0.0;
    std::chrono::system_clock::time_point updated;

    void deserialize(nlohmann::json const& parser);

    POSTGRES_CXX_TABLE("quotes", symbol, price, updated)
};
