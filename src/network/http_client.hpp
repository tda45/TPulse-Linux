#pragma once
#include <string>
#include <map>

// HttpResponse yapısı aynı kalıyor, böylece üst katmandaki (weather, crypto vb.) kodlar kırılmıyor.
struct HttpResponse {
    int statusCode = 0;
    std::string body;
    std::string headers;
    double elapsedMs = 0.0;
};

class HttpClient {
public:
    HttpClient();
    ~HttpClient();

    HttpResponse SendRequest(const std::string& url, const std::string& method, 
                             const std::string& data = "", 
                             const std::map<std::string, std::string>& extraHeaders = {});

private:
    // Linux'ta cURL oturumunu yöneten statik geri bildirim (callback) fonksiyonu
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userSpace);
};