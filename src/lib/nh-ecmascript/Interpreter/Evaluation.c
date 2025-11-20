// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Evaluation.h"
#include "Instantiation.h"

#include "../Engine/ExecutionContext.h"
#include "../Engine/CompletionOrReference.h"
#include "../StaticSemantics/Misc.h"

#include <string.h>
#include <stdio.h>

// EVALUATE ========================================================================================

//typedef enum {
//    VAL_UNDEFINED,
//    VAL_NUMBER,
//    VAL_STRING,
//    VAL_BOOLEAN,
//    VAL_OBJECT,
//    VAL_NATIVE_FUNCTION
//} ValueTag;
//
//typedef struct GCObject GCObject; /* forward for property map */
//
//typedef struct {
//    ValueTag tag;
//    union {
//        double number;
//        char *string;
//        bool boolean;
//        GCObject *obj;
//        /* native function pointer signature: (this, args, argc) -> value */
//        struct { struct JSValue (*fn)(struct JSValue thisv, struct JSValue *args, int argc); } native;
//    };
//} JSValue;
//
///* convenience constructors */
//static JSValue js_undefined(void) { JSValue v = { .tag = VAL_UNDEFINED }; return v; }
//static JSValue js_number(double d)   { JSValue v = { .tag = VAL_NUMBER, .number = d }; return v; }
//static JSValue js_bool(bool b)       { JSValue v = { .tag = VAL_BOOLEAN, .boolean = b }; return v; }
//static JSValue js_string_dup(const char *s) {
//    JSValue v = { .tag = VAL_STRING, .string = s ? strdup(s) : NULL }; return v;
//}
//
///* A tiny object model: we support two kinds:
//   - plain object (map of string -> JSValue)
//   - array object (vector of JSValue + length property)
//*/
//typedef enum { O_KIND_MAP, O_KIND_ARRAY } ObjectKind;
//
//struct GCProperty { char *key; JSValue value; struct GCProperty *next; };
//
//struct GCObject {
//    ObjectKind kind;
//    struct GCProperty *props; /* simple linked list map */
//    /* array storage if kind == O_KIND_ARRAY */
//    JSValue *elements;
//    size_t length;
//    size_t capacity;
//};
//
///* create array object */
//static GCObject *gc_new_array(size_t capacity) {
//    GCObject *o = calloc(1, sizeof(GCObject));
//    o->kind = O_KIND_ARRAY;
//    o->elements = NULL;
//    o->length = 0;
//    o->capacity = 0;
//    if (capacity) {
//        o->elements = malloc(sizeof(JSValue) * capacity);
//        o->capacity = capacity;
//    }
//    return o;
//}
//
///* push element */
//static void gc_array_push(GCObject *o, JSValue v) {
//    if (!o || o->kind != O_KIND_ARRAY) return;
//    if (o->length + 1 > o->capacity) {
//        size_t nc = o->capacity ? o->capacity * 2 : 4;
//        JSValue *tmp = realloc(o->elements, nc * sizeof(JSValue));
//        o->elements = tmp; o->capacity = nc;
//    }
//    o->elements[o->length++] = v;
//}
//
///* property helpers (simple linked-list map) */
//static void gc_set_prop(GCObject *o, const char *key, JSValue v) {
//    if (!o) return;
//    struct GCProperty **pp = &o->props;
//    while (*pp) {
//        if (strcmp((*pp)->key, key) == 0) { (*pp)->value = v; return; }
//        pp = &(*pp)->next;
//    }
//    struct GCProperty *p = malloc(sizeof(*p));
//    p->key = strdup(key);
//    p->value = v;
//    p->next = NULL;
//    *pp = p;
//}
//static bool gc_get_prop(GCObject *o, const char *key, JSValue *out) {
//    if (!o) return false;
//    struct GCProperty *p = o->props;
//    while (p) {
//        if (strcmp(p->key, key) == 0) { if (out) *out = p->value; return true; }
//        p = p->next;
//    }
//    /* arrays have numeric indices and "length" */
//    if (o->kind == O_KIND_ARRAY) {
//        if (strcmp(key, "length") == 0) {
//            if (out) *out = js_number((double)o->length);
//            return true;
//        }
//    }
//    return false;
//}
//
///* ---------- Environment (lexical) ---------- */
///* Simple linked-list environment record */
//typedef struct EnvRec {
//    char *name;
//    JSValue value;
//    bool immutable; /* const */
//    struct EnvRec *next;
//} EnvRec;
//
//typedef struct Env {
//    EnvRec *bindings;    /* linked list of bindings in this env */
//    struct Env *outer;   /* parent environment */
//} Env;
//
//static Env *env_new(Env *outer) {
//    Env *e = calloc(1, sizeof(Env));
//    e->bindings = NULL;
//    e->outer = outer;
//    return e;
//}
//
//static void env_create_binding_local(Env *e, const char *name, JSValue val, bool immutable) {
//    EnvRec *r = malloc(sizeof(EnvRec));
//    r->name = strdup(name);
//    r->value = val;
//    r->immutable = immutable;
//    r->next = e->bindings;
//    e->bindings = r;
//}
//
//static EnvRec *env_find_in_env(Env *e, const char *name) {
//    for (EnvRec *r = e->bindings; r; r = r->next) if (strcmp(r->name, name) == 0) return r;
//    return NULL;
//}
//
///* lookup following scope chain */
//static EnvRec *env_resolve(Env *e, const char *name) {
//    Env *cur = e;
//    while (cur) {
//        EnvRec *r = env_find_in_env(cur, name);
//        if (r) return r;
//        cur = cur->outer;
//    }
//    return NULL;
//}
//
///* get binding value; if not found returns undefined but caller should detect missing */
//static JSValue env_get(Env *e, const char *name) {
//    EnvRec *r = env_resolve(e, name);
//    if (r) return r->value;
//    return js_undefined();
//}
//
///* set binding; if exists, set in that env; otherwise create in nearest var env (here top env) */
//static bool env_set(Env *e, const char *name, JSValue val) {
//    EnvRec *r = env_resolve(e, name);
//    if (r) {
//        if (r->immutable) return false;
//        r->value = val;
//        return true;
//    }
//    /* not found → create in current environment (simple semantics) */
//    env_create_binding_local(e, name, val, false);
//    return true;
//}
//
///* ---------- Utility conversions ---------- */
//static double js_to_number(JSValue v) {
//    switch (v.tag) {
//        case VAL_NUMBER: return v.number;
//        case VAL_BOOLEAN: return v.boolean ? 1.0 : 0.0;
//        case VAL_STRING: return strtod(v.string ? v.string : "0", NULL);
//        default: return NAN;
//    }
//}
//static char *js_to_string_dup(JSValue v) {
//    if (v.tag == VAL_STRING) return strdup(v.string ? v.string : "");
//    if (v.tag == VAL_NUMBER) { char buf[64]; snprintf(buf, sizeof(buf), "%.17g", v.number); return strdup(buf); }
//    if (v.tag == VAL_BOOLEAN) return strdup(v.boolean ? "true" : "false");
//    return strdup("undefined");
//}
//
/* ---------- Interpreter core: evaluate AST nodes ---------- */

