#ifndef NH_MAKE_H
#define NH_MAKE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nhcore.h"
#include "nhdocs.h"

#endif

/** @defgroup api_nhmake nhmake
 *  @brief Runtime for parsing and executing build scripts used by Netzhaut.
 *
 *  This module implements a big part of the Netzhaut build-system.
 */

/** @addtogroup api_nhmake
 *  @{
 */

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_MAKE_RESULT {
        NH_MAKE_SUCCESS,
        NH_MAKE_ERROR_INVALID_OPTION,
        NH_MAKE_ERROR_NULL_POINTER,
        NH_MAKE_ERROR_BAD_STATE,
        NH_MAKE_ERROR_GENERATE_DOCUMENTS_FAILURE,
        NH_MAKE_ERROR_DOWNLOAD_FAILED,
        NH_MAKE_ERROR_BUILD_EXTERNAL_FAILED,
        NH_MAKE_ERROR_COPY_EXTERNAL_HEADER_FAILED,
        NH_MAKE_ERROR_COPY_PUBLIC_LIBRARY_HEADER_FAILED,
        NH_MAKE_ERROR_CREATE_LIBRARY_INCLUDES_FAILED,
        NH_MAKE_ERROR_BUILD_LIBRARY_FAILED,
        NH_MAKE_ERROR_WGET_EXECUTION_FAILED,
        NH_MAKE_ERROR_UNZIP_EXECUTION_FAILED,
        NH_MAKE_ERROR_DOXYGEN_EXECUTION_FAILED,
        NH_MAKE_ERROR_GCC_EXECUTION_FAILED,
        NH_MAKE_ERROR_AR_EXECUTION_FAILED,
        NH_MAKE_ERROR_XXD_EXECUTION_FAILED,
        NH_MAKE_ERROR_CP_EXECUTION_FAILED,
        NH_MAKE_ERROR_LIBRARY_NOT_FOUND,
        NH_MAKE_ERROR_WGET_NOT_FOUND,
        NH_MAKE_ERROR_UNZIP_NOT_FOUND,
        NH_MAKE_ERROR_DOXYGEN_NOT_FOUND,
        NH_MAKE_ERROR_GCC_NOT_FOUND,
        NH_MAKE_ERROR_AR_NOT_FOUND,
        NH_MAKE_ERROR_CP_NOT_FOUND,
        NH_MAKE_ERROR_GENERATE_VERSION_DEPENDENT_DOCS,
        NH_MAKE_ERROR_GENERATE_DOCS_USING_DOXYGEN,
        NH_MAKE_ERROR_GET_PROJECT_DIRECTORY,
        NH_MAKE_ERROR_GET_WORK_DIRECTORY,
        NH_MAKE_ERROR_CANT_OPEN_README,
        NH_MAKE_ERROR_CANT_OPEN_HEADER,
        NH_MAKE_ERROR_CANT_OPEN_DIR,
        NH_MAKE_ERROR_CANT_DOWNLOAD_VULKAN_HEADERS,
        NH_MAKE_ERROR_CANT_DOWNLOAD_VOLK,
        NH_MAKE_ERROR_CANT_DOWNLOAD_FREETYPE,
        NH_MAKE_ERROR_CANT_DOWNLOAD_OPENSSL,
        NH_MAKE_ERROR_CANT_DOWNLOAD_PROJECT,
        NH_MAKE_ERROR_BUILD_XXD_FAILED,
        NH_MAKE_ERROR_BUILD_HELPER_LIBRARY_FAILED,
        NH_MAKE_ERROR_NOT_IMPLEMENTED,
        NH_MAKE_ERROR_CANT_CREATE_DIRECTORY,
        NH_MAKE_ERROR_CANT_CREATE_OBJECT_FILE,
        NH_MAKE_ERROR_CANT_CREATE_OBJECTS,
        NH_MAKE_ERROR_CANT_CREATE_LIBRARY,
        NH_MAKE_ERROR_CANT_CREATE_EXTERNAL_BIN_DIRECTORY,
        NH_MAKE_ERROR_CANT_CREATE_EXTERNAL_BIN_OBJECT_DIRECTORY,
        NH_MAKE_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY,
        NH_MAKE_ERROR_CANT_CREATE_BIN_DIRECTORY,
        NH_MAKE_ERROR_CANT_CREATE_SHARED_LIB,
        NH_MAKE_ERROR_CANT_CREATE_STATIC_LIB,
        NH_MAKE_ERROR_CANT_CREATE_INCLUDE_FILE,
        NH_MAKE_ERROR_CANT_CREATE_INCLUDE_DIRECTORY,
        NH_MAKE_ERROR_CANT_CREATE_EXTERNAL_DIRECTORY,
        NH_MAKE_ERROR_COPY_EXTERNAL_HEADER_USING_CP_FAILED,
        NH_MAKE_ERROR_COPY_PUBLIC_LIBRARY_HEADER_USING_CP_FAILED,
        NH_MAKE_ERROR_COPY_FAILED,
        NH_MAKE_ERROR_CANT_CREATE_NETZHAUT_OBJECT_FILES,
        NH_MAKE_ERROR_CANT_CREATE_IO_OBJECT_FILES,
        NH_MAKE_ERROR_CANT_CREATE_GRAPHICS_OBJECT_FILES,
        NH_MAKE_ERROR_CANT_CREATE_CSS_OBJECT_FILES,
        NH_MAKE_ERROR_CANT_CREATE_HTML_OBJECT_FILES,
        NH_MAKE_ERROR_CANT_CREATE_JAVASCRIPT_OBJECT_FILES,
        NH_MAKE_ERROR_CANT_CREATE_TTY_OBJECT_FILES,
        NH_MAKE_ERROR_CANT_CREATE_NETWORK_OBJECT_FILES,
        NH_MAKE_ERROR_CANT_CREATE_API_OBJECT_FILES,
        NH_MAKE_ERROR_THREAD_CREATION_FAILED,
        NH_MAKE_ERROR_INSTALL_ALL_LIBRARY_FAILED,
        NH_MAKE_ERROR_INSTALL_ALL_EXTERNAL_LIBRARY_FAILED,
        NH_MAKE_ERROR_INSTALL_ALL_WEB_BROWSER_FAILED,
        NH_MAKE_ERROR_INSTALL_ALL_LOGO_FAILED,
        NH_MAKE_ERROR_BUILD_WEB_BROWSER_FAILED,
    } NH_MAKE_RESULT;

    /**
     * Boolean values.
     */
    typedef enum NH_MAKE_BOOL {
        NH_MAKE_FALSE = 0, /**<Indicates false.*/
        NH_MAKE_TRUE = 1,  /**<Indicates true.*/
    } NH_MAKE_BOOL;

    typedef enum NH_MAKE_TOKEN {
        NH_MAKE_TOKEN_UNDEFINED,
        NH_MAKE_TOKEN_COMMA,
        NH_MAKE_TOKEN_IDENTIFIER,
        NH_MAKE_TOKEN_STRING,
        NH_MAKE_TOKEN_CURLY_BRACKET_RIGHT,
        NH_MAKE_TOKEN_CURLY_BRACKET_LEFT,
        NH_MAKE_TOKEN_ROUND_BRACKET_RIGHT,
        NH_MAKE_TOKEN_ROUND_BRACKET_LEFT,
        NH_MAKE_TOKEN_ANGLE_BRACKET_RIGHT,
        NH_MAKE_TOKEN_ANGLE_BRACKET_LEFT,
        NH_MAKE_TOKEN_HYPHEN_MINUS,
        NH_MAKE_TOKEN_COLON,
        NH_MAKE_TOKEN_EOF,
    } NH_MAKE_TOKEN;

    typedef enum NH_MAKE_DEFINITION {
        NH_MAKE_DEFINITION_UNDEFINED,
        NH_MAKE_DEFINITION_OPTION,
        NH_MAKE_DEFINITION_FUNCTION,
        NH_MAKE_DEFINITION_BLOCK,
        NH_MAKE_DEFINITION_IF,
    } NH_MAKE_DEFINITION;

    typedef enum NH_MAKE_SOURCE_CONTEXT {
        NH_MAKE_SOURCE_CONTEXT_UNDEFINED,
        NH_MAKE_SOURCE_CONTEXT_SHARED_LIBRARY,
        NH_MAKE_SOURCE_CONTEXT_STATIC_LIBRARY,
        NH_MAKE_SOURCE_CONTEXT_BINARY,
    } NH_MAKE_SOURCE_CONTEXT;

