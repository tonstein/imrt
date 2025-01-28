#pragma once

#include <imrt.h>

static uint32_t gainId = 1;
static ImRt::ParameterLayout gainLayout(gainId, "Gain", 0.0f, 2.0f, 1.0f);

static uint32_t panId = 2;
static ImRt::ParameterLayout panLayout(panId, "Pan", -1.0f, 1.0f, 0.0f);
