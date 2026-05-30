#include "crypto_handler.hpp"
#include "../../common/theme.hpp"
#include "../history/history_handler.hpp"
#include <iostream>
#include <curl/curl.h>
#include <iomanip>
#include <vector>

namespace Request {

    size_t CryptoHandler::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    std::string CryptoHandler::ExtractValue(const std::string& json, const std::string& key) {
        size_t pos = json.find("\"" + key + "\"");
        if (pos == std::string::npos) return "N/A";
        
        pos = json.find(":", pos);
        if (pos == std::string::npos) return "N/A";
        
        size_t start = pos + 1;
        while (start < json.size() && (json[start] == ' ' || json[start] == '"')) start++;
        
        size_t end = start;
        while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != '"') end++;
        
        return json.substr(start, end - start);
    }

    void CryptoHandler::FetchCoinData(CURL* curl, const std::string& coinId, const std::string& label) {
        std::string readBuffer;
        std::string url = "https://api.binance.com/api/v3/ticker/24hr?symbol=" + coinId + "USDT";

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

        if (curl_easy_perform(curl) == CURLE_OK) {
            std::string price = ExtractValue(readBuffer, "lastPrice");
            std::string change = ExtractValue(readBuffer, "priceChangePercent");
            
            if(price != "N/A" && price.length() > 8) price = price.substr(0, 8);
            if(change != "N/A" && change.length() > 5) change = change.substr(0, 5);

            // 🎨 Linux Renk Şovu: Düşüşse Kırmızı, Yükselişse Parlak Yeşil!
            std::string changeColor = Theme::RESET;
            if (change != "N/A") {
                if (change[0] == '-') {
                    changeColor = Theme::BRIGHT_RED;
                } else {
                    changeColor = Theme::BRIGHT_GREEN;
                }
            }

            std::cout << "  " << std::left << std::setw(12) << label 
                      << ": " << Theme::RESET << std::setw(10) << ("$" + price)
                      << " | 24s Degisim: " << changeColor << change << "%" << Theme::RESET << "\n";
        } else {
            std::cout << "  " << label << " verisi alinamadi.\n";
        }
    }

    void CryptoHandler::Execute() {
        Request::HistoryHandler::LogAction("CRYPTO", "SUCCESS");
        CURL* curl = curl_easy_init();
        if (!curl) return;

        std::cout << Theme::TEXT_GRAY << "[*] Kripto para fiyatlari Binance uzerinden cekiliyor...\n" << Theme::RESET;
        std::cout << Theme::TEXT_GRAY << "--------------------------------------------------\n" << Theme::RESET;
        std::cout << Theme::RESET << "💰 ANLIK PIYASA VERILERI (USDT)\n";
        std::cout << Theme::TEXT_GRAY << "==================================================\n" << Theme::RESET;

        FetchCoinData(curl, "BTC", "Bitcoin");
        FetchCoinData(curl, "ETH", "Ethereum");
        FetchCoinData(curl, "BNB", "BNB Coin");
        FetchCoinData(curl, "SOL", "Solana");

        std::cout << Theme::TEXT_GRAY << "--------------------------------------------------\n" << Theme::RESET;
        curl_easy_cleanup(curl);
    }
}