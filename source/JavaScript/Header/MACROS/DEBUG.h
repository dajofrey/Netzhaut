/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef NH_BEGIN
#undef NH_END
#undef NH_SILENT_END

#ifdef NH_DEV
    #define NH_BEGIN() {}
#else
    #define NH_BEGIN() 
#endif

#ifdef NH_DEV
    #define NH_END(result) {return result;}
#else
    #define NH_END(result) {return result;} 
#endif

#ifdef NH_DEV
    #define NH_SILENT_END() {return;}
#else
    #define NH_SILENT_END() {return;} 
#endif

