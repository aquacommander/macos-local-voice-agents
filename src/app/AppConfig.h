#pragma once
#include <string>

struct AppConfig {
    std::string modelType;              // "dummy" or "onnx"
    std::string modelPath;
    std::string outputDir;
    float sampleRate = 44100.0f;
    float defaultDurationSeconds = 8.0f;

    static AppConfig loadFromFile(const std::string& path);
};
