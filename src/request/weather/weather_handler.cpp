#include "weather_handler.hpp"
#include "../../network/http_client.hpp"
#include "../history/history_handler.hpp"
#include "../../common/theme.hpp"
#include <iostream>
#include <map>

void Request::WeatherHandler::Execute(const std::string& city) {
    HttpClient client;
    
    Request::HistoryHandler::LogAction("WEATHER", "SUCCESS");

    std::string url = "https://wttr.in/" + city;
    
    // 🎨 Bilgilendirme kısmını BRIGHT_GREEN ve TEXT_GRAY uyumuyla parlattık
    std::cout << Theme::TEXT_GRAY << "[*] Hava durumu verisi cekiliyor... " 
              << Theme::BRIGHT_GREEN << (city.empty() ? "Mevcut Konum" : city) << Theme::RESET << "\n";
    
    // wttr.in servisinin terminale saf ANSI çıktısı vermesi için User-Agent kandırmacası
    std::map<std::string, std::string> headers;
    headers["User-Agent"] = "curl/7.88.1";

    // Libcurl tabanlı yeni HttpClient ile isteği gönderiyoruz
    HttpResponse res = client.SendRequest(url, "GET", "", headers);

    std::cout << Theme::ANTHRACITE << "--------------------------------------------------" << Theme::RESET << "\n";
    
    if (res.statusCode == 200) {
        // Linux terminali wttr.in'den gelen renkli grafiksel tabloyu doğrudan pürüzsüzce çizer
        std::cout << res.body << "\n";
    } else {
        std::cout << Theme::BRIGHT_RED << "[!] Hava durumu alinamadi. Durum kodu: " << res.statusCode << Theme::RESET << "\n";
    }
    std::cout << Theme::ANTHRACITE << "--------------------------------------------------" << Theme::RESET << "\n";
}