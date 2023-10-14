// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Tagger.h"
#include "Git.h"

#include "../Changelog/Changelog.h"
#include "../Changelog/Version.h"
#include "../Common/Macros.h"

#include "../../nhcore/Util/Array.h"
#include "../../nhcore/System/Memory.h"

#include "../../nhencoding/Common/Macros.h"
#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhencoding/Encodings/UTF8.h"

#include "../../nhtty/TTY/Program.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

typedef struct nh_docs_TaggerRequest {
    NH_BOOL render;
    NH_BOOL update;
} nh_docs_TaggerRequest;

typedef struct nh_docs_Tagger {
    nh_docs_TaggerRequest Request;
    nh_docs_Commit *Commit_p; // The commit on which the tagger is working.
    nh_docs_Context *Contexts_p;   
    int contexts;
    int select;
    int draw;
    nh_Array Glyphs;
    nh_Array Lines;
    int tmp;
    int tmp2;
    int offset;
    int indexx;
} nh_docs_Tagger;

// INIT/DESTROY ====================================================================================

static void *nh_docs_initTagger(
    void *arg_p)
{
NH_DOCS_BEGIN()

    nh_docs_Tagger *Tagger_p = nh_core_allocate(sizeof(nh_docs_Tagger));
    NH_DOCS_CHECK_MEM_2(NULL, Tagger_p)

    memset(Tagger_p, 0, sizeof(nh_docs_Tagger));

    Tagger_p->select = 0;
    Tagger_p->draw = 0;
    Tagger_p->tmp = 0;
    Tagger_p->tmp2 = 0;
    Tagger_p->indexx = 0;
    Tagger_p->offset = 0;

    Tagger_p->Glyphs = nh_core_initArray(sizeof(nh_tty_Glyph), 255);
    Tagger_p->Lines = nh_core_initArray(sizeof(nh_Array), 255);
 
    Tagger_p->Request.render = NH_TRUE;
    Tagger_p->Request.update = NH_TRUE;

    Tagger_p->Commit_p = nh_docs_createCommitUsingGit("0");
    NH_DOCS_CHECK_NULL_2(NULL, Tagger_p->Commit_p)

    Tagger_p->Contexts_p = nh_docs_getContextsUsingGit(&Tagger_p->contexts, "0");
    NH_DOCS_CHECK_NULL_2(NULL, Tagger_p->Contexts_p)

NH_DOCS_END(Tagger_p)
}

static void nh_docs_destroyTagger(
    void *handle_p)
{
NH_DOCS_BEGIN()

    // TODO

    nh_core_free(handle_p);

NH_DOCS_SILENT_END()
}

// BUILD ===========================================================================================

static NH_BOOL nh_docs_scopeSet(
    nh_docs_Tagger *Tagger_p, NH_DOCS_SCOPE_E scope)
{
NH_DOCS_BEGIN()
 
    for (int i = 0; i < Tagger_p->contexts; ++i) {
        nh_docs_Context *Context_p = Tagger_p->Contexts_p+i;
        if (!Context_p->diffs) {continue;}
        if (Context_p->Workload.scope == scope) {
            NH_DOCS_END(NH_TRUE)
        }
    }

NH_DOCS_END(NH_FALSE)
}

static NH_DOCS_RESULT nh_docs_addName(
    NH_BYTE ***names_ppp, int *count_p, NH_BYTE *name_p)
{
NH_DOCS_BEGIN()
 
    if (!(*names_ppp)) {
        *names_ppp = malloc(sizeof(NH_BYTE*));
        if (!(*names_ppp)) {NH_DOCS_END(NH_DOCS_ERROR_BAD_STATE)}
    } else {
        *names_ppp = realloc(*names_ppp, sizeof(NH_BYTE*)*((*count_p)+1));
        if (!(*names_ppp)) {NH_DOCS_END(NH_DOCS_ERROR_BAD_STATE)}
    }

    (*names_ppp)[*count_p] = malloc(sizeof(NH_BYTE)*(strlen(name_p)+1));
    strcpy((*names_ppp)[(*count_p)++], name_p);

NH_DOCS_END(NH_DOCS_SUCCESS)
}

