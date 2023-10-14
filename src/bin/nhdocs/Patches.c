// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

// INCLUDE =========================================================================================

#include "Patches.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// FUNCTIONS =======================================================================================

// You must free the result if result is non-NULL.
static NH_BYTE *replaceString(
    NH_BYTE *orig, NH_BYTE *rep, NH_BYTE *with)
{
    NH_BYTE *result; // the return string
    NH_BYTE *ins;    // the next insert point
    NH_BYTE *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

int createPatchPages(
    NH_BYTE *path_p, NH_BYTE *source_p, NH_BYTE *destination_p)
{
    nh_docs_Changelog Changelog = nh_docs_parseChangelog(path_p);

    for (int i = 0; i < Changelog.commits; ++i)
    {
        if (!Changelog.Commits_p[i].id_p) {continue;}

        NH_BYTE path_p[255];
        memset(path_p, 0, 255);
        sprintf(path_p, "%s/%s.patch", source_p, Changelog.Commits_p[i].id_p);

        NH_BYTE *patch_p = nh_api_getFileData(path_p, NULL);
        if (!patch_p) {continue;}
     
        NH_BYTE *tmp_p = replaceString(patch_p, "<", "&lt");
        if (tmp_p) {
	    free(patch_p);
	    patch_p = tmp_p;
	}

        tmp_p = replaceString(patch_p, "@endhtmlonly", "at_endhtmlonly");
        if (tmp_p) {
	    free(patch_p);
	    patch_p = tmp_p;
	}

        NH_BYTE *md_p = malloc(sizeof(NH_BYTE)*(strlen(patch_p)+2048));
        if (!md_p) {return 1;}

        memset(md_p, 0, sizeof(NH_BYTE)*(strlen(patch_p)+2048));
        sprintf(md_p, "@htmlonly\n\n<pre>\n%s\n</pre>\n@endhtmlonly", patch_p);

        memset(path_p, 0, 255);
        sprintf(path_p, "%s/%s.md", destination_p, Changelog.Commits_p[i].id_p);
     
        nh_api_writeBytesToFile(path_p, md_p);
     
        free(md_p);
        free(patch_p);
    }

    nh_docs_freeChangelog(&Changelog);

    return 0;
}

