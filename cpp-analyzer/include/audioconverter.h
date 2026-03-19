#ifndef AUDIOCONVERTER_H
#define AUDIOCONVERTER_H

#include <cstdint>
#include <span>
#include <vector>

namespace AudioConverter
{

std::vector<double> toDoubleVector(std::span<const int16_t> data);

std::vector<int16_t> toIntVector(std::span<const double> data);

}; // namespace AudioConverter

#endif // AUDIOCONVERTER_H
