/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_LOCATION_H
#define NH_JS_LOCATION_H

#include "../../Header/Interpreter.h"

/** @addtogroup Functions
 *  @{
 */

    Nh_JS_Result NH_JS_LOCATION_Location(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    /**
     * Reloads the current document.
     */ 
    Nh_JS_Result NH_JS_LOCATION_reload(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Replaces the current document with a new one. This also replaces the history.
     */ 
    Nh_JS_Result NH_JS_LOCATION_replace(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Loads a new document.
     */ 
    Nh_JS_Result NH_JS_LOCATION_assign(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the anchor part (#) of a URL.
     */ 
    Nh_JS_Result NH_JS_LOCATION_hash(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the hostname and port number of a URL.
     */ 
    Nh_JS_Result NH_JS_LOCATION_host(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the hostname of a URL.
     */ 
    Nh_JS_Result NH_JS_LOCATION_hostName(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the entire URL.
     */ 
    Nh_JS_Result NH_JS_LOCATION_href(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Returns the protocol, hostname and port number of a URL.
     */ 
    Nh_JS_Result NH_JS_LOCATION_origin(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the pathname of a URL.
     */ 
    Nh_JS_Result NH_JS_LOCATION_pathName(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the port number of a URL.
     */ 
    Nh_JS_Result NH_JS_LOCATION_port(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the querystring part of a URL.
     */ 
    Nh_JS_Result NH_JS_LOCATION_search(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the protocol of a URL.
     */ 
    Nh_JS_Result NH_JS_LOCATION_protocol(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

/** @} */

#endif 
