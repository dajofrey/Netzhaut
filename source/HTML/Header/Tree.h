#ifndef NH_HTML_TREE_H
#define NH_HTML_TREE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/List.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_Tab Nh_Tab;
typedef struct Nh_GPU Nh_GPU;

#endif

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    /** 
     * HTML document tree.
     */
    typedef struct Nh_HTML_Tree { 

        Nh_HTML_Node *Root_p;    /**<A pointer to the root node.*/

        struct Nh_HTML_Flat {
            Nh_List Unformatted; /**<Contains nodes gathered using unformatted traversal.*/
            Nh_List Formatted;   /**<Contains nodes gathered using formatted traversal.*/
        } Flat;                  /**<Flattened tree for quick traversal. Must be kept up-to-date with the actual tree.*/

    } Nh_HTML_Tree;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    NH_RESULT Nh_HTML_recreateFlatTree(
        Nh_HTML_Tree *Tree_p, Nh_HTML_Node *Node_p, NH_BOOL unformatted
    );

    NH_RESULT Nh_HTML_computeFormattedTree(
        Nh_Tab *Tab_p
    );

    NH_RESULT Nh_HTML_destroyFormattedTree(
        Nh_HTML_Tree *Tree_p, Nh_GPU *GPU_p
    );

    NH_RESULT Nh_HTML_recomputeFormattedTree(
        Nh_Tab *Tab_p
    );

    NH_RESULT Nh_HTML_recomputeTrees(
        Nh_Tab *Tab_p
    );

    void Nh_HTML_destroyUnformattedTree(
        Nh_HTML_Tree *Tree_p
    );

    char *Nh_HTML_stringifyTree(
        Nh_HTML_Tree *Tree_p, NH_BOOL unformatted
    );

/** @} */

#endif
