#ifndef NH_JS_STRING_H
#define NH_JS_STRING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include <stdbool.h>

#include "Object.h"

#include "../../Header/Data.h"
#include "../../Header/Parser.h"

typedef struct Nh_JS_Result Nh_JS_Result;
typedef struct Nh_JS_Script Nh_JS_Script;
typedef struct Nh_JS_Object Nh_JS_Object;

#endif

#ifdef DOXYGEN_JS_API

/** @addtogroup JSCoreString String
 *  \ingroup JSCore
 *  @{
 */

    Constructor String();                                                 /**<Not yet implemented.*/
                                                                          /**<Not yet implemented.*/
    Static Method fromCharCode(num1 [, ...[, numN]]);                     /**<Not yet implemented.*/
    Static Method fromCodePoint(num1 [, ...[, numN]]);                    /**<Not yet implemented.*/
    Static Method raw();                                                  /**<Not yet implemented.*/

    Instance Property length;                                             /**<Not yet implemented.*/

    Instance Method charAt(index);                                        /**<Not yet implemented.*/
    Instance Method charCodeAt(index);                                    /**<Not yet implemented.*/
    Instance Method codePointAt(pos);                                     /**<Not yet implemented.*/
    Instance Method concat(str [, ...strN ]);                             /**<Not yet implemented.*/
    Instance Method includes(searchString [, position]);                  /**<Not yet implemented.*/
    Instance Method endsWith(searchString [, length]);                    /**<Not yet implemented.*/
    Instance Method indexOf(searchValue [, fromIndex]);                   /**<Not yet implemented.*/
    Instance Method lastIndexOf(searchValue [, fromIndex]);               /**<Not yet implemented.*/
    Instance Method localeCompare(compareString [, locales [, options]]); /**<Not yet implemented.*/
    Instance Method match(regexp);                                        /**<Not yet implemented.*/
    Instance Method matchAll(regexp);                                     /**<Not yet implemented.*/
    Instance Method normalize([form]);                                    /**<Not yet implemented.*/
    Instance Method padEnd(targetLength [, padString]);                   /**<Not yet implemented.*/
    Instance Method padStart(targetLength [, padString]);                 /**<Not yet implemented.*/
    Instance Method repeat(count);                                        /**<Not yet implemented.*/
    Instance Method replaceAll(searchFor, replaceWith);                   /**<Not yet implemented.*/
    Instance Method search(regexp);                                       /**<Not yet implemented.*/
    Instance Method slice(beginIndex[, endIndex]);                        /**<Not yet implemented.*/
    Instance Method split([sep [, limit] ]);                              /**<Not yet implemented.*/
    Instance Method startsWith(searchString [, length]);                  /**<Not yet implemented.*/
    Instance Method substr();                                             /**<Not yet implemented.*/
    Instance Method substring(indexStart [, indexEnd]);                   /**<Not yet implemented.*/
    Instance Method toLocaleLowerCase( [locale, ...locales]);             /**<Not yet implemented.*/
    Instance Method toLocaleUpperCase( [locale, ...locales]);             /**<Not yet implemented.*/
    Instance Method toLowerCase();                                        /**<Not yet implemented.*/ 
    Instance Method toString();                                           /**<Not yet implemented.*/ 
    Instance Method toUpperCase();                                        /**<Not yet implemented.*/ 
    Instance Method trim();                                               /**<Not yet implemented.*/ 
    Instance Method trimStart();                                          /**<Not yet implemented.*/ 
    Instance Method trimEnd();                                            /**<Not yet implemented.*/ 
    Instance Method valueOf();                                            /**<Not yet implemented.*/ 
    Instance Method @@iterator();                                         /**<Not yet implemented.*/ 

/** @} */

#else

/** @addtogroup JSCoreFunctions Functions
 *  \ingroup JSCore
 *  @{
 */

// API

    Nh_JS_Result Nh_JS_String_String(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result Nh_JS_String_fromCharCode(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_fromCodePoint(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_raw(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_length(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_charAt(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_charCodeAt(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_codePointAt(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_concat(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_includes(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_endsWith(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_indexOf(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_lastIndexOf(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_localeCompareales(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_match(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_matchAll(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_normalize(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_padEnd(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_padStart(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_repeat(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_replace(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_replaceAll(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_search(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_slice(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_split(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_startsWith(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_substr(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_substring(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_toLocaleLowerCase(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_toLocaleUpperCase(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_toLowerCase(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_toString(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_toUpperCase(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_trim(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_trimStart(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_trimEnd(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_valueOf(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_String_iterator(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

// INTERNAL

//    NH_RESULT Nh_JS_createFunctionData(
//        Nh_JS_Function *Function_p, Nh_JS_Object *Owner_p, int index
//    );

/** @} */

#endif 

#endif
