#include "volumeprocessor.h"
#include <algorithm>
#include <cmath>

void VolumeProcessor::setVolume(float volume)
{
    m_volume = volume;
}

float VolumeProcessor::getVolume() const
{
    return m_volume;
}

void VolumeProcessor::processAudio(std::vector<float>& source)
{
    std::ranges::for_each(source,
        [volume = std::sqrt(m_volume)](auto& sample)
    {
        sample *= volume;
    });
}
