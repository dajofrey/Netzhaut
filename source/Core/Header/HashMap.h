#ifndef NH_HASHMAP_H
#define NH_HASHMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "External/c_hashmap.h"

#include "../../API/Header/Netzhaut.h"

#endif

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_HashValue {
        char key_p[128];     
        unsigned int number;
    } Nh_HashValue;

    typedef struct Nh_HashMaps {
        map_t MediaTypes;
        struct {
            map_t Attributes;   
            map_t Tags; 
        } HTML;
        struct {
            map_t Properties;   
            map_t Colors;        
            map_t PseudoClasses;
            map_t PseudoElements; 
        } CSS;
        struct { 
            map_t Keywords;
            map_t EventTypes;
            map_t *Functions_p;
        } JS;
    } Nh_HashMaps;

/** @} */

/** @addtogroup CoreData Data
 *  \ingroup Core
 *  @{
 */

    extern Nh_HashMaps NH_HASHMAPS;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    NH_RESULT Nh_createHashMaps(
    );

    void Nh_freeHashMaps(
    );

    Nh_HashMaps *Nh_getHashMaps(
    );

/** @} */

#endif 
