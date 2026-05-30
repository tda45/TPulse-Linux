#include "subdomain_finder.hpp"
#include "../../ui/spinner.hpp"
#include "../../common/theme.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <sys/socket.h>  // Linux soket yapıları için
#include <netdb.h>       // getaddrinfo ve addrinfo POSIX standartları için

void Request::SubdomainFinder::Execute(const std::string& targetDomain) {
    // 🐧 Linux yerel soket altyapısı ön hazırlık istemez, doğrudan spinner'ı ateşliyoruz.
    Spinner spinner;
    spinner.start("Subdomain taramasi yapiliyor, lutfen bekleyin");

    // En yaygın kullanılan subdomain kelime listesi (Wordlist)
    std::vector<std::string> wordlist = {
        "www", "mail", "remote", "blog", "webmail", "server", "ns1", "ns2",
        "smtp", "vpn", "secure", "panel", "admin", "cloud", "api", "dev",
        "staging", "shop", "cpanel", "whm", "test", "ftp", "assets", "db"
    };

    std::vector<std::string> liveSubdomains;
    std::mutex mtx;

    // Her bir subdomaini asenkron kontrol eden lambda fonksiyonu
    auto checkSubdomain = [&](const std::string& sub) {
        std::string fullDomain = sub + "." + targetDomain;
        struct addrinfo hints{}, *result = nullptr;
        hints.ai_family = AF_INET;       // IPv4
        hints.ai_socktype = SOCK_STREAM; // TCP

        // Linux'ta getaddrinfo başarılı dönerse (0) subdomain aktiftir.
        if (getaddrinfo(fullDomain.c_str(), nullptr, &hints, &result) == 0) {
            std::lock_guard<std::mutex> lock(mtx);
            liveSubdomains.push_back(fullDomain);
            freeaddrinfo(result); // Bellek sızıntısını önlemek için Linux POSIX temizliği
        }
    };

    // Linux POSIX thread havuzunu oluşturup asenkron taramayı başlatıyoruz
    std::vector<std::thread> threads;
    for (const auto& sub : wordlist) {
        threads.emplace_back(checkSubdomain, sub);
    }

    // Tüm iş parçacıklarının bitmesini güvenle bekliyoruz
    for (auto& th : threads) {
        if (th.joinable()) th.join();
    }

    spinner.stop();

    std::cout << "\n" << Theme::BRIGHT_GREEN << "[+] Tarama Tamamlandi! Bulunan Canli Subdomainler (" << targetDomain << "):\n" << Theme::RESET;
    if (liveSubdomains.empty()) {
        std::cout << Theme::TEXT_GRAY << "   Hic aktif subdomain bulunamadi.\n" << Theme::RESET;
    } else {
        for (const auto& liveSub : liveSubdomains) {
            std::cout << "  " << Theme::BRIGHT_GREEN << "-> " << liveSub << Theme::RESET << "\n";
        }
    }
}