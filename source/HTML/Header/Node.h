#ifndef NH_HTML_NODE_H
#define NH_HTML_NODE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Input.h"
#include "Attribute.h"
#include "Text.h"

#include "../../CSS/Header/Properties.h"
#include "../../CSS/Header/Box.h"
#include "../../Core/Header/URI.h"

#include <stdbool.h>

#endif

/** @addtogroup HTMLData Data
 *  \ingroup HTML
 *  @{
 */
    extern const char *NH_HTML_TAGS_PP[];
    extern size_t NH_HTML_TAGS_PP_COUNT;

/** @} */

/** @addtogroup HTMLEnums Enums
 *  \ingroup HTML
 *  @{
 */

    typedef enum NH_HTML_TAG {
        NH_HTML_TAG_A,          /**<Not supported.*/ 
        NH_HTML_TAG_ABBR,       /**<Not supported.*/
        NH_HTML_TAG_ACRONYM,    /**<Not supported.*/
        NH_HTML_TAG_ADDRESS,    /**<Not supported.*/
        NH_HTML_TAG_APPLET,     /**<Not supported.*/
        NH_HTML_TAG_AREA,       /**<Not supported.*/
        NH_HTML_TAG_ARTICLE,    /**<Not supported.*/
        NH_HTML_TAG_ASIDE,      /**<Not supported.*/
        NH_HTML_TAG_AUDIO,      /**<Not supported.*/
        NH_HTML_TAG_B,          /**<Not supported.*/
        NH_HTML_TAG_BASE,       /**<Not supported.*/
        NH_HTML_TAG_BASEFONT,   /**<Not supported.*/
        NH_HTML_TAG_BDI,        /**<Not supported.*/
        NH_HTML_TAG_BDO,        /**<Not supported.*/
        NH_HTML_TAG_BIG,        /**<Not supported.*/
        NH_HTML_TAG_BLOCKQUOTE, /**<Not supported.*/
        NH_HTML_TAG_BODY,       /**<Supported.*/
        NH_HTML_TAG_BR,         /**<Not supported.*/
        NH_HTML_TAG_BUTTON,     /**<Not supported.*/
        NH_HTML_TAG_CANVAS,     /**<Not supported.*/
        NH_HTML_TAG_CAPTION,    /**<Not supported.*/
        NH_HTML_TAG_CENTER,     /**<Not supported.*/
        NH_HTML_TAG_CITE,       /**<Not supported.*/
        NH_HTML_TAG_CODE,       /**<Not supported.*/
        NH_HTML_TAG_COL,        /**<Not supported.*/
        NH_HTML_TAG_COLGROUP,   /**<Not supported.*/
        NH_HTML_TAG_DATA,       /**<Not supported.*/
        NH_HTML_TAG_DATALIST,   /**<Not supported.*/
        NH_HTML_TAG_DD,         /**<Not supported.*/
        NH_HTML_TAG_DEL,        /**<Not supported.*/
        NH_HTML_TAG_DETAILS,    /**<Not supported.*/
        NH_HTML_TAG_DFN,        /**<Not supported.*/
        NH_HTML_TAG_DIALOG,     /**<Not supported.*/
        NH_HTML_TAG_DIR,        /**<Not supported.*/
        NH_HTML_TAG_DIV,        /**<Not supported.*/
        NH_HTML_TAG_DL,         /**<Not supported.*/
        NH_HTML_TAG_DT,         /**<Not supported.*/
        NH_HTML_TAG_EM,         /**<Not supported.*/
        NH_HTML_TAG_EMBED,      /**<Not supported.*/
        NH_HTML_TAG_FIELDSET,   /**<Not supported.*/
        NH_HTML_TAG_FIGCAPTION, /**<Not supported.*/
        NH_HTML_TAG_FIGURE,     /**<Not supported.*/
        NH_HTML_TAG_FONT,       /**<Not supported.*/
        NH_HTML_TAG_FOOTER,     /**<Not supported.*/
        NH_HTML_TAG_FORM,       /**<Not supported.*/
        NH_HTML_TAG_FRAME,      /**<Not supported.*/
        NH_HTML_TAG_FRAMESET,   /**<Not supported.*/
        NH_HTML_TAG_H1,         /**<Partially supported.*/ 
        NH_HTML_TAG_H2,         /**<Partially supported.*/
        NH_HTML_TAG_H3,         /**<Partially supported.*/
        NH_HTML_TAG_H4,         /**<Partially supported.*/
        NH_HTML_TAG_H5,         /**<Partially supported.*/
        NH_HTML_TAG_H6,         /**<Partially supported.*/
        NH_HTML_TAG_HEAD,       /**<Supported.*/
        NH_HTML_TAG_HEADER,     /**<Not supported.*/ 
        NH_HTML_TAG_HR,         /**<Not supported.*/ 
        NH_HTML_TAG_HTML,       /**<Supported.*/
        NH_HTML_TAG_I,          /**<Not supported.*/
        NH_HTML_TAG_IFRAME,     /**<Not supported.*/
        NH_HTML_TAG_IMG,        /**<Partially supported.*/
        NH_HTML_TAG_INPUT,      /**<Not supported.*/ 
        NH_HTML_TAG_INS,        /**<Not supported.*/
        NH_HTML_TAG_KBD,        /**<Not supported.*/
        NH_HTML_TAG_LABEL,      /**<Not supported.*/
        NH_HTML_TAG_LEGEND,     /**<Not supported.*/
        NH_HTML_TAG_LI,         /**<Partially supported.*/ 
        NH_HTML_TAG_LINK,       /**<Partially supported.*/
        NH_HTML_TAG_MAIN,       /**<Not supported.*/ 
        NH_HTML_TAG_MAP,        /**<Not supported.*/
        NH_HTML_TAG_MARK,       /**<Not supported.*/
        NH_HTML_TAG_META,       /**<Not supported.*/
        NH_HTML_TAG_METER,      /**<Not supported.*/
        NH_HTML_TAG_NAV,        /**<Not supported.*/
        NH_HTML_TAG_NOFRAMES,   /**<Not supported.*/
        NH_HTML_TAG_NOSCRIPT,   /**<Not supported.*/
        NH_HTML_TAG_OBJECT,     /**<Not supported.*/
        NH_HTML_TAG_OL,         /**<Not supported.*/
        NH_HTML_TAG_OPTGROUP,   /**<Not supported.*/
        NH_HTML_TAG_OPTION,     /**<Partially supported.*/
        NH_HTML_TAG_OUTPUT,     /**<Not supported.*/
        NH_HTML_TAG_P,          /**<Partially supported.*/
        NH_HTML_TAG_PARAM,      /**<Not supported.*/ 
        NH_HTML_TAG_PICTURE,    /**<Not supported.*/
        NH_HTML_TAG_PRE,        /**<Not supported.*/
        NH_HTML_TAG_PROGRESS,   /**<Not supported.*/
        NH_HTML_TAG_Q,          /**<Not supported.*/
        NH_HTML_TAG_RP,         /**<Not supported.*/
        NH_HTML_TAG_RT,         /**<Not supported.*/
        NH_HTML_TAG_RUBY,       /**<Not supported.*/
        NH_HTML_TAG_S,          /**<Not supported.*/
        NH_HTML_TAG_SAMP,       /**<Not supported.*/
        NH_HTML_TAG_SCRIPT,     /**<Not supported.*/
        NH_HTML_TAG_SECTION,    /**<Not supported.*/
        NH_HTML_TAG_SELECT,     /**<Partially supported.*/
        NH_HTML_TAG_SMALL,      /**<Not supported.*/
        NH_HTML_TAG_SOURCE,     /**<Not supported.*/
        NH_HTML_TAG_SPAN,       /**<Not supported.*/
        NH_HTML_TAG_STRIKE,     /**<Not supported.*/
        NH_HTML_TAG_STRONG,     /**<Not supported.*/
        NH_HTML_TAG_STYLE,      /**<Not supported.*/
        NH_HTML_TAG_SUB,        /**<Not supported.*/
        NH_HTML_TAG_SUMMARY,    /**<Not supported.*/
        NH_HTML_TAG_SUP,        /**<Not supported.*/
        NH_HTML_TAG_SVG,        /**<Not supported.*/
        NH_HTML_TAG_TABLE,      /**<Not supported.*/
        NH_HTML_TAG_TBODY,      /**<Not supported.*/
        NH_HTML_TAG_TD,         /**<Not supported.*/
        NH_HTML_TAG_TEMPLATE,   /**<Not supported.*/
        NH_HTML_TAG_TEXTAREA,   /**<Not supported.*/
        NH_HTML_TAG_TFOOT,      /**<Not supported.*/
        NH_HTML_TAG_TH,         /**<Not supported.*/
        NH_HTML_TAG_THEAD,      /**<Not supported.*/
        NH_HTML_TAG_TIME,       /**<Not supported.*/
        NH_HTML_TAG_TITLE,      /**<Not supported.*/
        NH_HTML_TAG_TR,         /**<Not supported.*/
        NH_HTML_TAG_TRACK,      /**<Not supported.*/
        NH_HTML_TAG_TT,         /**<Not supported.*/
        NH_HTML_TAG_U,          /**<Not supported.*/
        NH_HTML_TAG_UL,         /**<Partially supported.*/
        NH_HTML_TAG_VAR,        /**<Not supported.*/ 
        NH_HTML_TAG_VIDEO,      /**<Not supported.*/
        NH_HTML_TAG_WBR,        /**<Not supported.*/
        NH_HTML_TAG_MENU,
        NH_HTML_TAG_HGROUP,
        NH_HTML_TAG_MATH,
        NH_HTML_TAG_TEXT,
        NH_HTML_TAG_UNDEFINED,  /**<Not supported.*/
    } NH_HTML_TAG;

