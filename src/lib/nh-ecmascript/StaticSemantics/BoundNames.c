// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "BoundNames.h"

#include "../Engine/Type.h"
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include <stdio.h>
#include <string.h>

// DATA ============================================================================================

static NH_BYTE *synthetic_p = "*default*";

// HELPER ==========================================================================================

void nh_ecmascript_freeBoundNames(
    nh_List *Names_p)
{
NH_ECMASCRIPT_BEGIN()

    for (int i = 0; i < Names_p->size; ++i) {
        if (((nh_ecmascript_InputElement*)Names_p->pp[i])->type == -1) {
            nh_core_free(Names_p->pp[i]);
        }
    }

    nh_core_freeList(Names_p, NH_FALSE);

NH_ECMASCRIPT_SILENT_END()
}

static nh_List nh_ecmascript_getSyntheticName()
{
NH_ECMASCRIPT_BEGIN()

    nh_List List = nh_core_initList(1);

    nh_ecmascript_InputElement *InputElement_p = nh_core_allocate(sizeof(nh_ecmascript_InputElement));
    NH_ECMASCRIPT_CHECK_MEM_2(List, InputElement_p)

    InputElement_p->type = -1;
    InputElement_p->String.p = synthetic_p;
    InputElement_p->String.length  = strlen(synthetic_p);

    nh_core_appendToList(&List, InputElement_p);

NH_ECMASCRIPT_END(List)
}

// BOUND NAMES =====================================================================================

// https://tc39.es/ecma262/#sec-static-semantics-boundnames
nh_List nh_ecmascript_getBoundNames(
    nh_ecmascript_ParseNode *Node_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER :
        {
            nh_List Result = nh_core_initList(1);
            nh_core_appendToList(&Result, &((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->Value_p->String);
            NH_ECMASCRIPT_END(Result)
            break;
        }
        case NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->Value_p == NULL) {
                NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))
            }
            else { // must be 'yield' or 'await'
                nh_List Result = nh_core_initList(1);
                nh_core_appendToList(&Result, &((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->Value_p->String);
                NH_ECMASCRIPT_END(Result)
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LEXICAL_DECLARATION :
            NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[1]))
            break;

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_LIST :
        {
            nh_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 3) {
                nh_List BindingNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[2]);
                nh_core_appendItemsToList(&Names, &BindingNames);
                nh_core_freeList(&BindingNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }
        break;

        case NH_ECMASCRIPT_PARSE_NODE_LEXICAL_BINDING :
            NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST :
        {
            nh_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 3) {
                nh_List DeclarationNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[2]);
                nh_core_appendItemsToList(&Names, &DeclarationNames);
                nh_core_freeList(&DeclarationNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION :
            NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_OBJECT_BINDING_PATTERN :
            if (Node_p->Children.size == 5) {
                nh_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
                nh_List RestNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[3]);
                nh_core_appendItemsToList(&Names, &RestNames);
                nh_core_freeList(&RestNames, NH_FALSE);
                NH_ECMASCRIPT_END(Names)
            }
            if (Node_p->Children.size >= 3) {
                NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[1]))
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_PROPERTY :
            NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_PROPERTY_LIST :
        {
            nh_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 3) {
                nh_List PropertyNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Names, &PropertyNames);
                nh_core_freeList(&PropertyNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }

//        case NH_ECMASCRIPT_PARSE_NODE_ARRAY_BINDING_PATTERN :
//            if (Node_p->Children.size == 1) {
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT) {
//                    NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))
//                }
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT_LIST) {
//                    NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))
//                }
//            }
//            else if (Node_p->Children.size == 2) {
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ELISION) {
//                    if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[1])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT) {
//                        NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[1]))
//                    }
//                }
//                else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[1])->type == NH_ECMASCRIPT_PARSE_NODE_ELISION) {
//                    if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT_LIST) {
//                        NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))
//                    }
//                }
//                else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[1])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT) {
//                    nh_List BindingElementListNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                    nh_List BindingRestElementNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
//                    for (int i = 0; i < BindingRestElementNames.size; ++i) {
//                        nh_core_appendToList(&BindingElementListNames, BindingRestElementNames.pp[i]);
//                    }
//                    nh_core_freeList(&BindingRestElementNames, NH_FALSE);
//                    NH_ECMASCRIPT_END(BindingElementListNames)
//                }
//            }
//            else if (Node_p->Children.size == 3) {
//                nh_List BindingElementListNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                nh_List BindingRestElementNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[2]);
//                for (int i = 0; i < BindingRestElementNames.size; ++i) {
//                    nh_core_appendToList(&BindingElementListNames, BindingRestElementNames.pp[i]);
//                }
//                nh_core_freeList(&BindingRestElementNames, NH_FALSE);
//                NH_ECMASCRIPT_END(BindingElementListNames)
//            }
//            break;

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT :
            NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_PATTERN :
            NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT_LIST :
        {
            nh_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_List ElementNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Names, &ElementNames);
                nh_core_freeList(&ElementNames, NH_FALSE);
            }
            NH_ECMASCRIPT_END(Names)
        }

