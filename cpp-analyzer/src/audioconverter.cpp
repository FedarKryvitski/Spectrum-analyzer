#include "audioconverter.h"

QByteArray AudioConverter::fromFloatArray(const std::span<float>& source)
{
    const int size = source.size();
    QByteArray result(size * sizeof(int16_t), 0);
    for (int i = 0; i < size; ++i){
        int16_t elem = static_cast<int16_t>(source[i] * 32'768);
        int8_t high = elem >> 8;
        int8_t low = elem & 255;
        result[i * 2] = low;
        result[i * 2 + 1] = high;
    }
    return result;
}

std::vector<float> AudioConverter::fromByteArray(const std::span<int8_t>& source)
{
    const int size = source.size();
    std::vector<float> result(size / sizeof(int16_t), 0);

    for (int i = 0; i < size; i += 2){
        uint8_t low = static_cast<uint8_t>(source[i]);
        uint8_t high = static_cast<uint8_t>(source[i + 1]);
        int16_t elem = (high << 8) | low;
        float value = static_cast<float>(elem) / 32'768.f;
        result[i / 2] = value;
    }
    return result;
}

std::vector<float> AudioConverter::fromByteArray(const QByteArray& source)
{
    const int size = source.size();
    std::vector<float> result(size / sizeof(int16_t), 0);

    for (int i = 0; i < size; i += 2){
        uint8_t low = static_cast<uint8_t>(source[i]);
        uint8_t high = static_cast<uint8_t>(source[i + 1]);
        int16_t elem = (high << 8) | low;
        float value = static_cast<float>(elem) / 32'768.f;
        result[i / 2] = value;
    }
    return result;
}

std::vector<float> AudioConverter::createMono(const std::span<float>& source)
{
    const int size = source.size();
    std::vector<float> result(size / 2);
    for (int i = 0; i < size / 2; ++i){
        float left = source[i * 2];
        float right = source[i * 2 + 1];
        result[i] = (left + right) / 2;
    }
    return result;
}

std::vector<float> AudioConverter::createStereo(const std::span<float>& source)
{
    const int size = source.size();
    std::vector<float> result(size * 2);
    for (int i = 0; i < size; ++i){
        float val = source[i];
        result[i * 2] = val;
        result[i * 2 + 1] = val;
    }
    return result;
}

