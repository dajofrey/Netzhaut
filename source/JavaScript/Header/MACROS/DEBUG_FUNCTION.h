/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef NH_BEGIN
#undef NH_END
#undef NH_SILENT_END

#ifdef NH_DEV
    #include "../Debug.h"
    #define NH_BEGIN() {Nh_JS_debugFunction(Script_p, Function_p, aCount, Arguments_p, __func__, 0);}
#else
    #define NH_BEGIN() 
#endif

#ifdef NH_DEV
    #define NH_END(result) {Nh_JS_Result foo = result; Nh_JS_debugFunction(Script_p, Function_p, 1, &foo, __func__, 1); return foo;}
#else
    #define NH_END(result) {return result;} 
#endif

#ifdef NH_DEV
    #define NH_SILENT_END() {return;}
#else
    #define NH_SILENT_END() {return;} 
#endif

