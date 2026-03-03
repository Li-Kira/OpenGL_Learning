
#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <simd/simd.h>  // For SIMD types
#include <type_traits>  // For type traits

class Logger {
public:
    enum class Level {
        Info,
        Warning,
        Error
    };

    // 基础字符串版本
    static void log(Level level, const std::string& message) {
        logMessage(level, message);
    }

    // 支持字符串 + 数值的拼接
    template<typename T>
    static void log(Level level, const T& message) {
        std::stringstream ss;
        ss << message;
        logMessage(level, ss.str());
    }

    // 支持多参数输出
    template<typename First, typename... Rest>
    static void log(Level level, const First& first, const Rest&... rest) {
        std::stringstream ss;
        ss << first;
        (ss << ... << rest);
        logMessage(level, ss.str());
    }

    // 辅助函数：将任意类型转换为字符串
    template<typename T>
    static std::string toString(const T& value) {
        if constexpr (std::is_same_v<T, simd::float3>) {
            return formatFloat3(value);
        }
        else if constexpr (std::is_same_v<T, simd::float4>) {
            return formatFloat4(value);
        }
        else if constexpr (std::is_same_v<T, simd::float4x4>) {
            return formatFloat4x4(value);
        }
        else {
            std::ostringstream ss;
            ss << value;
            return ss.str();
        }
    }

    static void init(const std::string& filePath) {
        _logFile.open(filePath, std::ios::out | std::ios::app);

        if (_logFile.is_open()) {
            _logFile << "\n=== Log session started ===\n";
        } else {
            std::cerr << "\033[1;31m[ERROR]\033[0m Could not initialize log file: " << filePath << std::endl;
        }
    }

    static void shutdown() {
        if (_logFile.is_open()) {
            _logFile.close();
        }
    }

private:
    static std::ofstream _logFile;

    static void logMessage(Level level, const std::string& message) {
        const std::string colorReset = "\033[0m";
        std::string color, levelStr;

        switch (level) {
            case Level::Info: 
                color = "\033[1;32m";
                levelStr = "INFO";
                break;
            case Level::Warning:
                color = "\033[1;33m";
                levelStr = "WARNING";
                break;
            case Level::Error:
                color = "\033[1;31m";
                levelStr = "ERROR";
                break;
        }

        // Console output (colored)
        std::ostream& stream = (level == Level::Error) ? std::cerr : std::cout;
        stream << color << "[" << levelStr << "] "  << colorReset << message << std::endl;

        // File output (no color codes)
        if (_logFile.is_open()) {
            _logFile << "[" << levelStr << "] " << message << std::endl;
        }
    }

    static std::string formatFloat3(const simd::float3& vec) {
        return "(" + std::to_string(vec.x) + ", " +
                     std::to_string(vec.y) + ", " +
                     std::to_string(vec.z) + ")";
    }

    static std::string formatFloat4(const simd::float4& vec) {
        return "(" + std::to_string(vec.x) + ", " +
                     std::to_string(vec.y) + ", " +
                     std::to_string(vec.z) + ", " +
                     std::to_string(vec.w) + ")";
    }

    static std::string formatFloat4x4(const simd::float4x4& mat) {
        std::string result = "[\n";
        for (int i = 0; i < 4; ++i) {
            result += " [";
            for (int j = 0; j < 4; ++j) {
                result += std::to_string(mat.columns[i][j]);
                if (j < 3) result += ", ";
            }
            result += "]\n";
        }
        return result + "]";
    }
};

// Initialize static member
std::ofstream Logger::_logFile;