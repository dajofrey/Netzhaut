// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "BoundNames.h"

#include "../Engine/Type.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include <stdio.h>
#include <string.h>

// DATA ============================================================================================

static char *synthetic_p = "*default*";

// HELPER ==========================================================================================

void nh_ecmascript_freeBoundNames(
    nh_core_List *Names_p)
{
    for (int i = 0; i < Names_p->size; ++i) {
        if (((nh_ecmascript_InputElement*)Names_p->pp[i])->type == -1) {
            nh_core_free(Names_p->pp[i]);
        }
    }

    nh_core_freeList(Names_p, false);

    return;
}

static nh_core_List nh_ecmascript_getSyntheticName()
{
    nh_core_List List = nh_core_initList(1);

    nh_ecmascript_InputElement *InputElement_p = nh_core_allocate(sizeof(nh_ecmascript_InputElement));
    NH_CORE_CHECK_MEM_2(List, InputElement_p)

    InputElement_p->type = -1;
    InputElement_p->String.p = synthetic_p;
    InputElement_p->String.length  = strlen(synthetic_p);

    nh_core_appendToList(&List, InputElement_p);

    return List;
}

// BOUND NAMES =====================================================================================

// https://tc39.es/ecma262/#sec-static-semantics-boundnames
nh_core_List nh_ecmascript_getBoundNames(
    nh_ecmascript_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_ECMASCRIPT_PARSE_NODE_IDENTIFIER :
        {
            nh_core_List Result = nh_core_initList(1);
            nh_core_appendToList(&Result, &((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->Value_p->String);
            return Result;
            break;
        }
        case NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER :
            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->Value_p == NULL) {
                return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            }
            else { // must be 'yield' or 'await'
                nh_core_List Result = nh_core_initList(1);
                nh_core_appendToList(&Result, &((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->Value_p->String);
                return Result;
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_LEXICAL_DECLARATION :
            return nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
            break;

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_LIST :
        {
            nh_core_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 3) {
                nh_core_List BindingNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[2]);
                nh_core_appendItemsToList(&Names, &BindingNames);
                nh_core_freeList(&BindingNames, false);
            }
            return Names;
        }
        break;

        case NH_ECMASCRIPT_PARSE_NODE_LEXICAL_BINDING :
            return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION_LIST :
        {
            nh_core_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 3) {
                nh_core_List DeclarationNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[2]);
                nh_core_appendItemsToList(&Names, &DeclarationNames);
                nh_core_freeList(&DeclarationNames, false);
            }
            return Names;
        }

        case NH_ECMASCRIPT_PARSE_NODE_VARIABLE_DECLARATION :
            return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_OBJECT_BINDING_PATTERN :
            if (Node_p->Children.size == 5) {
                nh_core_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
                nh_core_List RestNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[3]);
                nh_core_appendItemsToList(&Names, &RestNames);
                nh_core_freeList(&RestNames, false);
                return Names;
            }
            if (Node_p->Children.size >= 3) {
                return nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
            }
            break;

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_PROPERTY :
            return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_PROPERTY_LIST :
        {
            nh_core_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 3) {
                nh_core_List PropertyNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Names, &PropertyNames);
                nh_core_freeList(&PropertyNames, false);
            }
            return Names;
        }

//        case NH_ECMASCRIPT_PARSE_NODE_ARRAY_BINDING_PATTERN :
//            if (Node_p->Children.size == 1) {
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT) {
//                    return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                }
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT_LIST) {
//                    return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                }
//            }
//            else if (Node_p->Children.size == 2) {
//                if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_ELISION) {
//                    if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[1])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT) {
//                        return nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
//                    }
//                }
//                else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[1])->type == NH_ECMASCRIPT_PARSE_NODE_ELISION) {
//                    if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT_LIST) {
//                        return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                    }
//                }
//                else if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[1])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT) {
//                    nh_core_List BindingElementListNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                    nh_core_List BindingRestElementNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
//                    for (int i = 0; i < BindingRestElementNames.size; ++i) {
//                        nh_core_appendToList(&BindingElementListNames, BindingRestElementNames.pp[i]);
//                    }
//                    nh_core_freeList(&BindingRestElementNames, false);
//                    return BindingElementListNames;
//                }
//            }
//            else if (Node_p->Children.size == 3) {
//                nh_core_List BindingElementListNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                nh_core_List BindingRestElementNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[2]);
//                for (int i = 0; i < BindingRestElementNames.size; ++i) {
//                    nh_core_appendToList(&BindingElementListNames, BindingRestElementNames.pp[i]);
//                }
//                nh_core_freeList(&BindingRestElementNames, false);
//                return BindingElementListNames;
//            }
//            break;

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_REST_ELEMENT :
            return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_PATTERN :
            return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);

        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT_LIST :
        {
            nh_core_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
            if (Node_p->Children.size == 2) {
                nh_core_List ElementNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
                nh_core_appendItemsToList(&Names, &ElementNames);
                nh_core_freeList(&ElementNames, false);
            }
            return Names;
        }

//        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELISION_ELEMENT :
//            if (Node_p->Children.size == 1) {
//                return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//            }
//            else {return nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);}
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_BINDING_ELEMENT :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_PATTERN) {
//                return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_SINGLE_NAME_BINDING :
//            return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FOR_DECLARATION :
//            return nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
//
//        case NH_ECMASCRIPT_PARSE_NODE_FOR_BINDING :
//            return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//
//        case NH_ECMASCRIPT_PARSE_NODE_FUNCTION_DECLARATION :
//            if (((nh_ecmascript_ParseNode*)Node_p->Children.pp[0])->type == NH_ECMASCRIPT_PARSE_NODE_BINDING_IDENTIFIER) {
//                return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//            }
//            else {return nh_ecmascript_getSyntheticName();}
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETERS :
//            if (Node_p->Children.size == 2) {
//                nh_core_List FormalParameterListNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//                nh_core_List FunctionRestParameterNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
//                for (int i = 0; i < FunctionRestParameterNames.size; ++i) {
//                    nh_core_appendToList(&FormalParameterListNames, FunctionRestParameterNames.pp[i]);
//                }
//                nh_core_freeList(&FunctionRestParameterNames, false);
//                return FormalParameterListNames;
//            }
//            break;
//
//        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER_LIST :
//        {
//            nh_core_List Names = nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
//            if (Node_p->Children.size == 2) {
//                nh_core_List FormalParameterNames = nh_ecmascript_getBoundNames(Node_p->Children.pp[1]);
//                for (int i = 0; i < FormalParameterNames.size; ++i) {
//                    nh_core_appendToList(&Names, FormalParameterNames.pp[i]);
//                }
//                nh_core_freeList(&FormalParameterNames, false);
//            }
//            return Names;
//        }
//
//        case NH_ECMASCRIPT_PARSE_NODE_FORMAL_PARAMETER :
//            return nh_ecmascript_getBoundNames(Node_p->Children.pp[0]);
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

    return nh_core_initList(8);
}

