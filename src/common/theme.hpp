#pragma once
#include <string>
#include <iostream>
#include <cstdio>

namespace Theme {
    // Gece Kırmızısı ve Karanlık Tonlar
    const std::string RESET       = "\033[0m";
    const std::string DARK_RED    = "\033[38;2;139;0;0m";     // #8B0000
    const std::string CRIMSON     = "\033[38;2;163;0;0m";     // #A30000
    const std::string BRIGHT_RED  = "\033[38;2;255;51;51m";   // Hata/Uyarı için
    const std::string ANTHRACITE  = "\033[38;2;40;40;40m";    // Panel çizgileri
    const std::string TEXT_GRAY   = "\033[38;2;180;180;180m"; // Mat metin
    const std::string BRIGHT_GREEN = "\033[92m";

    inline void PrintHeader() {
        // Ayrık duran orta çizgiler kaldırıldı ve jilet gibi TPULSE olarak birleştirildi
        std::string banner = 
            R"(  _______ _____  _    _ _       _____ ______ )" "\n"
            R"( |__   __|  __ \| |  | | |     / ____|  ____|)" "\n"
            R"(    | |  | |__) | |  | | |    | (___ | |__   )" "\n"
            R"(    | |  |  ___/| |  | | |     \___ \\|  __|  )" "\n"
            R"(    | |  | |    | |__| | |____ ____) | |____ )" "\n"
            R"(    |_|  |_|     \____/|______|_____/|______|)" "\n"
            R"(V1.0 - Linux Edition                         )" "\n";
        std::printf("%s%s%s", DARK_RED.c_str(), banner.c_str(), RESET.c_str());
        std::printf("%s==================================================%s\n", ANTHRACITE.c_str(), RESET.c_str());
    }
}