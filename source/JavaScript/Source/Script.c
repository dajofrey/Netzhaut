// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Script.h"
#include "../Header/Interpreter.h"
#include "../Header/Macros.h"
#include "../Header/Loader.h"
#include "../Header/Variable.h"

#include "../Header/Scripts/SelectOption.h"
#include "../Header/Scripts/Scroll.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/HashMap.h"
#include "../../Core/Header/String.h"
#include "../../Core/Header/List.h"
#include "../../Core/Header/Memory.h"
#include "../../Core/Header/File.h"

#include NH_DEBUG
#include NH_UTILS
#include NH_DEFAULT_CHECK

#include <string.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_JS_addScript(
    Nh_List *Scripts_p, Nh_JS_Script *Prototype_p
);

static NH_RESULT Nh_JS_addDefaultScripts(
    Nh_HTML_Document *Document_p
);
static NH_RESULT Nh_JS_addScriptsFromScriptElements(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p
);

static Nh_JS_Parsable *Nh_JS_getFunctionFromBlock(
    Nh_JS_Script *Script_p, char *name_p, Nh_JS_BlockStatement *Block_p
);

static NH_RESULT Nh_JS_stringifyParsable(
    Nh_JS_Parsable *Parsable_p, int depth, Nh_String *String_p
);
static NH_RESULT Nh_JS_stringifyOperator(
    NH_JS_OPERATOR operator_, int depth, Nh_String *String_p
);

// INIT ============================================================================================

void Nh_JS_initScript(
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

    NH_INIT_LIST(Script_p->Container.Variables)
    NH_INIT_LIST(Script_p->DOM.EventListeners)

    Script_p->Container.Parsables_p   = NULL;
    Script_p->Container.parsableCount = 0;
    Script_p->Container.Parent_p      = NULL;

    Script_p->Flags.parsed        = false;
    Script_p->Flags.external      = false;
    Script_p->Flags.interpreted   = false;
    Script_p->Flags.loaded        = false;
    Script_p->Flags.internalScript = false;

    Script_p->Debug.depth = 0;

NH_SILENT_END()
}

// GET ==============================================================================================

Nh_JS_Script *Nh_JS_getScript(
    Nh_List *Scripts_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(Scripts_p, index))
}

Nh_JS_Document *Nh_JS_getDocument(
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

    if (!Script_p->Flags.loaded) {NH_END(NULL)}

NH_END(Script_p->DOM.Objects.Document_p->data_p)
}

char *Nh_JS_getDefaultScript(
    NH_INTERNAL_FILE file)
{
NH_BEGIN()

    switch (file)
    {
        case NH_INTERNAL_FILE_SELECT_OPTION : NH_END(SelectOptionJS) break;
        case NH_INTERNAL_FILE_SCROLL : NH_END(ScrollJS) break;
    }

NH_END(NULL)
}

// ADD ==============================================================================================

NH_RESULT Nh_JS_addScripts(
    Nh_HTML_Document *Document_p, bool addDefaultScripts)
{
NH_BEGIN()

    if (addDefaultScripts) {NH_CHECK(Nh_JS_addDefaultScripts(Document_p))}
    NH_CHECK(Nh_JS_addScriptsFromScriptElements(Document_p, Document_p->Tree.Root_p))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_addScript(
    Nh_List *Scripts_p, Nh_JS_Script *Prototype_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Scripts_p, Prototype_p)

    Nh_JS_Script *Script_p = Nh_allocate(sizeof(Nh_JS_Script));
    NH_CHECK_MEM(Script_p)
   
    Nh_JS_initScript(Script_p); 

    Script_p->URI = Prototype_p->URI;
    Script_p->Flags.internalScript = Prototype_p->Flags.internalScript;
    Script_p->Flags.external       = Prototype_p->Flags.external;

    Nh_addListItem(Scripts_p, Script_p);

NH_END(NH_SUCCESS)
}

// ADD SCRIPTS =====================================================================================

