#pragma once
#include <atomic>
#include <thread>
#include <string>

class Spinner {
private:
    std::atomic<bool> running{false};
    std::thread spinnerThread;
public:
    void start(const std::string& message = "Lutfen bekleyin");
    void stop();
    ~Spinner();
};