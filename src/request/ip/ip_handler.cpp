#include "ip_handler.hpp"
#include "../../common/theme.hpp"
#include "../history/history_handler.hpp"
#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <string>

namespace Request {

    size_t IpHandler::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    void IpHandler::Execute() {
        Request::HistoryHandler::LogAction("IP_QUERY", "SUCCESS");
        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cout << Theme::BRIGHT_RED << "[!] CURL baslatilamadi!\n" << Theme::RESET;
            return;
        }

        std::string readBuffer;
        std::string url = "http://ip-api.com/line/?lang=tr"; 

        std::cout << Theme::TEXT_GRAY << "[*] Ag ve IP bilgileri sorgulaniyor...\n" << Theme::RESET;
        std::cout << Theme::TEXT_GRAY << "--------------------------------------------------\n" << Theme::RESET;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cout << Theme::BRIGHT_RED << "[!] Istek basarisiz oldu: " << curl_easy_strerror(res) << "\n" << Theme::RESET;
            return;
        }

        std::vector<std::string> lines;
        size_t pos = 0;
        while ((pos = readBuffer.find('\n')) != std::string::npos) {
            lines.push_back(readBuffer.substr(0, pos));
            readBuffer.erase(0, pos + 1);
        }

        if (lines.size() >= 14 && lines[0] == "success") {
            // 🎨 Başlık kısmını parlatmak için BRIGHT_GREEN rengini devreye soktuk
            std::cout << Theme::BRIGHT_GREEN << "🌐 AG VE DIS IP OZETI\n" << Theme::RESET;
            std::cout << Theme::TEXT_GRAY << "==================================================\n" << Theme::RESET;
            std::cout << Theme::CRIMSON << "  IP Adresi       : " << Theme::RESET << lines[13] << "\n";
            std::cout << Theme::CRIMSON << "  Servis Saglayici: " << Theme::RESET << lines[10] << "\n";
            std::cout << Theme::CRIMSON << "  Ulke / Sehir    : " << Theme::RESET << lines[1] << " / " << lines[5] << "\n";
            std::cout << Theme::CRIMSON << "  Zaman Dilimi    : " << Theme::RESET << lines[9] << "\n";
            std::cout << Theme::CRIMSON << "  Koordinatlar    : " << Theme::RESET << lines[7] << ", " << lines[8] << "\n";
            std::cout << Theme::TEXT_GRAY << "--------------------------------------------------\n" << Theme::RESET;
        } else {
            std::cout << Theme::BRIGHT_RED << "[!] Saglayicidan gecersiz veri dondu.\n" << Theme::RESET;
        }
    }
}