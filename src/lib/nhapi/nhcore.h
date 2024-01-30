#ifndef NH_CORE_H
#define NH_CORE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include <stdint.h>

#endif

/** @defgroup api_nhcore nhcore
 *  @brief Initialize, run and terminate Netzhaut, among other critical functions. 
 */

/** @addtogroup api_nhcore
 *  @{
 */

// ENUMS ===========================================================================================

    /**
     * Boolean values.
     */
    typedef enum NH_BOOL {
        NH_FALSE = 0, /**<Indicates false.*/
        NH_TRUE = 1,  /**<Indicates true.*/
    } NH_BOOL;

    /**
     * Alternative boolean values.
     */
    typedef enum NH_SWITCH {
        NH_OFF = 0, /**<Indicates off.*/
        NH_ON = 1,  /**<Indicates on.*/
    } NH_SWITCH;

    typedef enum NH_SIGNAL {
        NH_SIGNAL_OK,
        NH_SIGNAL_DONE,
        NH_SIGNAL_FINISH,
        NH_SIGNAL_FREE,
        NH_SIGNAL_ERROR,
        NH_SIGNAL_IDLE,
        NH_SIGNAL_OPEN,
        NH_SIGNAL_CLOSE,
        NH_SIGNAL_UPDATE,
        NH_SIGNAL_INACTIVE,
        NH_SIGNAL_INIT,
        NH_SIGNAL_RESIZE,
        NH_SIGNAL_RESTART,
        NH_SIGNAL_SET_BACKGROUND_COLOR,
        NH_SIGNAL_SET_TITLE,
    } NH_SIGNAL;

    /**
     * Return values for functions.
     */
    typedef enum NH_CORE_RESULT 
    {
        NH_CORE_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_CORE_ERROR_NULL_POINTER,         
        NH_CORE_ERROR_BAD_STATE, 
        NH_CORE_ERROR_THREAD_INITIALIZATION,
        NH_CORE_ERROR_THREADPOOL_CREATION,
        NH_CORE_ERROR_MEMORY_INITIALIZATION,
        NH_CORE_ERROR_VULKAN_HOST_CREATION_FAILED,
        NH_CORE_ERROR_HTML_BASE_CREATION_FAILED,
        NH_CORE_ERROR_HTML_THREAD_START_FAILED,
        NH_CORE_ERROR_WINDOW_THREAD_START_FAILED,
        NH_CORE_ERROR_INSUFFICIENT_RESOURCES,
        NH_CORE_ERROR_MEMORY_ALLOCATION,
        NH_CORE_ERROR_PARAMETERS,
        NH_CORE_ERROR_NOT_ENOUGH_THREADS,
        NH_CORE_ERROR_UNKNOWN_FILE_FORMAT,
        NH_CORE_ERROR_TYPE_MISMATCH,
        NH_CORE_ERROR_STRING_LENGTH,
        NH_CORE_ERROR_INVALID_FILE_FORMAT,
        NH_CORE_ERROR_EXPECTED_TRUE,
        NH_CORE_ERROR_EXPECTED_FALSE,
        NH_CORE_ERROR_INVALID_ARGUMENT,

        NH_USAGE_ERROR_ALREADY_INITIALIZED,
        NH_USAGE_ERROR_INIT_MISSING,
        NH_USAGE_ERROR_STILL_RUNNING,

    } NH_CORE_RESULT;

// TYPEDEFS ========================================================================================

    /**
     * Opaque structure of the loader data.
     */
    typedef struct nh_Loader nh_Loader;
    
    typedef char          NH_BYTE;
    typedef unsigned char NH_UNSIGNED_BYTE;
    typedef int           NH_PIXEL;
    typedef float         NH_CLIP;

    /**
     * Opaque structure holding the actual data of @ref nh_LinkedList.
     */
    typedef struct nh_LinkedListItem nh_LinkedListItem;
    typedef struct nh_core_Workload nh_core_Workload;

    typedef void (*nh_core_logCallback_f)(
        NH_BYTE *node_p, NH_BYTE *options_p, NH_BYTE *message_p
    );

    typedef NH_CORE_RESULT (*nh_core_addLogCallback_f)(
        nh_core_logCallback_f logCallback_f
    );

// DATA ============================================================================================

    extern nh_Loader *NH_LOADER_P;

