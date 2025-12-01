#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include <vector>
#include <QByteArray>
#include <span>

namespace AudioConverter {

QByteArray fromDoubleArray(std::span<const double> source);
std::vector<double> fromByteArray(std::span<const int8_t> source);
std::vector<double> fromByteArray(const QByteArray& source);

std::vector<double> createMono(std::span<const double> source);
std::vector<double> createStereo(std::span<const double> source);

};

#endif // AUDIOCONVERTER_H
