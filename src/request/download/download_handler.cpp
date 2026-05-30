#include "download_handler.hpp"
#include "../../common/theme.hpp"
#include "../../network/http_client.hpp"
#include "../../ui/progress_bar.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdio> // std::remove için

void Request::DownloadHandler::Execute(const std::string& url, const std::string& outputFilename) {
    HttpClient client;
    
    std::cout << Theme::TEXT_GRAY << "[*] Dosya indiriliyor: " << Theme::CRIMSON << url << Theme::RESET << "\n";
    std::cout << Theme::TEXT_GRAY << "[*] Hedef dosya      : " << Theme::CRIMSON << outputFilename << Theme::RESET << "\n";

    std::ofstream outFile(outputFilename, std::ios::binary);
    if (!outFile.is_open()) {
        std::cout << Theme::BRIGHT_RED << "[!] Cikis dosyasi olusturulamadi!" << Theme::RESET << "\n";
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Linux uyumlu yeni libcurl tabanlı HttpClient isteği tetikliyor
    HttpResponse res = client.SendRequest(url, "GET", "");

    if (res.statusCode == 200 && !res.body.empty()) {
        size_t totalBytes = res.body.size();
        size_t downloadedBytes = 0;
        size_t chunkSize = 4096; 

        const char* dataPtr = res.body.data();

        while (downloadedBytes < totalBytes) {
            size_t remaining = totalBytes - downloadedBytes;
            size_t currentChunk = (remaining > chunkSize) ? chunkSize : remaining;

            outFile.write(dataPtr + downloadedBytes, currentChunk);
            downloadedBytes += currentChunk;

            double percentage = (static_cast<double>(downloadedBytes) / totalBytes) * 100.0;
            
            // Kırmızı ilerleme çubuğu Linux terminalinde akıyor
            ProgressBar::update(percentage, downloadedBytes, totalBytes);
            
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        
        outFile.close();

        auto end = std::chrono::high_resolution_clock::now();
        double elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();

        std::cout << "\n" << Theme::ANTHRACITE << "--------------------------------------------------" << Theme::RESET << "\n";
        // 🎨 Başarı rengi parlak yeşil yapıldı
        std::cout << Theme::BRIGHT_GREEN << "[+] INDIRME TAMAMLANDI!" << Theme::RESET << "\n";
        std::cout << Theme::TEXT_GRAY << "Boyut: " << Theme::CRIMSON << totalBytes << " Byte" << Theme::RESET << "\n";
        std::cout << Theme::TEXT_GRAY << "Sure : " << Theme::CRIMSON << elapsedMs << " ms" << Theme::RESET << "\n";
        std::cout << Theme::ANTHRACITE << "--------------------------------------------------" << Theme::RESET << "\n";
    } else {
        std::cout << Theme::BRIGHT_RED << "[!] Indirme basarisiz. HTTP: " << res.statusCode << Theme::RESET << "\n";
        outFile.close();
        std::remove(outputFilename.c_str()); 
    }
}