//        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELISION_ELEMENT :
//            if (Node_p->Children.size == 1) {
//                NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))
//            }
//            else {NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[1]))}
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_PATTERN) {
//                NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_SINGLE_NAME_BINDING :
//            NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FOR_DECLARATION :
//            NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[1]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING :
//            NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) {
//                NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))
//            }
//            else {NH_ECMASCRIPT_END(nh_ecmascript_getSyntheticName())}
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETERS :
//            if (Node_p->Children.size == 2) {
//                nh_List FormalParameterListNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                nh_List FunctionRestParameterNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
//                for (int i = 0; i < FunctionRestParameterNames.size; ++i) {
//                    nh_core_appendToList(&FormalParameterListNames, FunctionRestParameterNames.pp[i]);
//                }
//                nh_core_freeList(&FunctionRestParameterNames, NH_FALSE);
//                NH_ECMASCRIPT_END(FormalParameterListNames)
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER_LIST :
//        {
//            nh_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//            if (Node_p->Children.size == 2) {
//                nh_List FormalParameterNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
//                for (int i = 0; i < FormalParameterNames.size; ++i) {
//                    nh_core_appendToList(&Names, FormalParameterNames.pp[i]);
//                }
//                nh_core_freeList(&FormalParameterNames, NH_FALSE);
//            }
//            NH_ECMASCRIPT_END(Names)
//        }
//
//        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER :
//            NH_ECMASCRIPT_END(nh_ecmascript_getBoundNames(Node_p->Children.pp[0]))
//            break;
//
// ArrowParameters : CoverParenthesizedExpressionAndArrowParameterList
// 
//     Let formals be CoveredFormalsList of CoverParenthesizedExpressionAndArrowParameterList.
//     Return the BoundNames of formals.
// 
// GeneratorDeclaration : function * BindingIdentifier ( FormalParameters ) { GeneratorBody }
// 
//     Return the BoundNames of BindingIdentifier.
// 
// GeneratorDeclaration : function * ( FormalParameters ) { GeneratorBody }
// 
//     Return « "*default*" ».
// 
// AsyncGeneratorDeclaration : async function * BindingIdentifier ( FormalParameters ) { AsyncGeneratorBody }
// 
//     Return the BoundNames of BindingIdentifier.
// 
// AsyncGeneratorDeclaration : async function * ( FormalParameters ) { AsyncGeneratorBody }
// 
//     Return « "*default*" ».
// 
// ClassDeclaration : class BindingIdentifier ClassTail
// 
//     Return the BoundNames of BindingIdentifier.
// 
// ClassDeclaration : class ClassTail
// 
//     Return « "*default*" ».
// 
// AsyncFunctionDeclaration : async function BindingIdentifier ( FormalParameters ) { AsyncFunctionBody }
// 
//     Return the BoundNames of BindingIdentifier.
// 
// AsyncFunctionDeclaration : async function ( FormalParameters ) { AsyncFunctionBody }
// 
//     Return « "*default*" ».
// 
// CoverCallExpressionAndAsyncArrowHead : MemberExpression Arguments
// 
//     Let head be CoveredAsyncArrowHead of CoverCallExpressionAndAsyncArrowHead.
//     Return the BoundNames of head.
// 
// ImportDeclaration : import ImportClause FromClause ;
// 
//     Return the BoundNames of ImportClause.
// 
// ImportDeclaration : import ModuleSpecifier ;
// 
//     Return a new empty List.
// 
// ImportClause : ImportedDefaultBinding , NameSpaceImport
// 
//     Let names be the BoundNames of ImportedDefaultBinding.
//     Append to names the elements of the BoundNames of NameSpaceImport.
//     Return names.
// 
// ImportClause : ImportedDefaultBinding , NamedImports
// 
//     Let names be the BoundNames of ImportedDefaultBinding.
//     Append to names the elements of the BoundNames of NamedImports.
//     Return names.
// 
// NamedImports : { }
// 
//     Return a new empty List.
// 
// ImportsList : ImportsList , ImportSpecifier
// 
//     Let names be the BoundNames of ImportsList.
//     Append to names the elements of the BoundNames of ImportSpecifier.
//     Return names.
// 
// ImportSpecifier : IdentifierName as ImportedBinding
// 
//     Return the BoundNames of ImportedBinding.
// 
// ExportDeclaration :
// export ExportFromClause FromClause ;
// export NamedExports ;
// 
//     Return a new empty List.
// 
// ExportDeclaration : export VariableStatement
// 
//     Return the BoundNames of VariableStatement.
// 
// ExportDeclaration : export Declaration
// 
//     Return the BoundNames of Declaration.
// 
// ExportDeclaration : export default HoistableDeclaration
// 
//     Let declarationNames be the BoundNames of HoistableDeclaration.
//     If declarationNames does not include the element "*default*", append "*default*" to declarationNames.
//     Return declarationNames.
// 
// ExportDeclaration : export default ClassDeclaration
// 
//     Let declarationNames be the BoundNames of ClassDeclaration.
//     If declarationNames does not include the element "*default*", append "*default*" to declarationNames.
//     Return declarationNames.
// 
// ExportDeclaration : export default AssignmentExpression ;
// 
//     Return « "*default*" ».
    }

NH_ECMASCRIPT_END(nh_core_initList(8))
}

