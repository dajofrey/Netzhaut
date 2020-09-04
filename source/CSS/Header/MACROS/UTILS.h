/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_CSS_UTILITY_MACROS_H
#define NH_CSS_UTILITY_MACROS_H

#define NH_CSS_SHOW_TOP_BORDER(Node_p)                                          \
(                                                                               \
       Node_p->Computed.Properties.Border.Width.top > 0.0f                      \
    && Node_p->Computed.Properties.Border.Style.top != NH_CSS_BORDER_STYLE_NONE \
)

#define NH_CSS_SHOW_BOTTOM_BORDER(Node_p)                                          \
(                                                                                  \
       Node_p->Computed.Properties.Border.Width.bottom > 0.0f                      \
    && Node_p->Computed.Properties.Border.Style.bottom != NH_CSS_BORDER_STYLE_NONE \
)

#define NH_CSS_SHOW_RIGHT_BORDER(Node_p)                                          \
(                                                                                 \
       Node_p->Computed.Properties.Border.Width.right > 0.0f                      \
    && Node_p->Computed.Properties.Border.Style.right != NH_CSS_BORDER_STYLE_NONE \
)

#define NH_CSS_SHOW_LEFT_BORDER(Node_p)                                          \
(                                                                                \
       Node_p->Computed.Properties.Border.Width.left > 0.0f                      \
    && Node_p->Computed.Properties.Border.Style.left != NH_CSS_BORDER_STYLE_NONE \
)

#define NH_CSS_NORMALIZED_LENGTH(normalizedPixel) \
(                                                 \
    normalizedPixel + 1.0f                        \
)

#define NH_CSS_NORMALIZE_X_PIXEL(pixel, Tab_p)                         \
(                                                                      \
    (((float)pixel) / ((float)Tab_p->Info.pxSize_p[0])) * 2.0f - 1.0f \
)

#define NH_CSS_DENORMALIZE_X_PIXEL(normalizedPixel, Tab_p)                       \
(                                                                                \
    (((float)normalizedPixel + 1.0f) * ((float)Tab_p->Info.pxSize_p[0])) / 2.0f \
)

#define NH_CSS_NORMALIZE_Y_PIXEL(pixel, Tab_p)                         \
(                                                                      \
    (((float)pixel) / ((float)Tab_p->Info.pxSize_p[1])) * 2.0f - 1.0f \
)

#define NH_CSS_DENORMALIZE_Y_PIXEL(normalizedPixel, Tab_p)                       \
(                                                                                \
    (((float)normalizedPixel + 1.0f) * ((float)Tab_p->Info.pxSize_p[1])) / 2.0f \
)

#define NH_CSS_IS_SHEET(path)         \
(                                     \
    strlen(path) >= 4                 \
    &&  path[strlen(path) - 1] == 's' \
    &&  path[strlen(path) - 2] == 's' \
    &&  path[strlen(path) - 3] == 'c' \
    &&  path[strlen(path) - 4] == '.' \
)

#endif 
