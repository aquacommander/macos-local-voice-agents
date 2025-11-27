#include <exception>
#include <iostream>
#include <string>

#include "app/AppConfig.h"
#include "core/StableAudioEngine.h"
#include "core/GenerationConfig.h"

int main() {
    try {
        std::cout << "stable_audio_desktop start\n";

        AppConfig cfg = AppConfig::loadFromFile("../config/app_config.json");

        StableAudioEngine engine;
        if (!engine.initialize(cfg)) {
            std::cerr << "Engine initialization failed\n";
            return 1;
        }

        GenerationConfig gen;
        gen.prompt = "test prompt from desktop";
        gen.durationSeconds = cfg.defaultDurationSeconds;
        gen.sampleRate = cfg.sampleRate;
        gen.seed = 42;

        if (!engine.generateFromText(gen, "desktop_test.wav")) {
            std::cerr << "Generation failed\n";
            return 1;
        }

        std::cout << "Done. Check output/desktop_test.wav\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception\n";
        return 1;
    }
}