static NH_RESULT Nh_JS_addDefaultScripts(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    Nh_JS_Script Script = {0};
    Nh_JS_initScript(&Script);
    Script.Flags.internalScript = true;

    Script.URI = Nh_createInternalURN("js:selectoption");
    NH_CHECK(Nh_JS_addScript(&Document_p->Scripts, &Script))

    Script.URI = Nh_createInternalURN("js:scroll");
    NH_CHECK(Nh_JS_addScript(&Document_p->Scripts, &Script))

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_addScriptsFromScriptElements(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_SCRIPT) 
    {
        Nh_JS_Script Script = {0};
        Nh_HTML_Node *Script_p = Nh_getListItem(&Node_p->Children.Unformatted, 0); 

        if (Script_p != NULL && Script_p->text_p != NULL)
        {
            Script.Flags.external = false;
            Script.URI = Nh_createInternalURL(Script_p, NH_INTERNAL_URL_HTML_NODE_TEXT);
        }
        else {
            for (int i = 0; i < Node_p->Attributes.count; ++i) 
            {
                if (Nh_HTML_getAttribute(&Node_p->Attributes, i)->type == NH_HTML_ATTRIBUTE_SRC) 
                {
                    Script.Flags.external = true;
                    Script.URI = Nh_createInternalURL(Nh_HTML_getAttribute(&Node_p->Attributes, i), NH_INTERNAL_URL_HTML_NODE_ATTRIBUTE_VALUE);
                }
            }
        }
        NH_CHECK(Nh_JS_addScript(&Document_p->Scripts, &Script))
    }

    for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) {
        NH_CHECK(Nh_JS_addScriptsFromScriptElements(Document_p, Nh_getListItem(&Node_p->Children.Unformatted, i)))
    }

NH_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_JS_destroyScriptList(
    Nh_List *Scripts_p)
{
NH_BEGIN()

    if (Scripts_p == NULL) {NH_SILENT_END()}

    for (int i = 0; i < Scripts_p->count; ++i) 
    {
        Nh_JS_Script *Script_p = Nh_JS_getScript(Scripts_p, i);
        Nh_JS_destroyScript(Script_p);
    }

    Nh_destroyList(Scripts_p, true);

NH_SILENT_END()
}

void Nh_JS_destroyScript(
    Nh_JS_Script *Script_p)
{   
NH_BEGIN()

    Nh_JS_unloadScript(Script_p);
    Nh_JS_unparseScript(Script_p);
    Nh_freeURI(Script_p->URI);

NH_SILENT_END()
}

// GET FUNCTION ====================================================================================

Nh_JS_Parsable *Nh_JS_getFunctionDeclaration(
    Nh_JS_Script *Script_p, char *name_p)
{
NH_BEGIN()

#define REVERT() if (index != -1) {name_p[index] = '(';}

    int index = -1;
    for (int i = 0; i < strlen(name_p); ++i) {if (name_p[i] == '(') {index = i; break;}}
    if (index != -1) {name_p[index] = '\0';} // strip away parameters

    for (int i = 0; i < Script_p->Container.parsableCount; ++i) 
    {
        Nh_JS_Parsable *Parsable_p = &Script_p->Container.Parsables_p[i];
   
        switch (Parsable_p->type)
        {
            case NH_JS_PARSABLE_FUNCTION_DECLARATION :
                if (!strcmp(((Nh_JS_Identifier*)((Nh_JS_FunctionDeclaration*)Parsable_p->Data_p)->Name.Data_p)->name_p, name_p)) {
                    REVERT()
                    NH_END(Parsable_p)
                }
                break;
     
            case NH_JS_PARSABLE_BLOCK_STATEMENT : ;
                Nh_JS_Parsable *Function_p = Nh_JS_getFunctionFromBlock(Script_p, name_p, Parsable_p->Data_p);
                if (Function_p != NULL) {
                    REVERT()
                    NH_END(Function_p)
                }
                break;
        }
    }

    REVERT()

#undef REVERT

NH_END(NULL)
}

static Nh_JS_Parsable *Nh_JS_getFunctionFromBlock(
    Nh_JS_Script *Script_p, char *name_p, Nh_JS_BlockStatement *Block_p)
{
NH_BEGIN()

    for (int i = 0; i < Block_p->parsableCount; ++i) 
    {
        Nh_JS_Parsable *Parsable_p = &Block_p->Parsables_p[i];
   
        switch (Parsable_p->type)
        {
            case NH_JS_PARSABLE_FUNCTION_DECLARATION :
                if (!strcmp(((Nh_JS_Identifier*)((Nh_JS_FunctionDeclaration*)Parsable_p->Data_p)->Name.Data_p)->name_p, name_p)) {
                    NH_END(Parsable_p)
                }
                break;
     
            case NH_JS_PARSABLE_BLOCK_STATEMENT : ;
                void *Function_p = Nh_JS_getFunctionFromBlock(Script_p, name_p, Parsable_p->Data_p);
                if (Function_p != NULL) {NH_END(Function_p);}
                break;
        }
    }

NH_END(NULL)
}

// STRINGIFY =======================================================================================

