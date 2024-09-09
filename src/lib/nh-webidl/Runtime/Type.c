// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Type.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CREATE ==========================================================================================

NH_WEBIDL_TYPE nh_webidl_getType(
    nh_webidl_ParseNode *Type_p)
{
NH_WEBIDL_BEGIN()

    if (!Type_p || Type_p->type != NH_WEBIDL_PARSE_NODE_TYPE) {NH_WEBIDL_END(-1)}

    if (((nh_webidl_ParseNode*)Type_p->Children.pp[0])->type == NH_WEBIDL_PARSE_NODE_SINGLE_TYPE)
    {
        nh_webidl_ParseNode *SingleType_p = Type_p->Children.pp[0];

        if (SingleType_p->Token_p != NULL && SingleType_p->Token_p->String.p[0] == '*') {
            NH_WEBIDL_END(NH_WEBIDL_TYPE_WILDCARD)
        }
        if (SingleType_p->Token_p != NULL && !strcmp(SingleType_p->Token_p->String.p, "any")) {
            NH_WEBIDL_END(NH_WEBIDL_TYPE_ANY)
        }

        if (SingleType_p->Children.size > 0) {
            if (((nh_webidl_ParseNode*)SingleType_p->Children.pp[0])->type == NH_WEBIDL_PARSE_NODE_DISTINGUISHABLE_TYPE)
            {
                nh_webidl_ParseNode *DistinguishableType_p = SingleType_p->Children.pp[0];
    
                if (DistinguishableType_p->Children.size > 0) {      
                    if (((nh_webidl_ParseNode*)DistinguishableType_p->Children.pp[0])->type == NH_WEBIDL_PARSE_NODE_PRIMITIVE_TYPE)
                    {
                        nh_webidl_ParseNode *PrimitiveType_p = DistinguishableType_p->Children.pp[0];

                        if (PrimitiveType_p->Token_p != NULL && !strcmp(PrimitiveType_p->Token_p->String.p, "undefined")) {
                            NH_WEBIDL_END(NH_WEBIDL_TYPE_UNDEFINED)
                        }
                        if (PrimitiveType_p->Token_p != NULL && !strcmp(PrimitiveType_p->Token_p->String.p, "boolean")) {
                            NH_WEBIDL_END(NH_WEBIDL_TYPE_BOOLEAN)
                        }
                        if (PrimitiveType_p->Token_p != NULL && !strcmp(PrimitiveType_p->Token_p->String.p, "byte")) {
                            NH_WEBIDL_END(NH_WEBIDL_TYPE_BYTE)
                        }
                        if (PrimitiveType_p->Token_p != NULL && !strcmp(PrimitiveType_p->Token_p->String.p, "octet")) {
                            NH_WEBIDL_END(NH_WEBIDL_TYPE_OCTET)
                        }
                        if (PrimitiveType_p->Token_p != NULL && !strcmp(PrimitiveType_p->Token_p->String.p, "bigint")) {
                            NH_WEBIDL_END(NH_WEBIDL_TYPE_BIGINT)
                        }

                    }
                }
            }
        }

    }
    else {

    }

NH_WEBIDL_END(-1)
}

size_t nh_webidl_getAllocationSize(
    NH_WEBIDL_TYPE type)
{
NH_WEBIDL_BEGIN()

    size_t typeSize = 0;

    switch (type)
    {
        case NH_WEBIDL_TYPE_WILDCARD            : typeSize = 0; break;
        case NH_WEBIDL_TYPE_ANY                 : typeSize = sizeof(nh_webidl_Any); break;
        case NH_WEBIDL_TYPE_UNDEFINED           : typeSize = 0; break;
        case NH_WEBIDL_TYPE_BOOLEAN             : typeSize = 0; break;
        case NH_WEBIDL_TYPE_BYTE                : typeSize = sizeof(NH_WEBIDL_BYTE); break;
        case NH_WEBIDL_TYPE_OCTET               : typeSize = sizeof(NH_WEBIDL_OCTET); break;
        case NH_WEBIDL_TYPE_SHORT               : typeSize = sizeof(NH_WEBIDL_SHORT); break;
        case NH_WEBIDL_TYPE_UNSIGNED_SHORT      : typeSize = sizeof(NH_WEBIDL_UNSIGNED_SHORT); break;
        case NH_WEBIDL_TYPE_LONG                : typeSize = sizeof(NH_WEBIDL_LONG); break;
        case NH_WEBIDL_TYPE_UNSIGNED_LONG       : typeSize = sizeof(NH_WEBIDL_UNSIGNED_LONG); break;
        case NH_WEBIDL_TYPE_LONG_LONG           : typeSize = sizeof(NH_WEBIDL_LONG_LONG); break;
        case NH_WEBIDL_TYPE_UNSIGNED_LONG_LONG  : typeSize = sizeof(NH_WEBIDL_UNSIGNED_LONG_LONG); break; 
        case NH_WEBIDL_TYPE_FLOAT               : typeSize = sizeof(NH_WEBIDL_FLOAT); break;
        case NH_WEBIDL_TYPE_UNRESTRICTED_FLOAT  : typeSize = sizeof(NH_WEBIDL_UNRESTRICTED_FLOAT); break;
        case NH_WEBIDL_TYPE_DOUBLE              : typeSize = sizeof(NH_WEBIDL_DOUBLE); break;
        case NH_WEBIDL_TYPE_UNRESTRICTED_DOUBLE : typeSize = sizeof(NH_WEBIDL_UNRESTRICTED_DOUBLE); break;
        case NH_WEBIDL_TYPE_BIGINT              : typeSize = sizeof(nh_webidl_BigInt); break;
        case NH_WEBIDL_TYPE_DOMSTRING           : typeSize = sizeof(nh_webidl_DOMString); break;
        case NH_WEBIDL_TYPE_BYTESTRING          : typeSize = sizeof(nh_webidl_ByteString); break;
        case NH_WEBIDL_TYPE_USVSTRING           : typeSize = sizeof(nh_webidl_USVString); break;
        case NH_WEBIDL_TYPE_OBJECT              : typeSize = 0; break;
        case NH_WEBIDL_TYPE_SYMBOL              : typeSize = 0; break;
    }

NH_WEBIDL_END(typeSize > sizeof(void*) ? typeSize : 0)
}

