#include "http_client.hpp"
#include "../common/theme.hpp"
#include <curl/curl.h>
#include <chrono>
#include <iostream>

HttpClient::HttpClient() {
    // Linux'ta global curl altyapısını güvenli hale getiriyoruz
    static bool globalInitialized = false;
    if (!globalInitialized) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        globalInitialized = true;
    }
}

HttpClient::~HttpClient() {
    // cURL global kaynakları program kapanırken otomatik temizlenir.
}

// Sunucudan gelen ham veriyi HttpResponse::body içine yazan sihirli cURL fonksiyonu
size_t HttpClient::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userSpace) {
    size_t totalSize = size * nmemb;
    if (userSpace) {
        userSpace->append(static_cast<char*>(contents), totalSize);
    }
    return totalSize;
}

HttpResponse HttpClient::SendRequest(const std::string& url, const std::string& method, 
                                     const std::string& data, 
                                     const std::map<std::string, std::string>& extraHeaders) {
    HttpResponse response;
    CURL* curl = curl_easy_init();
    
    if (!curl) {
        std::cerr << Theme::BRIGHT_RED << "[!] cURL baslatilamadi!" << Theme::RESET << std::endl;
        return response;
    }

    struct curl_slist* headersList = nullptr;

    // ⏳ Zaman ölçümü başlıyor
    auto start = std::chrono::high_resolution_clock::now();

    // 🛠️ Temel cURL Ayarları
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.body);
    
    // Yönlendirmeleri otomatik takip et (Örn: http -> https geçişleri)
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); 
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 10 saniye zaman aşımı

    // 🚀 HTTP Metot Yönetimi
    if (method == "POST") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        if (!data.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        }
        headersList = curl_slist_append(headersList, "Content-Type: application/json");
    } else if (method == "GET") {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    } else {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
    }

    // ➕ Dinamik Ek Başlıkları (Headers) Ekleme
    for (const auto& [key, value] : extraHeaders) {
        std::string headerLine = key + ": " + value;
        headersList = curl_slist_append(headersList, headerLine.c_str());
    }

    if (headersList) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headersList);
    }

    // ⚡ İsteği Ateşle!
    CURLcode res = curl_easy_perform(curl);

    // 🏁 Zaman ölçümü bitiyor
    auto end = std::chrono::high_resolution_clock::now();
    response.elapsedMs = std::chrono::duration<double, std::milli>(end - start).count();

    // 📊 Durum Kontrolü ve Sonuç Çıkarma
    if (res == CURLE_OK) {
        long statusCode = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
        response.statusCode = static_cast<int>(statusCode);
    } else {
        std::cerr << Theme::BRIGHT_RED << "[!] HTTP Istegi basarisiz: " 
                  << curl_easy_strerror(res) << Theme::RESET << std::endl;
        response.statusCode = 0;
    }

    // 🧹 Temizlik ve Bellek Boşaltma
    if (headersList) {
        curl_slist_free_all(headersList);
    }
    curl_easy_cleanup(curl);

    return response;
}