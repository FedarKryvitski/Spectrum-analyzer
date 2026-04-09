#pragma once

#include <QString>

enum class InputType
{
    Microphone,
    File
};

struct AudioSessionConfig
{
    InputType inputType{InputType::Microphone};
    QString source;
};
