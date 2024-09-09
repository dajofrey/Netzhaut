// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Inline.h"
#include "Block.h"
#include "Float.h"

#include "../Main/FormattingContext.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/Array.h"
#include "../../nh-core/System/Memory.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// INLINE FORMATTING CONTEXT =======================================================================

typedef struct nh_css_InlineFormattingContext {
    nh_css_Canvas *Canvas_p;
    nh_css_LineBox *LineBox_p;
    NH_PIXEL height;
    int firstLineBox;
    int lineBoxes;
} nh_css_InlineFormattingContext;

static NH_CSS_RESULT nh_css_createInlineFormattingContext(
    nh_css_Canvas *Canvas_p, nh_css_InlineFormattingContext *Context_p, nh_css_PixelBox Container) 
{
NH_CSS_BEGIN()

    Context_p->Canvas_p  = Canvas_p;
    Context_p->firstLineBox = Canvas_p->LineBoxes.length;
    Context_p->lineBoxes = 1;

    Context_p->LineBox_p = nh_core_incrementArray(&Canvas_p->LineBoxes);
    NH_CSS_CHECK_MEM(Context_p->LineBox_p)

    Context_p->LineBox_p->shortened    = NH_FALSE;
    Context_p->LineBox_p->Size.width   = Container.Size.width;
    Context_p->LineBox_p->Size.height  = 0;
    Context_p->LineBox_p->Position.x   = Container.Position.x;
    Context_p->LineBox_p->Position.y   = Container.Position.y;
    Context_p->LineBox_p->currentX     = Context_p->LineBox_p->Position.x;
    Context_p->LineBox_p->RootInline_p = NULL;

    Context_p->height = 0;

NH_CSS_END(NH_CSS_SUCCESS)
}

static NH_CSS_RESULT nh_css_addLineBox(
    nh_css_InlineFormattingContext *Context_p) 
{
NH_CSS_BEGIN()

    nh_css_LineBox *NewLineBox_p = nh_core_incrementArray(&Context_p->Canvas_p->LineBoxes);
    NH_CSS_CHECK_MEM(NewLineBox_p)

    NewLineBox_p->shortened    = NH_FALSE;
    NewLineBox_p->Size.width   = Context_p->LineBox_p->Size.width;
    NewLineBox_p->Size.height  = 0;
    NewLineBox_p->Position.x   = Context_p->LineBox_p->Position.x;
    NewLineBox_p->Position.y   = Context_p->LineBox_p->Position.y + Context_p->LineBox_p->Size.height;
    NewLineBox_p->currentX     = NewLineBox_p->Position.x;
    NewLineBox_p->RootInline_p = NULL;

    Context_p->lineBoxes++;
    Context_p->LineBox_p = NewLineBox_p;

NH_CSS_END(NH_CSS_SUCCESS)
}

static void nh_css_updateInlineFormattingContextHeight(
    nh_css_InlineFormattingContext *Context_p) 
{
NH_CSS_BEGIN()

    NH_PIXEL height = 0;
    for (int i = Context_p->firstLineBox; i < Context_p->lineBoxes; ++i) {
        nh_css_LineBox *LineBox_p = &((nh_css_LineBox*)Context_p->Canvas_p->LineBoxes.p)[i];
        height += LineBox_p->Size.height;
    }

    Context_p->height = height;

NH_CSS_SILENT_END()
}

// ARRANGE TEXT ====================================================================================

static NH_CSS_RESULT nh_css_computeTextFragment(
    nh_css_Fragment *Fragment_p)
{
NH_CSS_BEGIN()

    NH_CSS_CHECK(nh_css_computeTextValues(Fragment_p))

    Fragment_p->Block.Size.width = Fragment_p->Text.Values.textWidth;
    Fragment_p->Block.Size.height = Fragment_p->Text.Values.textHeight;

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}

