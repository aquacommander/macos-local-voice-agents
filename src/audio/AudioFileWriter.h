#pragma once
#include <string>
#include <vector>

namespace AudioFileWriter {
    bool writeWavStereo(const std::string& path,
                        const std::vector<float>& left,
                        const std::vector<float>& right,
                        float sampleRate);
}
