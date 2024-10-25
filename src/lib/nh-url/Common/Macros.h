#ifndef NH_URL_COMMON_MACROS_H
#define NH_URL_COMMON_MACROS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS 

/** 
 * Netzhaut - Web Browser Engine 
 * Copyright (C) 2022  Dajo Frey 
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file. 
 */ 

#ifdef NH_LOG_FLOW 
    #include "Log.h" 
#endif 

#endif // DOXYGEN_SHOULD_SKIP_THIS 

// FLOW LOGGING ====================================================================================

#ifdef NH_LOG_FLOW 
    #define NH_URL_BEGIN() {_nh_url_logBegin(__FILE__, __func__);} 
#else 
    #define NH_URL_BEGIN() 
#endif 

#ifdef NH_LOG_FLOW 
    #define NH_URL_END(result) {_nh_url_logEnd(__FILE__, __func__); return result;} 
#else 
    #define NH_URL_END(result) {return result;} 
#endif 

#ifdef NH_LOG_FLOW 
    #define NH_URL_SILENT_END() {_nh_url_logEnd(__FILE__, __func__); return;} 
#else 
    #define NH_URL_SILENT_END() {return;} 
#endif 

#ifdef NH_LOG_FLOW 
    #define NH_URL_DIAGNOSTIC_END(result) {return _nh_url_logDiagnosticEnd(__FILE__, _
#else 
    #define NH_URL_DIAGNOSTIC_END(result) {return result;} 
#endif 

#endif // NH_URL_COMMON_MACROS_H
