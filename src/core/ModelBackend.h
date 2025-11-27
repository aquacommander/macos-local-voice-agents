#pragma once
#include <string>
#include <vector>

#include "core/GenerationConfig.h"
#include "app/AppConfig.h"

class ModelBackend {
public:
    virtual ~ModelBackend() = default;

    virtual bool initialize(const AppConfig& cfg) = 0;


    virtual bool generate(const GenerationConfig& genCfg,
                          std::vector<float>& outLeft,
                          std::vector<float>& outRight) = 0;

    virtual const char* name() const = 0;
};
