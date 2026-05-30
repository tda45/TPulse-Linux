#include "common/theme.hpp"
#include "request/post_handler.hpp"
#include "request/weather/weather_handler.hpp"
#include "request/download/download_handler.hpp"
#include "request/history/history_handler.hpp"
#include "request/ip/ip_handler.hpp"        
#include "request/crypto/crypto_handler.hpp" 
#include "request/network/network_tools.hpp"
#include "request/network/subdomain_finder.hpp"
#include "request/crypto/file_cipher.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // 🐧 Linux terminalleri UTF-8 ve ANSI renklerini doğrudan destekler.
    // Windows'a özel olan SetConsoleOutputCP ve windows.h bağımlılıkları tamamen kaldırıldı.

    Theme::PrintHeader();

    if (argc < 2) {
        std::cout << Theme::BRIGHT_RED << "Kullanım Hatalı!\n\n" << Theme::RESET;
        std::cout << Theme::CRIMSON << "1) POST İsteği:\n" << Theme::TEXT_GRAY << "  tpulse [URL] --post [DATA]\n\n";
        std::cout << Theme::CRIMSON << "2) Hava Durumu:\n" << Theme::TEXT_GRAY << "  tpulse --weather [Sehir_Adi]\n\n";
        std::cout << Theme::CRIMSON << "3) Dosya İndirme:\n" << Theme::TEXT_GRAY << "  tpulse [URL] --download [Kaydedilecek_Dosya_Adi]\n\n";
        std::cout << Theme::CRIMSON << "4) Ağ ve IP Bilgisi:\n" << Theme::TEXT_GRAY << "  tpulse --ip\n\n";
        std::cout << Theme::CRIMSON << "5) Kripto Takip:\n" << Theme::TEXT_GRAY << "  tpulse --crypto\n\n";
        std::cout << Theme::CRIMSON << "6) İşlem Geçmişi:\n" << Theme::TEXT_GRAY << "  tpulse --history\n\n";
        std::cout << Theme::CRIMSON << "7) Port Tarayıcı:\n" << Theme::TEXT_GRAY << "  tpulse --scan [IP_Adresi]\n\n";
        std::cout << Theme::CRIMSON << "8) DNS Çözümleyici:\n" << Theme::TEXT_GRAY << "  tpulse --dns [Domain_Adi]\n\n";
        std::cout << Theme::CRIMSON << "9) Subdomain Keşif Motoru:\n" << Theme::TEXT_GRAY << "  tpulse --subdomain [Domain_Adi]\n\n";
        std::cout << Theme::CRIMSON << "10) Dosya Şifreleme:\n" << Theme::TEXT_GRAY << "  tpulse --encrypt [Dosya_Yolu] [Anahtar]\n\n";
        std::cout << Theme::CRIMSON << "11) Dosya Şifre Çözme:\n" << Theme::TEXT_GRAY << "  tpulse --decrypt [Dosya_Yolu.enc] [Anahtar]\n" << Theme::RESET;
        
        Request::HistoryHandler::LogAction("USAGE_ERR", "FAILED");
        return 1;
    }

    std::string firstArg = argv[1];

    // TEKİL MODÜL KONTROLLERİ
    if (firstArg == "--weather") {
        std::string city = (argc > 2) ? argv[2] : "";
        Request::WeatherHandler::Execute(city);
        return 0;
    }
    if (firstArg == "--ip") {
        Request::IpHandler::Execute();
        return 0;
    }
    if (firstArg == "--crypto") {
        Request::CryptoHandler::Execute();
        return 0;
    }
    if (firstArg == "--history") {
        Request::HistoryHandler::Execute();
        return 0;
    }

    // 🔍 PORT TARAYICI MODÜLÜ TETİKLEMESİ
    if (firstArg == "--scan") {
        if (argc < 3) {
            std::cout << Theme::BRIGHT_RED << "[!] Lütfen taranacak IP adresini belirtin.\n" << Theme::RESET;
            Request::HistoryHandler::LogAction("SCAN", "FAILED");
            return 1;
        }
        std::string targetIp = argv[2];
        NetworkTools::scanPorts(targetIp);
        Request::HistoryHandler::LogAction("PORT_SCAN:" + targetIp, "SUCCESS");
        return 0;
    }

    // 🌐 DNS ÇÖZÜMLEYİCİ MODÜLÜ TETİKLEMESİ
    if (firstArg == "--dns") {
        if (argc < 3) {
            std::cout << Theme::BRIGHT_RED << "[!] Lütfen domain adını belirtin.\n" << Theme::RESET;
            Request::HistoryHandler::LogAction("DNS", "FAILED");
            return 1;
        }
        std::string domain = argv[2];
        NetworkTools::resolveDns(domain);
        Request::HistoryHandler::LogAction("DNS_RESOLVE:" + domain, "SUCCESS");
        return 0;
    }

    // 🦅 SUBDOMAIN KEŞİF MOTORU TETİKLEMESİ
    if (firstArg == "--subdomain") {
        if (argc < 3) {
            std::cout << Theme::BRIGHT_RED << "[!] Lütfen hedef domain adını belirtin.\n" << Theme::RESET;
            Request::HistoryHandler::LogAction("SUBDOMAIN", "FAILED");
            return 1;
        }
        std::string targetDomain = argv[2];
        Request::SubdomainFinder::Execute(targetDomain);
        Request::HistoryHandler::LogAction("SUBDOMAIN_FINDER:" + targetDomain, "SUCCESS");
        return 0;
    }

    // 🔒 DOSYA ŞİFRELEME MODÜLÜ TETİKLEMESİ
    if (firstArg == "--encrypt") {
        if (argc < 4) {
            std::cout << Theme::BRIGHT_RED << "[!] Kullanım: tpulse --encrypt [Dosya_Yolu] [Anahtar]\n" << Theme::RESET;
            Request::HistoryHandler::LogAction("ENCRYPT", "FAILED");
            return 1;
        }
        std::string filePath = argv[2];
        std::string key = argv[3];
        Request::FileCipher::Encrypt(filePath, key);
        Request::HistoryHandler::LogAction("FILE_ENCRYPT:" + filePath, "SUCCESS");
        return 0;
    }

    // 🔓 DOSYA ŞİFRE ÇÖZME MODÜLÜ TETİKLEMESİ
    if (firstArg == "--decrypt") {
        if (argc < 4) {
            std::cout << Theme::BRIGHT_RED << "[!] Kullanım: tpulse --decrypt [Dosya_Yolu.enc] [Anahtar]\n" << Theme::RESET;
            Request::HistoryHandler::LogAction("DECRYPT", "FAILED");
            return 1;
        }
        std::string filePath = argv[2];
        std::string key = argv[3];
        Request::FileCipher::Decrypt(filePath, key);
        Request::HistoryHandler::LogAction("FILE_DECRYPT:" + filePath, "SUCCESS");
        return 0;
    }

    std::string url = firstArg;
    std::string mode = (argc > 2) ? argv[2] : "";

    if (mode == "--post") {
        std::string data = (argc > 3) ? argv[3] : "{}";
        Request::PostHandler::Execute(url, data);
    } 
    else if (mode == "--download") {
        if (argc < 4) {
            std::cout << Theme::BRIGHT_RED << "[!] Lütfen kaydedilecek dosya adını belirtin.\n" << Theme::RESET;
            Request::HistoryHandler::LogAction("DOWNLOAD", "FAILED");
            return 1;
        }
        std::string outputName = argv[3];
        Request::DownloadHandler::Execute(url, outputName);
    } 
    else {
        std::cout << Theme::BRIGHT_RED << "[!] Geçersiz komut kombinasyonu!" << Theme::RESET << "\n";
        Request::HistoryHandler::LogAction("BAD_ARG:" + firstArg, "FAILED");
    }

    return 0;
}