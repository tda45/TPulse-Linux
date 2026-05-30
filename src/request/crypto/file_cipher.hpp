#pragma once
#include <string>

namespace Request {
    class FileCipher {
    public:
        static void Encrypt(const std::string& filePath, const std::string& key);
        static void Decrypt(const std::string& filePath, const std::string& key);
    };
}