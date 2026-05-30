#include "post_handler.hpp"
#include "../network/http_client.hpp"
#include "../common/theme.hpp"
#include <iostream>

void Request::PostHandler::Execute(const std::string& url, const std::string& payload) {
    HttpClient client;
    
    std::cout << Theme::TEXT_GRAY << "[*] Veri gonderiliyor: " << Theme::CRIMSON << url << Theme::RESET << "\n";
    
    // Libcurl tabanlı yeni HttpClient arka planda POST isteğini yönetir
    HttpResponse res = client.SendRequest(url, "POST", payload);

    std::cout << Theme::ANTHRACITE << "--------------------------------------------------" << Theme::RESET << "\n";
    
    // 🎨 HTTP Durumuna Göre Renk Ataması (2xx Başarılı ise Parlak Yeşil, Hata ise Parlak Kırmızı)
    std::string statusColor = (res.statusCode >= 200 && res.statusCode < 300) ? Theme::BRIGHT_GREEN : Theme::BRIGHT_RED;
    
    std::cout << Theme::TEXT_GRAY << "HTTP STATUS: " << statusColor << res.statusCode << Theme::RESET << "\n";
    std::cout << Theme::TEXT_GRAY << "SURE       : " << Theme::CRIMSON << res.elapsedMs << " ms" << Theme::RESET << "\n";
    
    std::cout << Theme::ANTHRACITE << "--------- [ RESPONSE BODY ] --------- " << Theme::RESET << "\n";
    if (!res.body.empty()) {
        std::cout << Theme::TEXT_GRAY << res.body << Theme::RESET << "\n";
    } else {
        std::cout << Theme::BRIGHT_RED << "< Bos veya Hatali Yanit >" << Theme::RESET << "\n";
    }
    std::cout << Theme::ANTHRACITE << "--------------------------------------------------" << Theme::RESET << "\n";
}