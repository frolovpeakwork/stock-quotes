#pragma once

#include <memory>

class Config;
class HttpClient;
class IexClient;
class Storage;

class App {
public:
    explicit App();
    App(App const& other) = delete;
    App& operator=(App const& other) = delete;
    App(App&& other) noexcept = delete;
    App& operator=(App&& other) noexcept = delete;
    ~App() noexcept;

    void run();

private:
    std::shared_ptr<Config>     cfg_;
    std::shared_ptr<HttpClient> http_;
    std::shared_ptr<IexClient>  iex_;
    std::shared_ptr<Storage>    storage_;
};
