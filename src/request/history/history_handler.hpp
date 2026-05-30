#pragma once
#include <string>
#include <vector>

namespace Request {

    struct LogEntry {
        std::string timestamp;
        std::string command;
        std::string status;
    };

    class HistoryHandler {
    public:
        static void LogAction(const std::string& command, const std::string& status = "SUCCESS");
        static void Execute();

    private:
        static const std::string g_HistoryFile;
        static std::string GetCurrentTimestamp();
    };
}