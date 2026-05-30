#pragma once
#include <string>

namespace Request {
    class IpHandler {
    public:
        static void Execute();
    private:
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    };
}