static NH_CSS_RESULT nh_css_updateTextSegments(
    nh_css_Fragment *Fragment_p, unsigned int newLength)
{
NH_CSS_BEGIN()

    unsigned int count = 0;
    for (int i = 0; i < Fragment_p->Text.Values.Text.Segments.length; ++i) {
        nh_gfx_TextSegment *Segment_p = &((nh_gfx_TextSegment*)Fragment_p->Text.Values.Text.Segments.p)[i];
        count += Segment_p->length;
        if (count > newLength) {
            Segment_p->length -= count - newLength;
            count = newLength;
        }
    }

NH_CSS_END(NH_CSS_SUCCESS)
}

static nh_css_Fragment *nh_css_splitTextFragment(
    nh_css_Fragment *Fragment_p, unsigned int newLength)
{
NH_CSS_BEGIN()

    unsigned int fullLength = Fragment_p->Text.length;
    Fragment_p->Text.length = newLength;

    NH_PIXEL prevWidth = Fragment_p->Block.Size.width;

    // recompute text fragment 
    NH_CSS_CHECK_2(NULL, nh_css_computeTextFragment(Fragment_p))

//    NH_CSS_CHECK_2(NULL, nh_css_updateTextSegments(Fragment_p, newLength))

    nh_css_Fragment *NewFragment_p = nh_core_allocate(sizeof(nh_css_Fragment));
    NH_CSS_CHECK_MEM_2(NULL, NewFragment_p)

    *NewFragment_p = nh_css_initFragment(Fragment_p->Node_p, NULL, Fragment_p->type, NULL);
    NewFragment_p->Text.text_p = Fragment_p->Text.text_p + newLength;
    NewFragment_p->Text.length = fullLength - newLength;

NH_CSS_END(NewFragment_p)
}

static NH_CSS_RESULT nh_css_arrangeUsingLineBox(
    nh_css_InlineFormattingContext *Context_p, nh_css_Fragment *Fragment_p)
{
NH_CSS_BEGIN()

    Fragment_p->Block.Position.x = Context_p->LineBox_p->currentX;
    Fragment_p->Block.Position.y = Context_p->LineBox_p->Position.y;

    NH_CSS_CHECK(nh_css_computeTextFragment(Fragment_p))

    Context_p->LineBox_p->currentX += Fragment_p->Block.Size.width;

    if (Context_p->LineBox_p->Size.height < Fragment_p->Block.Size.height) {
        Context_p->LineBox_p->Size.height = Fragment_p->Block.Size.height;
    }
    nh_css_updateInlineFormattingContextHeight(Context_p);

    if (Context_p->LineBox_p->currentX > (Context_p->LineBox_p->Position.x + Context_p->LineBox_p->Size.width)) {
        NH_CSS_END(NH_CSS_SIGNAL_SPLIT_INLINE_BOX)
    }

NH_CSS_END(NH_CSS_SUCCESS)
}

static NH_CSS_RESULT nh_css_arrangeTextFragment(
    nh_css_Canvas *Canvas_p, nh_css_InlineFormattingContext *Context_p, nh_css_Fragment **Fragment_pp)
{
NH_CSS_BEGIN()

    nh_css_LineBox *LineBox_p = Context_p->LineBox_p;
    nh_css_Fragment *Fragment_p = *Fragment_pp;

    NH_CSS_RESULT result = nh_css_arrangeUsingLineBox(Context_p, *Fragment_pp);

    if (result == NH_CSS_SIGNAL_SPLIT_INLINE_BOX) 
    {
        unsigned int newLength = 0;

        do {
            newLength = nh_gfx_getFittingTextLength(
                &(*Fragment_pp)->Text.Values.Text,
                Context_p->LineBox_p->Size.width - ((Context_p->LineBox_p->currentX - (*Fragment_pp)->Block.Size.width) - Context_p->LineBox_p->Position.x)
            );
            if (newLength == 0 && Context_p->LineBox_p->shortened) {
                nh_css_shiftLineBoxDownward(Canvas_p, Context_p->LineBox_p);
                nh_css_offsetLineBoxFromFloats(Canvas_p, Context_p->LineBox_p);
                NH_CSS_END(nh_css_arrangeTextFragment(Canvas_p, Context_p, Fragment_pp))
            }
        }
        while (newLength == 0 && Context_p->LineBox_p->shortened);

        *Fragment_pp = nh_css_splitTextFragment(*Fragment_pp, newLength);
        NH_CSS_CHECK_MEM(*Fragment_pp)

        NH_CSS_CHECK(nh_css_addLineBox(Context_p))
    }

NH_CSS_END(result)
}

