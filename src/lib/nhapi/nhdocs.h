#ifndef NH_DOCS_H
#define NH_DOCS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nhcore.h"
#include "nhtty.h"

#endif

/** @defgroup api_nhdocs nhdocs
 *  @brief TODO.
 */

/** @addtogroup api_nhdocs
 *  @{
 */

// DEFINES =========================================================================================

    #define NH_DOCS_MAX_AUTHOR_NAME 255
    #define NH_DOCS_MAX_AUTHOR_CONTACT 255
    #define NH_DOCS_MAX_CONTEXT_NAME 255
    #define NH_DOCS_MAX_CONTEXT_PATH 255
    #define NH_DOCS_MAX_COMMIT_ID 128
    #define NH_DOCS_MAX_COMMIT_DATE 64
    #define NH_DOCS_MAX_WORKLOAD_SCOPE 16

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_DOCS_RESULT {
        NH_DOCS_SUCCESS,
        NH_DOCS_ERROR_INVALID_OPTION,
        NH_DOCS_ERROR_NULL_POINTER,
        NH_DOCS_ERROR_BAD_STATE,
        NH_DOCS_ERROR_MEMORY_ALLOCATION,
    } NH_DOCS_RESULT;

    typedef enum NH_DOCS_CONTEXT_E {
        NH_DOCS_CONTEXT_UNDEFINED,
        NH_DOCS_CONTEXT_LIBRARY,
        NH_DOCS_CONTEXT_BINARY,
    } NH_DOCS_CONTEXT_E;

    typedef enum NH_DOCS_SCOPE_E {
        NH_DOCS_SCOPE_PATCH,
        NH_DOCS_SCOPE_MINOR,
        NH_DOCS_SCOPE_MAJOR,
        NH_DOCS_SCOPE_API,
    } NH_DOCS_SCOPE_E;

// STRUCTS =========================================================================================

    typedef struct nh_docs_Version {
        long api, major, minor, patch;
    } nh_docs_Version;

    typedef struct nh_docs_Author {
        NH_BYTE name_p[NH_DOCS_MAX_AUTHOR_NAME];
        NH_BYTE contact_p[NH_DOCS_MAX_AUTHOR_CONTACT];
    } nh_docs_Author;

    typedef struct nh_docs_Message {
        NH_BYTE **lines_pp;
        int lines;
    } nh_docs_Message;

    typedef struct nh_docs_Merge {
        NH_BYTE **ids_pp;
        int ids;
    } nh_docs_Merge;

    typedef struct nh_docs_Workload {
        nh_docs_Message Message;
        NH_DOCS_SCOPE_E scope;
        NH_BYTE **libs_pp;
        int libs;
        NH_BYTE **bins_pp;
        int bins;
    } nh_docs_Workload;

    typedef struct nh_docs_Commit {
        NH_BYTE id_p[NH_DOCS_MAX_COMMIT_ID];
        NH_BYTE date_p[NH_DOCS_MAX_COMMIT_DATE]; // strict ISO 8601 format
        nh_docs_Author Author;
        nh_docs_Merge Merge;
        nh_docs_Message Message;
        nh_docs_Workload *Workloads_p;
        int workloads;
    } nh_docs_Commit;

    typedef struct nh_docs_Changelog {
        nh_docs_Commit *Commits_p;
        int commits;
    } nh_docs_Changelog;

    typedef struct nh_docs_ChangelogPart {
        nh_docs_Commit **Commits_pp;
        int commits;
    } nh_docs_ChangelogPart;

    typedef struct nh_docs_Diff {
        NH_BYTE name_p[255];
        NH_BYTE **lines_pp;
        int lines;
    } nh_docs_Diff;

    typedef struct nh_docs_Context {
        NH_DOCS_CONTEXT_E type;
        NH_BYTE name_p[NH_DOCS_MAX_CONTEXT_NAME];
        NH_BYTE path_p[NH_DOCS_MAX_CONTEXT_PATH];
        nh_docs_Diff **Diffs_pp;
        int diffs;
        nh_docs_Workload Workload;
    } nh_docs_Context;

// TYPEDEFS ========================================================================================

    typedef struct nh_docs_ContextArray nh_docs_ContextArray;

// CHANGELOG FUNCTIONS =============================================================================

    /**
     * Todo.
     */
    nh_docs_Changelog nh_docs_parseChangelog(
        NH_BYTE *path_p
    );

    /**
     * Todo.
     */
    NH_DOCS_RESULT nh_docs_freeChangelog(
        nh_docs_Changelog *Changelog_p
    );

    /**
     * Todo.
     */
    nh_docs_ChangelogPart nh_docs_getChangelogForContext(
       nh_docs_Changelog *Changelog_p, nh_docs_Context Context
    );

    /**
     * Todo.
     */
    NH_DOCS_RESULT nh_docs_freeChangelogPart(
       nh_docs_ChangelogPart *Part_p
    );

    /**
     * Todo.
     */
    NH_DOCS_RESULT nh_docs_printChangelog(
        nh_docs_Changelog *Changelog_p
    );
    
    /**
     * Todo.
     */
    NH_DOCS_RESULT nh_docs_stringifyChangelog(
        nh_docs_Changelog *Changelog_p, NH_BYTE *result_p, int length
    );

    /**
     * Todo.
     */
    NH_DOCS_RESULT nh_docs_stringifyCommit(
        nh_docs_Commit *Commit_p, NH_BYTE *result_p, int length
    );

// CONTEXT FUNCTIONS ===============================================================================

    /**
     * Todo.
     */
    nh_docs_Context *nh_docs_getContext(
        nh_docs_ContextArray *ContextArray_p, long unsigned int index 
    );
    
    /**
     * Todo.
     */
    nh_docs_Context *nh_docs_getContextFromDescription(
        nh_docs_ContextArray *Array_p, NH_BYTE *name_p, NH_DOCS_CONTEXT_E type
    );

    /**
     * Todo.
     */
    nh_docs_ContextArray *nh_docs_getContextArray(
        nh_docs_Changelog *Changelog_p
    );

    /**
     * Todo.
     */
    NH_DOCS_RESULT nh_docs_freeContextArray(
        nh_docs_ContextArray *ContextArray_p
    );

// VERSION FUNCTIONS ===============================================================================

    /**
     * Todo.
     */
    nh_docs_Version nh_docs_getVersion(
        nh_docs_Changelog *Changelog_p, nh_docs_Context *Context_p, nh_docs_Commit *LastCommit_p
    );

    /**
     * Todo.
     */
    nh_docs_Version nh_docs_getRevision(
        nh_docs_Changelog *Changelog_p, nh_docs_Commit *LastCommit_p
    );

/** @} */

#endif // NH_DOCS_H
