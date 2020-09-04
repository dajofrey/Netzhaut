// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Functions.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/List.h"

#include NH_DEBUG

#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// IMPLEMENT ======================================================================================

void Nh_CSS_rgba(
    char *str_p, float rgba_p[4])
{
NH_BEGIN()

    int rgb_p[3] = {0};
    int alpha_p[3] = {0};
    bool percentage_p[3] = {false, false, false};

    int count = 0;
    char *c_p = str_p;
    for (int i = 0; i < strlen(c_p);) 
    {
        char c = c_p[i];
        if (c == '%' && count <= 3) {percentage_p[count-1] = true;}
        if (isdigit(c_p[i])) 
        {
            char *val_p = c_p+i;
            long val;
            if (count >= 3) { // alpha
                if (count >= 6) {break;}
                val = strtol(val_p, &val_p, 10);
                alpha_p[count - 3] = (int)val;
                count++;
            } else { // rgb
                val = strtol(val_p, &val_p, 10);
                rgb_p[count++] = (int)val;
            }
            int digits = (int)val == 0 ? 1 : floor(log10(abs((int)val))) + 1;
            i += digits;
        } else {
            ++i;
        }
    }

    for (int i = 0; i < 3; ++i) {
        rgba_p[i] = 
            percentage_p[i] ? ((float)rgb_p[i]) / 100.0 : ((float)rgb_p[i]) / 255.0;
    }

    if (count == 3) {rgba_p[3] = 1.0f;} 
    else {
        if (count == 5) {alpha_p[2] = 0;}
        int digits1 = alpha_p[1] == 0 ? 1 : floor(log10(abs(alpha_p[1]))) + 1;
        int digits2 = alpha_p[2] == 0 ? 0 : floor(log10(abs(alpha_p[2]))) + 1;
        float alpha = alpha_p[0] + (alpha_p[1] / pow(10.0, (long)digits1 + digits2));
        rgba_p[3] = alpha;
    }

NH_SILENT_END()
}

void Nh_CSS_hsla(
    char *str_p, float rgba_p[4])
{
NH_BEGIN()

     for (int i = 0; i < 4; ++i) {rgba_p[i] = 1.0f;}

NH_SILENT_END()
}

