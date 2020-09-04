#ifndef NH_VK_MACROS_H
#define NH_VK_MACROS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../Core/Header/Macros.h"
#include "../../../CSS/Header/Macros.h"

#undef NH_DEFAULT_CHECK
#undef NH_CUSTOM_CHECK

#define NH_DEFAULT_CHECK "../../../Core/Header/MACROS/DEFAULT_CHECK.h"
#define NH_CUSTOM_CHECK "../../../Core/Header/MACROS/CUSTOM_CHECK.h"

#undef NH_DEBUG
#define NH_DEBUG "../../../Core/Header/MACROS/DEBUG.h"

#undef NH_UTILS
#define NH_UTILS "../../../Core/Header/MACROS/UTILS.h"

#undef NH_HTML_UTILS
#define NH_HTML_UTILS "../../../HTML/Header/MACROS/UTILS.h"

#undef NH_CSS_UTILS
#define NH_CSS_UTILS "../../../CSS/Header/MACROS/UTILS.h"

#endif

/** \defgroup Vulkan 
 *  \brief Vulkan module.
 *  \ingroup Graphics
 */

#endif 
