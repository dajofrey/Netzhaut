#ifndef NH_HTML_TEXT_H
#define NH_HTML_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    
/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Tree.h"

#include "../../Core/Header/Window.h"
#include "../../API/Header/Netzhaut.h"

#include <stdint.h>

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_HTML_Document Nh_HTML_Document;

#endif

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    /**
     * Contains text-related data for @ref Nh_HTML_Node. 
     */
    typedef struct Nh_HTML_Text {
        float yOffset;
        float width;         /**<Text width.*/
        float *vertices_p;   /**<Normalized vertices.*/
        uint32_t *indices_p; /**<Vertex indices.*/
    } Nh_HTML_Text;               

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    void Nh_HTML_initText(
        Nh_HTML_Text *Text_p
    );

    NH_BOOL Nh_HTML_isTextNode(
        Nh_HTML_Node *Node_p
    );

    NH_RESULT Nh_HTML_createNormalizedText(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );

    NH_RESULT Nh_HTML_createTextNodes(
        Nh_Tab *Tab_p, Nh_HTML_Node *Parent_p, Nh_HTML_Node *Node_p
    );

    void Nh_HTML_destroyText(
        Nh_HTML_Node *Node_p
    );

    void Nh_HTML_destroyFormattedTextNodes(
        Nh_HTML_Tree *Tree_p
    );

    char *Nh_HTML_getText(
        Nh_HTML_Node *Node_p, NH_BOOL recursive
    );

/** @} */

#endif 
