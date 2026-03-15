#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include <QByteArray>
#include <span>
#include <vector>

namespace AudioConverter
{

QByteArray fromDoubleArray(std::span<const double> source);
std::vector<double> fromByteArray(std::span<const int8_t> source);
std::vector<double> fromByteArray(const QByteArray &source);

std::vector<double> createMono(std::span<const double> source);
std::vector<double> createStereo(std::span<const double> source);

}; // namespace AudioConverter

#endif // AUDIOCONVERTER_H