static NH_DOCS_RESULT nh_docs_setWorkload(
    nh_docs_Tagger *Tagger_p, nh_docs_Workload *Workload_p, NH_DOCS_SCOPE_E scope)
{
NH_DOCS_BEGIN()
 
    Workload_p->scope = scope;

    for (int i = 0; i < Tagger_p->contexts; ++i) {
        nh_docs_Context *Context_p = Tagger_p->Contexts_p+i;
        if (!Context_p->diffs) {continue;}
        if (Context_p->Workload.scope != scope) {continue;}
        if (Context_p->type == NH_DOCS_CONTEXT_LIBRARY) {
            NH_DOCS_CHECK(nh_docs_addName(&Workload_p->libs_pp, &Workload_p->libs, Context_p->name_p))
        }
        if (Context_p->type == NH_DOCS_CONTEXT_BINARY) {
            NH_DOCS_CHECK(nh_docs_addName(&Workload_p->bins_pp, &Workload_p->bins, Context_p->name_p))
        }
    }

NH_DOCS_END(NH_DOCS_SUCCESS)
}

static NH_DOCS_RESULT nh_docs_updateCommit(
    nh_docs_Tagger *Tagger_p)
{
NH_DOCS_BEGIN()
 
    if (Tagger_p->Commit_p->workloads && Tagger_p->Commit_p->Workloads_p) {
        nh_core_free(Tagger_p->Commit_p->Workloads_p);
        Tagger_p->Commit_p->Workloads_p = NULL;
        Tagger_p->Commit_p->workloads = 0;
    }

    Tagger_p->Commit_p->workloads =
        nh_docs_scopeSet(Tagger_p, NH_DOCS_SCOPE_PATCH)
      + nh_docs_scopeSet(Tagger_p, NH_DOCS_SCOPE_MINOR)
      + nh_docs_scopeSet(Tagger_p, NH_DOCS_SCOPE_MAJOR)
      + nh_docs_scopeSet(Tagger_p, NH_DOCS_SCOPE_API);

    Tagger_p->Commit_p->Workloads_p = 
        nh_core_allocate(sizeof(nh_docs_Workload)*Tagger_p->Commit_p->workloads);

    memset(Tagger_p->Commit_p->Workloads_p, 0, sizeof(nh_docs_Workload)*Tagger_p->Commit_p->workloads);

    nh_docs_Workload *Workload_p = Tagger_p->Commit_p->Workloads_p;
    for (NH_DOCS_SCOPE_E scope = 0; scope < NH_DOCS_SCOPE_API+1; ++scope) {
        if (nh_docs_scopeSet(Tagger_p, scope)) {
            nh_docs_setWorkload(Tagger_p, Workload_p++, scope);
        }
    }

NH_DOCS_END(NH_DOCS_SUCCESS)
}

// RENDER COMMIT ===================================================================================

static NH_DOCS_RESULT nh_docs_setNextGlyph(
    nh_Array *Glyphs_p, NH_BYTE codepoint)
{
NH_DOCS_BEGIN()

    nh_tty_Glyph *Glyph_p = nh_core_incrementArray(Glyphs_p);
    NH_DOCS_CHECK_MEM(Glyph_p)
    memset(Glyph_p, 0, sizeof(nh_tty_Glyph));
    Glyph_p->codepoint = codepoint;
 
NH_DOCS_END(NH_DOCS_SUCCESS)
}

static NH_DOCS_RESULT nh_docs_renderCommit(
    nh_docs_Tagger *Tagger_p)
{
NH_DOCS_BEGIN()

    nh_docs_Changelog Changelog;
    Changelog.Commits_p = Tagger_p->Commit_p;
    Changelog.commits = 1;

    NH_BYTE changelog_p[2048];
    memset(changelog_p, 0, 2048);
    nh_docs_stringifyChangelog(&Changelog, changelog_p, 2047);
    for (int i = 0; i < strlen(changelog_p); ++i) {
        nh_docs_setNextGlyph(&Tagger_p->Glyphs, changelog_p[i]);
    }

NH_DOCS_END(NH_DOCS_SUCCESS)
}

// RENDER CONTEXTS =================================================================================

static int nh_docs_getContextsWidth(
    nh_docs_Tagger *Tagger_p)
{
NH_DOCS_BEGIN()

    int max = 0;
    for (int i = 0; i < Tagger_p->Lines.length; ++i) {
        if (((nh_Array*)Tagger_p->Lines.p)[i].length > max) {
            max = ((nh_Array*)Tagger_p->Lines.p)[i].length;
        }
    }

NH_DOCS_END(max)
}

