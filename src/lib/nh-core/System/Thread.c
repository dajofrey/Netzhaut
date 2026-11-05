/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022 Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Thread.h"
#include "Process.h"
#include "Memory.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#if defined(_WIN32) || defined(WIN32)
static SRWLOCK NH_THREAD_LOCK = SRWLOCK_INIT;
static CONDITION_VARIABLE NH_THREAD_WAKE = CONDITION_VARIABLE_INIT;
static __declspec(thread) nh_core_Thread *NH_CURRENT_THREAD;
static void lock(void) {AcquireSRWLockExclusive(&NH_THREAD_LOCK);}
static void unlock(void) {ReleaseSRWLockExclusive(&NH_THREAD_LOCK);}
static void wake(void) {WakeAllConditionVariable(&NH_THREAD_WAKE);}
static void waitForChange(void)
{
    if (!SleepConditionVariableSRW(&NH_THREAD_WAKE, &NH_THREAD_LOCK, INFINITE, 0)) {
        fprintf(stderr, "Netzhaut: workload wait failed: %lu\n", (unsigned long)GetLastError());
        abort();
    }
}
#else
#include <unistd.h>
static pthread_mutex_t NH_THREAD_LOCK = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t NH_THREAD_WAKE = PTHREAD_COND_INITIALIZER;
static __thread nh_core_Thread *NH_CURRENT_THREAD;
static void checkThreadResult(int rc)
{
    if (rc) {
        fprintf(stderr, "Netzhaut: thread synchronization failed: %s\n", strerror(rc));
        abort();
    }
}
static void lock(void) {checkThreadResult(pthread_mutex_lock(&NH_THREAD_LOCK));}
static void unlock(void) {checkThreadResult(pthread_mutex_unlock(&NH_THREAD_LOCK));}
static void wake(void) {checkThreadResult(pthread_cond_broadcast(&NH_THREAD_WAKE));}
static void waitForChange(void) {checkThreadResult(pthread_cond_wait(&NH_THREAD_WAKE, &NH_THREAD_LOCK));}
#endif

static nh_core_ThreadPool *pool(void) {return &NH_PROCESS_POOL.Main.ThreadPool;}

static void initWorkload(nh_core_Workload *workload)
{
    int waiters = workload->waiters;
    memset(workload, 0, sizeof(*workload));
    workload->waiters = waiters;
    workload->signal = NH_SIGNAL_INACTIVE;
}

/* The registry and command queues are locked, never user callbacks or I/O. */
static NH_API_RESULT runWorkload(nh_core_Workload *workload, bool *idle)
{
    lock();
    if (workload->Thread_p != NH_CURRENT_THREAD || workload->signal == NH_SIGNAL_INACTIVE) {
        unlock();
        return NH_API_SUCCESS;
    }
    NH_SIGNAL phase = workload->signal;
    if (workload->activating && phase != NH_SIGNAL_INIT) {
        unlock();
        return NH_API_SUCCESS;
    }
    if (workload->deactivate && !workload->running) phase = NH_SIGNAL_FREE;
    if (phase == NH_SIGNAL_FREE && workload->running) {
        unlock();
        return NH_API_SUCCESS;
    }
    nh_core_WorkloadCommand *command = NULL;
    if (phase != NH_SIGNAL_INIT && phase != NH_SIGNAL_FREE) {
        command = workload->FirstCommand_p;
        if (command) {
            workload->FirstCommand_p = command->next_p;
            if (!workload->FirstCommand_p) workload->LastCommand_p = NULL;
        }
    }
    ++workload->running;
    unlock();

    nh_core_Workload *previous = NH_CURRENT_THREAD->CurrentWorkload_p;
    NH_CURRENT_THREAD->CurrentWorkload_p = workload;
    nh_core_SystemTime start = nh_core_getSystemTime();
    NH_SIGNAL result = NH_SIGNAL_OK;
    void *args = workload->args_p;
    if (phase == NH_SIGNAL_INIT) {
        if (workload->init_f) args = workload->init_f(workload);
    } else if (phase == NH_SIGNAL_FREE) {
        if (workload->free_f) workload->free_f(args);
    } else if (command && !command->dummy) {
        if (workload->runCommand_f) result = workload->runCommand_f(args, command);
    } else {
        result = workload->run_f(args);
    }
    NH_CURRENT_THREAD->CurrentWorkload_p = previous;

    lock();
    --workload->running;
    workload->Timing.turnAround = nh_core_getSystemTimeDiffInSeconds(start, nh_core_getSystemTime());
    if (phase == NH_SIGNAL_INIT) {
        workload->args_p = args;
        workload->initialized = true;
        workload->signal = NH_SIGNAL_DONE;
    } else if (phase == NH_SIGNAL_FREE) {
        initWorkload(workload);
    } else {
        workload->signal = result;
        if (result == NH_SIGNAL_IDLE) ++workload->SignalCounter.idle;
        else {
            if (result == NH_SIGNAL_OK) ++workload->SignalCounter.ok;
            if (idle) *idle = false;
        }
        if (result == NH_SIGNAL_FINISH || result == NH_SIGNAL_ERROR) workload->deactivate = true;
        if (result == NH_SIGNAL_ERROR) {
            fprintf(stderr, "Netzhaut: workload failed: %s\n", workload->name_p ? workload->name_p : "(unnamed)");
            pool()->failed = true;
        }
    }
    if (command) command->done = true;
    wake();
    unlock();
    return result == NH_SIGNAL_ERROR ? NH_API_ERROR_BAD_STATE : NH_API_SUCCESS;
}