// STRUCTS =========================================================================================

    typedef struct nh_SystemTime {
        long seconds; 
        long milliseconds; 
        long microseconds; 
    } nh_SystemTime;

    /**
     * Generic linked-list which holds data pointers. 
     */
    typedef struct nh_Array {
        int elementSize;             /**<Number of items.*/
        int allocatedLengthPerChunk;
        int allocatedLength;
        unsigned long length;
        NH_BYTE *p;   /**<Opaque pointer to the first item in the list. A list with \ref count == 0 should have this set as NULL. The actual data should always be queried using @ref nh_core_getFromLinkedList.*/
    } nh_Array;

    /**
     * Dynamic list implementation using a two dimensional array. 
     */
    typedef struct nh_List {
        unsigned long chunkSize;   
        unsigned long chunkCount;
        unsigned long size;
        void **pp;
    } nh_List;

    typedef struct nh_ArrayList {
        nh_List Arrays;
        int elementSize;             /**<Number of items.*/
        int allocatedLengthPerChunk;
    } nh_ArrayList;

    /**
     * Generic linked-list which holds data pointers. 
     */
    typedef struct nh_LinkedList {
        int count;                 /**<Number of items.*/
        nh_LinkedListItem *Head_p; /**<Opaque pointer to the first item in the list. A list with \ref count == 0 should have this set as NULL. The actual data should always be queried using @ref nh_core_getFromLinkedList.*/
    } nh_LinkedList;

    typedef struct nh_RingBufferMarker {
        int index;
        int overflow;
    } nh_RingBufferMarker;

    typedef struct nh_RingBuffer {
        nh_RingBufferMarker Marker; /**<For convenience.*/ 
        int itemCount, itemByteSize; 
        int index; 
        int overflow; 
        void *data_p; 
    } nh_RingBuffer; 

    typedef struct nh_BigInt {
        int TODO;
    } nh_BigInt;

    typedef struct nh_PixelPosition {
        NH_PIXEL x;
        NH_PIXEL y;
    } nh_PixelPosition;

    typedef struct nh_PixelSize {
        NH_PIXEL width; 
        NH_PIXEL height; 
    } nh_PixelSize; 
 
    typedef struct nh_ClipPosition {
        NH_CLIP x; 
        NH_CLIP y; 
    } nh_ClipPosition; 
 
    typedef struct nh_ClipSize { 
        NH_CLIP width; 
        NH_CLIP height; 
    } nh_ClipSize; 
 
    typedef struct nh_Color { 
        float r, g, b, a; 
    } nh_Color; 
 
    /**
     * A string is an array of NH_BYTE's. 
     */
    typedef nh_Array nh_String;
    
    /**
     * An ASCII string is a string.
     */
    typedef nh_String nh_ASCIIString;


// GENERAL FUNCTIONS ===============================================================================

    /**
     * Todo. 
     */
    NH_CORE_RESULT nh_api_initialize(
        char *path_p, char *config_p, int length
    );

    /**
     * Todo. 
     */
    NH_CORE_RESULT nh_api_terminate(
    );

    /**
     * Todo. 
     */
    unsigned int nh_api_run(
    );

    /**
     * Todo. 
     */
    NH_BOOL nh_api_keepRunning(
    );

    /**
     * Todo. 
     */
    NH_CORE_RESULT nh_api_terminate(
    );

    /**
     * Todo. 
     */
    NH_CORE_RESULT nh_api_addLogCallback(
        nh_core_logCallback_f logCallback_f
    );

    /**
     * Todo.
     */
    nh_core_Workload *nh_api_getWorkload(
        void *args_p
    );

// FILE FUNCTIONS ==================================================================================

    NH_BYTE *nh_api_getFileData(
        const NH_BYTE* path_p, long *size_p
    );

    NH_CORE_RESULT nh_api_writeBytesToFile(
        NH_BYTE *filePath_p, NH_BYTE *bytes_p
    );

// CONFIG FUNCTIONS ================================================================================

    NH_CORE_RESULT nh_api_registerConfig(
        NH_BYTE *absolutePath_p, int length
    );

    NH_CORE_RESULT nh_api_loadConfig(
        NH_BYTE *data_p, int length
    );

    void *nh_api_createMonitorInterface(
    );
    
    void nh_api_freeMonitorInterface(
        void *Interface_p
    );

/** @} */

#endif // NH_CORE_H
