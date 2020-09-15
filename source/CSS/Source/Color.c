// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Color.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Config.h"
#include "../../Core/Header/HashMap.h"

#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include NH_DEBUG

// DATA ============================================================================================

const char *NH_CSS_COLORS_PP[] = 
{
    "aliceblue",  	
    "antiquewhite",  	
    "aqua", 
    "aquamarine",
    "azure",	
    "beige",	
    "bisque", 	
    "black",	
    "blanchedalmond",
    "blue",
    "blueviolet", 	
    "brown",	
    "burlywood",  	
    "cadetblue",	
    "chartreuse", 	
    "chocolate",	
    "coral",	
    "cornflowerblue", 
    "cornsilk",	
    "crimson",	
    "cyan",
    "darkblue",
    "darkcyan",	
    "darkgoldenrod",
    "darkgray",
    "darkgrey",	
    "darkgreen", 	
    "darkkhaki",	
    "darkmagenta",  	
    "darkolivegreen", 
    "darkorange",
    "darkorchid",	
    "darkred",	
    "darksalmon",  	
    "darkseagreen", 	
    "darkslateblue", 
    "darkslategray",  
    "darkslategrey",  
    "darkturquoise",  
    "darkviolet",  	
    "deeppink",	
    "deepskyblue",  	
    "dimgray",	
    "dimgrey",	
    "dodgerblue", 	
    "firebrick",	
    "floralwhite",  	
    "forestgreen",	
    "fuchsia",	
    "gainsboro",  	
    "ghostwhite", 	
    "gold",
    "goldenrod",
    "gray",
    "grey",	 
    "green", 	
    "greenyellow", 	
    "honeydew",	
    "hotpink",	
    "indianred",   	
    "indigo",	
    "ivory",	
    "khaki",	
    "lavender",  	
    "lavenderblush",
    "lawngreen",
    "lemonchiffon",  	
    "lightblue",	
    "lightcoral", 	
    "lightcyan",	
    "lightgoldenrod",
    "lightgray",
    "lightgrey",	
    "lightgreen", 	
    "lightpink",	
    "lightsalmon",  	
    "lightseagreen",
    "lightskyblue",
    "lightslategray", 
    "lightslategrey",
    "lightsteelblue",
    "lightyellow",
    "lime",
    "limegreen",	
    "linen",	
    "magenta",  	
    "maroon",	
    "mediumaquamarine",
    "mediumblue",
    "mediumorchid",  	
    "mediumpurple",	
    "mediumseagreen", 	
    "mediumslateblue",	
    "mediumspringgreen",	
    "mediumturquoise",	
    "mediumvioletred",	
    "midnightblue",
    "mintcream",	
    "mistyrose",	
    "moccasin",	
    "navajowhite",  	
    "navy",
    "oldlace",  	
    "olive",	
    "olivedrab",  	
    "orange",	
    "orangered",  	
    "orchid",	
    "palegoldenrod",  
    "palegreen",
    "paleturquoise",  
    "palevioletred",
    "papayawhip",
    "peachpuff",	
    "peru",
    "pink",
    "plum",
    "powderblue",  	
    "purple",	
    "rebeccapurple",  
    "red",
    "rosybrown",
    "royalblue",	
    "saddlebrown", 	
    "salmon",	
    "sandybrown",	
    "seagreen",	
    "seashell",	
    "sienna",	
    "silver",	
    "skyblue", 	
    "slateblue",  	
    "slategray",	
    "slategrey",	
    "snow",
    "springgreen",
    "steelblue",	
    "tan",
    "teal",
    "thistle",  	
    "tomato",	
    "turquoise",  	
    "violet",	
    "wheat",	
    "white",	
    "whitesmoke",  	
    "yellow",	
    "yellowgreen",  	
};

size_t NH_CSS_COLORS_PP_COUNT = sizeof(NH_CSS_COLORS_PP) / sizeof(NH_CSS_COLORS_PP[0]);

// GET =============================================================================================