/* Forward */
static nh_ecmascript_Completion nh_ecmascript_evaluateNode(
    nh_ecmascript_ASTNode *Node_p, nh_ecmascript_Environment *Environment_p
);

/* Evaluate a list (nh_core_List) of AST nodes and return last value */
static nh_ecmascript_Completion nh_ecmascript_evaluateList(
    nh_core_List *List_p, nh_ecmascript_Environment *Environment_p)
{
    if (!List_p) return nh_ecmascript_normalEmptyCompletion();
    nh_ecmascript_Completion last = nh_ecmascript_normalEmptyCompletion();
    for (size_t i = 0; i < List_p->size; ++i) {
        nh_ecmascript_ASTNode *Node_p = (nh_ecmascript_ASTNode*)List_p->pp[i];
        last = nh_ecmascript_evaluateNode(Node_p, Environment_p);
    }
    return last;
}

///* helper: evaluate identifier as reference (value) */
//static JSValue eval_identifier(nh_ecmascript_ASTNode *id, Env *env) {
//    if (!id || id->type != NH_ECMASCRIPT_AST_NODE_IDENTIFIER) return js_undefined();
//    return env_get(env, id->Identifier.name_p);
//}
//
///* evaluate MemberExpression: supports obj.prop and arr[index] basic */
//static JSValue eval_member_expression(nh_ecmascript_ASTNode *mem, Env *env) {
//    if (!mem) return js_undefined();
//    nh_ecmascript_ASTNode *objNode = mem->MemberExpression.Object_p;
//    nh_ecmascript_ASTNode *propNode = mem->MemberExpression.Property_p;
//    JSValue objv = eval_node(objNode, env);
//
//    if (objv.tag == VAL_OBJECT && objv.obj) {
//        if (!mem->MemberExpression.computed) {
//            /* property name is identifier node */
//            if (propNode->type == NH_ECMASCRIPT_AST_NODE_IDENTIFIER) {
//                const char *prop = propNode->Identifier.name_p;
//                JSValue out;
//                if (gc_get_prop(objv.obj, prop, &out)) return out;
//                return js_undefined();
//            }
//        } else {
//            /* computed: evaluate property expression to number or string */
//            JSValue keyv = eval_node(propNode, env);
//            if (keyv.tag == VAL_NUMBER) {
//                /* index access */
//                double idx = keyv.number;
//                long i = (long)idx;
//                if (objv.obj->kind == O_KIND_ARRAY && i >= 0 && (size_t)i < objv.obj->length) {
//                    return objv.obj->elements[i];
//                }
//                return js_undefined();
//            } else {
//                char *k = js_to_string_dup(keyv);
//                JSValue out;
//                if (gc_get_prop(objv.obj, k, &out)) { free(k); return out; }
//                free(k);
//                return js_undefined();
//            }
//        }
//    }
//    return js_undefined();
//}
//
///* evaluate assignment left-hand target (identifier or member) and set value */
//static bool assign_to_lhs(nh_ecmascript_ASTNode *lhs, JSValue val, Env *env) {
//    if (!lhs) return false;
//    if (lhs->type == NH_ECMASCRIPT_AST_NODE_IDENTIFIER) {
//        return env_set(env, lhs->Identifier.name_p, val);
//    } else if (lhs->type == NH_ECMASCRIPT_AST_NODE_MEMBER_EXPRESSION) {
//        /* set property on object */
//        nh_ecmascript_ASTNode *objNode = lhs->MemberExpression.Object_p;
//        nh_ecmascript_ASTNode *propNode = lhs->MemberExpression.Property_p;
//        JSValue objv = eval_node(objNode, env);
//        if (objv.tag == VAL_OBJECT && objv.obj) {
//            if (!lhs->MemberExpression.computed) {
//                if (propNode->type == NH_ECMASCRIPT_AST_NODE_IDENTIFIER) {
//                    gc_set_prop(objv.obj, propNode->Identifier.name_p, val);
//                    return true;
//                }
//            } else {
//                JSValue keyv = eval_node(propNode, env);
//                if (keyv.tag == VAL_NUMBER) {
//                    long idx = (long)keyv.number;
//                    if (objv.obj->kind == O_KIND_ARRAY) {
//                        /* ensure capacity */
//                        if ((size_t)idx >= objv.obj->capacity) {
//                            size_t nc = objv.obj->capacity ? objv.obj->capacity*2 : 4;
//                            while (nc <= (size_t)idx) nc *= 2;
//                            objv.obj->elements = realloc(objv.obj->elements, nc * sizeof(JSValue));
//                            objv.obj->capacity = nc;
//                        }
//                        if ((size_t)idx >= objv.obj->length) objv.obj->length = idx + 1;
//                        objv.obj->elements[idx] = val;
//                        return true;
//                    }
//                } else {
//                    char *k = js_to_string_dup(keyv);
//                    gc_set_prop(objv.obj, k, val);
//                    free(k);
//                    return true;
//                }
//            }
//        }
//    }
//    return false;
//}