char *Nh_JS_stringifyScript(
    Nh_JS_Script *Script_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_String *String_p = Nh_allocateString(NULL);

    NH_CHECK(NULL, Nh_appendToString(String_p, "{\n"))
//    NH_CHECK(NULL, Nh_appendFormatToString(String_p, "  \e[1;32mScript\e[0m: %s\n", Script_p->Info.path_p))
    NH_CHECK(NULL, Nh_appendFormatToString(String_p, "  Total parsable count: %d\n", Script_p->Container.parsableCount))

    for (int i = 0; i < Script_p->Container.parsableCount; ++i) {
        NH_CHECK(NULL, Nh_appendToString(String_p, "\n"))
        NH_CHECK(NULL, Nh_JS_stringifyParsable(&Script_p->Container.Parsables_p[i], 1, String_p))
    }

    NH_CHECK(NULL, Nh_appendToString(String_p, "}\n"))

    char *string_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);
  
#include NH_DEFAULT_CHECK
 
NH_END(string_p)
}

static NH_RESULT Nh_JS_stringifyParsable(
    Nh_JS_Parsable *Parsable_p, int depth, Nh_String *String_p)
{
NH_BEGIN()

#define INDENT() for (int ind = 0; ind < depth; ++ind) {NH_CHECK(Nh_appendToString(String_p, "  "))}

    INDENT()

    switch (Parsable_p->type)
    {
        case NH_JS_PARSABLE_STRING_LITERAL : 

            NH_CHECK(Nh_appendFormatToString(String_p, "String: %s\n", ((Nh_JS_StringLiteral*)Parsable_p->Data_p)->value_p))
            break;

        case NH_JS_PARSABLE_IDENTIFIER :

            NH_CHECK(Nh_appendFormatToString(String_p, "Identifier: %s\n", ((Nh_JS_Identifier*)Parsable_p->Data_p)->name_p))
            break;

        case NH_JS_PARSABLE_VARIABLE_DECLARATION : 

            switch (((Nh_JS_VariableDeclaration*)Parsable_p->Data_p)->kind)
            {
                case NH_JS_KEYWORD_VAR   : NH_CHECK(Nh_appendToString(String_p, "\e[1;32mVariable\e[0m (kind: var) \e[1;32m{\e[0m\n")) break;
                case NH_JS_KEYWORD_CONST : NH_CHECK(Nh_appendToString(String_p, "\e[1;32mVariable\e[0m (kind: const) \e[1;32m{\e[0m\n")) break;
                default                  : NH_CHECK(Nh_appendToString(String_p, "\e[1;32mVariable\e[0m (kind: undefined) \e[1;32m{\e[0m\n")) break;
            }

            INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "  Identifier: %s\n", ((Nh_JS_Identifier*)((Nh_JS_VariableDeclaration*)Parsable_p->Data_p)->Declarator.Identifier.Data_p)->name_p))

            if (((Nh_JS_VariableDeclaration*)Parsable_p->Data_p)->Declarator.Parsable.type != -1) {
                NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_VariableDeclaration*)Parsable_p->Data_p)->Declarator.Parsable, depth + 1, String_p))
            }
            
            INDENT() NH_CHECK(Nh_appendToString(String_p, "\e[1;32m}\e[0m\n"))

            break;

        case NH_JS_PARSABLE_CALL_EXPRESSION :

            NH_CHECK(Nh_appendToString(String_p, "\e[1;34mFunction Call {\e[0m\n"))
            
            NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_CallExpression*)Parsable_p->Data_p)->Parsable, depth + 1, String_p))
            for (int i = 0; i < ((Nh_JS_CallExpression*)Parsable_p->Data_p)->argumentCount; ++i) {
                NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_CallExpression*)Parsable_p->Data_p)->Arguments_p[i], depth + 1, String_p))
            }

            INDENT() NH_CHECK(Nh_appendToString(String_p, "\e[1;34m}\e[0m\n"))

            break;

        case NH_JS_PARSABLE_MEMBER_EXPRESSION :

            NH_CHECK(Nh_appendToString(String_p, "\e[1;35mMember {\e[0m\n"))
            
            NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_MemberExpression*)Parsable_p->Data_p)->Left, depth + 1, String_p))
            NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_MemberExpression*)Parsable_p->Data_p)->Right, depth + 1, String_p))

            INDENT() NH_CHECK(Nh_appendToString(String_p, "\e[1;35m}\e[0m\n"))

            break;

        case NH_JS_PARSABLE_FUNCTION_DECLARATION :

            NH_CHECK(Nh_appendToString(String_p, "\e[1;34mFunction {\e[0m\n"))

            INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "  Identifier: %s\n", ((Nh_JS_Identifier*)((Nh_JS_FunctionDeclaration*)Parsable_p->Data_p)->Name.Data_p)->name_p))

            for (int i = 0; i < ((Nh_JS_FunctionDeclaration*)Parsable_p->Data_p)->parameterCount; ++i) {
                INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "  Identifier: %s\n", ((Nh_JS_Identifier*)((Nh_JS_FunctionDeclaration*)Parsable_p->Data_p)->Parameters_p[i].Data_p)->name_p))
            }

            NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_FunctionDeclaration*)Parsable_p->Data_p)->Block, depth + 1, String_p))

            INDENT() NH_CHECK(Nh_appendToString(String_p, "\e[1;34m}\e[0m\n"))

            break;

        case NH_JS_PARSABLE_BLOCK_STATEMENT :

            NH_CHECK(Nh_appendToString(String_p, "\e[1;31mBlock {\e[0m\n"))

            for (int i = 0; i < ((Nh_JS_BlockStatement*)Parsable_p->Data_p)->Variables.count; ++i) {
                Nh_JS_Variable *Variable_p = Nh_getListItem(&((Nh_JS_BlockStatement*)Parsable_p->Data_p)->Variables, i);
                INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "  Hoisted Variable: %s\n", Variable_p->name_p))
            }
            for (int i = 0; i < ((Nh_JS_BlockStatement*)Parsable_p->Data_p)->parsableCount; ++i) {
                NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_BlockStatement*)Parsable_p->Data_p)->Parsables_p[i], depth + 1, String_p))
            }

            INDENT() NH_CHECK(Nh_appendToString(String_p, "\e[1;31m}\e[0m\n"))

            break;

        case NH_JS_PARSABLE_IF_STATEMENT :

            NH_CHECK(Nh_appendToString(String_p, "\e[1;32mIf {\e[0m\n"))

            for (int i = 0; i < ((Nh_JS_IfStatement*)Parsable_p->Data_p)->parsableCount; ++i) {
                NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_IfStatement*)Parsable_p->Data_p)->Parsables_p[i], depth + 1, String_p))
            }

            INDENT() NH_CHECK(Nh_appendToString(String_p, "\e[1;32m}\e[0m\n"))

            break;

        case NH_JS_PARSABLE_BINARY_EXPRESSION :

            NH_CHECK(Nh_appendToString(String_p, "\e[1;33mBinary Expression {\e[0m\n"))

            NH_CHECK(Nh_JS_stringifyOperator(((Nh_JS_BinaryExpression*)Parsable_p->Data_p)->operator_, depth + 1, String_p))
            NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_BinaryExpression*)Parsable_p->Data_p)->Left, depth + 1, String_p))
            NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_BinaryExpression*)Parsable_p->Data_p)->Right, depth + 1, String_p))

            INDENT() NH_CHECK(Nh_appendToString(String_p, "\e[1;33m}\e[0m\n"))

            break;

        case NH_JS_PARSABLE_ASSIGNMENT_EXPRESSION :

            NH_CHECK(Nh_appendToString(String_p, "\e[1;33mAssignment {\e[0m\n"))

            NH_CHECK(Nh_JS_stringifyOperator(((Nh_JS_AssignmentExpression*)Parsable_p->Data_p)->operator_, depth + 1, String_p))
            NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_AssignmentExpression*)Parsable_p->Data_p)->Left, depth + 1, String_p))
            NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_AssignmentExpression*)Parsable_p->Data_p)->Right, depth + 1, String_p))

            INDENT() NH_CHECK(Nh_appendToString(String_p, "\e[1;33m}\e[0m\n"))

            break;

        case NH_JS_PARSABLE_FOR_STATEMENT :
           
            switch (((Nh_JS_ForStatement*)Parsable_p->Data_p)->type)
            {
                case NH_JS_ITERATION_STATEMENT_FOR          : NH_CHECK(Nh_appendToString(String_p, "\e[1;33mFor Loop {\e[0m\n")) break;
                case NH_JS_ITERATION_STATEMENT_FOR_IN       : NH_CHECK(Nh_appendToString(String_p, "\e[1;33mFor-in Loop {\e[0m\n")) break;
                
                case NH_JS_ITERATION_STATEMENT_FOR_OF       : 
                    NH_CHECK(Nh_appendToString(String_p, "\e[1;33mFor-of Loop {\e[0m\n"))
                    NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_ForStatement*)Parsable_p->Data_p)->Expressions_p[0], depth + 1, String_p))
                    NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_ForStatement*)Parsable_p->Data_p)->Expressions_p[1], depth + 1, String_p))
                    NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_ForStatement*)Parsable_p->Data_p)->Statement, depth + 1, String_p))
                    break;
                
                case NH_JS_ITERATION_STATEMENT_FOR_AWAIT_OF : NH_CHECK(Nh_appendToString(String_p, "\e[1;33mFor-await-of Loop {\e[0m\n")) break;
            }

            INDENT() NH_CHECK(Nh_appendToString(String_p, "\e[1;33m}\e[0m\n"))

            break;

        case NH_JS_PARSABLE_BOOLEAN_TRUE  :
        case NH_JS_PARSABLE_BOOLEAN_FALSE :

            NH_CHECK(Nh_appendToString(String_p, Parsable_p->type == NH_JS_PARSABLE_BOOLEAN_TRUE ? "Bool: true\n" : "Bool: false\n"))
            break;

        case NH_JS_PARSABLE_NUMERIC_LITERAL :

            NH_CHECK(Nh_appendFormatToString(String_p, "Number:  %lf\n", *((double*)Parsable_p->Data_p)))
            break;

        case NH_JS_PARSABLE_CONSTRUCTOR_CALL :

            NH_CHECK(Nh_appendToString(String_p, "\e[1;33mConstructor Call {\e[0m\n"))
            NH_CHECK(Nh_JS_stringifyParsable(&((Nh_JS_ConstructorCall*)Parsable_p->Data_p)->CallExpression, depth + 1, String_p))
            INDENT() NH_CHECK(Nh_appendToString(String_p, "\e[1;33m}\e[0m\n"))
            break;
    }

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_JS_stringifyOperator(
    NH_JS_OPERATOR operator_, int depth, Nh_String *String_p)
{
NH_BEGIN()

    INDENT()

    switch (operator_)
    {
        case NH_JS_OPERATOR_ASSIGN            : NH_CHECK(Nh_appendToString(String_p, "Operator:  ="))  break;  
        case NH_JS_OPERATOR_EQUAL             : NH_CHECK(Nh_appendToString(String_p, "Operator:  ==")) break;  
        case NH_JS_OPERATOR_GREATER           : NH_CHECK(Nh_appendToString(String_p, "Operator:  >"))  break;    
        case NH_JS_OPERATOR_LESS              : NH_CHECK(Nh_appendToString(String_p, "Operator:  <"))  break;    
        case NH_JS_OPERATOR_INCREMENT         : NH_CHECK(Nh_appendToString(String_p, "Operator:  ++")) break;  
        case NH_JS_OPERATOR_DECREMENT         : NH_CHECK(Nh_appendToString(String_p, "Operator:  --")) break;  
        case NH_JS_OPERATOR_UNEQUAL           : NH_CHECK(Nh_appendToString(String_p, "Operator:  !=")) break;
        case NH_JS_OPERATOR_GREATER_EQUAL     : NH_CHECK(Nh_appendToString(String_p, "Operator:  >=")) break;
        case NH_JS_OPERATOR_LESS_EQUAL        : NH_CHECK(Nh_appendToString(String_p, "Operator:  <=")) break;
        case NH_JS_OPERATOR_SUBTRACT_ASSIGN   : NH_CHECK(Nh_appendToString(String_p, "Operator:  -=")) break;
        case NH_JS_OPERATOR_ADD_ASSIGN        : NH_CHECK(Nh_appendToString(String_p, "Operator:  +=")) break;
        case NH_JS_OPERATOR_SHORT_CIRCUIT_AND : NH_CHECK(Nh_appendToString(String_p, "Operator:  &&")) break;
        case NH_JS_OPERATOR_SHORT_CIRCUIT_OR  : NH_CHECK(Nh_appendToString(String_p, "Operator:  ||")) break;
        case NH_JS_OPERATOR_AND               : NH_CHECK(Nh_appendToString(String_p, "Operator:  &"))  break;
        case NH_JS_OPERATOR_OR                : NH_CHECK(Nh_appendToString(String_p, "Operator:  |"))  break;
        case NH_JS_OPERATOR_NOT               : NH_CHECK(Nh_appendToString(String_p, "Operator:  !"))  break;
        case NH_JS_OPERATOR_SUBTRACT          : NH_CHECK(Nh_appendToString(String_p, "Operator:  -")) break;
        case NH_JS_OPERATOR_ADD               : NH_CHECK(Nh_appendToString(String_p, "Operator:  +")) break;
        case NH_JS_OPERATOR_MULTIPLY          : NH_CHECK(Nh_appendToString(String_p, "Operator:  *")) break;
        case NH_JS_OPERATOR_DIVIDE            : NH_CHECK(Nh_appendToString(String_p, "Operator:  /")) break;
    }

    NH_CHECK(Nh_appendToString(String_p, "\n"))

NH_END(NH_SUCCESS)
}

