#ifndef NH_HTML_DOCUMENT_H
#define NH_HTML_DOCUMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Input.h"
#include "Node.h"

#include "../../CSS/Header/Properties.h"
#include "../../Core/Header/URI.h"

#include <stdbool.h>

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

    /**
     * Represents a HTML document. It holds the document tree and all other related resources.
     */
    typedef struct Nh_HTML_Document {

        Nh_URI URI;          /**<Location of the document. Can be used to get data using @ref Nh_getData.*/
        Nh_HTML_Input Input; /**<Holds information for input handling.*/

        Nh_HTML_Tree Tree;   /**<The HTML document tree.*/ 

        Nh_List Scripts;     /**<Holds all associated JS scripts in the form of @ref Nh_JS_Script.*/ 
        Nh_List Sheets;      /**<Holds all associated CSS data in the form of @ref NH_CSS_Sheet.*/

    } Nh_HTML_Document;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    /**
     * Creates a @ref Nh_HTML_Document structure required for further computing.
     *
     * @param  location_p The location of the HTML document specified by a set of chars. This will be parsed by @ref Nh_createURI.
     * @param  Tab_p      The tab that will contain the document.
     * @return            @ref NH_SUCCESS on success, various error codes on failure.
     */
    NH_RESULT Nh_HTML_createDocument(
        char *location_p, Nh_Tab *Tab_p
    );

    NH_RESULT Nh_HTML_computeDocument(
        Nh_Tab *Tab_p, NH_BOOL resize
    );

    NH_RESULT Nh_HTML_updateDocument(
        Nh_Tab *Tab_p
    );

    NH_RESULT Nh_HTML_initDocument(
        char *location_p, Nh_HTML_Document *Document_p
    );

    NH_RESULT Nh_HTML_destroyDocument(
        Nh_HTML_Document *Document_p, Nh_GPU *GPU_p, NH_BOOL resize
    );

    char *Nh_HTML_stringifyDocument(
        Nh_HTML_Document *Document_p, NH_BOOL unformatted
    );

    char *Nh_HTML_getDefaultTestDocument(
        NH_INTERNAL_FILE file
    );

/** @} */

#endif
