#ifndef VOLUMEPROCESSOR_H
#define VOLUMEPROCESSOR_H

#include <vector>

class VolumeProcessor final
{
public:
    VolumeProcessor() noexcept = default;

    void setVolume(float volume);
    float getVolume() const;
    void processAudio(std::vector<float>& source);

private:
    float m_volume{1.0};
};

#endif // VOLUMEPROCESSOR_H