static NH_DOCS_RESULT nh_docs_setNextLine(
    nh_Array *Lines_p, NH_BYTE *string_p, NH_BOOL reverse, int offset)
{
NH_DOCS_BEGIN()

    nh_Array *Glyphs_p = nh_core_incrementArray(Lines_p);
    NH_DOCS_CHECK_MEM(Glyphs_p)

    *Glyphs_p = nh_core_initArray(sizeof(nh_tty_Glyph), 255);

    for (size_t i = 0; i < offset; ++i) {
        nh_tty_Glyph *Glyph_p = nh_core_incrementArray(Glyphs_p);
        NH_DOCS_CHECK_MEM(Glyph_p)
        memset(Glyph_p, 0, sizeof(nh_tty_Glyph));
        Glyph_p->codepoint = reverse ? '-' : ' ';
    }

    NH_BYTE *p = "[undefined]";
    if (!strlen(string_p)) {string_p = p;}

    for (size_t i = 0; i < strlen(string_p); ++i) {
        if (string_p[i] == '\n') {continue;}
        nh_tty_Glyph *Glyph_p = nh_core_incrementArray(Glyphs_p);
        NH_DOCS_CHECK_MEM(Glyph_p)
        memset(Glyph_p, 0, sizeof(nh_tty_Glyph));
        Glyph_p->codepoint = string_p[i];
    }
 
NH_DOCS_END(NH_DOCS_SUCCESS)
}

static nh_docs_Diff *nh_docs_getSelectedDiff(
    nh_docs_Tagger *Tagger_p)
{
NH_DOCS_BEGIN()

    for (int c = 0, index = 0; c < Tagger_p->contexts; ++c) {
        nh_docs_Context *Context_p = &Tagger_p->Contexts_p[c];
        if (!Context_p->diffs) {continue;}
        if (index++ == Tagger_p->indexx) {NH_DOCS_END(NULL)}
        for (int f = 0; f < Context_p->diffs; ++f) {
            if (index++ == Tagger_p->indexx) {NH_DOCS_END(Context_p->Diffs_pp[f])}
        }
    }
 
NH_DOCS_END(NULL)
}

static nh_docs_Context *nh_docs_getSelectedContext(
    nh_docs_Tagger *Tagger_p)
{
NH_DOCS_BEGIN()

    for (int c = 0, index = 0; c < Tagger_p->contexts; ++c) {
        nh_docs_Context *Context_p = &Tagger_p->Contexts_p[c];
        if (!Context_p->diffs) {continue;}
        if (index++ == Tagger_p->indexx) {NH_DOCS_END(Context_p)}
        for (int f = 0; f < Context_p->diffs; ++f) {
            if (index++ == Tagger_p->indexx) {NH_DOCS_END(Context_p)}
        }
    }
 
NH_DOCS_END(NULL)
}

static NH_DOCS_RESULT nh_docs_renderContexts(
    nh_docs_Tagger *Tagger_p)
{
NH_DOCS_BEGIN()

    int index = Tagger_p->indexx+1;

    for (int c = 0; c < Tagger_p->contexts; ++c) {
        if (!Tagger_p->Contexts_p[c].diffs) {continue;}
        nh_docs_Context *Context_p = Tagger_p->Contexts_p+c;
        NH_BYTE context_p[1024] = {0};
        switch (Tagger_p->Contexts_p[c].type) {
            case NH_DOCS_CONTEXT_LIBRARY : 
                sprintf(context_p, "lib %s [%s]", Context_p->name_p, nh_docs_getScopeString(Context_p->Workload.scope)); break;
            case NH_DOCS_CONTEXT_BINARY : 
                sprintf(context_p, "bin %s [%s]", Context_p->name_p, nh_docs_getScopeString(Context_p->Workload.scope)); break;
            default :
                sprintf(context_p, "??? %s [%s]", "[undefined]", nh_docs_getScopeString(Context_p->Workload.scope)); break;
        }
        nh_docs_setNextLine(&Tagger_p->Lines, context_p, --index == 0, 1);
        for (int f = 0; f < Tagger_p->Contexts_p[c].diffs; ++f) {
            --index;
            if (nh_docs_getSelectedContext(Tagger_p) == &Tagger_p->Contexts_p[c]) {
                NH_BYTE name_p[255] = {0};
                sprintf(name_p, "> %s",  Tagger_p->Contexts_p[c].Diffs_pp[f]->name_p);
                nh_docs_setNextLine(&Tagger_p->Lines, name_p, index == 0, 1);
            }
        }
    }
 
NH_DOCS_END(NH_DOCS_SUCCESS)
}

// UPDATE ==========================================================================================

