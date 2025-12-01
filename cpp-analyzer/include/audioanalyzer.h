#ifndef AUDIOANALYZER_H
#define AUDIOANALYZER_H

#include <stdlib.h>

namespace AudioAnalyzer {

class AudioAnalyzer
{
public:
    explicit AudioAnalyzer(size_t sampleRate = 44100) noexcept;
    virtual ~AudioAnalyzer() = default;

private:
    size_t sampleRate_;
};

} // namespace AudioAnalyzer

#endif // AUDIOANALYZER_H
