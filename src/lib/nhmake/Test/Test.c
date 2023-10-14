// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Test.h"
#include "Channel.h"
#include "Process.h"
#include "Library.h"

#include "../Core/Runtime.h"
#include "../Parser/Variables.h"
#include "../Common/Macros.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#if defined(__linux__) || defined(__APPLE__)
    #include <sys/wait.h>
#endif

// TEST ============================================================================================

typedef int (*nh_make_initializer_f)();
typedef void *(*nh_make_customLoader_f)(NH_BYTE *libraryName_p, NH_BYTE *functionName_p);
typedef int (*nh_make_test_f)(int arguments, nh_make_TestArgument *Arguments_p, NH_BYTE *output_p, int maxOutputLength);

static NH_MAKE_RESULT nh_make_runTests(
    nh_make_TestEnvironment *TestEnvironment_p)
{
NH_MAKE_BEGIN()

    void *initLib_p = NULL;
    void *loadLib_p = NULL;

    if (TestEnvironment_p->Initializer_p) 
    {
        initLib_p = nh_make_openLibrary(TestEnvironment_p->Initializer_p->libraryName_p);
        NH_MAKE_CHECK_NULL(initLib_p)

        nh_make_initializer_f initializer_f = nh_make_loadSymbol(initLib_p, TestEnvironment_p->Initializer_p->functionName_p);
        NH_MAKE_CHECK_NULL(initializer_f)

        // run custom initializer
        if (initializer_f()) {NH_MAKE_END(NH_MAKE_ERROR_BAD_STATE)}
    }

    nh_make_customLoader_f customLoader_f = NULL;

    if (TestEnvironment_p->Loader_p) 
    {
        loadLib_p = nh_make_openLibrary(TestEnvironment_p->Loader_p->libraryName_p);
        NH_MAKE_CHECK_NULL(loadLib_p)

        customLoader_f = nh_make_loadSymbol(loadLib_p, TestEnvironment_p->Loader_p->functionName_p);
        NH_MAKE_CHECK_NULL(customLoader_f)
    }

    NH_MAKE_BOOL match = NH_MAKE_FALSE;

    for (int i = 0; i < TestEnvironment_p->TestArray_p->length; ++i) 
    {
        nh_make_Test *Test_p = &TestEnvironment_p->TestArray_p->Tests_p[i];
        nh_make_test_f test_f = NULL;

        if (strcmp(Test_p->Context_p->name_p, TestEnvironment_p->target_p)) {continue;}

        if (customLoader_f) {
            test_f = customLoader_f(Test_p->Context_p->name_p, Test_p->name_p);
        }
        else {
            Test_p->dl_p = nh_make_openLibrary(Test_p->Context_p->name_p);
            NH_MAKE_CHECK_NULL(Test_p->dl_p)
            test_f = nh_make_loadSymbol(Test_p->dl_p, Test_p->name_p);
        }

        NH_MAKE_CHECK_NULL(test_f)

        NH_BYTE result_p[1024];
        memset(result_p, 0, 1024);

        // perform the test
        int result = test_f(Test_p->arguments, Test_p->Arguments_p, result_p, 1024);

        nh_make_messagef("%d: %s", result, result_p);

        match = NH_MAKE_TRUE;
    }

    if (match) {
        nh_make_messagef("");
    }

NH_MAKE_END(NH_MAKE_SUCCESS)
}

static nh_make_TestEnvironment nh_make_initializeTestEnvironment(
    nh_make_Runtime *Runtime_p, nh_make_ExternalFunction *Initializer_p, nh_make_ExternalFunction *Loader_p,
    NH_BYTE *target_p)
{
NH_MAKE_BEGIN()

    nh_make_TestEnvironment TestEnvironment;
    TestEnvironment.target_p = target_p;

    nh_make_Variable *InitLibrary_p = nh_make_getVariable(&Runtime_p->VariableArray, "TEST_INITIALIZER_SOURCE");
    nh_make_Variable *InitFunction_p = nh_make_getVariable(&Runtime_p->VariableArray, "TEST_INITIALIZER");

    if (InitLibrary_p && InitFunction_p) {
        Initializer_p->libraryName_p = InitLibrary_p->values_pp[0];
        Initializer_p->functionName_p = InitFunction_p->values_pp[0];
        TestEnvironment.Initializer_p = Initializer_p;
    }
    else {TestEnvironment.Initializer_p = NULL;}
  
    nh_make_Variable *LoadLibrary_p = nh_make_getVariable(&Runtime_p->VariableArray, "TEST_LOADER_SOURCE");
    nh_make_Variable *LoadFunction_p = nh_make_getVariable(&Runtime_p->VariableArray, "TEST_LOADER");

    if (LoadLibrary_p && LoadFunction_p) {
        Loader_p->libraryName_p = LoadLibrary_p->values_pp[0];
        Loader_p->functionName_p = LoadFunction_p->values_pp[0];
        TestEnvironment.Loader_p = Loader_p;
    }
    else {TestEnvironment.Loader_p = NULL;}

NH_MAKE_END(TestEnvironment)
}

