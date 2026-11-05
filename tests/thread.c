#include "../src/lib/nh-core/System/Process.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

nh_ProcessPool NH_PROCESS_POOL;
void nh_core_debug(char *text) {fprintf(stderr, "%s\n", text);}
void nh_core_checkForks(void) {}
int nh_core_activeForks(void) {return 0;}

typedef struct State {
    nh_core_Thread *owner;
    unsigned runs, freed;
    int value;
    bool finish, fail;
    bool deactivate_self;
    struct State *child;
} State;

static void *init(nh_core_Workload *workload)
{
    State *s = workload->args_p;
    s->owner = nh_core_getThread();
    return s;
}

static NH_SIGNAL run(void *arg)
{
    State *s = arg;
    assert(s->owner == nh_core_getThread());
    assert(nh_core_getWorkloadArg() == s);
    __atomic_add_fetch(&s->runs, 1, __ATOMIC_SEQ_CST);
    if (s->child) {
        int value = 1;
        assert(nh_core_executeWorkloadCommand(s->child, 0, &value, sizeof(value)));
        assert(nh_core_getWorkloadArg() == s);
    }
    if (s->deactivate_self) {
        assert(nh_core_deactivateWorkload(nh_core_getWorkload(s)) == NH_API_SUCCESS);
        assert(!__atomic_load_n(&s->freed, __ATOMIC_SEQ_CST));
    }
    if (s->fail) return NH_SIGNAL_ERROR;
    return s->finish ? NH_SIGNAL_FINISH : NH_SIGNAL_IDLE;
}

static void destroy(void *arg)
{
    State *s = arg;
    assert(s->owner == nh_core_getThread());
    __atomic_add_fetch(&s->freed, 1, __ATOMIC_SEQ_CST);
}

static NH_SIGNAL command(void *arg, nh_core_WorkloadCommand *cmd)
{
    State *s = arg;
    assert(s->owner == nh_core_getThread());
    s->value += *(int*)cmd->p;
    cmd->result_p = &s->value;
    return NH_SIGNAL_OK;
}

static void *sendCommands(void *arg)
{
    for (int i = 0; i < 200; ++i) {
        int value = 1;
        assert(nh_core_executeWorkloadCommand(arg, 0, &value, sizeof(value)));
    }
    return NULL;
}

static void test(int count)
{
    nh_core_ThreadPool *threads = &NH_PROCESS_POOL.Main.ThreadPool;
    assert(nh_core_initThreadPool(threads, count) == NH_API_SUCCESS);
    assert(nh_core_activeThreads() == count);
    nh_sleepMs(30);
    State main = {0}, worker = {0};
    assert(nh_core_activateWorkload(init, run, destroy, command, &main, true, 0) == &main);
    assert(main.owner == &threads->Main);
    nh_core_waitForCompletion(nh_core_getWorkload(&main), NH_SIGNAL_OK);
    assert(main.runs == 1);
    assert(nh_core_activateWorkload(init, run, destroy, command, &worker, false, count ? count : 1) == &worker);
    assert(worker.owner == (count ? &threads->Threads_p[count - 1] : &threads->Main));
    assert(nh_core_activeWorkloads(true) == 1 && nh_core_keepRunning());
    int value = 7;
    assert(nh_core_executeWorkloadCommand(&worker, 1, &value, sizeof(value)) == &worker.value);
    assert(worker.value == value);
    if (count) {
        pthread_t senders[2];
        for (int i = 0; i < 2; ++i) assert(!pthread_create(&senders[i], NULL, sendCommands, &worker));
        for (int i = 0; i < 2; ++i) assert(!pthread_join(senders[i], NULL));
        assert(worker.value == value + 400);
    }
    assert(nh_core_executeWorkload(&worker) == NH_API_SUCCESS);
    assert(__atomic_load_n(&worker.runs, __ATOMIC_SEQ_CST) > 0);
    assert(!nh_core_activateWorkload(init, run, destroy, NULL, &worker, false, -1));
    if (count) assert(!nh_core_activateWorkload(init, run, destroy, NULL, &worker, false, count + 1));
    assert(nh_core_deactivateWorkload(nh_core_getWorkload(&main)) == NH_API_SUCCESS);
    assert(!nh_core_keepRunning());
    assert(nh_core_freeThreadPool(threads) == NH_API_SUCCESS);
    assert(main.freed == 1 && worker.freed == 1);
    assert(nh_core_activeThreads() == 0);
}

static void testTermination(bool failure)
{
    nh_core_ThreadPool *threads = &NH_PROCESS_POOL.Main.ThreadPool;
    assert(nh_core_initThreadPool(threads, 1) == NH_API_SUCCESS);
    State state = {.finish = !failure, .fail = failure};
    assert(nh_core_activateWorkload(init, run, destroy, NULL, &state, false, 1) == &state);
    for (int i = 0; i < 1000 && !__atomic_load_n(&state.freed, __ATOMIC_SEQ_CST); ++i) nh_sleepMs(1);
    assert(state.freed == 1);
    if (failure) assert(nh_core_run() == -1);
    assert(nh_core_freeThreadPool(threads) == NH_API_SUCCESS);
}

static void testNested(void)
{
    nh_core_ThreadPool *threads = &NH_PROCESS_POOL.Main.ThreadPool;
    assert(nh_core_initThreadPool(threads, 1) == NH_API_SUCCESS);
    State child = {0}, parent = {.child = &child, .deactivate_self = true};
    assert(nh_core_activateWorkload(init, run, destroy, command, &child, false, 1) == &child);
    assert(nh_core_activateWorkload(init, run, destroy, NULL, &parent, false, 1) == &parent);
    for (int i = 0; i < 1000 && !__atomic_load_n(&parent.freed, __ATOMIC_SEQ_CST); ++i) nh_sleepMs(1);
    assert(parent.freed == 1 && child.value == 1);
    assert(nh_core_freeThreadPool(threads) == NH_API_SUCCESS);
    assert(child.freed == 1);
}

int main(void)
{
    assert(nh_core_initThreadPool(&NH_PROCESS_POOL.Main.ThreadPool, -1) != NH_API_SUCCESS);
    assert(nh_core_initThreadPool(&NH_PROCESS_POOL.Main.ThreadPool, NH_MAX_THREADS + 1) != NH_API_SUCCESS);
    test(0);
    test(1);
    test(2);
    testTermination(false);
    testTermination(true);
    testNested();
    puts("thread tests passed");
    return 0;
}
