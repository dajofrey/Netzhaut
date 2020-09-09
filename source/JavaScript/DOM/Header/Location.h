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

    Nh_JS_Result Nh_JS_Location_Location(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    /**
     * Reloads the current document.
     */ 
    Nh_JS_Result Nh_JS_Location_reload(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Replaces the current document with a new one. This also replaces the history.
     */ 
    Nh_JS_Result Nh_JS_Location_replace(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Loads a new document.
     */ 
    Nh_JS_Result Nh_JS_Location_assign(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the anchor part (#) of a URL.
     */ 
    Nh_JS_Result Nh_JS_Location_hash(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the hostname and port number of a URL.
     */ 
    Nh_JS_Result Nh_JS_Location_host(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the hostname of a URL.
     */ 
    Nh_JS_Result Nh_JS_Location_hostName(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the entire URL.
     */ 
    Nh_JS_Result Nh_JS_Location_href(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Returns the protocol, hostname and port number of a URL.
     */ 
    Nh_JS_Result Nh_JS_Location_origin(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the pathname of a URL.
     */ 
    Nh_JS_Result Nh_JS_Location_pathName(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the port number of a URL.
     */ 
    Nh_JS_Result Nh_JS_Location_port(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the querystring part of a URL.
     */ 
    Nh_JS_Result Nh_JS_Location_search(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Sets or returns the protocol of a URL.
     */ 
    Nh_JS_Result Nh_JS_Location_protocol(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

/** @} */

#endif 
