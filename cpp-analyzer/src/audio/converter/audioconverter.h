#pragma once

#include <cstdint>
#include <span>
#include <vector>

namespace AudioConverter
{

std::vector<double> toDoubleVector(std::span<const int16_t> data);

std::vector<int16_t> toIntVector(std::span<const double> data);

}; // namespace AudioConverter
