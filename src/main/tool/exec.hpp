#pragma once

#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#define popen  _popen
#define pclose _pclose
#endif

// Runs command inside shell and returns stdout of execution
std::string exec(const char *command) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
           nullptr) {
        result += buffer.data();
    }
    return result;
}

#ifdef _WIN32
#undef popen
#undef pclose
#endif
