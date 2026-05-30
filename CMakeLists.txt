cmake_minimum_required(VERSION 3.15)
project(TPulse LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 🐧 Windows Çapraz Bağımlılık Sızıntılarını Önleme Ayarları
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# 🔍 Linux Sistem Kütüphanelerini Doğru Yöntemle Buluyoruz
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl) # İnternet katmanı için Linux yerel curl paketini bağlar

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

# Tüm kaynak dosyalarını derinlemesine tarıyoruz
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

add_executable(tpulse ${SOURCES})

# 📂 Başlık (Include) Dizinlerini Tanımlıyoruz
target_include_directories(tpulse PRIVATE 
    src
    ${CURL_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

# 🔗 Linux Kütüphane Bağlantıları (Linker)
target_link_libraries(tpulse PRIVATE 
    ${CURL_LIBRARIES}     # PkgConfig'den gelen saf Linux libcurl kütüphaneleri
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads      # Linux thread yönetimi (pthread)
)

# 🐧 Linux için derleme optimizasyon bayrakları
target_compile_options(tpulse PRIVATE -Wall -Wextra -O2)