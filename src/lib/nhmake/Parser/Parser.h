#ifndef NH_MAKE_PARSER_PARSER_H
#define NH_MAKE_PARSER_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Tokenizer.h"

#include "../Core/File.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhmake_structs
 *  @{
 */

    typedef union nh_make_Definition nh_make_Definition;

    typedef struct nh_make_Block {
        NH_MAKE_DEFINITION type;
        unsigned int definitions;
        nh_make_Definition *Definitions_p;
    } nh_make_Block;

    typedef struct nh_make_Option {
        NH_MAKE_DEFINITION type;
        NH_MAKE_BOOL longOption;
        int arguments;
        NH_BYTE **arguments_pp;
        NH_BYTE *name_p;
        NH_BYTE *description_p;
        nh_make_Definition *Block_p;
    } nh_make_Option;

    typedef struct nh_make_If {
        NH_MAKE_DEFINITION type;
        NH_BYTE *string_p;
        nh_make_Definition *Block_p;
    } nh_make_If;

    typedef union nh_make_Definition {
        NH_MAKE_DEFINITION type;
        nh_make_Option Option;
        nh_make_Function Function;
        nh_make_Block Block;
        nh_make_If If;
    } nh_make_Definition;

    typedef struct nh_make_Parser {
        NH_MAKE_BOOL executed;
        NH_MAKE_BOOL expectExpression;
        unsigned int definitions;
        nh_make_Definition *Definitions_p;
    } nh_make_Parser;

    typedef struct nh_make_ParserArray {
        int length;
        nh_make_Parser *Parsers_p;
    } nh_make_ParserArray;

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    void nh_make_initParserArray(
        nh_make_ParserArray *Array_p
    );

    NH_MAKE_RESULT nh_make_appendParser(
        nh_make_ParserArray *Array_p, nh_make_File *File_p, NH_MAKE_BOOL showParseTree
    ); 

/** @} */

#endif // NH_MAKE_PARSER_PARSER_H
