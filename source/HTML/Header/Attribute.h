#ifndef NH_HTML_ATTRIBUTES_H
#define NH_HTML_ATTRIBUTES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Graphics/Header/Texture.h"
#include "../../Core/Header/List.h"
#include "../../API/Header/Netzhaut.h"

typedef struct Nh_Tab Nh_Tab;
typedef struct Nh_HTML_Node Nh_HTML_Node;

#endif

/** @addtogroup HTMLData Data
 *  \ingroup HTML
 *  @{
 */

    extern const char *NH_HTML_ATTRIBUTES_PP[];
    extern size_t NH_HTML_ATTRIBUTES_PP_COUNT;

/** @} */

/** @addtogroup HTMLEnums Enums
 *  \ingroup HTML
 *  @{
 */

    typedef enum NH_HTML_ATTRIBUTE {
        NH_HTML_ATTRIBUTE_ACCEPT,              /*<Not supported.*/ 
        NH_HTML_ATTRIBUTE_ACCEPT_CHARSET,      /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ACCESS_KEY,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ACTION,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ALIGN,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ALT,                 /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ASYNC,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_AUTO_COMPLETE,       /*<Not supported.*/
        NH_HTML_ATTRIBUTE_AUTO_FOCUS,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_AUTO_PLAY,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_BG_COLOR,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_BORDER,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_CHARSET,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_CHECKED,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_CITE,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_CLASS,               /*<Supported.*/
        NH_HTML_ATTRIBUTE_COLOR,               /*<Not supported.*/ 
        NH_HTML_ATTRIBUTE_COLS,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_COLSPAN,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_CONTENT,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_CONTENT_EDITABLE,    /*<Not supported.*/
        NH_HTML_ATTRIBUTE_CONTROLS,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_COORDS,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DATA,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DATA_,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DATETIME,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DEFAULT,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DEFER,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DIR,                 /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DIR_NAME,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DISABLED,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DOWNLOAD,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DRAGGABLE,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_DROPZONE,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ENCTYPE,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_FOR,                 /*<Not supported.*/
        NH_HTML_ATTRIBUTE_FORM,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_FORM_ACTION,         /*<Not supported.*/
        NH_HTML_ATTRIBUTE_HEADERS,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_HEIGHT,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_HIDDEN,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_HIGH,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_HREF,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_HREF_LANG,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_HTTP_EQUIV,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ID,                  /*<Supported.*/
        NH_HTML_ATTRIBUTE_ISMAP,               /*<Not supported.*/ 
        NH_HTML_ATTRIBUTE_KIND,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_LABEL,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_LANG,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_LIST,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_LOOP,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_LOW,                 /*<Not supported.*/
        NH_HTML_ATTRIBUTE_MAX,                 /*<Not supported.*/
        NH_HTML_ATTRIBUTE_MAX_LENGTH,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_MEDIA,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_METHOD,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_MIN,                 /*<Not supported.*/
        NH_HTML_ATTRIBUTE_MULTIPLE,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_MUTED,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_NAME,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_NO_VALIDATE,         /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_ABORT,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_AFTER_PRINT,      /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_BEFORE_PRINT,     /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_BEFORE_UNLOAD,    /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_BLUR,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_CAN_PLAY,         /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_CAN_PLAYTHROUGH,  /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_CHANGE,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_CLICK,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_CONTEXTMENU,      /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_COPY,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_CUECHANGE,        /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_CUT,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_DBLCLICK,         /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_DRAG,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_DRAG_END,         /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_DRAG_ENTER,       /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_DRAG_LEAVE,       /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_DRAG_OVER,        /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_DRAG_START,       /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_DROP,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_DURATION_CHANGE,  /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_EMPTIED,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_ENDED,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_ERROR,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_FOCUS,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_HASH_CHANGE,      /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_INPUT,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_INVALID,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_KEY_DOWN,         /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_KEY_PRESS,        /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_KEY_UP,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_LOAD,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_LOADED_DATA,      /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_LOADED_META_DATA, /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_LOAD_START,       /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_MOUSE_DOWN,       /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_MOUSE_MOVE,       /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_MOUSE_OUT,        /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_MOUSE_OVER,       /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_MOUSE_UP,         /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_MOUSE_WHEEL,      /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_OFFLINE,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_ONLINE,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_PAGE_HIDE,        /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_PAGE_SHOW,        /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_PASTE,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_PAUSE,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_PLAY,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_PLAYING,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_POPSTATE,         /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_PROGRESS,         /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_RATE_CHANGE,      /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_RESET,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_RESIZE,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_SCROLL,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_SEARCH,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_SEEKED,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_SEEKING,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_SELECT,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_STALLED,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_STORAGE,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_SUBMIT,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_SUSPEND,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_TIME_UPDATE,      /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_TOGGLE,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_UNLOAD,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_VOLUME_CHANGE,    /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_WAITING,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ON_WHEEL,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_OPEN,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_OPTIMUM,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_PATTERN,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_PLACEHOLDER,         /*<Not supported.*/
        NH_HTML_ATTRIBUTE_POSTER,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_PRELOAD,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_READONLY,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_REL,                 /*<Not supported.*/
        NH_HTML_ATTRIBUTE_REQUIRED,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_REVERSED,            /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ROWS,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_ROWSPAN,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_SANDBOX,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_SCOPE,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_SELECTED,            /*<Partially supported.*/
        NH_HTML_ATTRIBUTE_SHAPE,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_SIZE_,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_SIZES,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_SPAN,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_SPELLCHECK,          /*<Not supported.*/
        NH_HTML_ATTRIBUTE_SRC,                 /*<Partially supported.*/
        NH_HTML_ATTRIBUTE_SRCDOC,              /*<Not supported.*/ 
        NH_HTML_ATTRIBUTE_SRCLANG,             /*<Not supported.*/
        NH_HTML_ATTRIBUTE_SRCSET,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_START,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_STEP,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_STYLE,               /*<Supported.*/
        NH_HTML_ATTRIBUTE_TABINDEX,            /*<Not supported.*/ 
        NH_HTML_ATTRIBUTE_TARGET,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_TITLE,               /*<Not supported.*/
        NH_HTML_ATTRIBUTE_TRANSLATE,           /*<Not supported.*/
        NH_HTML_ATTRIBUTE_TYPE,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_USEMAP,              /*<Not supported.*/
        NH_HTML_ATTRIBUTE_VALUE,               /*<Partially supported.*/
        NH_HTML_ATTRIBUTE_WIDTH,               /*<Not supported.*/ 
        NH_HTML_ATTRIBUTE_WRAP,                /*<Not supported.*/
        NH_HTML_ATTRIBUTE_LINK,
        NH_HTML_ATTRIBUTE_ALINK,
        NH_HTML_ATTRIBUTE_VLINK,
        NH_HTML_ATTRIBUTE_COUNT,
    } NH_HTML_ATTRIBUTE;

    typedef enum NH_HTML_TYPE {
        NH_HTML_TYPE_BUTTON,
        NH_HTML_TYPE_CHECKBOX,
        NH_HTML_TYPE_COLOR,
        NH_HTML_TYPE_DATE,
        NH_HTML_TYPE_DATETIME,
        NH_HTML_TYPE_DATETIME_LOCAL,
        NH_HTML_TYPE_EMAIL,
        NH_HTML_TYPE_FILE,
        NH_HTML_TYPE_HIDDEN,
        NH_HTML_TYPE_IMAGE,
        NH_HTML_TYPE_MONTH,
        NH_HTML_TYPE_NUMBER,
        NH_HTML_TYPE_PASSWORD,
        NH_HTML_TYPE_RADIO,
        NH_HTML_TYPE_RANGE,
        NH_HTML_TYPE_RESET,
        NH_HTML_TYPE_SEARCH,
        NH_HTML_TYPE_SUBMIT,
        NH_HTML_TYPE_TEL,
        NH_HTML_TYPE_TEXT,
        NH_HTML_TYPE_TIME,
        NH_HTML_TYPE_URL,
        NH_HTML_TYPE_WEEK,
    } NH_HTML_TYPE;

