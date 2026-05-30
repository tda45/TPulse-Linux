#include "history_handler.hpp"
#include "../../common/theme.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace Request {

    const std::string HistoryHandler::g_HistoryFile = "tpulse_history.log";

    std::string HistoryHandler::GetCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        
        // 🐧 Linux/POSIX için Thread-Safe Zaman Dönüşümü (localtime_r)
        std::tm local_time{};
        localtime_r(&now_time, &local_time);

        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_time);
        return std::string(buffer);
    }

    void HistoryHandler::LogAction(const std::string& command, const std::string& status) {
        std::ofstream outFile(g_HistoryFile, std::ios::app);
        if (!outFile) return;

        outFile << "[" << GetCurrentTimestamp() << "] | " << command << " | " << status << "\n";
        outFile.close();
    }

    void HistoryHandler::Execute() {
        std::ifstream inFile(g_HistoryFile);
        
        std::cout << Theme::TEXT_GRAY << "[*] T-PULSE Islem gecmisi yukleniyor...\n" << Theme::RESET;
        std::cout << Theme::TEXT_GRAY << "--------------------------------------------------\n" << Theme::RESET;
        std::cout << Theme::RESET << "📜 ISLEM GECMISI (SON ISLEMLER)\n";
        std::cout << Theme::TEXT_GRAY << "==================================================\n" << Theme::RESET;

        if (!inFile) {
            std::cout << "  Henuz hicbir islem kaydi bulunmuyor.\n";
            std::cout << Theme::TEXT_GRAY << "--------------------------------------------------\n" << Theme::RESET;
            return;
        }

        std::string line;
        std::vector<std::string> allLogs;

        while (std::getline(inFile, line)) {
            if (!line.empty()) {
                allLogs.push_back(line);
            }
        }
        inFile.close();

        if (allLogs.empty()) {
            std::cout << "  Henuz hicbir islem kaydi bulunmuyor.\n";
        } else {
            int count = 0;
            for (auto it = allLogs.rbegin(); it != allLogs.rend() && count < 10; ++it, ++count) {
                std::string log = *it;
                size_t firstPipe = log.find('|');
                size_t secondPipe = log.find('|', firstPipe + 1);

                if (firstPipe != std::string::npos && secondPipe != std::string::npos) {
                    std::string timePart = log.substr(0, firstPipe - 1);
                    std::string cmdPart = log.substr(firstPipe + 2, secondPipe - firstPipe - 3);
                    std::string statusPart = log.substr(secondPipe + 2);

                    // 🎨 Linux terminal şovu: Başarılı işlemler parlak yeşil, hatalar parlak kırmızı!
                    std::string statusColor = (statusPart == "SUCCESS") ? Theme::BRIGHT_GREEN : Theme::BRIGHT_RED;

                    std::cout << Theme::TEXT_GRAY << timePart << Theme::RESET 
                              << " -> " << Theme::CRIMSON << std::left << std::setw(15) << cmdPart << Theme::RESET
                              << " [" << statusColor << statusPart << Theme::RESET << "]\n";
                } else {
                    std::cout << "  " << log << "\n";
                }
            }
        }

        std::cout << Theme::TEXT_GRAY << "--------------------------------------------------\n" << Theme::RESET;
    }
}