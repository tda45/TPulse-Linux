#pragma once
#include <string>
#include <curl/curl.h>

namespace Request {
    class CryptoHandler {
    public:
        static void Execute();
    private:
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
        static std::string ExtractValue(const std::string& json, const std::string& key);
        static void FetchCoinData(CURL* curl, const std::string& coinId, const std::string& label);
    };
}