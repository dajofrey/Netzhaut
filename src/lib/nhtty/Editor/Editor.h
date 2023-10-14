#ifndef NH_TTY_EDITOR_H
#define NH_TTY_EDITOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "TreeListing.h"
#include "FileEditor.h"

#include "../TTY/Program.h"
#include "../Common/Includes.h"

#include "../../nhcore/Util/String.h"

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_EditorView {
        nh_tty_FileEditorView FileEditor;
        nh_tty_TreeListingView TreeListing;
        int height;
    } nh_tty_EditorView;

    typedef struct nh_tty_Editor {
        nh_tty_EditorView View;
        int focus;
        NH_BOOL insertMode;
        NH_BOOL treeListing;
        nh_tty_TreeListing TreeListing;
        nh_tty_FileEditor FileEditor;
        nh_SystemTime LastUpdate;
        double updateIntervalInSeconds;
    } nh_tty_Editor;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_ProgramPrototype *nh_tty_createEditorPrototype(
    );

/** @} */

#endif 