static NH_RESULT Nh_CSS_getColorFromName(
    char *name_p, char *hex_p)
{
NH_BEGIN()

    if (name_p == NULL || strlen(name_p) > 255) {NH_END(NH_ERROR_BAD_STATE)}

    char nameCpy_p[256] = {'\0'};
    strcpy(nameCpy_p, name_p);
    for (int i = 0; i < strlen(nameCpy_p); ++i) {nameCpy_p[i] = tolower(nameCpy_p[i]);}

    Nh_HashValue *HashValue_p;
    
    if (hashmap_get(NH_HASHMAPS.CSS.Colors, nameCpy_p, (void**)&HashValue_p) != MAP_OK) {
        NH_END(NH_ERROR_BAD_STATE)
    }
    
    switch (HashValue_p->number) 
    {
        case NH_CSS_COLOR_NAME_ALICE_BLUE:          strcpy(hex_p, "#F0F8FF"); break;
        case NH_CSS_COLOR_NAME_ANTIQUE_WHITE:       strcpy(hex_p, "#FAEBD7"); break;  	
        case NH_CSS_COLOR_NAME_AQUA:                strcpy(hex_p, "#00FFFF"); break;
        case NH_CSS_COLOR_NAME_AQUAMARINE:          strcpy(hex_p, "#7FFFD4"); break;
        case NH_CSS_COLOR_NAME_AZURE:	            strcpy(hex_p, "#F0FFFF"); break;
        case NH_CSS_COLOR_NAME_BEIGE:	            strcpy(hex_p, "#F5F5DC"); break;
        case NH_CSS_COLOR_NAME_BISQUE: 	            strcpy(hex_p, "#FFE4C4"); break;
        case NH_CSS_COLOR_NAME_BLACK:	            strcpy(hex_p, "#000000"); break;
        case NH_CSS_COLOR_NAME_BLANCHED_ALMOND:     strcpy(hex_p, "#FFEBCD"); break;
        case NH_CSS_COLOR_NAME_BLUE:                strcpy(hex_p, "#0000FF"); break;
        case NH_CSS_COLOR_NAME_BLUE_VIOLET:	    strcpy(hex_p, "#8A2BE2"); break;
        case NH_CSS_COLOR_NAME_BROWN:	            strcpy(hex_p, "#A52A2A"); break;
        case NH_CSS_COLOR_NAME_BURLY_WOOD:          strcpy(hex_p, "#DEB887"); break;
        case NH_CSS_COLOR_NAME_CADET_BLUE:	    strcpy(hex_p, "#5F9EA0"); break;
        case NH_CSS_COLOR_NAME_CHARTREUSE: 	    strcpy(hex_p, "#7FFF00"); break;
        case NH_CSS_COLOR_NAME_CHOCOLATE:	    strcpy(hex_p, "#D2691E"); break;
        case NH_CSS_COLOR_NAME_CORAL:	            strcpy(hex_p, "#FF7F50"); break;
        case NH_CSS_COLOR_NAME_CORNFLOWER_BLUE:     strcpy(hex_p, "#6495ED"); break;
        case NH_CSS_COLOR_NAME_CORNSILK:            strcpy(hex_p, "#FFF8DC"); break;
        case NH_CSS_COLOR_NAME_CRIMSON:	            strcpy(hex_p, "#DC143C"); break;
        case NH_CSS_COLOR_NAME_CYAN:                strcpy(hex_p, "#00FFFF"); break;
        case NH_CSS_COLOR_NAME_DARK_BLUE:           strcpy(hex_p, "#00008B"); break;
        case NH_CSS_COLOR_NAME_DARK_CYAN:	    strcpy(hex_p, "#008B8B"); break;
        case NH_CSS_COLOR_NAME_DARK_GOLDEN_ROD:     strcpy(hex_p, "#B8860B"); break;
        case NH_CSS_COLOR_NAME_DARK_GRAY:           strcpy(hex_p, "#A9A9A9"); break;
        case NH_CSS_COLOR_NAME_DARK_GREY:	    strcpy(hex_p, "#A9A9A9"); break;
        case NH_CSS_COLOR_NAME_DARK_GREEN: 	    strcpy(hex_p, "#006400"); break;
        case NH_CSS_COLOR_NAME_DARK_KHAKI:	    strcpy(hex_p, "#BDB76B"); break;
        case NH_CSS_COLOR_NAME_DARK_MAGENTA:        strcpy(hex_p, "#8B008B"); break;
        case NH_CSS_COLOR_NAME_DARK_OLIVE_GREEN:    strcpy(hex_p, "#556B2F"); break;
        case NH_CSS_COLOR_NAME_DARK_ORANGE:         strcpy(hex_p, "#FF8C00"); break;
        case NH_CSS_COLOR_NAME_DARK_ORCHID:	    strcpy(hex_p, "#9932CC"); break;
        case NH_CSS_COLOR_NAME_DARK_RED:	    strcpy(hex_p, "#8B0000"); break;
        case NH_CSS_COLOR_NAME_DARK_SALMON:         strcpy(hex_p, "#E9967A"); break;
        case NH_CSS_COLOR_NAME_DARK_SEA_GREEN:      strcpy(hex_p, "#8FBC8F"); break;
        case NH_CSS_COLOR_NAME_DARK_SLATE_BLUE:     strcpy(hex_p, "#483D8B"); break;
        case NH_CSS_COLOR_NAME_DARK_SLATE_GRAY:     strcpy(hex_p, "#2F4F4F"); break;
        case NH_CSS_COLOR_NAME_DARK_SLATE_GREY:     strcpy(hex_p, "#2F4F4F"); break;
        case NH_CSS_COLOR_NAME_DARK_TURQUOISE:      strcpy(hex_p, "#00CED1"); break;
        case NH_CSS_COLOR_NAME_DARK_VIOLET:	    strcpy(hex_p, "#9400D3"); break;
        case NH_CSS_COLOR_NAME_DEEP_PINK:	    strcpy(hex_p, "#FF1493"); break;
        case NH_CSS_COLOR_NAME_DEEP_SKY_BLUE:       strcpy(hex_p, "#00BFFF"); break;
        case NH_CSS_COLOR_NAME_DIM_GRAY:            strcpy(hex_p, "#696969"); break;
        case NH_CSS_COLOR_NAME_DIM_GREY:	    strcpy(hex_p, "#696969"); break;
        case NH_CSS_COLOR_NAME_DODGER_BLUE: 	    strcpy(hex_p, "#1E90FF"); break;
        case NH_CSS_COLOR_NAME_FIRE_BRICK:	    strcpy(hex_p, "#B22222"); break;
        case NH_CSS_COLOR_NAME_FLORAL_WHITE:        strcpy(hex_p, "#FFFAF0"); break;
        case NH_CSS_COLOR_NAME_FOREST_GREEN:	    strcpy(hex_p, "#228B22"); break;
        case NH_CSS_COLOR_NAME_FUCHSIA:	            strcpy(hex_p, "#FF00FF"); break;
        case NH_CSS_COLOR_NAME_GAINSBORO:  	    strcpy(hex_p, "#DCDCDC"); break;
        case NH_CSS_COLOR_NAME_GHOST_WHITE: 	    strcpy(hex_p, "#F8F8FF"); break;
        case NH_CSS_COLOR_NAME_GOLD:                strcpy(hex_p, "#FFD700"); break;
        case NH_CSS_COLOR_NAME_GOLDEN_ROD:          strcpy(hex_p, "#DAA520"); break;
        case NH_CSS_COLOR_NAME_GRAY:                strcpy(hex_p, "#808080"); break;
        case NH_CSS_COLOR_NAME_GREY:	            strcpy(hex_p, "#808080"); break;
        case NH_CSS_COLOR_NAME_GREEN: 	            strcpy(hex_p, "#008000"); break;
        case NH_CSS_COLOR_NAME_GREEN_YELLOW:        strcpy(hex_p, "#ADFF2F"); break;
        case NH_CSS_COLOR_NAME_HONEY_DEW:	    strcpy(hex_p, "#F0FFF0"); break;
        case NH_CSS_COLOR_NAME_HOT_PINK:	    strcpy(hex_p, "#FF69B4"); break;
        case NH_CSS_COLOR_NAME_INDIAN_RED:          strcpy(hex_p, "#CD5C5C"); break;
        case NH_CSS_COLOR_NAME_INDIGO:	            strcpy(hex_p, "#4B0082"); break;
        case NH_CSS_COLOR_NAME_IVORY:	            strcpy(hex_p, "#FFFF00"); break;
        case NH_CSS_COLOR_NAME_KHAKI:	            strcpy(hex_p, "#F0E68C"); break;
        case NH_CSS_COLOR_NAME_LAVENDER:            strcpy(hex_p, "#E6E6FA"); break;
        case NH_CSS_COLOR_NAME_LAVENDER_BLUSH:      strcpy(hex_p, "#FFF0F5"); break;
        case NH_CSS_COLOR_NAME_LAWN_GREEN:          strcpy(hex_p, "#7CFC00"); break;
        case NH_CSS_COLOR_NAME_LEMON_CHIFFON:  	    strcpy(hex_p, "#FFFACD"); break;
        case NH_CSS_COLOR_NAME_LIGHT_BLUE:          strcpy(hex_p, "#ADD8E6"); break;
        case NH_CSS_COLOR_NAME_LIGHT_CORAL: 	    strcpy(hex_p, "#F08080"); break;
        case NH_CSS_COLOR_NAME_LIGHT_CYAN:	    strcpy(hex_p, "#E0FFFF"); break;
        case NH_CSS_COLOR_NAME_LIGHT_GOLDEN_ROD:    strcpy(hex_p, "#FAFAD2"); break;
        case NH_CSS_COLOR_NAME_LIGHT_GRAY:          strcpy(hex_p, "#D3D3D3"); break;
        case NH_CSS_COLOR_NAME_LIGHT_GREY:	    strcpy(hex_p, "#D3D3D3"); break;
        case NH_CSS_COLOR_NAME_LIGHT_GREEN: 	    strcpy(hex_p, "#90EE90"); break;
        case NH_CSS_COLOR_NAME_LIGHT_PINK:	    strcpy(hex_p, "#FFB6C1"); break;
        case NH_CSS_COLOR_NAME_LIGHT_SALMON:  	    strcpy(hex_p, "#FFA07A"); break;
        case NH_CSS_COLOR_NAME_LIGHT_SEA_GREEN:     strcpy(hex_p, "#20B2AA"); break;
        case NH_CSS_COLOR_NAME_LIGHT_SKY_BLUE:      strcpy(hex_p, "#87CEFA"); break;
        case NH_CSS_COLOR_NAME_LIGHT_SLATE_GRAY:    strcpy(hex_p, "#778899"); break;
        case NH_CSS_COLOR_NAME_LIGHT_SLATE_GREY:    strcpy(hex_p, "#778899"); break;
        case NH_CSS_COLOR_NAME_LIGHT_STEEL_BLUE:    strcpy(hex_p, "#B0C4DE"); break;
        case NH_CSS_COLOR_NAME_LIGHT_YELLOW:        strcpy(hex_p, "#FFFFE0"); break;
        case NH_CSS_COLOR_NAME_LIME:                strcpy(hex_p, "#00FF00"); break;
        case NH_CSS_COLOR_NAME_LIME_GREEN:	    strcpy(hex_p, "#32CD32"); break;
        case NH_CSS_COLOR_NAME_LINEN:	            strcpy(hex_p, "#FAF0E6"); break;
        case NH_CSS_COLOR_NAME_MAGENTA:             strcpy(hex_p, "#FF00FF"); break;
        case NH_CSS_COLOR_NAME_MAROON:	            strcpy(hex_p, "#800000"); break;
        case NH_CSS_COLOR_NAME_MEDIUM_AQUAMARINE:   strcpy(hex_p, "#66CDAA"); break;
        case NH_CSS_COLOR_NAME_MEDIUM_BLUE:         strcpy(hex_p, "#0000CD"); break;
        case NH_CSS_COLOR_NAME_MEDIUM_ORCHID:       strcpy(hex_p, "#BA55D3"); break; 	
        case NH_CSS_COLOR_NAME_MEDIUM_PURPLE:	    strcpy(hex_p, "#9370DB"); break;
        case NH_CSS_COLOR_NAME_MEDIUM_SEA_GREEN:    strcpy(hex_p, "#3CB371"); break;
        case NH_CSS_COLOR_NAME_MEDIUM_SLATE_BLUE:   strcpy(hex_p, "#7B68EE"); break;
        case NH_CSS_COLOR_NAME_MEDIUM_SPRING_GREEN: strcpy(hex_p, "#00FA9A"); break;
        case NH_CSS_COLOR_NAME_MEDIUM_TURQUOISE:    strcpy(hex_p, "#48D1CC"); break;
        case NH_CSS_COLOR_NAME_MEDIUM_VIOLET_RED:   strcpy(hex_p, "#C71585"); break;
        case NH_CSS_COLOR_NAME_MIDNIGHT_BLUE:       strcpy(hex_p, "#191970"); break;
        case NH_CSS_COLOR_NAME_MINT_CREAM:          strcpy(hex_p, "#F5FFFA"); break;
        case NH_CSS_COLOR_NAME_MISTY_ROSE:          strcpy(hex_p, "#FFE4E1"); break;
        case NH_CSS_COLOR_NAME_MOCCASIN:            strcpy(hex_p, "#FFE4B5"); break;
        case NH_CSS_COLOR_NAME_NAVAJO_WHITE:        strcpy(hex_p, "#FFDEAD"); break;
        case NH_CSS_COLOR_NAME_NAVY:                strcpy(hex_p, "#000080"); break;
        case NH_CSS_COLOR_NAME_OLD_LACE:            strcpy(hex_p, "#FDF5E6"); break;
        case NH_CSS_COLOR_NAME_OLIVE:	            strcpy(hex_p, "#808000"); break;
        case NH_CSS_COLOR_NAME_OLIVE_DRAB:  	    strcpy(hex_p, "#6B8E23"); break;
        case NH_CSS_COLOR_NAME_ORANGE:	            strcpy(hex_p, "#FFA500"); break;
        case NH_CSS_COLOR_NAME_ORANGE_RED:  	    strcpy(hex_p, "#FF4500"); break;
        case NH_CSS_COLOR_NAME_ORCHID:	            strcpy(hex_p, "#DA70D6"); break;
        case NH_CSS_COLOR_NAME_PALE_GOLDEN_ROD:     strcpy(hex_p, "#EEE8AA"); break;
        case NH_CSS_COLOR_NAME_PALE_GREEN:          strcpy(hex_p, "#98FB98"); break;
        case NH_CSS_COLOR_NAME_PALE_TURQUOISE:      strcpy(hex_p, "#AFEEEE"); break;
        case NH_CSS_COLOR_NAME_PALE_VIOLET_RED:     strcpy(hex_p, "#DB7093"); break;
        case NH_CSS_COLOR_NAME_PAPAYA_WHIP:         strcpy(hex_p, "#FFEFD5"); break;
        case NH_CSS_COLOR_NAME_PEACH_PUFF:	    strcpy(hex_p, "#FFDAB9"); break;
        case NH_CSS_COLOR_NAME_PERU:                strcpy(hex_p, "#CD853F"); break;
        case NH_CSS_COLOR_NAME_PINK:                strcpy(hex_p, "#FFC0CB"); break;
        case NH_CSS_COLOR_NAME_PLUM:                strcpy(hex_p, "#DDA0DD"); break;
        case NH_CSS_COLOR_NAME_POWDER_BLUE: 	    strcpy(hex_p, "#B0E0E6"); break;
        case NH_CSS_COLOR_NAME_PURPLE:	            strcpy(hex_p, "#800080"); break;
        case NH_CSS_COLOR_NAME_REBECCA_PURPLE:      strcpy(hex_p, "#663399"); break;
        case NH_CSS_COLOR_NAME_RED:                 strcpy(hex_p, "#FF0000"); break;
        case NH_CSS_COLOR_NAME_ROSY_BROWN:          strcpy(hex_p, "#BC8F8F"); break;
        case NH_CSS_COLOR_NAME_ROYAL_BLUE:	    strcpy(hex_p, "#4169E1"); break;
        case NH_CSS_COLOR_NAME_SADDLE_BROWN:        strcpy(hex_p, "#8B4513"); break;
        case NH_CSS_COLOR_NAME_SALMON:	            strcpy(hex_p, "#FA8072"); break;
        case NH_CSS_COLOR_NAME_SANDY_BROWN:	    strcpy(hex_p, "#F4A460"); break;
        case NH_CSS_COLOR_NAME_SEA_GREEN:	    strcpy(hex_p, "#2E8B57"); break;
        case NH_CSS_COLOR_NAME_SEA_SHELL:	    strcpy(hex_p, "#FFF5EE"); break;
        case NH_CSS_COLOR_NAME_SIENNA:	            strcpy(hex_p, "#A0552D"); break;
        case NH_CSS_COLOR_NAME_SILVER:	            strcpy(hex_p, "#C0C0C0"); break;
        case NH_CSS_COLOR_NAME_SKY_BLUE: 	    strcpy(hex_p, "#87CEEB"); break;
        case NH_CSS_COLOR_NAME_SLATE_BLUE:  	    strcpy(hex_p, "#6A5ACD"); break;
        case NH_CSS_COLOR_NAME_SLATE_GRAY:	    strcpy(hex_p, "#708090"); break;
        case NH_CSS_COLOR_NAME_SLATE_GREY:	    strcpy(hex_p, "#708090"); break;
        case NH_CSS_COLOR_NAME_SNOW:                strcpy(hex_p, "#FFFAFA"); break;
        case NH_CSS_COLOR_NAME_SPRING_GREEN:        strcpy(hex_p, "#00FF7F"); break;
        case NH_CSS_COLOR_NAME_STEEL_BLUE:	    strcpy(hex_p, "#4682B4"); break;
        case NH_CSS_COLOR_NAME_TAN:                 strcpy(hex_p, "#D2B48C"); break;
        case NH_CSS_COLOR_NAME_TEAL:                strcpy(hex_p, "#008080"); break;
        case NH_CSS_COLOR_NAME_THISTLE:  	    strcpy(hex_p, "#D8BFD8"); break;
        case NH_CSS_COLOR_NAME_TOMATO:	            strcpy(hex_p, "#FF6347"); break;
        case NH_CSS_COLOR_NAME_TURQUOISE:  	    strcpy(hex_p, "#40E0D0"); break;
        case NH_CSS_COLOR_NAME_VIOLET:	            strcpy(hex_p, "#EE82EE"); break;
        case NH_CSS_COLOR_NAME_WHEAT:	            strcpy(hex_p, "#F5DEB3"); break;
        case NH_CSS_COLOR_NAME_WHITE:	            strcpy(hex_p, "#FFFFFF"); break;
        case NH_CSS_COLOR_NAME_WHITE_SMOKE: 	    strcpy(hex_p, "#F5F5F5"); break;
        case NH_CSS_COLOR_NAME_YELLOW:	            strcpy(hex_p, "#FFFF00"); break;
        case NH_CSS_COLOR_NAME_YELLOW_GREEN:        strcpy(hex_p, "#9ACD32"); break;	
    }

NH_END(NH_SUCCESS)
}

