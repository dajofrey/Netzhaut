/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_CONSOLE_H
#define NH_JS_CONSOLE_H

#include "../../Header/Interpreter.h"

/** @addtogroup Functions
 *  @{
 */

    Nh_JS_Result Nh_JS_Console_Console(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    /**
     * Writes an error message to the console if the assertion is false.
     */ 
    Nh_JS_Result Nh_JS_Console_assert(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Clears the console.
     */ 
    Nh_JS_Result Nh_JS_Console_clear(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Logs the number of times that this particular call to count() has been called.
     */ 
    Nh_JS_Result Nh_JS_Console_count(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Outputs an error message to the console.
     */ 
    Nh_JS_Result Nh_JS_Console_error(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Creates a new inline group in the console. This indents following console messages by an additional level, until console.groupEnd() is called. 
     */ 
    Nh_JS_Result Nh_JS_Console_group(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Creates a new inline group in the console. However, the new group is created collapsed. The user will need to use the disclosure button to expand it
     */ 
    Nh_JS_Result Nh_JS_Console_groupCollapsed(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Exits the current inline group in the console.
     */ 
    Nh_JS_Result Nh_JS_Console_groupEnd(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Outputs an informational message to the console. 
     */ 
    Nh_JS_Result Nh_JS_Console_info(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Outputs a message to the console.
     */ 
    Nh_JS_Result Nh_JS_Console_log(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Displays tabular data as a table.
     */ 
    Nh_JS_Result Nh_JS_Console_table(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Starts a timer (can track how long an operation takes).
     */ 
    Nh_JS_Result Nh_JS_Console_time(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Stops a timer that was previously started by console.time().
     */ 
    Nh_JS_Result Nh_JS_Console_timeEnd(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Outputs a stack trace to the console.
     */ 
    Nh_JS_Result Nh_JS_Console_trace(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    /**
     * Outputs a warning message to the console.
     */ 
    Nh_JS_Result Nh_JS_Console_warn(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

/** @} */

#endif 
