/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_IO_X11_MACROS_H
#define NH_IO_X11_MACROS_H

#include "../../Header/Macros.h"

#undef NH_DEFAULT_CHECK
#undef NH_CUSTOM_CHECK

#define NH_DEFAULT_CHECK "../../../Core/Header/MACROS/DEFAULT_CHECK.h"
#define NH_CUSTOM_CHECK "../../../Core/Header/MACROS/CUSTOM_CHECK.h"

#undef NH_DEBUG
#define NH_DEBUG "../../../Core/Header/MACROS/DEBUG.h"

#endif 
