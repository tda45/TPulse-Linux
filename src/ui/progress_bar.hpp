#pragma once
#include <cstddef> // size_t tanımı için Linux/GCC standardı

class ProgressBar {
public:
    static void update(double percentage, size_t downloadedBytes, size_t totalBytes);
};