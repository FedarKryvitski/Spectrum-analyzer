#ifndef AUDIOANALYZER_H
#define AUDIOANALYZER_H

namespace AudioAnalyzer {

class AudioAnalyzer
{
public:
    explicit AudioAnalyzer(float sampleRate = 48000.f) noexcept;
    virtual ~AudioAnalyzer() = default;

private:
    float sampleRate_;
};

} // namespace AudioAnalyzer

#endif // AUDIOANALYZER_H