// ARRANGE INLINE BOX ==============================================================================

nh_css_Fragment *nh_css_splitInlineBox(
    nh_css_Fragment *Box_p, int currentIndex, nh_css_Fragment *NewChild_p)
{
NH_CSS_BEGIN()

    int offset = currentIndex + 1;

    nh_css_Fragment *NewFragment_p = nh_core_allocate(sizeof(nh_css_Fragment));
    NH_CSS_CHECK_MEM_2(NULL, NewFragment_p)

    *NewFragment_p = nh_css_initFragment(Box_p->Node_p, Box_p->Parent_p, Box_p->type, NULL);

    nh_core_appendToList(&NewFragment_p->Children, NewChild_p);
    NewChild_p->Parent_p = NewFragment_p;

    for (int i = offset; i < Box_p->Children.size; ++i) {
        nh_core_appendToList(&NewFragment_p->Children, Box_p->Children.pp[i]);
    }
    for (int i = offset; i < Box_p->Children.size;) {
        nh_core_removeFromList(&Box_p->Children, NH_FALSE, Box_p->Children.size - 1);
    }

NH_CSS_END(NewFragment_p)
}

static void nh_css_updateInlineBoxSize(
    nh_css_Fragment *Fragment_p, nh_css_Fragment *Child_p)
{
NH_CSS_BEGIN()

    Fragment_p->Block.Size.width += Child_p->Block.Size.width;
    if (Fragment_p->Block.Size.height < Child_p->Block.Size.height) {
        Fragment_p->Block.Size.height = Child_p->Block.Size.height;
    }

NH_CSS_SILENT_END()
}

static NH_CSS_RESULT nh_css_arrangeInlineBox(
    nh_css_Canvas *Canvas_p, nh_css_InlineFormattingContext *Context_p, nh_css_Fragment **Fragment_pp)
{
NH_CSS_BEGIN()

    if (nh_css_startsFormattingContext((*Fragment_pp)->Node_p, NH_CSS_FORMATTING_CONTEXT_INLINE)) {
        Context_p->LineBox_p->RootInline_p = *Fragment_pp;
        if (!Canvas_p->_float) {nh_css_offsetLineBoxFromFloats(Canvas_p, Context_p->LineBox_p);}
    }

    (*Fragment_pp)->Block.Position.x = Context_p->LineBox_p->currentX;
    (*Fragment_pp)->Block.Position.y = Context_p->LineBox_p->Position.y;

    NH_CSS_CHECK(nh_css_computeBoxValues(*Fragment_pp))

    for (int i = 0; i < (*Fragment_pp)->Children.size; ++i) 
    {
        nh_css_Fragment *Child_p = (*Fragment_pp)->Children.pp[i];
        nh_css_Fragment *PrevChild_p = Child_p;

        NH_CSS_RESULT result;
        if (Child_p->type == NH_CSS_FRAGMENT_TEXT) {
            result = nh_css_arrangeTextFragment(Canvas_p, Context_p, &Child_p);
        }
        else if (nh_css_inPrimaryFormattingContext(Child_p->Node_p, NH_CSS_FORMATTING_CONTEXT_INLINE)) {
            result = nh_css_arrangeInlineBox(Canvas_p, Context_p, &Child_p);
        }
        else {
            NH_CSS_END(NH_CSS_ERROR_BAD_STATE)
        }

        if (result) {
            if (result == NH_CSS_SIGNAL_SPLIT_INLINE_BOX) {
                nh_css_updateInlineBoxSize(*Fragment_pp, PrevChild_p);
                *Fragment_pp = nh_css_splitInlineBox(*Fragment_pp, i, Child_p);
            }
            NH_CSS_DIAGNOSTIC_END(result)
        }

        nh_css_updateInlineBoxSize(*Fragment_pp, Child_p);
    }

NH_CSS_END(NH_CSS_SUCCESS)
}

