#pragma once
#include <string>

namespace Request {
    class WeatherHandler {
    public:
        // Şehir girilmezse varsayılan olarak konumu otomatik bulur
        static void Execute(const std::string& city = "");
    };
}