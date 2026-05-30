#pragma once
#include <string>

namespace Request {
    class PostHandler {
    public:
        static void Execute(const std::string& url, const std::string& payload);
    };
}