#pragma once

#include <imrt.h>

static ImRt::ParameterLayout gainLayout(1, "Gain", 0.0f, 2.0f, 1.0f);
static ImRt::ParameterLayout panLayout(2, "Pan", -1.0f, 1.0f, 0.0f);
static ImRt::ParameterLayout muteLayout(3, "Mute", 0.0f, 1.0f, 0.0f);
