#include "audio/AudioFileWriter.h"
#include <algorithm>
#include <cstdint>
#include <fstream>

namespace {

void writeLE(std::ofstream& out, uint32_t value, int bytes) {
    for (int i = 0; i < bytes; ++i) {
        out.put(static_cast<char>(value & 0xFF));
        value >>= 8;
    }
}

} // namespace

namespace AudioFileWriter {

bool writeWavStereo(const std::string& path,
                    const std::vector<float>& left,
                    const std::vector<float>& right,
                    float sampleRate) {
    if (left.size() != right.size() || left.empty()) return false;

    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    const uint16_t numChannels = 2;
    const uint32_t sr = static_cast<uint32_t>(sampleRate);
    const uint16_t bitsPerSample = 16;
    const uint32_t numFrames = static_cast<uint32_t>(left.size());
    const uint32_t byteRate = sr * numChannels * bitsPerSample / 8;
    const uint16_t blockAlign = numChannels * bitsPerSample / 8;
    const uint32_t dataSize = numFrames * blockAlign;
    const uint32_t chunkSize = 36 + dataSize;

    // RIFF
    out.write("RIFF", 4);
    writeLE(out, chunkSize, 4);
    out.write("WAVE", 4);

    // fmt
    out.write("fmt ", 4);
    writeLE(out, 16, 4);             // Subchunk1Size
    writeLE(out, 1, 2);              // PCM
    writeLE(out, numChannels, 2);
    writeLE(out, sr, 4);
    writeLE(out, byteRate, 4);
    writeLE(out, blockAlign, 2);
    writeLE(out, bitsPerSample, 2);

    // data
    out.write("data", 4);
    writeLE(out, dataSize, 4);

    for (uint32_t i = 0; i < numFrames; ++i) {
        float l = std::clamp(left[i], -1.0f, 1.0f);
        float r = std::clamp(right[i], -1.0f, 1.0f);

        int16_t li = static_cast<int16_t>(l * 32767.0f);
        int16_t ri = static_cast<int16_t>(r * 32767.0f);

        writeLE(out, static_cast<uint16_t>(li), 2);
        writeLE(out, static_cast<uint16_t>(ri), 2);
    }

    return true;
}

} // namespace AudioFileWriter
