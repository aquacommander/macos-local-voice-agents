#include <exception>
#include <iostream>
#include <string>

#include "app/AppConfig.h"
#include "core/StableAudioEngine.h"
#include "core/GenerationConfig.h"

int main(int argc, char** argv) {
    try {
        std::string prompt = "default cli prompt";
        if (argc > 1) {
            prompt.clear();
            for (int i = 1; i < argc; ++i) {
                if (i > 1) prompt += ' ';
                prompt += argv[i];
            }
        }

        AppConfig cfg = AppConfig::loadFromFile("../config/app_config.json");

        StableAudioEngine engine;
        if (!engine.initialize(cfg)) {
            std::cerr << "Engine initialization failed\n";
            return 1;
        }

        GenerationConfig gen;
        gen.prompt = prompt;
        gen.durationSeconds = cfg.defaultDurationSeconds;
        gen.sampleRate = cfg.sampleRate;
        gen.seed = 123;

        if (!engine.generateFromText(gen, "cli_test.wav")) {
            std::cerr << "Generation failed\n";
            return 1;
        }

        std::cout << "CLI done. Check output/cli_test.wav\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception\n";
        return 1;
    }
}
