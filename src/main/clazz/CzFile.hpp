#pragma once

#include "Clazz.hpp"
#include <filesystem>
#include <string>

class CzFile final : public Clazz {
    std::filesystem::path path;

  public:
    CzFile(std::filesystem::path path) : path(std::move(path)) {}

    CzFile(const std::string &path) : CzFile(std::filesystem::path(path)) {}

    CzFile(const char *path) : CzFile(std::filesystem::path(path)) {}

    p<Constants> constants() override { return nullptr; }
};