// STRUCTS =========================================================================================

    typedef struct nh_make_Function {
        NH_MAKE_DEFINITION type;
        NH_BYTE *name_p;
        unsigned int arguments;
        NH_BYTE **arguments_pp;
        NH_MAKE_TOKEN *argumentTypes_p;
    } nh_make_Function;

    typedef struct nh_make_SourceContext {
        NH_MAKE_SOURCE_CONTEXT type;
        NH_BYTE *path_p;
        NH_BYTE *name_p;
        NH_BYTE *compileArgs_p;
        NH_BYTE *linkArgs_p;
        NH_BYTE *outputPath_p;
        nh_docs_Version Version;
    } nh_make_SourceContext;

    typedef struct nh_make_SourceContextArray {
        int length;
        int maxNameLength;
        nh_make_SourceContext *SourceContexts_p; // TODO Change name to "p".
    } nh_make_SourceContextArray;

    typedef struct nh_make_ValueArray {
        NH_BYTE **values_pp;
        int length;
    } nh_make_ValueArray;

    typedef struct nh_make_TestArgument {
        NH_BYTE *p;
    } nh_make_TestArgument;

// TYPEDEFS ========================================================================================

    typedef struct nh_make_Runtime nh_make_Runtime;
    typedef struct nh_make_CommitNode nh_make_CommitNode;

    typedef NH_MAKE_RESULT (*nh_make_functionCallback_f)(
        nh_make_Runtime *Runtime_p, nh_make_Function *Function_p
    );

    typedef NH_MAKE_RESULT (*nh_make_sourceContextCallback_f)(
        nh_make_Runtime *Runtime_p, nh_make_SourceContext *Context_p
    );

