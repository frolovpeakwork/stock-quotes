#include "HttpClient.h"

#include "Errors.h"

static size_t
callback(const char* const in, size_t const size, size_t const num, std::string* const out) {
    auto const total = size * num;
    out->append(in, total);
    return total;
}

HttpClient::HttpClient()
    : handle_{curl_easy_init()} {
    curl_easy_setopt(handle_, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(handle_, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(handle_, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle_, CURLOPT_WRITEFUNCTION, callback);
}

HttpClient::~HttpClient() noexcept {
    curl_easy_cleanup(handle_);
}

std::string HttpClient::get(std::string const& url) const {
    std::string data{};
    curl_easy_setopt(handle_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle_, CURLOPT_WRITEDATA, &data);

    auto code = 0;
    curl_easy_perform(handle_);
    curl_easy_getinfo(handle_, CURLINFO_RESPONSE_CODE, &code);
    ASSERT(code == 200, "fail to request " << url << ": " << code);

    return data;
}
