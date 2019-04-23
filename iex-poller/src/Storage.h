#pragma once

#include <vector>
#include <postgres/Connection.h>

struct Company;
struct Quote;

class Storage {
public:
    explicit Storage();
    Storage(Storage const& other) = delete;
    Storage& operator=(Storage const& other) = delete;
    Storage(Storage&& other) noexcept = delete;
    Storage& operator=(Storage&& other) noexcept = delete;
    ~Storage() noexcept;

    void migrate();
    void updateCompanies(std::vector<Company> const& companies);
    void updateQuotes(std::vector<Quote> const& quotes);

private:
    postgres::Connection conn_;
};
