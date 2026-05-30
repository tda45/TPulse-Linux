#pragma once
#include <string>
#include <vector>

namespace Request {
    class SubdomainFinder {
    public:
        static void Execute(const std::string& targetDomain);
    };
}