int nh_core_run(void)
{
    if (!NH_CURRENT_THREAD) {
        fprintf(stderr, "Netzhaut: scheduler called from an unregistered thread\n");
        return -1;
    }
    bool idle = true;
    int count = 0;
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        lock();
        bool assigned = pool()->Workloads_p[i].Thread_p == NH_CURRENT_THREAD;
        unlock();
        if (!assigned) continue;
        if (runWorkload(&pool()->Workloads_p[i], &idle) != NH_API_SUCCESS) return -1;
        ++count;
    }
    lock();
    bool failed = pool()->failed;
    unlock();
    if (idle) nh_sleepMs(10);
    return failed ? -1 : count;
}

#if defined(_WIN32) || defined(WIN32)
static DWORD WINAPI runThread(void *arg)
#else
static void *runThread(void *arg)
#endif
{
    NH_CURRENT_THREAD = arg;
    NH_API_RESULT initialized = nh_core_initThreadMemory();
    lock();
    NH_CURRENT_THREAD->started = true;
    if (initialized != NH_API_SUCCESS) pool()->failed = true;
    wake();
    unlock();
    if (initialized != NH_API_SUCCESS) {
        fprintf(stderr, "Netzhaut: worker memory initialization failed\n");
        return 0;
    }
    for (;;) {
        lock();
        bool stop = pool()->stopping;
        unlock();
        if (stop) break;
        nh_core_run();
    }
    NH_CURRENT_THREAD = NULL;
    return 0;
}

static NH_API_RESULT nh_core_startThread(nh_core_Thread *thread)
{
#if defined(_WIN32) || defined(WIN32)
    thread->handle = CreateThread(NULL, 0, runThread, thread, 0, &thread->id);
    if (!thread->handle) {
        fprintf(stderr, "Netzhaut: CreateThread failed: %lu\n", (unsigned long)GetLastError());
        return NH_API_ERROR_BAD_STATE;
    }
#else
    int rc = pthread_create(&thread->id, NULL, runThread, thread);
    if (rc) {
        fprintf(stderr, "Netzhaut: pthread_create failed: %s\n", strerror(rc));
        return NH_API_ERROR_BAD_STATE;
    }
#endif
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_initThreadPool(nh_core_ThreadPool *threads, int threadCount)
{
    if (threadCount < 0 || threadCount > NH_MAX_THREADS) {
        fprintf(stderr, "Netzhaut: nh-core.threads must be between 0 and %d\n", NH_MAX_THREADS);
        return NH_API_ERROR_BAD_STATE;
    }
    memset(threads, 0, sizeof(*threads));
    threads->ready = true;
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) initWorkload(&threads->Workloads_p[i]);
    NH_CURRENT_THREAD = &threads->Main;
#if defined(_WIN32) || defined(WIN32)
    threads->Main.id = GetCurrentThreadId();
#else
    threads->Main.id = pthread_self();
#endif
    for (int i = 0; i < threadCount; ++i) {
        if (nh_core_startThread(&threads->Threads_p[i]) != NH_API_SUCCESS) {
            nh_core_freeThreadPool(threads);
            return NH_API_ERROR_BAD_STATE;
        }
        lock();
        ++threads->threadCount;
        while (!threads->Threads_p[i].started) waitForChange();
        bool failed = threads->failed;
        unlock();
        if (failed) {
            nh_core_freeThreadPool(threads);
            return NH_API_ERROR_BAD_STATE;
        }
    }
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_freeThreadPool(nh_core_ThreadPool *threads)
{
    if (NH_CURRENT_THREAD != &threads->Main) {
        fprintf(stderr, "Netzhaut: thread pool shutdown must run on main\n");
        return NH_API_ERROR_BAD_STATE;
    }
    lock();
    threads->closing = true;
    unlock();
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) nh_core_deactivateWorkload(&threads->Workloads_p[i]);
    lock();
    threads->stopping = true;
    wake();
    unlock();
    NH_API_RESULT result = NH_API_SUCCESS;
    for (int i = 0; i < threads->threadCount; ++i) {
#if defined(_WIN32) || defined(WIN32)
        if (WaitForSingleObject(threads->Threads_p[i].handle, INFINITE) != WAIT_OBJECT_0 ||
            !CloseHandle(threads->Threads_p[i].handle)) {
            fprintf(stderr, "Netzhaut: worker join failed: %lu\n", (unsigned long)GetLastError());
            result = NH_API_ERROR_BAD_STATE;
        }
#else
        int rc = pthread_join(threads->Threads_p[i].id, NULL);
        if (rc) {
            fprintf(stderr, "Netzhaut: worker join failed: %s\n", strerror(rc));
            result = NH_API_ERROR_BAD_STATE;
        }
#endif
    }
    lock();
    threads->threadCount = 0;
    threads->ready = false;
    unlock();
    NH_CURRENT_THREAD = NULL;
    return result;
}

