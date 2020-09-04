#ifndef NH_CSS_PROPERTIES_H
#define NH_CSS_PROPERTIES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Data.h"
#include "Background.h"
#include "Selector.h"
#include "Text.h"
#include "Border.h"
#include "List.h"
#include "Position.h"
#include "Animation.h"

#include "../../API/Header/Netzhaut.h"

typedef struct Nh_Tab Nh_Tab;
typedef struct Nh_HTML_Node Nh_HTML_Node;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_BACKFACE_VISIBILITY {
        NH_CSS_BACKFACE_VISIBILITY_VISIBLE, 	
        NH_CSS_BACKFACE_VISIBILITY_HIDDEN,  	
    } NH_CSS_BACKFACE_VISIBILITY;
    
/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct NH_CSS_Properties {
    
        NH_CSS_Animation Animation; 
        NH_CSS_Background Background;  
        NH_CSS_Position Position;      
        NH_CSS_Border Border;
        NH_CSS_Sides Margin;
        NH_CSS_Sides Padding;          
        NH_CSS_Text Text;              
        NH_CSS_List List;              
     
        NH_CSS_BACKFACE_VISIBILITY backfaceVisibility;
    
    } NH_CSS_Properties;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_initProperties(
        NH_CSS_Properties *Properties_p
    );

    void Nh_CSS_setDefaultProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );
    
    void Nh_CSS_setDefaultProperty(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type
    );

    NH_RESULT Nh_CSS_computeProperties(
        Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, void **Properties_pp
    );

    NH_RESULT Nh_CSS_updateProperties(
       Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
    );
   
    void Nh_CSS_destroyProperties(
        NH_CSS_Properties *Properties_p
    );

/** @} */

#endif 