/**
 * Converts RGB values to sRGB color space.
 * More info: 
 *     https://en.wikipedia.org/wiki/SRGB#Specification_of_the_transformation
 *     https://learnopengl.com/Advanced-Lighting/Gamma-Correction
 */ 
static void Nh_CSS_toSRGB(
    double gamma, float *rgba_p)
{
NH_BEGIN()

    for (int i = 0; i < 3; ++i) {rgba_p[i] = pow((double)rgba_p[i], gamma);}

NH_SILENT_END()
}

static void Nh_CSS_rgba(
    char *str_p, float rgba_p[4])
{
NH_BEGIN()

    int rgb_p[3] = {0};
    int alpha_p[3] = {0};
    NH_BOOL percentage_p[3] = {NH_FALSE, NH_FALSE, NH_FALSE};

    int count = 0;
    char *c_p = str_p;
    for (int i = 0; i < strlen(c_p);) 
    {
        char c = c_p[i];
        if (c == '%' && count <= 3) {percentage_p[count-1] = NH_TRUE;}
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

static void Nh_CSS_hsla(
    char *str_p, float rgba_p[4])
{
NH_BEGIN()

     for (int i = 0; i < 4; ++i) {rgba_p[i] = 1.0f;}

NH_SILENT_END()
}

void Nh_CSS_getColor(
    char *color_p, float values_p[4])
{
NH_BEGIN()

         if (strstr(color_p, "#"))   {Nh_CSS_parseHexColor(color_p, values_p);} 
    else if (strstr(color_p, "rgb")) {Nh_CSS_rgba(color_p, values_p);} 
    else if (strstr(color_p, "hsl")) {Nh_CSS_hsla(color_p, values_p);}
    else 
    {
        char hex_p[16];
        if (Nh_CSS_getColorFromName(color_p, hex_p) == NH_SUCCESS) {
            Nh_CSS_parseHexColor(hex_p, values_p);
        } 
    }
    Nh_CSS_toSRGB(Nh_getConfig()->Settings.gamma, values_p);

NH_SILENT_END()
}

void Nh_CSS_parseHexColor(
    char *str_p, float rgba_p[4])
{
NH_BEGIN()

    int hex_p[4];
    memmove(&str_p[0], &str_p[1], strlen(str_p) - 1);

    if (strlen(str_p) <= 7) {
        sscanf(str_p, "%02x%02x%02x", &hex_p[0], &hex_p[1], &hex_p[2]);
        for (int i = 0; i < 3; ++i) {rgba_p[i] = ((float)hex_p[i] / 255.0f);}
        rgba_p[3] = 1.0f;
    }
    else {
        sscanf(str_p, "%02x%02x%02x%02x", &hex_p[0], &hex_p[1], &hex_p[2], &hex_p[3]);
        for (int i = 0; i < 4; ++i) {rgba_p[i] = ((float)hex_p[i] / 255.0f);}
    }
    
NH_SILENT_END()
}