static nh_ecmascript_Completion nh_ecmascript_evaluateNode(
    nh_ecmascript_ASTNode *Node_p, nh_ecmascript_Environment *Environment_p)
{
    if (!Node_p) return nh_ecmascript_normalEmptyCompletion();

    switch (Node_p->type) {
        case NH_ECMASCRIPT_AST_NODE_PROGRAM:
            /* Evaluate program body in given env; return last value */
            return nh_ecmascript_evaluateList(&Node_p->Program.Body, Environment_p);

//        case NH_ECMASCRIPT_AST_NODE_BLOCK_STATEMENT:
//            /* new lexical environment for block (simplified: same env for now) */
//            return nh_ecmascript_evalList(&node->BlockStatement.Body, env);
//
//        case NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATION: {
//            /* iterate declarators */
//            size_t n = nh_core_list_size(&node->VariableDeclaration.Declarators);
//            for (size_t i = 0; i < n; ++i) {
//                nh_ecmascript_ASTNode *decl = (nh_ecmascript_ASTNode*) nh_core_list_get(&node->VariableDeclaration.Declarators, i);
//                if (decl && decl->type == NH_ECMASCRIPT_AST_NODE_VARIABLE_DECLARATOR) {
//                    const char *name = decl->VariableDeclarator.Id_p->Identifier.name_p;
//                    JSValue initv = js_undefined();
//                    if (decl->VariableDeclarator.Init_p) initv = eval_node(decl->VariableDeclarator.Init_p, env);
//                    /* create binding in current env (var/let/const handling simplified) */
//                    env_create_binding_local(env, name, initv, (strcmp(node->VariableDeclaration.kind_p, "const")==0));
//                }
//            }
//            return js_undefined();
//        }
//
//        case NH_ECMASCRIPT_AST_NODE_EXPRESSION_STATEMENT:
//            return eval_node(node->ExpressionStatement.Expression_p, env);
//
//        case NH_ECMASCRIPT_AST_NODE_LITERAL: {
//            if (node->Literal.type == NH_ECMASCRIPT_AST_LITERAL_NUMBER) {
//                return js_number(node->Literal.number);
//            } else if (node->Literal.type == NH_ECMASCRIPT_AST_LITERAL_STRING) {
//                return js_string_dup(node->Literal.string_p);
//            } else if (node->Literal.type == NH_ECMASCRIPT_AST_LITERAL_BOOL) {
//                return js_bool(node->Literal.boolean);
//            } else {
//                return js_undefined();
//            }
//        }
//
//        case NH_ECMASCRIPT_AST_NODE_IDENTIFIER:
//            return eval_identifier(node, env);
//
//        case NH_ECMASCRIPT_AST_NODE_BINARY_EXPRESSION: {
//            JSValue L = eval_node(node->BinaryExpression.Left_p, env);
//            JSValue R = eval_node(node->BinaryExpression.Right_p, env);
//            const char *op = node->BinaryExpression.operator_p;
//            if (!op) return js_undefined();
//            if (strcmp(op, "+") == 0) {
//                /* numeric + or string concat */
//                if (L.tag == VAL_STRING || R.tag == VAL_STRING) {
//                    char *ls = js_to_string_dup(L);
//                    char *rs = js_to_string_dup(R);
//                    size_t sz = strlen(ls) + strlen(rs) + 1;
//                    char *buf = malloc(sz);
//                    strcpy(buf, ls);
//                    strcat(buf, rs);
//                    JSValue out = js_string_dup(buf);
//                    free(buf); free(ls); free(rs);
//                    return out;
//                } else {
//                    return js_number(js_to_number(L) + js_to_number(R));
//                }
//            } else if (strcmp(op, "-") == 0) {
//                return js_number(js_to_number(L) - js_to_number(R));
//            } else if (strcmp(op, "*") == 0) {
//                return js_number(js_to_number(L) * js_to_number(R));
//            } else if (strcmp(op, "/") == 0) {
//                return js_number(js_to_number(L) / js_to_number(R));
//            } else if (strcmp(op, "<") == 0) {
//                return js_bool(js_to_number(L) < js_to_number(R));
//            } else if (strcmp(op, ">") == 0) {
//                return js_bool(js_to_number(L) > js_to_number(R));
//            } else {
//                return js_undefined();
//            }
//        }
//
//        case NH_ECMASCRIPT_AST_NODE_ASSIGNMENT_EXPRESSION: {
//            /* evaluate right side */
//            JSValue rv = eval_node(node->AssignmentExpression.Right_p, env);
//
//            /* handle compound operators like += */
//            const char *op = node->AssignmentExpression.operator_p;
//            if (op && strlen(op) > 1 && op[strlen(op)-1] == '=') {
//                /* e.g., "+=" -> compute left + right then assign */
//                char opch = op[0];
//                /* get current left value */
//                JSValue current;
//                if (node->AssignmentExpression.Left_p->type == NH_ECMASCRIPT_AST_NODE_IDENTIFIER) {
//                    current = eval_identifier(node->AssignmentExpression.Left_p, env);
//                } else {
//                    current = eval_node(node->AssignmentExpression.Left_p, env);
//                }
//                JSValue computed = js_undefined();
//                if (opch == '+') computed = js_number(js_to_number(current) + js_to_number(rv));
//                else if (opch == '-') computed = js_number(js_to_number(current) - js_to_number(rv));
//                else if (opch == '*') computed = js_number(js_to_number(current) * js_to_number(rv));
//                else if (opch == '/') computed = js_number(js_to_number(current) / js_to_number(rv));
//                else computed = rv;
//                assign_to_lhs(node->AssignmentExpression.Left_p, computed, env);
//                return computed;
//            } else {
//                /* plain assignment */
//                assign_to_lhs(node->AssignmentExpression.Left_p, rv, env);
//                return rv;
//            }
//        }
//
//        case NH_ECMASCRIPT_AST_NODE_UPDATE_EXPRESSION: {
//            /* only postfix i++ supported (prefix can be added) */
//            nh_ecmascript_ASTNode *arg = node->UpdateExpression.Argument_p;
//            JSValue cur = eval_node(arg, env);
//            double curd = js_to_number(cur);
//            JSValue newv = js_number(curd + 1.0);
//            /* set back */
//            assign_to_lhs(arg, newv, env);
//            if (node->UpdateExpression.prefix) return newv;
//            return cur;
//        }
//
//        case NH_ECMASCRIPT_AST_NODE_MEMBER_EXPRESSION:
//            return eval_member_expression(node, env);
//
//        case NH_ECMASCRIPT_AST_NODE_CALL_EXPRESSION: {
//            /* Evaluate callee */
//            JSValue callee = eval_node(node->CallExpression.Caller_p, env);
//            /* evaluate arguments */
//            size_t n = nh_core_list_size(&node->CallExpression.Arguments);
//            JSValue *args = calloc(n, sizeof(JSValue));
//            for (size_t i = 0; i < n; ++i) {
//                nh_ecmascript_ASTNode *a = (nh_ecmascript_ASTNode*) nh_core_list_get(&node->CallExpression.Arguments, i);
//                args[i] = eval_node(a, env);
//            }
//            /* native function invocation */
//            if (callee.tag == VAL_NATIVE_FUNCTION) {
//                JSValue thisv = js_undefined();
//                JSValue out = callee.native.fn(thisv, args, (int)n);
//                free(args);
//                return out;
//            } else {
//                free(args);
//                return js_undefined(); /* non-native functions not implemented yet */
//            }
//        }
//
//        case NH_ECMASCRIPT_AST_NODE_FOR_STATEMENT: {
//            /* ForStatement.Init, Test, Update, Body */
//            /* Create new lexical env for loop */
//            Env *loopEnv = env_new(env);
//            if (node->ForStatement.Init_p) nh_ecmascript_evaluateNode(node->ForStatement.Init_p, loopEnv);
//            while (true) {
//                if (node->ForStatement.Test_p) {
//                    JSValue t = nh_ecmascript_evaluateNode(node->ForStatement.Test_p, loopEnv);
//                    if (! (t.tag == VAL_BOOLEAN ? t.boolean : (js_to_number(t) != 0.0)) ) break;
//                }
//                /* execute body */
//                nh_ecmascript_evaluteNode(node->ForStatement.Body_p, loopEnv);
//                /* update */
//                if (node->ForStatement.Update_p) nh_ecmascript_evaluateNode(node->ForStatement.Update_p, loopEnv);
//            }
//            /* TODO: free loopEnv bindings if necessary */
//            return js_undefined();
//        }

        default:
            /* Not implemented: return undefined */
            return nh_ecmascript_normalEmptyCompletion();
    }
}

