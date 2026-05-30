#include "progress_bar.hpp"
#include "../common/theme.hpp"
#include <iostream>
#include <iomanip>

void ProgressBar::update(double percentage, size_t downloadedBytes, size_t totalBytes) {
    int barWidth = 40;
    int pos = static_cast<int>(barWidth * (percentage / 100.0));

    // \r ile satır başına dönüyoruz
    std::cout << "\r" << Theme::TEXT_GRAY << "[";
    
    // 🎨 İlerleme çubuğunu yeni BRIGHT_GREEN rengimizle çiziyoruz
    std::cout << Theme::BRIGHT_GREEN;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    
    // Çubuğun bittiği yerde rengi griye çekip istatistikleri basıyoruz
    std::cout << Theme::TEXT_GRAY << "] ";

    // Boyutları MB cinsinden hesaplama
    double downloadedMB = downloadedBytes / (1024.0 * 1024.0);
    double totalMB = totalBytes / (1024.0 * 1024.0);

    // Yüzdeyi parlak yeşil, MB detaylarını beyaz/gri basarak okunabilirliği artırdık
    std::cout << Theme::BRIGHT_GREEN << std::fixed << std::setprecision(1) << percentage << "% " << Theme::RESET
              << Theme::TEXT_GRAY << "(" << downloadedMB << " MB / " << totalMB << " MB)" << Theme::RESET << std::flush;
}