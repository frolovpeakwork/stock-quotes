#include "App.h"

#include <thread>
#include "Config.h"
#include "Data.h"
#include "HttpClient.h"
#include "IexClient.h"
#include "Storage.h"

App::App()
    : cfg_{std::make_shared<Config>()},
      http_{std::make_shared<HttpClient>()},
      iex_{std::make_shared<IexClient>(cfg_, http_)},
      storage_{std::make_shared<Storage>()} {
}

App::~App() noexcept = default;

void App::run() {
    storage_->migrate();
    storage_->updateCompanies(iex_->getCompanies());
    while (true) {
        storage_->updateQuotes(iex_->getQuotes());
        std::this_thread::sleep_for(cfg_->interval());
    }
}