nh_ecmascript_Completion nh_ecmascript_evaluateScript(
    nh_ecmascript_Script *Script_p)
{
    nh_ecmascript_Environment *GlobalEnvironment_p = Script_p->Realm_p->GlobalEnvironment_p;

    nh_ecmascript_ExecutionContext ScriptContext;
    ScriptContext.Function_p = NULL;
    ScriptContext.Realm_p = Script_p->Realm_p;
    ScriptContext.ScriptOrModule.isScript = true;
    ScriptContext.ScriptOrModule.handle_p = Script_p;
    ScriptContext.VariableEnvironment_p = GlobalEnvironment_p;
    ScriptContext.LexicalEnvironment_p = GlobalEnvironment_p;
    ScriptContext.PrivateEnvironment_p = NULL;

    // Ignore "9. Suspend the running execution context." for now

    nh_core_pushStack(&Script_p->Realm_p->Agent_p->ExecutionContextStack, &ScriptContext);
 
    nh_ecmascript_ASTNode *Root_p = Script_p->ECMAScriptCode_p;
    nh_ecmascript_Completion Result = nh_ecmascript_globalDeclarationInstantiation(Root_p, GlobalEnvironment_p);

    if (1) {
        nh_ecmascript_evaluateNode(Root_p, GlobalEnvironment_p);
    }

    return nh_ecmascript_normalEmptyCompletion();
}
