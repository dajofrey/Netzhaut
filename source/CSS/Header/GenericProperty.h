#ifndef NH_CSS_GENERIC_PROPERTY_H
#define NH_CSS_GENERIC_PROPERTY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Data.h"
#include "Selector.h"
#include "Sheet.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_Tab Nh_Tab;

#endif

/** @addtogroup CSSData Data
 *  \ingroup CSS
 *  @{
 */
    extern const char *NH_CSS_PROPERTIES_PP[];
    extern size_t NH_CSS_PROPERTIES_PP_COUNT;

/** @} */

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_PROPERTY {
        NH_CSS_PROPERTY__WEBKIT_LINE_CLAMP,          /**<*/
        NH_CSS_PROPERTY_ALIGN_CONTENT,               /**<*/    
        NH_CSS_PROPERTY_ALIGN_ITEMS,                 /**<*/             	
        NH_CSS_PROPERTY_ALIGN_SELF,                  /**<*/            
        NH_CSS_PROPERTY_ALL,                         /**<*/    	
        NH_CSS_PROPERTY_ANIMATION,                   /**<*/  	               
        NH_CSS_PROPERTY_ANIMATION_DELAY,             /**<*/ 
        NH_CSS_PROPERTY_ANIMATION_DIRECTION,         /**<*/
        NH_CSS_PROPERTY_ANIMATION_DURATION,          /**<*/      
        NH_CSS_PROPERTY_ANIMATION_FILL_MODE,         /**<*/   
        NH_CSS_PROPERTY_ANIMATION_ITERATION_COUNT,   /**<*/
        NH_CSS_PROPERTY_ANIMATION_NAME,              /**<*/ 
        NH_CSS_PROPERTY_ANIMATION_PLAY_STATE,        /**<*/
        NH_CSS_PROPERTY_ANIMATION_TIMING_FUNCTION,   /**<*/
        NH_CSS_PROPERTY_BACKFACE_VISIBILITY,         /**<*/  
        NH_CSS_PROPERTY_BACKGROUND,                  /**<*/  
        NH_CSS_PROPERTY_BACKGROUND_ATTACHMENT,       /**<*/
        NH_CSS_PROPERTY_BACKGROUND_BLEND_MODE,       /**<*/
        NH_CSS_PROPERTY_BACKGROUND_CLIP,             /**<*/
        NH_CSS_PROPERTY_BACKGROUND_COLOR,            /**<*/
        NH_CSS_PROPERTY_BACKGROUND_IMAGE,            /**<*/ 
        NH_CSS_PROPERTY_BACKGROUND_ORIGIN,           /**<*/
        NH_CSS_PROPERTY_BACKGROUND_POSITION,         /**<*/
        NH_CSS_PROPERTY_BACKGROUND_REPEAT,           /**<*/
        NH_CSS_PROPERTY_BACKGROUND_SIZE,             /**<*/
        NH_CSS_PROPERTY_BLOCK_SIZE,                  /**<*/
        NH_CSS_PROPERTY_BORDER,                      /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK,                /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK_COLOR,          /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK_END,            /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK_END_COLOR,      /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK_END_STYLE,      /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK_END_WIDTH,      /**<*/ 
        NH_CSS_PROPERTY_BORDER_BLOCK_START,          /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK_START_COLOR,    /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK_START_STYLE,    /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK_START_WIDTH,    /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK_STYLE,          /**<*/
        NH_CSS_PROPERTY_BORDER_BLOCK_WIDTH,          /**<*/
        NH_CSS_PROPERTY_BORDER_BOTTOM,               /**<*/
        NH_CSS_PROPERTY_BORDER_BOTTOM_COLOR,         /**<*/
        NH_CSS_PROPERTY_BORDER_BOTTOM_LEFT_RADIUS,   /**<*/ 
        NH_CSS_PROPERTY_BORDER_BOTTOM_RIGHT_RADIUS,  /**<*/
        NH_CSS_PROPERTY_BORDER_BOTTOM_STYLE,         /**<*/
        NH_CSS_PROPERTY_BORDER_BOTTOM_WIDTH,         /**<*/
        NH_CSS_PROPERTY_BORDER_COLLAPSE,             /**<*/
        NH_CSS_PROPERTY_BORDER_COLOR,                /**<*/
        NH_CSS_PROPERTY_BORDER_END_END_RADIUS,       /**<*/
        NH_CSS_PROPERTY_BORDER_END_START_RADIUS,     /**<*/
        NH_CSS_PROPERTY_BORDER_IMAGE,                /**<*/
        NH_CSS_PROPERTY_BORDER_IMAGE_OUTSET,         /**<*/
        NH_CSS_PROPERTY_BORDER_IMAGE_REPEAT,         /**<*/
        NH_CSS_PROPERTY_BORDER_IMAGE_SLICE,          /**<*/
        NH_CSS_PROPERTY_BORDER_IMAGE_SOURCE,         /**<*/
        NH_CSS_PROPERTY_BORDER_IMAGE_WIDTH,          /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE,               /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_COLOR,         /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_END,           /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_END_COLOR,     /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_END_STYLE,     /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_END_WIDTH,     /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_START,         /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_START_COLOR,   /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_START_STYLE,   /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_START_WIDTH,   /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_STYLE,         /**<*/
        NH_CSS_PROPERTY_BORDER_INLINE_WIDTH,         /**<*/
        NH_CSS_PROPERTY_BORDER_LEFT,                 /**<*/
        NH_CSS_PROPERTY_BORDER_LEFT_COLOR,           /**<*/
        NH_CSS_PROPERTY_BORDER_LEFT_STYLE,           /**<*/ 
        NH_CSS_PROPERTY_BORDER_LEFT_WIDTH,           /**<*/
        NH_CSS_PROPERTY_BORDER_RADIUS,               /**<*/
        NH_CSS_PROPERTY_BORDER_RIGHT,                /**<*/
        NH_CSS_PROPERTY_BORDER_RIGHT_COLOR,          /**<*/
        NH_CSS_PROPERTY_BORDER_RIGHT_STYLE,          /**<*/ 
        NH_CSS_PROPERTY_BORDER_RIGHT_WIDTH,          /**<*/
        NH_CSS_PROPERTY_BORDER_SPACING,              /**<*/
        NH_CSS_PROPERTY_BORDER_START_END_RADIUS,     /**<*/
        NH_CSS_PROPERTY_BORDER_START_START_RADIUS,   /**<*/
        NH_CSS_PROPERTY_BORDER_STYLE,                /**<*/
        NH_CSS_PROPERTY_BORDER_TOP,                  /**<*/
        NH_CSS_PROPERTY_BORDER_TOP_COLOR,            /**<*/
        NH_CSS_PROPERTY_BORDER_TOP_LEFT_RADIUS,      /**<*/ 
        NH_CSS_PROPERTY_BORDER_TOP_RIGHT_RADIUS,     /**<*/
        NH_CSS_PROPERTY_BORDER_TOP_STYLE,            /**<*/
        NH_CSS_PROPERTY_BORDER_TOP_WIDTH,            /**<*/
        NH_CSS_PROPERTY_BORDER_WIDTH,                /**<*/
        NH_CSS_PROPERTY_BOTTOM,                      /**<*/
        NH_CSS_PROPERTY_BOX_DECORATION_BREAK,        /**<*/
        NH_CSS_PROPERTY_BOX_SHADOW,                  /**<*/
        NH_CSS_PROPERTY_BOX_SIZING,                  /**<*/
        NH_CSS_PROPERTY_BREAK_AFTER,                 /**<*/
        NH_CSS_PROPERTY_BREAK_BEFORE,                /**<*/
        NH_CSS_PROPERTY_BREAK_INSIDE,                /**<*/
        NH_CSS_PROPERTY_CAPTION_SIDE,                /**<*/
        NH_CSS_PROPERTY_CARET_COLOR,                 /**<*/
        NH_CSS_PROPERTY_CLEAR,                       /**<*/
        NH_CSS_PROPERTY_CLIP,                        /**<*/
        NH_CSS_PROPERTY_CLIP_PATH,                   /**<*/
        NH_CSS_PROPERTY_COLOR,                       /**<*/
        NH_CSS_PROPERTY_COLOR_ADJUST,                /**<*/ 
        NH_CSS_PROPERTY_COLUMN_COUNT,                /**<*/
        NH_CSS_PROPERTY_COLUMN_FILL,                 /**<*/
        NH_CSS_PROPERTY_COLUMN_GAP,                  /**<*/
        NH_CSS_PROPERTY_COLUMN_RULE,                 /**<*/
        NH_CSS_PROPERTY_COLUMN_RULE_COLOR,           /**<*/
        NH_CSS_PROPERTY_COLUMN_RULE_STYLE,           /**<*/
        NH_CSS_PROPERTY_COLUMN_RULE_WIDTH,           /**<*/
        NH_CSS_PROPERTY_COLUMN_SPAN,                 /**<*/
        NH_CSS_PROPERTY_COLUMN_WIDTH,                /**<*/
        NH_CSS_PROPERTY_COLUMNS,                     /**<*/
        NH_CSS_PROPERTY_CONTENT,                     /**<*/
        NH_CSS_PROPERTY_COUNTER_INCREMENT,           /**<*/
        NH_CSS_PROPERTY_COUNTER_RESET,               /**<*/
        NH_CSS_PROPERTY_COUNTER_SET,                 /**<*/
        NH_CSS_PROPERTY_CURSOR,                      /**<*/
        NH_CSS_PROPERTY_DIRECTION,                   /**<*/
        NH_CSS_PROPERTY_DISPLAY,                     /**<*/
        NH_CSS_PROPERTY_EMPTY_CELLS,                 /**<*/
        NH_CSS_PROPERTY_FILTER,                      /**<*/
        NH_CSS_PROPERTY_FLEX,                        /**<*/
        NH_CSS_PROPERTY_FLEX_BASIS,                  /**<*/
        NH_CSS_PROPERTY_FLEX_DIRECTION,              /**<*/
        NH_CSS_PROPERTY_FLEX_FLOW,                   /**<*/
        NH_CSS_PROPERTY_FLEX_GROW,                   /**<*/
        NH_CSS_PROPERTY_FLEX_SHRINK,                 /**<*/
        NH_CSS_PROPERTY_FLEX_WRAP,                   /**<*/
        NH_CSS_PROPERTY_FLOAT,                       /**<*/
        NH_CSS_PROPERTY_FONT,                        /**<*/
        NH_CSS_PROPERTY_FONT_FAMILY,                 /**<*/
        NH_CSS_PROPERTY_FONT_FEATURE_SETTINGS,       /**<*/ 
        NH_CSS_PROPERTY_FONT_KERNING,                /**<*/
        NH_CSS_PROPERTY_FONT_LANGUAGE_OVERRIDE,      /**<*/
        NH_CSS_PROPERTY_FONT_OPTICAL_SIZING,         /**<*/
        NH_CSS_PROPERTY_FONT_SIZE,                   /**<*/
        NH_CSS_PROPERTY_FONT_SIZE_ADJUST,            /**<*/ 
        NH_CSS_PROPERTY_FONT_STRETCH,                /**<*/
        NH_CSS_PROPERTY_FONT_STYLE,                  /**<*/
        NH_CSS_PROPERTY_FONT_SYNTHESIS,              /**<*/
        NH_CSS_PROPERTY_FONT_VARIANT,                /**<*/
        NH_CSS_PROPERTY_FONT_VARIANT_ALTERNATES,     /**<*/
        NH_CSS_PROPERTY_FONT_VARIANT_CAPS,           /**<*/
        NH_CSS_PROPERTY_FONT_VARIANT_EAST_ASIAN,     /**<*/
        NH_CSS_PROPERTY_FONT_VARIANT_LIGATURES,      /**<*/
        NH_CSS_PROPERTY_FONT_VARIANT_NUMERIC,        /**<*/
        NH_CSS_PROPERTY_FONT_VARIANT_POSITION,       /**<*/
        NH_CSS_PROPERTY_FONT_WEIGHT,                 /**<*/
        NH_CSS_PROPERTY_GAP,                         /**<*/  
        NH_CSS_PROPERTY_GRID,                        /**<*/
        NH_CSS_PROPERTY_GRID_AREA,                   /**<*/
        NH_CSS_PROPERTY_GRID_AUTO_COLUMNS,           /**<*/
        NH_CSS_PROPERTY_GRID_AUTO_FLOW,              /**<*/
        NH_CSS_PROPERTY_GRID_AUTO_ROWS,              /**<*/
        NH_CSS_PROPERTY_GRID_COLUMN,                 /**<*/
        NH_CSS_PROPERTY_GRID_COLUMN_END,             /**<*/
        NH_CSS_PROPERTY_GRID_COLUMN_GAP,             /**<*/
        NH_CSS_PROPERTY_GRID_COLUMN_START,           /**<*/
        NH_CSS_PROPERTY_GRID_GAP,                    /**<*/
        NH_CSS_PROPERTY_GRID_ROW,                    /**<*/
        NH_CSS_PROPERTY_GRID_ROW_END,                /**<*/
        NH_CSS_PROPERTY_GRID_ROW_GAP,                /**<*/
        NH_CSS_PROPERTY_GRID_ROW_START,              /**<*/
        NH_CSS_PROPERTY_GRID_TEMPLATE,               /**<*/
        NH_CSS_PROPERTY_GRID_TEMPLATE_AREAS,         /**<*/
        NH_CSS_PROPERTY_GRID_TEMPLATE_COLUMNS,       /**<*/
        NH_CSS_PROPERTY_GRID_TEMPLATE_ROWS,          /**<*/
        NH_CSS_PROPERTY_HANGING_PUNCTUATION,         /**<*/
        NH_CSS_PROPERTY_HEIGHT,                      /**<*/
        NH_CSS_PROPERTY_HYPHENS,                     /**<*/
        NH_CSS_PROPERTY_IMAGE_RENDERING,             /**<*/
        NH_CSS_PROPERTY_INLINE_SIZE,                 /**<*/
        NH_CSS_PROPERTY_INSET,                       /**<*/
        NH_CSS_PROPERTY_INSET_BLOCK,                 /**<*/
        NH_CSS_PROPERTY_INSET_BLOCK_END,             /**<*/
        NH_CSS_PROPERTY_INSET_BLOCK_START,           /**<*/
        NH_CSS_PROPERTY_INSET_INLINE,                /**<*/
        NH_CSS_PROPERTY_INSET_INLINE_END,            /**<*/
        NH_CSS_PROPERTY_INSET_INLINE_START,          /**<*/
        NH_CSS_PROPERTY_ISOLATION,                   /**<*/
        NH_CSS_PROPERTY_JUSTIFY_CONTENT,             /**<*/
        NH_CSS_PROPERTY_JUSTIFY_ITEMS,               /**<*/
        NH_CSS_PROPERTY_JUSTIFY_SELF,                /**<*/
        NH_CSS_PROPERTY_LEFT,                        /**<*/
        NH_CSS_PROPERTY_LETTER_SPACING,              /**<*/
        NH_CSS_PROPERTY_LINE_BREAK,                  /**<*/
        NH_CSS_PROPERTY_LINE_HEIGHT,                 /**<*/
        NH_CSS_PROPERTY_LIST_STYLE,                  /**<*/
        NH_CSS_PROPERTY_LIST_STYLE_IMAGE,            /**<*/
        NH_CSS_PROPERTY_LIST_STYLE_POSITION,         /**<*/
        NH_CSS_PROPERTY_LIST_STYLE_TYPE,             /**<*/ 
        NH_CSS_PROPERTY_MARGIN,                      /**<*/
        NH_CSS_PROPERTY_MARGIN_BLOCK,                /**<*/
        NH_CSS_PROPERTY_MARGIN_BLOCK_END,            /**<*/
        NH_CSS_PROPERTY_MARGIN_BLOCK_START,          /**<*/
        NH_CSS_PROPERTY_MARGIN_BOTTOM,               /**<*/ 
        NH_CSS_PROPERTY_MARGIN_INLINE,               /**<*/
        NH_CSS_PROPERTY_MARGIN_INLINE_END,           /**<*/
        NH_CSS_PROPERTY_MARGIN_INLINE_START,         /**<*/
        NH_CSS_PROPERTY_MARGIN_LEFT,                 /**<*/
        NH_CSS_PROPERTY_MARGIN_RIGHT,                /**<*/
        NH_CSS_PROPERTY_MARGIN_TOP,                  /**<*/
        NH_CSS_PROPERTY_MASK,                        /**<*/
        NH_CSS_PROPERTY_MASK_CLIP,                   /**<*/
        NH_CSS_PROPERTY_MASK_COMPOSITE,              /**<*/
        NH_CSS_PROPERTY_MASK_IMAGE,                  /**<*/
        NH_CSS_PROPERTY_MASK_MODE,                   /**<*/
        NH_CSS_PROPERTY_MASK_ORIGIN,                 /**<*/
        NH_CSS_PROPERTY_MASK_POSITION,               /**<*/
        NH_CSS_PROPERTY_MASK_REPEAT,                 /**<*/
        NH_CSS_PROPERTY_MASK_SIZE,                   /**<*/
        NH_CSS_PROPERTY_MASK_TYPE,                   /**<*/
        NH_CSS_PROPERTY_MAX_HEIGHT,                  /**<*/
        NH_CSS_PROPERTY_MAX_WIDTH,                   /**<*/
        NH_CSS_PROPERTY_MIN_BLOCK_SIZE,              /**<*/
        NH_CSS_PROPERTY_MIN_HEIGHT,                  /**<*/
        NH_CSS_PROPERTY_MIN_INLINE_SIZE,             /**<*/
        NH_CSS_PROPERTY_MIN_WIDTH,                   /**<*/
        NH_CSS_PROPERTY_MIX_BLEND_MODE,              /**<*/
        NH_CSS_PROPERTY_OBJECT_FIT,                  /**<*/
        NH_CSS_PROPERTY_OBJECT_POSITION,             /**<*/
        NH_CSS_PROPERTY_OPACITY,                     /**<*/
        NH_CSS_PROPERTY_ORDER,                       /**<*/
        NH_CSS_PROPERTY_ORPHANS,                     /**<*/
        NH_CSS_PROPERTY_OUTLINE,                     /**<*/
        NH_CSS_PROPERTY_OUTLINE_COLOR,               /**<*/
        NH_CSS_PROPERTY_OUTLINE_OFFSET,              /**<*/
        NH_CSS_PROPERTY_OUTLINE_STYLE,               /**<*/
        NH_CSS_PROPERTY_OUTLINE_WIDTH,               /**<*/
        NH_CSS_PROPERTY_OVERFLOW,                    /**<*/
        NH_CSS_PROPERTY_OVERFLOW_WRAP,               /**<*/
        NH_CSS_PROPERTY_OVERFLOW_X,                  /**<*/
        NH_CSS_PROPERTY_OVERFLOW_Y,                  /**<*/
        NH_CSS_PROPERTY_PADDING,                     /**<*/
        NH_CSS_PROPERTY_PADDING_BLOCK,               /**<*/
        NH_CSS_PROPERTY_PADDING_BLOCK_END,           /**<*/
        NH_CSS_PROPERTY_PADDING_BLOCK_START,         /**<*/
        NH_CSS_PROPERTY_PADDING_BOTTOM,              /**<*/
        NH_CSS_PROPERTY_PADDING_INLINE,              /**<*/
        NH_CSS_PROPERTY_PADDING_INLINE_END,          /**<*/
        NH_CSS_PROPERTY_PADDING_INLINE_START,        /**<*/
        NH_CSS_PROPERTY_PADDING_LEFT,                /**<*/
        NH_CSS_PROPERTY_PADDING_RIGHT,               /**<*/
        NH_CSS_PROPERTY_PADDING_TOP,                 /**<*/
        NH_CSS_PROPERTY_PAGE_BREAK_AFTER,            /**<*/
        NH_CSS_PROPERTY_PAGE_BREAK_BEFORE,           /**<*/
        NH_CSS_PROPERTY_PAGE_BREAK_INSIDE,           /**<*/
        NH_CSS_PROPERTY_PERSPECTIVE,                 /**<*/
        NH_CSS_PROPERTY_PERSPECTIVE_ORIGIN,          /**<*/
        NH_CSS_PROPERTY_PLACE_CONTENT,               /**<*/
        NH_CSS_PROPERTY_PLACE_ITEMS,                 /**<*/
        NH_CSS_PROPERTY_PLACE_SELF,                  /**<*/
        NH_CSS_PROPERTY_POINTER_EVENTS,              /**<*/
        NH_CSS_PROPERTY_POSITION,                    /**<*/
        NH_CSS_PROPERTY_QUOTES,                      /**<*/
        NH_CSS_PROPERTY_RESIZE,                      /**<*/
        NH_CSS_PROPERTY_RIGHT,                       /**<*/
        NH_CSS_PROPERTY_ROTATE,                      /**<*/
        NH_CSS_PROPERTY_ROW_GAP,                     /**<*/
        NH_CSS_PROPERTY_SCALE,                       /**<*/
        NH_CSS_PROPERTY_SCROLL_BEHAVIOR,             /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN,               /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN_BLOCK,         /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN_BLOCK_END,     /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN_BLOCK_START,   /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN_BOTTOM,        /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN_INLINE,        /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN_INLINE_END,    /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN_INLINE_START,  /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN_LEFT,          /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN_RIGHT,         /**<*/
        NH_CSS_PROPERTY_SCROLL_MARGIN_TOP,           /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING,              /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING_BLOCK,        /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING_BLOCK_END,    /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING_BLOCK_START,  /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING_BOTTOM,       /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING_INLINE,       /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING_INLINE_END,   /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING_INLINE_START, /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING_LEFT,         /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING_RIGHT,        /**<*/
        NH_CSS_PROPERTY_SCROLL_PADDING_TOP,          /**<*/
        NH_CSS_PROPERTY_SCROLL_SNAP_ALIGN,           /**<*/
        NH_CSS_PROPERTY_SCROLL_SNAP_STOP,            /**<*/
        NH_CSS_PROPERTY_SCROLL_SNAP_TYPE,            /**<*/
        NH_CSS_PROPERTY_SCROLLBAR_COLOR,             /**<*/
        NH_CSS_PROPERTY_SCROLLBAR_WIDTH,             /**<*/
        NH_CSS_PROPERTY_SHAPE_IMAGE_THRESHOLD,       /**<*/
        NH_CSS_PROPERTY_SHAPE_MARGIN,                /**<*/
        NH_CSS_PROPERTY_SHAPE_OUTSIDE,               /**<*/
        NH_CSS_PROPERTY_TAB_SIZE,                    /**<*/
        NH_CSS_PROPERTY_TABLE_LAYOUT,                /**<*/
        NH_CSS_PROPERTY_TEXT_ALIGN,                  /**<*/
        NH_CSS_PROPERTY_TEXT_ALIGN_LAST,             /**<*/
        NH_CSS_PROPERTY_TEXT_COMBINE_UPRIGHT,        /**<*/
        NH_CSS_PROPERTY_TEXT_DECORATION,             /**<*/
        NH_CSS_PROPERTY_TEXT_DECORATION_COLOR,       /**<*/
        NH_CSS_PROPERTY_TEXT_DECORATION_LINE,        /**<*/
        NH_CSS_PROPERTY_TEXT_DECORATION_STYLE,       /**<*/
        NH_CSS_PROPERTY_TEXT_DECORATION_THICKNESS,   /**<*/
        NH_CSS_PROPERTY_TEXT_EMPHASIS,               /**<*/
        NH_CSS_PROPERTY_TEXT_EMPHASIS_COLOR,         /**<*/
        NH_CSS_PROPERTY_TEXT_EMPHASIS_POSITION,      /**<*/
        NH_CSS_PROPERTY_TEXT_EMPHASIS_STYLE,         /**<*/
        NH_CSS_PROPERTY_TEXT_INDENT,                 /**<*/
        NH_CSS_PROPERTY_TEXT_JUSTIFY,                /**<*/
        NH_CSS_PROPERTY_TEXT_ORIENTATION,            /**<*/
        NH_CSS_PROPERTY_TEXT_OVERFLOW,               /**<*/
        NH_CSS_PROPERTY_TEXT_RENDERING,              /**<*/
        NH_CSS_PROPERTY_TEXT_SHADOW,                 /**<*/
        NH_CSS_PROPERTY_TEXT_TRANSFORM,              /**<*/
        NH_CSS_PROPERTY_TEXT_UNDERLINE_OFFSET,       /**<*/
        NH_CSS_PROPERTY_TEXT_UNDERLINE_POSITION,     /**<*/
        NH_CSS_PROPERTY_TOP,                         /**<*/
        NH_CSS_PROPERTY_TOUCH_ACTION,                /**<*/
        NH_CSS_PROPERTY_TRANSFORM,                   /**<*/
        NH_CSS_PROPERTY_TRANSFORM_BOX,               /**<*/
        NH_CSS_PROPERTY_TRANSFORM_ORIGIN,            /**<*/
        NH_CSS_PROPERTY_TRANSFORM_STYLE,             /**<*/
        NH_CSS_PROPERTY_TRANSITION,                  /**<*/
        NH_CSS_PROPERTY_TRANSITION_DELAY,            /**<*/
        NH_CSS_PROPERTY_TRANSITION_DURATION,         /**<*/
        NH_CSS_PROPERTY_TRANSITION_PROPERTY,         /**<*/
        NH_CSS_PROPERTY_TRANSITION_TIMING_FUNCTION,  /**<*/
        NH_CSS_PROPERTY_TRANSLATE,                   /**<*/
        NH_CSS_PROPERTY_UNICODE_BIDI,                /**<*/
        NH_CSS_PROPERTY_USER_SELECT,                 /**<*/
        NH_CSS_PROPERTY_VISIBILITY,                  /**<*/
        NH_CSS_PROPERTY_WHITE_SPACE,                 /**<*/
        NH_CSS_PROPERTY_WIDOWS,                      /**<*/
        NH_CSS_PROPERTY_WIDTH,                       /**<*/
        NH_CSS_PROPERTY_WILL_CHANGE,                 /**<*/
        NH_CSS_PROPERTY_WORD_BREAK,                  /**<*/
        NH_CSS_PROPERTY_WORD_SPACING,                /**<*/
        NH_CSS_PROPERTY_WORD_WRAP,                   /**<*/
        NH_CSS_PROPERTY_WRITING_MODE,                /**<*/
        NH_CSS_PROPERTY_VERTICAL_ALIGN,              /**<*/
        NH_CSS_PROPERTY_Z_INDEX,                     /**<*/
        NH_CSS_PROPERTY_COUNT,
    } NH_CSS_PROPERTY;