static NH_TTY_RESULT nh_docs_updateTagger(
    nh_tty_Program *Program_p)
{
NH_DOCS_BEGIN()

    nh_docs_Tagger *Tagger_p = Program_p->handle_p;

    if (Tagger_p->Request.update) {
        NH_DOCS_CHECK_2(NH_TTY_ERROR_BAD_STATE, nh_docs_updateCommit(Tagger_p))
        Tagger_p->Request.update = NH_FALSE;
        Program_p->refresh = NH_TRUE;
    }
    if (Tagger_p->Request.render) {
        nh_core_freeArray(&Tagger_p->Glyphs);
        for (int i = 0; i < Tagger_p->Lines.length; ++i) {
            nh_core_freeArray(((nh_Array*)Tagger_p->Lines.p)+i);
        }
        nh_core_freeArray(&Tagger_p->Lines);
        Tagger_p->Glyphs = nh_core_initArray(sizeof(nh_tty_Glyph), 255);
        Tagger_p->Lines = nh_core_initArray(sizeof(nh_Array), 255);
        NH_DOCS_CHECK_2(NH_TTY_ERROR_BAD_STATE, nh_docs_renderCommit(Tagger_p))
        NH_DOCS_CHECK_2(NH_TTY_ERROR_BAD_STATE, nh_docs_renderContexts(Tagger_p))
        Tagger_p->Request.render = NH_FALSE;
        Program_p->refresh = NH_TRUE;
    }

NH_DOCS_END(NH_TTY_SUCCESS)
}

// DRAW ============================================================================================

static NH_TTY_RESULT nh_docs_drawTaggerRow(
    nh_tty_Program *Program_p, nh_tty_Glyph *Glyphs_p, int width, int height, int row)
{
NH_DOCS_BEGIN()

    nh_docs_Tagger *Tagger_p = Program_p->handle_p;

    if (row == 0) {
        Tagger_p->tmp = 0;
        Tagger_p->tmp2 = Tagger_p->offset+4;
        Tagger_p->draw = 0;
        NH_DOCS_END(NH_TTY_SUCCESS)
    }

    width -= 2;
    Glyphs_p++;

    if (Tagger_p->draw == 0) {
        int offset = (row-1) * width;
        if (Tagger_p->Glyphs.length <= offset) {
            Tagger_p->draw = 1;
            NH_DOCS_END(NH_TTY_SUCCESS)
        }
        int count = Tagger_p->Glyphs.length - offset > width ? width : Tagger_p->Glyphs.length - offset;
        memcpy(Glyphs_p, ((nh_tty_Glyph*)Tagger_p->Glyphs.p)+offset, sizeof(nh_tty_Glyph)*count);
    }

     if (Tagger_p->draw == 1) {
        nh_Array *Line_p = nh_core_getFromArray(&Tagger_p->Lines, Tagger_p->tmp);
        if (!Line_p) {NH_DOCS_END(NH_TTY_SUCCESS)}
        for (int i = 0; i < Line_p->length && i < width; ++i) {
            Glyphs_p[i] = ((nh_tty_Glyph*)Line_p->p)[i];;
        }

        nh_docs_Diff *Diff_p = nh_docs_getSelectedDiff(Tagger_p);
        if (Diff_p) {
            for (int i = 0, j = nh_docs_getContextsWidth(Tagger_p)+2; Diff_p->lines > Tagger_p->tmp2 && i < strlen(Diff_p->lines_pp[Tagger_p->tmp2]) && j < width; ++i, ++j) {
                Glyphs_p[j].codepoint = Diff_p->lines_pp[Tagger_p->tmp2][i] == '\n' 
                    ? 0 : Diff_p->lines_pp[Tagger_p->tmp2][i];
            }
        }

        Tagger_p->tmp++;
        Tagger_p->tmp2++;
    }

NH_DOCS_END(NH_TTY_SUCCESS)
}

// INPUT ===========================================================================================

static int nh_docs_getTotalLines(
    nh_docs_Tagger *Tagger_p)
{
NH_DOCS_BEGIN()

    int count = 0;
    for (int i = 0; i < Tagger_p->contexts; ++i) {
        if (!Tagger_p->Contexts_p[i].diffs) {continue;}
        count += Tagger_p->Contexts_p[i].diffs + 1;
    }

NH_DOCS_END(count)
}

