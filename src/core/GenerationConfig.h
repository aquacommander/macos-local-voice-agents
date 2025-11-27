#pragma once
#include <string>

struct GenerationConfig {
    std::string prompt;
    float durationSeconds = 8.0f;
    float sampleRate = 44100.0f;
    unsigned int seed = 0;      // for deterministic generation later

    // future: bpm, key, scale
    float bpm = 120.0f;
    std::string key = "C";
    std::string scale = "major";
};
