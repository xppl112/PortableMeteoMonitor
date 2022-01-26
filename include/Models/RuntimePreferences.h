#pragma once

#include "Models/Enums/Mode.h"

struct RuntimePreferences
{
public:
    Mode mode = Mode::STANDARD;
    bool networkEnabled = true;
};