static NH_TTY_RESULT nh_docs_handleTaggerInput(
    nh_tty_Program *Program_p, nh_wsi_Event Event)
{
NH_DOCS_BEGIN()

    nh_docs_Tagger *Tagger_p = Program_p->handle_p;

    if (Event.type != NH_WSI_EVENT_KEYBOARD) {NH_DOCS_END(NH_TTY_SUCCESS)}
    if (Event.Keyboard.trigger != NH_WSI_TRIGGER_PRESS) {NH_DOCS_END(NH_TTY_SUCCESS)}

    NH_ENCODING_UTF32 c = Event.Keyboard.codepoint;

    switch (c)
    {
        case 'w' :
            if (Tagger_p->indexx > 0) {Tagger_p->indexx--;}
            Tagger_p->offset = 0;
            Tagger_p->Request.render = NH_TRUE;
            break;
        case 's' :
            if (Tagger_p->indexx < nh_docs_getTotalLines(Tagger_p)-1) {Tagger_p->indexx++;}
            Tagger_p->offset = 0;
            Tagger_p->Request.render = NH_TRUE;
            break;
        case 'a' :
            nh_docs_Context *Context_p = nh_docs_getSelectedContext(Tagger_p);
            if (Context_p->Workload.scope > 0) {
                Context_p->Workload.scope--;
                Tagger_p->Request.update = NH_TRUE;
                Tagger_p->Request.render = NH_TRUE;
            }
            break;
        case 'd' :
            Context_p = nh_docs_getSelectedContext(Tagger_p);
            if (Context_p->Workload.scope < 3) {
                Context_p->Workload.scope++;
                Tagger_p->Request.update = NH_TRUE; 
                Tagger_p->Request.render = NH_TRUE;
            }
            break;
        case 'k' :
            if (Tagger_p->offset > 0) {Tagger_p->offset--;};
            Tagger_p->Request.render = NH_TRUE;
            break;
        case 'j' :
            Tagger_p->offset++;
            Tagger_p->Request.render = NH_TRUE;
            break;
        case 'l' :
            Tagger_p->select++;
            Tagger_p->Request.render = NH_TRUE;
            break;
        case 'h' :
            Tagger_p->select--;
            Tagger_p->Request.render = NH_TRUE;
            break;
        case 127 :
            Tagger_p->Request.render = NH_TRUE;
            break;
    }

NH_DOCS_END(NH_TTY_SUCCESS)
}

// COMMANDS ========================================================================================

typedef enum NH_DOCS_TAGGER_COMMAND_E {
    NH_DOCS_TAGGER_COMMAND_TAG = 0,
    NH_DOCS_TAGGER_COMMAND_UNTAG,
    NH_DOCS_TAGGER_COMMAND_LOAD,
    NH_DOCS_TAGGER_COMMAND_E_COUNT,
} NH_DOCS_TAGGER_COMMAND_E;

static NH_TTY_RESULT nh_docs_handleTaggerCommand(
    nh_tty_Program *Program_p, nh_List *Arguments_p)
{
NH_DOCS_BEGIN()

    nh_docs_Tagger *Tagger_p = Program_p->handle_p;

    NH_BYTE commit_p[2048];
    memset(commit_p, 0, 2048);
    nh_docs_stringifyCommit(Tagger_p->Commit_p, commit_p, 2047);

    NH_BYTE tagName_p[255];
    sprintf(tagName_p, "tmp_%s", Tagger_p->Commit_p->id_p);

    NH_BYTE command_p[4096] = {0};
    FILE *proc_p = NULL;

    switch (Program_p->command)
    {
         case NH_DOCS_TAGGER_COMMAND_TAG :
             sprintf(command_p, "git tag -a %s %s -m \"%s\"", tagName_p, Tagger_p->Commit_p->id_p, commit_p);
             proc_p = popen(command_p, "r");
             NH_DOCS_CHECK_MEM_2(NH_TTY_ERROR_BAD_STATE, proc_p)
             pclose(proc_p);
             break;
         case NH_DOCS_TAGGER_COMMAND_UNTAG :
             sprintf(command_p, "git tag -d %s", tagName_p);
             proc_p = popen(command_p, "r");
             NH_DOCS_CHECK_MEM_2(NH_TTY_ERROR_BAD_STATE, proc_p)
             pclose(proc_p);
             break;
         case NH_DOCS_TAGGER_COMMAND_LOAD :
            if (Arguments_p && Arguments_p->size == 1) {
                 // TODO Free stuff.
                 nh_encoding_UTF32String *Index_p = Arguments_p->pp[0];
                 nh_encoding_UTF8String Index = nh_encoding_encodeUTF8(Index_p->p, Index_p->length);
                 Tagger_p->Commit_p = nh_docs_createCommitUsingGit(Index.p);
                 NH_DOCS_CHECK_NULL_2(NH_TTY_ERROR_BAD_STATE, Tagger_p->Commit_p)
                 Tagger_p->Contexts_p = nh_docs_getContextsUsingGit(&Tagger_p->contexts, Index.p);
                 NH_DOCS_CHECK_NULL_2(NH_TTY_ERROR_BAD_STATE, Tagger_p->Contexts_p)
                 Tagger_p->Request.render = NH_TRUE;
             }
             break;
    }

NH_DOCS_END(NH_TTY_SUCCESS)
}