static NH_MAKE_RESULT nh_make_runTestEnvironment(
    nh_make_TestEnvironment *TestEnvironment_p, nh_make_TestArray *TestArray_p)
{
NH_MAKE_BEGIN()

    TestEnvironment_p->Process = nh_make_fork();
    TestEnvironment_p->TestArray_p = TestArray_p;

    if (TestEnvironment_p->Process.id == 0) {
        exit(nh_make_runTests(TestEnvironment_p));
    }

    pid_t w;
    int wstatus;

    do {
        w = waitpid(TestEnvironment_p->Process.id, &wstatus, WUNTRACED | WCONTINUED);
        if (w == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
//        if (WIFEXITED(wstatus)) {
//            printf("exited, status=%d\n", WEXITSTATUS(wstatus));
//        } else if (WIFSIGNALED(wstatus)) {
//            printf("killed by signal %d\n", WTERMSIG(wstatus));
//        } else if (WIFSTOPPED(wstatus)) {
//            printf("stopped by signal %d\n", WSTOPSIG(wstatus));
//        } else if (WIFCONTINUED(wstatus)) {
//            printf("continued\n");
//        }
    } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

NH_MAKE_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_test(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    nh_make_ExternalFunction Initializer;
    nh_make_ExternalFunction Loader;

    nh_make_TestEnvironment TestEnvironment = 
        nh_make_initializeTestEnvironment(Runtime_p, &Initializer, &Loader, name_p);

    NH_MAKE_CHECK(nh_make_runTestEnvironment(&TestEnvironment, &Runtime_p->TestArray))

NH_MAKE_END(NH_MAKE_SUCCESS)
}

//NH_BYTE *nh_make_getTestResult(
//    nh_make_Process *Fork_p)
//{
//NH_MAKE_BEGIN()
//
//    if (Fork_p->id != 0) {
//        NH_BYTE *receive_p = nh_make_readFromChannel(&Fork_p->IPC.Out, NULL);
//        if (receive_p != NULL) {
//            NH_MAKE_CHECK(nh_make_handleIPCReceive(receive_p))
//        }
//// TODO free?
//    }
//
//NH_MAKE_END(NH_MAKE_SIGNAL_OK)
//}
//
//NH_MAKE_RESULT nh_make_destroyTestEnvironment(
//    nh_make_TestEnvironment *TestEnvironment_p)
//{
//NH_MAKE_BEGIN()
//
//NH_MAKE_END(NH_MAKE_SUCCESS)
//}


// TEST ARRAY ======================================================================================

void nh_make_initTestArray(
    nh_make_TestArray *Array_p)
{
NH_MAKE_BEGIN()

    Array_p->length = 0;
    Array_p->Tests_p = NULL;

NH_MAKE_SILENT_END()
}

static nh_make_Test nh_make_initTest()
{
NH_MAKE_BEGIN()

    nh_make_Test Test;
    Test.Context_p = NULL;
    Test.name_p = NULL;
    Test.dl_p = NULL;
    Test.arguments = 0;
    Test.Arguments_p = NULL;

NH_MAKE_END(Test)
}

NH_MAKE_RESULT nh_make_addTest(
    nh_make_Runtime *Runtime_p, nh_make_TestArray *TestArray_p, nh_make_Function *Function_p)
{
NH_MAKE_BEGIN()

// TODO add test depending on lib/bin etc. not just name

//    nh_make_SourceContext *Context_p = NULL;
//    nh_make_Test *Test_p = NULL;
//
//    for (int i = 0; i < Function_p->arguments; ++i) 
//    {
//        if (Function_p->argumentTypes_p[i] == NH_MAKE_TOKEN_IDENTIFIER) {
//            Context_p = nh_make_getSourceContext(Runtime_p, Function_p->arguments_pp[i], type);
//        }
//        else {
//
//            if (!Context_p) {
//                NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)
//            }
//
//            if (!Test_p)
//            {
//                if (!TestArray_p->Tests_p) {
//                    TestArray_p->Tests_p = malloc(sizeof(nh_make_Test));
//                    NH_MAKE_CHECK_NULL(TestArray_p->Tests_p)
//                }
//                else {
//                    TestArray_p->Tests_p = realloc(TestArray_p->Tests_p, sizeof(nh_make_Test) * (TestArray_p->length + 1));
//                    NH_MAKE_CHECK_NULL(TestArray_p->Tests_p)
//                }
//        
//                Test_p = &TestArray_p->Tests_p[TestArray_p->length];
//                *Test_p = nh_make_initTest();
//
//                Test_p->Context_p = Context_p;
//                Test_p->name_p = malloc(strlen(Function_p->arguments_pp[i]) + 1);
//                NH_MAKE_CHECK_NULL(Test_p->name_p)
//                sprintf(Test_p->name_p, Function_p->arguments_pp[i]);
//        
//                TestArray_p->length++;
//                
//                NH_BYTE offset_p[64];
//                nh_make_getIndentAfterSourceContext(Context_p->name_p, offset_p, 64, ContextArray_p);
//                nh_make_messagef("[%s]%s Add test \"%s\"", Context_p->name_p, offset_p, Test_p->name_p);
//            }
//            else
//            {
//                if (!Test_p->Arguments_p) {
//                    Test_p->Arguments_p = malloc(sizeof(nh_make_TestArgument));
//                    NH_MAKE_CHECK_NULL(Test_p->Arguments_p)
//                }
//                else {
//                    Test_p->Arguments_p = realloc(Test_p->Arguments_p, sizeof(nh_make_TestArgument) * (Test_p->arguments + 1));
//                    NH_MAKE_CHECK_NULL(Test_p->Arguments_p)
//                }
//        
//                nh_make_TestArgument *TestArgument_p = &Test_p->Arguments_p[Test_p->arguments];
//
//                TestArgument_p->p = malloc(strlen(Function_p->arguments_pp[i]) + 1);
//                NH_MAKE_CHECK_NULL(TestArgument_p->p)
//                sprintf(TestArgument_p->p, Function_p->arguments_pp[i]);
//        
//                Test_p->arguments++;
//            }
//        }
//    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

