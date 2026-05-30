#pragma once
#include <string>

namespace Request {
    class DownloadHandler {
    public:
        static void Execute(const std::string& url, const std::string& outputFilename);
    };
}