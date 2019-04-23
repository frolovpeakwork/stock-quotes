cmake_minimum_required(VERSION 3.8)
project(IexPoller)

add_subdirectory(deps/fmt)
add_subdirectory(deps/json)
add_subdirectory(deps/postgres-cxx-client)
find_package(CURL REQUIRED)

add_executable(IexPoller
        src/App.cpp
        src/Config.cpp
        src/Data.cpp
        src/Errors.cpp
        src/HttpClient.cpp
        src/IexClient.cpp
        src/main.cpp
        src/Storage.cpp
        )

target_include_directories(IexPoller
        PRIVATE
        deps/fmt/include
        deps/json/include
        )

target_link_libraries(IexPoller
        PRIVATE
        fmt
        PostgresCxxClient::PostgresCxxClient
        ${CURL_LIBRARIES}
        )