/** @} */

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    /**
     * Represents a HTML document node.
     */
    typedef struct Nh_HTML_Node {

        NH_HTML_TAG tag;                   /**<The node tag.*/ 

        Nh_List Attributes;                /**<Holds HTML attributes.*/
        Nh_List Properties;                /**<Holds structs of type @ref Nh_CSS_GenericProperty . They are used to compute the final properties in @ref Nh_CSS_Properties. Properties are added by associating CSS using @ref Nh_CSS_associateSheets .*/

        char *text_p;                      /**<Text of this node.*/

        struct Computed {
            Nh_HTML_Text Text;
            Nh_CSS_Box Margin;               
            Nh_HTML_Attributes Attributes;    
            NH_CSS_Properties Properties;     
        } Computed;                        /**<Holds computed node data.*/

        struct Vulkan {
            Nh_List Buffers;           
            Nh_List Uniforms;          
            Nh_List Descriptors;
        } Vulkan;                          /**<Holds Vulkan API data.*/

        struct Nh_HTML_Node *Parent_p;     /**<Pointer to the node parent.*/

        struct Children {
            Nh_List Unformatted;           /**<List of unformatted node children.*/
            Nh_List Formatted;             /**<List of formatted node children.*/
        } Children;                        /**<Contains children of the node.*/

    } Nh_HTML_Node;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    NH_RESULT Nh_HTML_initNode(
        Nh_HTML_Node *Node_p, Nh_HTML_Node *Parent_p, NH_HTML_TAG tag
    );

    NH_RESULT Nh_HTML_computeNode(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_BOOL text
    ); 

    NH_RESULT Nh_HTML_recomputeNode(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_BOOL text
    );

    void Nh_HTML_destroyNode(
        Nh_HTML_Node *Node_p, NH_BOOL destroyTextData
    );

    NH_RESULT Nh_HTML_replaceChild(
        Nh_Tab *Tab_p, Nh_HTML_Node *Replace_p, Nh_HTML_Node *Replacement_p
    );

    NH_RESULT Nh_HTML_replaceChildren(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Node *Replace_p
    );

    NH_RESULT Nh_HTML_prependChild(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Node *Prepend_p
    );

    Nh_HTML_Node *Nh_HTML_getNode(
        Nh_Tab *Tab_p, NH_HTML_TAG tag, NH_BOOL unformatted
    );

    NH_BOOL Nh_HTML_isMetaNode(
        Nh_HTML_Node *Node_p
    );

/** @} */

#endif
