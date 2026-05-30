#include "spinner.hpp"
#include "../common/theme.hpp"
#include <iostream>
#include <chrono>
#include <thread>

void Spinner::start(const std::string& message) {
    // Eğer halihazırda çalışan bir thread varsa önce onu güvenle durduralım
    if (running) stop();

    running = true;
    spinnerThread = std::thread([this, message]() {
        const char spinnerChars[] = {'|', '/', '-', '\\'};
        int i = 0;
        
        while (running) {
            // 🎨 Sol taraftaki yükleme ikonunu yeşil, mesajı ise gri yapıyoruz
            std::cout << "\r[" << Theme::BRIGHT_GREEN << spinnerChars[i % 4] << Theme::RESET 
                      << "] " << Theme::TEXT_GRAY << message << "..." << Theme::RESET << std::flush;
            
            i++;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Spinner stop() çağrıldığında, terminal satırını tamamen temizleyip imleci başa alır
        // Linux terminallerinde satırı temizlemek için tam karakter uzunluğu kadar boşluk basıyoruz
        std::cout << "\r" << std::string(message.length() + 15, ' ') << "\r" << std::flush;
    });
}

void Spinner::stop() {
    if (running) {
        running = false; // Thread döngüsünü kırar
        if (spinnerThread.joinable()) {
            spinnerThread.join(); // İş parçacığının güvenle sonlanmasını bekler
        }
    }
}

Spinner::~Spinner() {
    stop();
}