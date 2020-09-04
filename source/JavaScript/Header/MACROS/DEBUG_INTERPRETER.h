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
    #define NH_BEGIN() {Nh_JS_debugInterpreter(Script_p, __func__, Parsable_p, 0);}
#else
    #define NH_BEGIN() 
#endif

#ifdef NH_DEV
    #define NH_END(result) {Nh_JS_Result foo = result; Nh_JS_debugInterpreter(Script_p, NULL, &foo, 1); return foo;}
#else
    #define NH_END(result) {return result;} 
#endif

#ifdef NH_DEV
    #define NH_SILENT_END() {return;}
#else
    #define NH_SILENT_END() {return;} 
#endif

