#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
#include "Logger.h"


class FileUtils{
public:
    static std::string readFileContent(const std::string& path)
    {
        std::filesystem::path execPath = std::filesystem::current_path();
        std::filesystem::path fullPath = execPath / path;

        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + fullPath.string());
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        Logger::log(Logger::Level::Info, "Successfully read file: ", fullPath.string());
        return buffer.str();
    }
};
