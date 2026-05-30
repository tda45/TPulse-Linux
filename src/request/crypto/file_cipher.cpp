#include "file_cipher.hpp"
#include "../../ui/progress_bar.hpp"
#include "../../common/theme.hpp"
#include <iostream>
#include <fstream>
#include <vector>

bool processFile(const std::string& inputPath, const std::string& outputPath, const std::string& key) {
    std::ifstream inFile(inputPath, std::ios::binary | std::ios::ate);
    if (!inFile.is_open()) {
        std::cout << Theme::BRIGHT_RED << "[!] Kaynak dosya acilamadi! Lutfen dosya yolunu ve konumunu kontrol edin.\n" << Theme::RESET;
        return false;
    }

    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::ofstream outFile(outputPath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cout << Theme::BRIGHT_RED << "[!] Hedef dosya olusturulamadi!\n" << Theme::RESET;
        inFile.close();
        return false;
    }

    if (fileSize == 0) {
        inFile.close();
        outFile.close();
        return true;
    }

    size_t bufferSize = 4096;
    std::vector<char> buffer(bufferSize);
    size_t processedBytes = 0;
    size_t keyIndex = 0;

    while (inFile.read(buffer.data(), bufferSize) || inFile.gcount() > 0) {
        size_t bytesRead = inFile.gcount();
        
        for (size_t i = 0; i < bytesRead; ++i) {
            buffer[i] ^= key[keyIndex % key.length()];
            keyIndex++;
        }

        outFile.write(buffer.data(), bytesRead);
        processedBytes += bytesRead;

        double percentage = (static_cast<double>(processedBytes) / fileSize) * 100.0;
        ProgressBar::update(percentage, processedBytes, fileSize);
    }

    inFile.close();
    outFile.close();
    std::cout << "\n";
    return true;
}

void Request::FileCipher::Encrypt(const std::string& filePath, const std::string& key) {
    std::cout << Theme::TEXT_GRAY << "[*] Dosya sifreleniyor: " << Theme::CRIMSON << filePath << Theme::RESET << "\n";
    if (processFile(filePath, filePath + ".enc", key)) {
        // 🎨 Başarı durumunu parlak yeşile çektik
        std::cout << Theme::BRIGHT_GREEN << "[+] Sifreleme tamamlandi! -> " << filePath << ".enc\n" << Theme::RESET;
    }
}

void Request::FileCipher::Decrypt(const std::string& filePath, const std::string& key) {
    std::cout << Theme::TEXT_GRAY << "[*] Sifreli dosya cozuluyor: " << Theme::CRIMSON << filePath << Theme::RESET << "\n";
    
    std::string outPath = filePath;
    if (outPath.length() > 4 && outPath.substr(outPath.length() - 4) == ".enc") {
        outPath = outPath.substr(0, outPath.length() - 4);
    } else {
        outPath = outPath + ".dec";
    }

    if (processFile(filePath, outPath, key)) {
        // 🎨 Başarı durumunu parlak yeşile çektik
        std::cout << Theme::BRIGHT_GREEN << "[+] Dosya cozme tamamlandi! -> " << outPath << "\n" << Theme::RESET;
    }
}