// PROTOTYPE =======================================================================================

static NH_DOCS_RESULT nh_docs_destroyTaggerPrototype(
    nh_tty_ProgramPrototype *Prototype_p)
{
NH_DOCS_BEGIN()

    nh_encoding_freeUTF32(&Prototype_p->Name);
    nh_encoding_freeUTF32(&Prototype_p->CommandNames_p[0]);
    nh_core_free(Prototype_p->CommandNames_p);
    nh_core_free(Prototype_p);

NH_DOCS_END(NH_DOCS_SUCCESS)
}

nh_tty_ProgramPrototype *nh_docs_createTaggerPrototype()
{
NH_DOCS_BEGIN()

    nh_tty_ProgramPrototype *Prototype_p = nh_core_allocate(sizeof(nh_tty_ProgramPrototype));
    NH_DOCS_CHECK_MEM_2(NULL, Prototype_p)

    memset(Prototype_p, 0, sizeof(nh_tty_ProgramPrototype));

    Prototype_p->Callbacks.init_f = nh_docs_initTagger;
    Prototype_p->Callbacks.draw_f = nh_docs_drawTaggerRow;
    Prototype_p->Callbacks.handleInput_f = nh_docs_handleTaggerInput;
    Prototype_p->Callbacks.update_f = nh_docs_updateTagger;
    Prototype_p->Callbacks.handleCommand_f = nh_docs_handleTaggerCommand;
    Prototype_p->Callbacks.destroyPrototype_f = nh_docs_destroyTaggerPrototype;
    Prototype_p->Callbacks.destroy_f = nh_docs_destroyTagger;

    NH_ENCODING_UTF32 name_p[7] = {'t', 'a', 'g', 'g', 'e', 'r'};
    Prototype_p->Name = nh_encoding_initUTF32(6);
    NH_ENCODING_CHECK_2(NULL, nh_encoding_appendUTF32(&Prototype_p->Name, name_p, 6))

    nh_encoding_UTF32String *CommandNames_p =
        nh_core_allocate(sizeof(nh_encoding_UTF32String)*NH_DOCS_TAGGER_COMMAND_E_COUNT);
    NH_DOCS_CHECK_MEM_2(NULL, CommandNames_p)

    NH_ENCODING_UTF32 command1_p[3] = {'t', 'a', 'g'};
    NH_ENCODING_UTF32 command2_p[5] = {'u', 'n', 't', 'a', 'g'};
    NH_ENCODING_UTF32 command3_p[4] = {'l', 'o', 'a', 'd'};

    CommandNames_p[NH_DOCS_TAGGER_COMMAND_TAG] = nh_encoding_initUTF32(3);
    CommandNames_p[NH_DOCS_TAGGER_COMMAND_UNTAG] = nh_encoding_initUTF32(5);
    CommandNames_p[NH_DOCS_TAGGER_COMMAND_LOAD] = nh_encoding_initUTF32(4);

    NH_ENCODING_CHECK_2(NULL, nh_encoding_appendUTF32(&CommandNames_p[NH_DOCS_TAGGER_COMMAND_TAG], command1_p, 3))
    NH_ENCODING_CHECK_2(NULL, nh_encoding_appendUTF32(&CommandNames_p[NH_DOCS_TAGGER_COMMAND_UNTAG], command2_p, 5))
    NH_ENCODING_CHECK_2(NULL, nh_encoding_appendUTF32(&CommandNames_p[NH_DOCS_TAGGER_COMMAND_LOAD], command3_p, 4))

    Prototype_p->CommandNames_p = CommandNames_p;
    Prototype_p->commands = NH_DOCS_TAGGER_COMMAND_E_COUNT;

NH_DOCS_END(Prototype_p)
}