/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct NH_CSS_GenericProperty {
        NH_CSS_Sheet *Sheet_p;
        NH_CSS_SELECTOR selector;
        NH_CSS_PROPERTY type;
        NH_CSS_Pseudo Pseudo;
        void *selector_p;
        char **values_pp;     
        int valueCount;      
        bool active;          
        bool update;
    } NH_CSS_GenericProperty;
    
/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_configureGenericProperty(
        Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Property_p
    );
    
    void Nh_CSS_getGenericProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Properties_pp[NH_CSS_PROPERTY_COUNT]
    );
    
    NH_RESULT Nh_CSS_addGenericProperty(
        Nh_List *Properties_p, NH_CSS_GenericProperty *Copy_p
    );
    
    NH_CSS_GenericProperty *Nh_CSS_getProperty(
        Nh_List *Properties_p, int index
    );
    
    void Nh_CSS_destroyGenericProperties(
        Nh_List *Properties_p
    );
  
    NH_RESULT Nh_CSS_deactivate(
        Nh_Tab *Tab_p, NH_CSS_PSEUDO_CLASS pseudoClass
    );
    
    NH_RESULT Nh_CSS_activate(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Property_p, NH_CSS_PSEUDO_CLASS pseudoClass
    );

    NH_RESULT Nh_CSS_activateChild(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Property_p, NH_CSS_PSEUDO_CLASS pseudoClass
    );

/** @} */

#endif 
