#pragma once

#include <memory>
#include <string>

#include "app/AppConfig.h"
#include "core/GenerationConfig.h"
#include "core/ModelBackend.h"   

class StableAudioEngine {
public:
    StableAudioEngine() = default;
    ~StableAudioEngine();        

    bool initialize(const AppConfig& cfg);

    bool generateFromText(const GenerationConfig& genCfg,
                          const std::string& outputPath);

private:
    AppConfig config_;
    std::unique_ptr<ModelBackend> backend_;
};
