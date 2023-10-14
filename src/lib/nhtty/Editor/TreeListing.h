#ifndef NH_TTY_TREE_LISTING_H
#define NH_TTY_TREE_LISTING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "FileEditor.h"

#include "../Common/Includes.h"
#include "../TTY/Program.h"

#include "../../nhcore/Util/Time.h"
#include "../../nhcore/Util/List.h"
#include "../../nhcore/Util/String.h"

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_TreeListingNodeOverflow {
        int offset;
        nh_SystemTime LastShift;
    } nh_tty_TreeListingNodeOverflow;

    typedef struct nh_tty_TreeListingNode {
        NH_BOOL open;
        NH_BOOL unsaved;
        nh_tty_File *File_p;
        nh_encoding_UTF32String Path;
        nh_List Children; 
        struct nh_tty_TreeListingNode *Parent_p;
        nh_tty_TreeListingNodeOverflow Overflow;
    } nh_tty_TreeListingNode;
 
    typedef struct nh_tty_TreeListing {
        nh_tty_TreeListingNode *Root_p;
        NH_BOOL preview;
        NH_BOOL dirty;
        nh_tty_TreeListingNode *Preview_p;
        int current;
        nh_Array RenderLines;
        NH_BYTE wrkDir_p[2048];
    } nh_tty_TreeListing;

    typedef struct nh_tty_TreeListingView {
        int width;
        int current;
        int offset;
        int maxOffset;
    } nh_tty_TreeListingView;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_TreeListing nh_tty_initTreeListing(
    );

    NH_TTY_RESULT nh_tty_freeTreeListing(
        nh_tty_TreeListing *TreeListing_p
    );

    NH_TTY_RESULT nh_tty_handleTreeListingInput(
        nh_tty_Program *Program_p, NH_ENCODING_UTF32 c
    );

    NH_TTY_RESULT nh_tty_drawTreeListingRow(
        nh_tty_Program *Program_p, nh_tty_Glyph *Glyph_p, int width, int height, int row
    );

    NH_TTY_RESULT nh_tty_setTreeListingCursor(
        nh_tty_Program *Program_p, nh_tty_File *File_p 
    );

    nh_tty_TreeListingNode *nh_tty_insertTreeListingNode(
        nh_tty_TreeListing *Listing_p, NH_ENCODING_UTF32 *name_p, int length
    );

/** @} */

#endif 