bool nh_core_keepRunning(void)
{
    nh_core_checkForks();
    return nh_core_activeWorkloads(true) > 0 || nh_core_activeForks() > 0;
}

nh_core_Workload *nh_core_getWorkloads(void) {return pool()->Workloads_p;}
nh_core_Thread *nh_core_getThread(void) {return NH_CURRENT_THREAD;}

static nh_core_Workload *findWorkload(void *args)
{
    if (!args) return NULL;
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        nh_core_Workload *workload = &pool()->Workloads_p[i];
        if (workload->signal != NH_SIGNAL_INACTIVE && workload->initialized && workload->args_p == args) return workload;
    }
    return NULL;
}

nh_core_Workload *nh_core_getWorkload(void *args)
{
    lock();
    nh_core_Workload *workload = findWorkload(args);
    unlock();
    return workload;
}

nh_core_Thread *nh_core_getThreadFromArgs(void *args)
{
    lock();
    nh_core_Workload *workload = findWorkload(args);
    nh_core_Thread *thread = workload ? workload->Thread_p : NULL;
    unlock();
    return thread;
}

/* This is the legacy graphics-resource index, not a workload placement number. */
int nh_core_getThreadIndex(void)
{
    for (int i = 0; i < NH_MAX_THREADS; ++i) {
        if (NH_CURRENT_THREAD == &pool()->Threads_p[i]) return i;
    }
    return 0;
}

static void nh_core_assignToThread(nh_core_Workload *workload, int thread)
{
    workload->Thread_p = thread == 0 || pool()->threadCount == 0 ?
        &pool()->Main : &pool()->Threads_p[thread - 1];
}

void nh_core_waitForCompletion(nh_core_Workload *workload, NH_SIGNAL signal)
{
    if (!workload) {
        fprintf(stderr, "Netzhaut: cannot wait for a NULL workload\n");
        return;
    }
    lock();
    ++workload->waiters;
    if (workload->Thread_p == NH_CURRENT_THREAD) {
        unlock();
        runWorkload(workload, NULL);
        lock();
    } else {
        while (signal == NH_SIGNAL_DONE ? !workload->initialized : workload->signal != signal) waitForChange();
    }
    --workload->waiters;
    unlock();
}

void nh_core_runOrSleep(nh_core_Workload *workload)
{
    lock();
    bool own = workload->Thread_p == NH_CURRENT_THREAD;
    unlock();
    if (own) runWorkload(workload, NULL);
    else nh_sleepMs(1);
}

void *nh_core_activateWorkload(
    void *(*init_f)(nh_core_Workload*), NH_SIGNAL (*run_f)(void*), void (*free_f)(void*),
    NH_SIGNAL (*runCommand_f)(void*, nh_core_WorkloadCommand*), void *args, bool crucial, int thread)
{
    lock();
    if (!run_f || !pool()->ready || pool()->closing || pool()->failed ||
        thread < 0 || (pool()->threadCount && thread > pool()->threadCount)) {
        fprintf(stderr, "Netzhaut: invalid workload activation (thread %d, workers %d)\n", thread, pool()->threadCount);
        unlock();
        return NULL;
    }
    nh_core_Workload *workload = NULL;
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        if (pool()->Workloads_p[i].signal == NH_SIGNAL_INACTIVE && !pool()->Workloads_p[i].waiters) {
            workload = &pool()->Workloads_p[i];
            break;
        }
    }
    if (!workload) {
        fprintf(stderr, "Netzhaut: workload pool exhausted\n");
        unlock();
        return NULL;
    }
    workload->args_p = args;
    workload->init_f = init_f;
    workload->run_f = run_f;
    workload->free_f = free_f;
    workload->runCommand_f = runCommand_f;
    workload->crucial = crucial;
    workload->signal = NH_SIGNAL_INIT;
    workload->activating = true;
    nh_core_assignToThread(workload, thread);
    ++workload->waiters;
    wake();
    unlock();
    nh_core_waitForCompletion(workload, NH_SIGNAL_DONE);
    lock();
    args = workload->args_p;
    workload->activating = false;
    --workload->waiters;
    unlock();
    return args;
}

