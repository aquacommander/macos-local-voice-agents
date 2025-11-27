// src/core/ModelBackendDummy.cpp

#include "core/ModelBackend.h"
#include <cmath>
#include <iostream>

class ModelBackendDummy : public ModelBackend {
public:
    bool initialize(const AppConfig& cfg) override {
        sampleRate_ = cfg.sampleRate;
        std::cout << "[DummyBackend] Initialized, sr=" << sampleRate_ << "\n";
        return true;
    }

    bool generate(const GenerationConfig& genCfg,
                  std::vector<float>& outLeft,
                  std::vector<float>& outRight) override {
        const float sr = genCfg.sampleRate;
        const float dur = genCfg.durationSeconds;
        const int numSamples = static_cast<int>(sr * dur);

        outLeft.resize(numSamples);
        outRight.resize(numSamples);

        float baseFreq = 220.0f;
        for (int i = 0; i < numSamples; ++i) {
            float t = static_cast<float>(i) / sr;
            float freq = baseFreq
                       + 110.0f * std::sin(2.0f * 3.14159265f * 0.25f * t);
            float v = std::sin(2.0f * 3.14159265f * freq * t) * 0.5f;
            outLeft[i] = v;
            outRight[i] = v;
        }

        std::cout << "[DummyBackend] Generated " << numSamples
                  << " samples for prompt: '" << genCfg.prompt << "'\n";
        return true;
    }

    const char* name() const override { return "dummy"; }

private:
    float sampleRate_ = 44100.0f;
};

// Factory used by StableAudioEngine.cpp
ModelBackend* createDummyBackend() {
    return new ModelBackendDummy();
}
