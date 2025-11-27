#include "app/AppConfig.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace {

// super tiny helper: find "key": value in JSON-like text
std::string findStringField(const std::string& text, const std::string& key,
                            const std::string& defaultValue = "") {
    auto pos = text.find("\"" + key + "\"");
    if (pos == std::string::npos) return defaultValue;
    pos = text.find(':', pos);
    if (pos == std::string::npos) return defaultValue;
    pos = text.find('"', pos);
    if (pos == std::string::npos) return defaultValue;
    auto end = text.find('"', pos + 1);
    if (end == std::string::npos) return defaultValue;
    return text.substr(pos + 1, end - pos - 1);
}

float findFloatField(const std::string& text, const std::string& key,
                     float defaultValue) {
    auto pos = text.find("\"" + key + "\"");
    if (pos == std::string::npos) return defaultValue;
    pos = text.find(':', pos);
    if (pos == std::string::npos) return defaultValue;
    auto end = text.find_first_of(",}\n", pos + 1);
    if (end == std::string::npos) end = text.size();
    std::string numStr = text.substr(pos + 1, end - pos - 1);
    // trim
    numStr.erase(0, numStr.find_first_not_of(" \t\r\n"));
    numStr.erase(numStr.find_last_not_of(" \t\r\n") + 1);
    try {
        return std::stof(numStr);
    } catch (...) {
        return defaultValue;
    }
}

} // namespace

AppConfig AppConfig::loadFromFile(const std::string& path) {
    AppConfig cfg;

    std::ifstream in(path);
    if (!in) {
        std::cerr << "AppConfig: could not open " << path
                  << ", using defaults.\n";
        cfg.modelType = "dummy";
        cfg.modelPath = "";
        cfg.outputDir = "output";
        cfg.sampleRate = 44100.0f;
        cfg.defaultDurationSeconds = 8.0f;
        return cfg;
    }

    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string text = buffer.str();

    cfg.modelType = findStringField(text, "model_type", "dummy");
    cfg.modelPath = findStringField(text, "model_path", "");
    cfg.outputDir = findStringField(text, "output_dir", "output");
    cfg.sampleRate = findFloatField(text, "sample_rate", 44100.0f);
    cfg.defaultDurationSeconds =
        findFloatField(text, "default_duration_seconds", 8.0f);

    std::cout << "AppConfig loaded from " << path << "\n";
    return cfg;
}
