#include "core/ModelBackend.h"
#include <iostream>



class ModelBackendOnnx : public ModelBackend {
public:
    bool initialize(const AppConfig& cfg) override {
#ifndef USE_ONNXRUNTIME
        std::cerr << "[OnnxBackend] Built without ONNX Runtime, falling back.\n";
        return false;
#else
        try {
            Ort::SessionOptions session_options;
            session_options.SetIntraOpNumThreads(1);
            session_options.SetGraphOptimizationLevel(
                GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

            env_ = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING,
                                              "stable-audio");
            session_ = std::make_unique<Ort::Session>(
                *env_, cfg.modelPath.c_str(), session_options);

            sampleRate_ = cfg.sampleRate;
            std::cout << "[OnnxBackend] Loaded model from " << cfg.modelPath
                      << "\n";
            return true;
        } catch (const std::exception& e) {
            std::cerr << "[OnnxBackend] init failed: " << e.what() << "\n";
            return false;
        }
#endif
    }

    bool generate(const GenerationConfig& genCfg,
                  std::vector<float>& outLeft,
                  std::vector<float>& outRight) override {
#ifndef USE_ONNXRUNTIME
        (void)genCfg;
        (void)outLeft;
        (void)outRight;
        std::cerr << "[OnnxBackend] ONNX Runtime not enabled.\n";
        return false;
#else
        // TODO: This is where you wire stable-audio-open-small's actual inputs.
        // Pseudocode outline (you must adapt to the real model IO):
        //
        // 1. Build input tensors: tokenized prompt, duration, seed, etc.
        // 2. Call session_->Run(...)
        // 3. Extract stereo PCM from outputs into outLeft/outRight.

        std::cerr << "[OnnxBackend] generate() not implemented yet.\n";
        return false;
#endif
    }

    const char* name() const override { return "onnx"; }

private:
#ifdef USE_ONNXRUNTIME
    std::unique_ptr<Ort::Env> env_;
    std::unique_ptr<Ort::Session> session_;
#endif
    float sampleRate_ = 44100.0f;
};

// Factory
ModelBackend* createOnnxBackend() {
    return new ModelBackendOnnx();
}
