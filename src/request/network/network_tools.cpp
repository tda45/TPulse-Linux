#include "network_tools.hpp"
#include "../../ui/spinner.hpp"
#include "../../common/theme.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>
#include <unistd.h>      // close() fonksiyonu için Linux POSIX standardı
#include <sys/socket.h>  // Linux soket çekirdeği kütüphanesi
#include <arpa/inet.h>   // inet_pton ve inet_ntop fonksiyonları için
#include <netdb.h>       // getaddrinfo ve addrinfo yapıları için
#include <sys/select.h>  // Zaman aşımı (Timeout) yapıları için

void NetworkTools::scanPorts(const std::string& targetIp) {
    // 🐧 Linux'ta WSAStartup ön yükleme ihtiyacı yoktur. Doğrudan spinner ile başlıyoruz.
    Spinner spinner;
    spinner.start("Portlar taraniyor, lutfen bekleyin");

    std::vector<int> commonPorts = {21, 22, 23, 25, 53, 80, 110, 135, 139, 443, 445, 1433, 3306, 3389, 8080};
    std::vector<int> openPorts;
    std::mutex mtx;

    auto checkPort = [&](int port) {
        // Linux'ta standart soket tanımlayıcısı (file descriptor) düz bir tamsayıdır (int).
        int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock < 0) return;

        // ⏱️ Linux stili Bağlantı Zaman Aşımı (Timeout) Ayarı -> struct timeval
        struct timeval timeout{};
        timeout.tv_sec = 1;  // 1 Saniye Zaman Aşımı
        timeout.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

        struct sockaddr_in clientService{};
        clientService.sin_family = AF_INET;
        inet_pton(AF_INET, targetIp.c_str(), &clientService.sin_addr);
        clientService.sin_port = htons(port);

        // Linux connect() fonksiyonu başarılı bir bağlantıda 0 değerini döndürür.
        if (connect(sock, (struct sockaddr*)&clientService, sizeof(clientService)) == 0) {
            std::lock_guard<std::mutex> lock(mtx);
            openPorts.push_back(port);
        }
        
        close(sock); // Windows'taki closesocket yerine standart Linux close()
    };

    std::vector<std::thread> threads;
    for (int port : commonPorts) {
        threads.emplace_back(checkPort, port);
    }

    for (auto& th : threads) {
        if (th.joinable()) th.join();
    }

    spinner.stop();

    std::cout << "\n" << Theme::BRIGHT_GREEN << "[+] Tarama Tamamlandi! Halka Acik Portlar (" << targetIp << "):\n" << Theme::RESET;
    if (openPorts.empty()) {
        std::cout << "  Hic acik port bulunamadi.\n";
    } else {
        for (int port : openPorts) {
            std::cout << "  -> Port " << port << " [" << Theme::BRIGHT_GREEN << "ACIK" << Theme::RESET << "]\n";
        }
    }
}

void NetworkTools::resolveDns(const std::string& domain) {
    Spinner spinner;
    spinner.start("DNS kayitlari sorgulaniyor");

    struct addrinfo hints{}, *result = nullptr;
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(domain.c_str(), nullptr, &hints, &result) != 0) {
        spinner.stop();
        std::cout << "\n" << Theme::BRIGHT_RED << "[-] DNS cozulemedi: " << domain << "\n" << Theme::RESET;
        return;
    }

    spinner.stop();
    std::cout << "\n" << Theme::BRIGHT_GREEN << "[+] DNS Sonuclari (" << domain << "):\n" << Theme::RESET;
    
    for (struct addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        char ipstr[INET_ADDRSTRLEN];
        void* addr = &((struct sockaddr_in*)ptr->ai_addr)->sin_addr;
        inet_ntop(ptr->ai_family, addr, ipstr, sizeof(ipstr));
        std::cout << "  -> IP Adresi: " << ipstr << "\n";
    }

    freeaddrinfo(result);
}