// FUNCTIONS =======================================================================================

    /**
     * Todo.
     */
    NH_MAKE_RESULT nh_make_initialize(
    );
    
    /**
     * Todo.
     */
    void nh_make_terminate(
    );

    /**
     * Todo.
     */
    nh_make_Runtime *nh_make_createRuntime(
        NH_BYTE *name_p
    );

    /**
     * Todo.
     */
    void nh_make_destroyRuntime(
        nh_make_Runtime *Runtime_p
    );

    /**
     * Todo.
     */
    NH_MAKE_RESULT nh_make_run(
        nh_make_Runtime *Runtime_p, int argc, NH_BYTE **argv_pp
    );

    /**
     * Todo.
     */
    NH_MAKE_RESULT nh_make_addFile(
        nh_make_Runtime *Runtime_p, NH_BYTE *path_p
    );

    /**
     * Todo.
     */
    void nh_make_setFunctionCallback(
        nh_make_Runtime *Runtime_p, nh_make_functionCallback_f functionCallback_f
    );

    /**
     * Todo.
     */
    void nh_make_setSourceContextCallback(
        nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f
    );

    /**
     * Todo.
     */
    void nh_make_setBeforeBuildCallback(
        nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f
    );

    /**
     * Todo.
     */
    void nh_make_setAfterBuildCallback(
        nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f
    );

    /**
     * Todo.
     */
    nh_make_SourceContextArray *nh_make_getSourceContextArray(
        nh_make_Runtime *Runtime_p
    );

    /**
     * Todo.
     */
    nh_make_SourceContext *nh_make_getSourceContext(
        nh_make_Runtime *Runtime_p, NH_BYTE *name_p, NH_MAKE_SOURCE_CONTEXT type
    );

    /**
     * TODO switch to variable based setting.
     */
    void nh_make_setQuiet(
        nh_make_Runtime *Runtime_p, NH_MAKE_BOOL quiet
    );
    
    /**
     * Todo.
     */
    void nh_make_setShowParseTree(
        nh_make_Runtime *Runtime_p, NH_MAKE_BOOL showParseTree
    );

    /**
     * Todo.
     */
    NH_MAKE_RESULT nh_make_setVariable(
        nh_make_Runtime *Runtime_p, NH_BYTE *variables_p, NH_BYTE **values_pp, int valueCount
    );

    /**
     * Todo.
     */
    nh_make_ValueArray nh_make_getVariableValues(
        nh_make_Runtime *Runtime_p, NH_BYTE *variable_p
    );

    /**
     * Todo.
     */
    int nh_make_isRunning(
    );
    
    /**
     * Todo.
     */
    NH_MAKE_RESULT nh_make_sleepMs(
        int milliseconds
    );

    /**
     * Todo.
     */
    NH_BYTE *nh_make_getProcessDirectory(
    );

    /**
     * Todo.
     */
    NH_MAKE_RESULT nh_make_messagef(
        NH_BYTE *format_p, ...
    );

/** @} */

#endif // NH_MAKE_H
