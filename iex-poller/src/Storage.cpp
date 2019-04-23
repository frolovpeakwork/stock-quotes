#include "Storage.h"

#include <postgres/Statement.h>
#include "Data.h"

static auto constexpr MIGRATION = R"(
CREATE TABLE IF NOT EXISTS companies (
    symbol TEXT PRIMARY KEY,
    name   TEXT,
    logo   TEXT
);

CREATE TABLE IF NOT EXISTS quotes (
    symbol  TEXT REFERENCES companies (symbol) ON DELETE CASCADE,
    price   DOUBLE PRECISION,
    updated TIMESTAMP,
    PRIMARY KEY (symbol, updated)
);
)";

Storage::Storage() {
    conn_.check();
}

Storage::~Storage() noexcept = default;

void Storage::migrate() {
    conn_.execRaw(MIGRATION).check();
}

void Storage::updateCompanies(std::vector<Company> const& companies) {
    auto const range = std::pair{companies.begin(), companies.end()};
    auto const sql   = "INSERT INTO companies ("
                       + postgres::Statement<Company>::fields()
                       + ") VALUES "
                       + postgres::RangeStatement::placeholders(range.first, range.second)
                       + " ON CONFLICT (symbol) DO UPDATE"
                         " SET name = EXCLUDED.name, logo = EXCLUDED.logo";
    conn_.exec(postgres::Command{sql, range}).check();
}

void Storage::updateQuotes(std::vector<Quote> const& quotes) {
    auto const range = std::pair{quotes.begin(), quotes.end()};
    auto const sql   = "INSERT INTO quotes ("
                       + postgres::Statement<Quote>::fields()
                       + ") VALUES "
                       + postgres::RangeStatement::placeholders(range.first, range.second)
                       + " ON CONFLICT DO NOTHING";
    conn_.exec(postgres::Command{sql, range}).check();
}
