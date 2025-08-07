#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include <vector>
#include <QByteArray>
#include <span>

namespace AudioConverter {

QByteArray fromFloatArray(const std::span<float>& source);
std::vector<float> fromByteArray(const std::span<int8_t>& source);
std::vector<float> fromByteArray(const QByteArray& source);

std::vector<float> createMono(const std::span<float>& source);
std::vector<float> createStereo(const std::span<float>& source);

};

#endif // AUDIOCONVERTER_H
