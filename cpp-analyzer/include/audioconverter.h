#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include <vector>
#include <QByteArray>
#include <span>

namespace AudioConverter {

QByteArray fromFloatArray(std::span<const float> source);
std::vector<float> fromByteArray(std::span<const int8_t> source);
std::vector<float> fromByteArray(const QByteArray& source);

std::vector<float> createMono(std::span<const float> source);
std::vector<float> createStereo(std::span<const float> source);

};

#endif // AUDIOCONVERTER_H
