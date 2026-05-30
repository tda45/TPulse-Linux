#pragma once
#include <string>
#include <vector>

class NetworkTools {
public:
    static void scanPorts(const std::string& targetIp);
    static void resolveDns(const std::string& domain);
};