// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "nhdocs.h"

#include "../nhcore/Loader/Loader.h"

#include "../nhdocs/Changelog/Changelog.h"
#include "../nhdocs/Changelog/Version.h"
#include "../nhdocs/Changelog/Context.h"
#include "../nhdocs/Tagger/Tagger.h"

#include <unistd.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CHANGELOG FUNCTIONS =============================================================================

nh_docs_Changelog nh_docs_parseChangelog(
    NH_BYTE *path_p)
{
    nh_docs_Changelog Changelog;
    Changelog.Commits_p = NULL;
    Changelog.commits = 0;
    nh_docs_parseChangelog_f parseChangelog_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_parseChangelog");
    return parseChangelog_f ? parseChangelog_f(path_p) : Changelog;
}

NH_DOCS_RESULT nh_docs_freeChangelog(
    nh_docs_Changelog *Changelog_p)
{
    nh_docs_freeChangelog_f freeChangelog_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_freeChangelog");
    if (freeChangelog_f) {freeChangelog_f(Changelog_p);}
    else {return NH_DOCS_ERROR_BAD_STATE;}
}

nh_docs_ChangelogPart nh_docs_getChangelogForContext(
   nh_docs_Changelog *Changelog_p, nh_docs_Context Context)
{
    nh_docs_ChangelogPart ChangelogPart;
    ChangelogPart.Commits_pp = NULL;
    ChangelogPart.commits = 0;
    nh_docs_getChangelogForContext_f getChangelogForContext_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_getChangelogForContext");
    return getChangelogForContext_f ? getChangelogForContext_f(Changelog_p, Context) : ChangelogPart;
}

NH_DOCS_RESULT nh_docs_freeChangelogPart(
   nh_docs_ChangelogPart *ChangelogPart_p)
{
    nh_docs_freeChangelogPart_f freeChangelogPart_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_freeChangelogPart");
    if (freeChangelogPart_f) {freeChangelogPart_f(ChangelogPart_p);}
    else {return NH_DOCS_ERROR_BAD_STATE;}
}

NH_DOCS_RESULT nh_docs_printChangelog(
    nh_docs_Changelog *Changelog_p)
{
    nh_docs_printChangelog_f printChangelog_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_printChangelog");
    return printChangelog_f ? printChangelog_f(Changelog_p) : NH_DOCS_ERROR_BAD_STATE;
}

NH_DOCS_RESULT nh_docs_stringifyChangelog(
    nh_docs_Changelog *Changelog_p, NH_BYTE *result_p, int length)
{
    nh_docs_stringifyChangelog_f stringifyChangelog_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_stringifyChangelog");
    return stringifyChangelog_f ? stringifyChangelog_f(Changelog_p, result_p, length) : NH_DOCS_ERROR_BAD_STATE;
}

NH_DOCS_RESULT nh_docs_stringifyCommit(
    nh_docs_Commit *Commit_p, NH_BYTE *result_p, int length)
{
    nh_docs_stringifyCommit_f stringifyCommit_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_stringifyCommit");
    return stringifyCommit_f ? stringifyCommit_f(Commit_p, result_p, length) : NH_DOCS_ERROR_BAD_STATE;
}

// CONTEXT FUNCTIONS ===============================================================================

nh_docs_Context *nh_docs_getContext(
    nh_docs_ContextArray *ContextArray_p, long unsigned int index)
{
    nh_docs_getContext_f getContext_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_getContext");
    return getContext_f ? getContext_f(ContextArray_p, index) : NULL;
}

nh_docs_Context *nh_docs_getContextFromDescription(
    nh_docs_ContextArray *ContextArray_p, NH_BYTE *name_p, NH_DOCS_CONTEXT_E type)
{
    nh_docs_getContextFromDescription_f getContextFromDescription_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_getContextFromDescription");
    return getContextFromDescription_f ? getContextFromDescription_f(ContextArray_p, name_p, type) : NULL;
}

nh_docs_ContextArray *nh_docs_getContextArray(
    nh_docs_Changelog *Changelog_p)
{
    nh_docs_getContextArray_f getContextArray_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_getContextArray");
    return getContextArray_f ? getContextArray_f(Changelog_p) : NULL;
}

NH_DOCS_RESULT nh_docs_freeContextArray(
    nh_docs_ContextArray *ContextArray_p)
{
    nh_docs_freeContextArray_f freeContextArray_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_freeContextArray");
    return freeContextArray_f ? freeContextArray_f(ContextArray_p) : NH_DOCS_ERROR_BAD_STATE;
}

// VERSION FUNCTIONS ===============================================================================

nh_docs_Version nh_docs_getVersion(
    nh_docs_Changelog *Changelog_p, nh_docs_Context *Context_p, nh_docs_Commit *LastCommit_p)
{
    nh_docs_Version Version;
    memset(&Version, 0, sizeof(Version));
    nh_docs_getVersion_f getVersion_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_getVersion");
    return getVersion_f ? getVersion_f(Changelog_p, Context_p, LastCommit_p) : Version;
}

nh_docs_Version nh_docs_getRevision(
    nh_docs_Changelog *Changelog_p, nh_docs_Commit *LastCommit_p)
{
    nh_docs_Version Revision;
    memset(&Revision, 0, sizeof(Revision));
    nh_docs_getRevision_f getRevision_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadSymbol_f(NH_MODULE_DOCS, 0, "nh_docs_getRevision");
    return getRevision_f ? getRevision_f(Changelog_p, LastCommit_p) : Revision;
}

