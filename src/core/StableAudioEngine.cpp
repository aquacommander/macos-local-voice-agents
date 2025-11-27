#include "core/StableAudioEngine.h"
#include "audio/AudioFileWriter.h"
#include <filesystem>
#include <iostream>
#include <vector>

// factory declarations implemented in other .cpp files
ModelBackend* createDummyBackend();
ModelBackend* createOnnxBackend();

StableAudioEngine::~StableAudioEngine() = default;

bool StableAudioEngine::initialize(const AppConfig& cfg) {
    config_ = cfg;

    // Choose backend
    if (config_.modelType == "onnx") {
        std::cout << "StableAudioEngine: using ONNX backend\n";
        backend_.reset(createOnnxBackend());
    } else {
        std::cout << "StableAudioEngine: using dummy backend\n";
        backend_.reset(createDummyBackend());
    }

    if (!backend_) {
        std::cerr << "StableAudioEngine: backend creation failed\n";
        return false;
    }

    if (!backend_->initialize(config_)) {
        std::cerr << "StableAudioEngine: backend initialization failed\n";
        return false;
    }

    // ensure output directory exists
    try {
        if (!config_.outputDir.empty()) {
            std::filesystem::create_directories(config_.outputDir);
        }
    } catch (const std::exception& e) {
        std::cerr << "StableAudioEngine: failed to create output dir: "
                  << e.what() << "\n";
        return false;
    }

    return true;
}



#include <filesystem>   // make sure this include is at the top
// ...other includes...

bool StableAudioEngine::generateFromText(const GenerationConfig& genCfg,
                                         const std::string& outputPath) {
    if (!backend_) {
        std::cerr << "StableAudioEngine: backend not initialized\n";
        return false;
    }

    std::vector<float> left, right;
    if (!backend_->generate(genCfg, left, right)) {
        std::cerr << "StableAudioEngine: backend generation failed\n";
        return false;
    }

    // Build filesystem path first…
    std::filesystem::path outPath =
        std::filesystem::path(config_.outputDir) / outputPath;

    // …then convert it explicitly to std::string
    std::string fullPath = outPath.string();

    if (!AudioFileWriter::writeWavStereo(fullPath, left, right,
                                         genCfg.sampleRate)) {
        std::cerr << "StableAudioEngine: failed to write file " << fullPath
                  << "\n";
        return false;
    }

    std::cout << "StableAudioEngine: wrote " << fullPath << "\n";
    return true;
}