/** @} */

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    typedef struct Nh_HTML_Attribute {
        NH_HTML_ATTRIBUTE type; 
        char *value_p;
    } Nh_HTML_Attribute;
    
    typedef struct Nh_HTML_Attributes {
        char *onClick_p;           
        char *id_p;       
        char *class_p;
        Nh_Gfx_Texture *Texture_p; 
        NH_HTML_TYPE type;        
        bool selected;
        char *value_p;
    } Nh_HTML_Attributes;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    NH_RESULT Nh_HTML_computeAttributes(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );

    NH_RESULT Nh_HTML_handleAttributeChange(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_HTML_ATTRIBUTE type
    );

    void Nh_HTML_getAttributes(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Attribute *Attributes_pp[NH_HTML_ATTRIBUTE_COUNT]
    );

    NH_RESULT Nh_HTML_addAttribute(
        Nh_List *Attributes_p, NH_HTML_ATTRIBUTE type, char *value_p
    );
    
    Nh_HTML_Attribute *Nh_HTML_getAttribute(
        Nh_List *Attributes_p, int index
    );

    Nh_HTML_Attribute *Nh_HTML_getAttributeFromType(
        Nh_List *Attributes_p, NH_HTML_ATTRIBUTE type
    );

    void Nh_HTML_removeAttribute(
       Nh_List *Attributes_p, NH_HTML_ATTRIBUTE type
    );
  
    void Nh_HTML_destroyAttributes(
        Nh_List *Attributes_p
    );

    const char** Nh_HTML_getAttributeNames(
        size_t *size_p
    );
    
    const char* Nh_HTML_getAttributeName(
        int attribute
    );

/** @} */

#endif 