static void nh_css_alignTextRecursively(
    nh_css_Fragment *Fragment_p, NH_PIXEL value)
{
NH_CSS_BEGIN()

    Fragment_p->Block.Position.x += value;

    for (int j = 0; j < Fragment_p->Children.size; ++j) {
        nh_css_Fragment *Child_p = Fragment_p->Children.pp[j];
        nh_css_alignTextRecursively(Child_p, value);
    }

NH_CSS_SILENT_END()
}

static NH_CSS_RESULT nh_css_alignText(
    nh_css_InlineFormattingContext *Context_p)
{
NH_CSS_BEGIN()

    for (int i = Context_p->firstLineBox; i < Context_p->firstLineBox + Context_p->lineBoxes; ++i)
    {
        nh_css_LineBox *LineBox_p = &((nh_css_LineBox*)Context_p->Canvas_p->LineBoxes.p)[i];
        nh_css_Fragment *RootInline_p = LineBox_p->RootInline_p;

        if (RootInline_p->Box.Values.textAlignAll == NH_CSS_TEXT_ALIGN_CENTER) {
            NH_PIXEL diff = LineBox_p->Size.width - RootInline_p->Block.Size.width;
            if (diff > 0) {
                nh_css_alignTextRecursively(LineBox_p->RootInline_p, diff / 2);
            }
        }
        else if (RootInline_p->Box.Values.textAlignAll == NH_CSS_TEXT_ALIGN_RIGHT) {
            NH_PIXEL diff = LineBox_p->Size.width - RootInline_p->Block.Size.width;
            if (diff > 0) {
                nh_css_alignTextRecursively(LineBox_p->RootInline_p, diff);
            }
        }
    }

NH_CSS_END(NH_CSS_SUCCESS)
}

NH_CSS_RESULT nh_css_arrangeInlineFormattingContext(
    nh_css_Canvas *Canvas_p, nh_css_Fragment *Fragment_p, NH_PIXEL yOffset, int *newFragments_p)
{
NH_CSS_BEGIN()

    NH_CSS_CHECK(nh_css_computeBoxValues(Fragment_p))

    nh_css_PixelBox Container = nh_css_getContentBox(&Fragment_p->Parent_p->Block, &Fragment_p->Box.Values);
    Container.Position.y += yOffset;

//    NH_CSS_CHECK(nh_css_collapse(Fragment_p))

    nh_css_InlineFormattingContext Context;
    NH_CSS_CHECK(nh_css_createInlineFormattingContext(Canvas_p, &Context, Container))

    NH_CSS_RESULT result;
    nh_css_Fragment *RootInline_p = Fragment_p;
    nh_css_Fragment *PrevFragment_p = Fragment_p;

    while (result = nh_css_arrangeInlineBox(Canvas_p, &Context, &Fragment_p)) {
        if (result == NH_CSS_SIGNAL_SPLIT_INLINE_BOX) {
            int index = nh_core_getListIndex(&Fragment_p->Parent_p->Children, PrevFragment_p);
            nh_core_insertIntoList(&Fragment_p->Parent_p->Children, Fragment_p, index + 1);
            PrevFragment_p = Fragment_p;
            (*newFragments_p)++;
        }
        else {NH_CSS_DIAGNOSTIC_END(result)}
    }

    nh_css_alignText(&Context);

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}