NH_API_RESULT nh_core_deactivateWorkload(nh_core_Workload *workload)
{
    NH_CORE_CHECK_NULL(workload)
    lock();
    if (workload->signal == NH_SIGNAL_INACTIVE) {
        unlock();
        return NH_API_SUCCESS;
    }
    workload->deactivate = true;
    /* A callback may deactivate itself; free only after its stack unwinds. */
    if (workload->Thread_p == NH_CURRENT_THREAD && workload->running) {
        unlock();
        return NH_API_SUCCESS;
    }
    ++workload->waiters;
    wake();
    unlock();
    nh_core_waitForCompletion(workload, NH_SIGNAL_INACTIVE);
    lock();
    --workload->waiters;
    unlock();
    return NH_API_SUCCESS;
}

void *nh_core_getWorkloadArg(void)
{
    return NH_CURRENT_THREAD && NH_CURRENT_THREAD->CurrentWorkload_p ? NH_CURRENT_THREAD->CurrentWorkload_p->args_p : NULL;
}

static NH_API_RESULT execute(void *handle, nh_core_WorkloadCommand *command)
{
    lock();
    nh_core_Workload *workload = findWorkload(handle);
    if (!workload || workload->deactivate) {
        fprintf(stderr, "Netzhaut: command targets an inactive workload\n");
        unlock();
        return NH_API_ERROR_BAD_STATE;
    }
    ++workload->waiters;
    if (workload->LastCommand_p) workload->LastCommand_p->next_p = command;
    else workload->FirstCommand_p = command;
    workload->LastCommand_p = command;
    wake();
    while (!command->done && workload->signal != NH_SIGNAL_INACTIVE) {
        if (workload->Thread_p == NH_CURRENT_THREAD) {
            unlock();
            runWorkload(workload, NULL);
            lock();
        } else waitForChange();
    }
    bool done = command->done;
    --workload->waiters;
    unlock();
    if (!done) fprintf(stderr, "Netzhaut: workload stopped before command completed\n");
    return done ? NH_API_SUCCESS : NH_API_ERROR_BAD_STATE;
}

void *nh_core_executeWorkloadCommand(void *handle, int type, void *p, int byteSize)
{
    if (byteSize < 0 || (byteSize && !p)) {
        fprintf(stderr, "Netzhaut: invalid workload command data\n");
        return NULL;
    }
    nh_core_WorkloadCommand command = {.type = type, .p = p, .length = byteSize};
    if (byteSize) {
        command.p = nh_core_allocate((size_t)byteSize + 1);
        NH_CORE_CHECK_MEM_2(NULL, command.p)
        memcpy(command.p, p, byteSize);
        ((char*)command.p)[byteSize] = 0;
    }
    NH_API_RESULT result = execute(handle, &command);
    if (byteSize) nh_core_free(command.p);
    return result == NH_API_SUCCESS ? command.result_p : NULL;
}

NH_API_RESULT nh_core_executeWorkload(void *handle)
{
    nh_core_WorkloadCommand command = {.dummy = true};
    return execute(handle, &command);
}

int nh_core_activeThreads(void)
{
    lock();
    int count = pool()->threadCount;
    unlock();
    return count;
}

int nh_core_activeWorkloads(bool onlyCrucial)
{
    lock();
    int count = 0;
    for (int i = 0; i < NH_MAX_WORKLOADS; ++i) {
        nh_core_Workload *workload = &pool()->Workloads_p[i];
        if (workload->signal != NH_SIGNAL_INACTIVE && (!onlyCrucial || workload->crucial)) ++count;
    }
    unlock();
    return count;
}

NH_API_RESULT nh_sleepMs(int milliseconds)
{
    if (milliseconds < 0) {
        fprintf(stderr, "Netzhaut: negative sleep duration\n");
        return NH_API_ERROR_BAD_STATE;
    }
#if defined(_WIN32) || defined(WIN32)
    Sleep(milliseconds);
#else
    struct timespec delay = {milliseconds / 1000, (milliseconds % 1000) * 1000000L};
    while (nanosleep(&delay, &delay)) {
        if (errno != EINTR) {
            fprintf(stderr, "Netzhaut: nanosleep failed: %s\n", strerror(errno));
            return NH_API_ERROR_BAD_STATE;
        }
    }
#endif
    return NH_API_SUCCESS;
}
