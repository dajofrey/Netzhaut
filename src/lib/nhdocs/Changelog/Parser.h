#ifndef NH_DOCS_CHANGELOG_PARSER_H
#define NH_DOCS_CHANGELOG_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nhdocs_enums
 *  @{
 */

    typedef enum NH_DOCS_COMMIT_NODE_E {
        NH_DOCS_COMMIT_NODE_UNDEFINED,
        NH_DOCS_COMMIT_NODE_ROOT,
        NH_DOCS_COMMIT_NODE_WORKLOAD,
        NH_DOCS_COMMIT_NODE_MESSAGE,
        NH_DOCS_COMMIT_NODE_AUTHOR,
        NH_DOCS_COMMIT_NODE_DATE,
        NH_DOCS_COMMIT_NODE_ID,
        NH_DOCS_COMMIT_NODE_MERGE,
        NH_DOCS_COMMIT_NODE_WORKLOAD_BIN,
        NH_DOCS_COMMIT_NODE_WORKLOAD_LIB,
        NH_DOCS_COMMIT_NODE_WORKLOAD_SCOPE,
        NH_DOCS_COMMIT_NODE_WORKLOAD_MESSAGE,
        NH_DOCS_COMMIT_NODE_AUTHOR_NAME,
        NH_DOCS_COMMIT_NODE_AUTHOR_CONTACT,
    } NH_DOCS_COMMIT_NODE_E;

/** @} */

/** @addtogroup lib_nhdocs_structs
 *  @{
 */

    typedef struct nh_docs_CommitNode {
        NH_DOCS_COMMIT_NODE_E type;
        NH_BYTE *text_p;
        int length;
        struct nh_docs_CommitNode *Parent_p;
        struct nh_docs_CommitNode **Children_pp;
        int children; 
    } nh_docs_CommitNode;

/** @} */

/** @addtogroup lib_nhdocs_functions
 *  @{
 */

    nh_docs_CommitNode *nh_docs_parseCommits(
        NH_BYTE *path_p
    );

    nh_docs_Commit *nh_docs_getCommits(
        nh_docs_CommitNode *Root_p, int *commits_p
    );

/** @} */

#endif // NH_DOCS_CHANGELOG_PARSER_H
