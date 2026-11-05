#include "../src/lib/nh-core/Common/Config.c"

#include <assert.h>

static void *values[1];
static nh_core_List setting = {.size = 1, .pp = values};

nh_core_List *nh_core_getGlobalConfigSetting(char *ns, const char *name)
{
    (void)ns;
    assert(!strcmp(name, "nh-core.threads"));
    return &setting;
}

void nh_core_debug(char *text) {fprintf(stderr, "%s\n", text);}

int main(void)
{
    const char *valid[] = {"0", "1", "2", "64"};
    const int expected[] = {0, 1, 2, 64};
    for (unsigned i = 0; i < sizeof(valid) / sizeof(*valid); ++i) {
        nh_core_Config config = {0};
        values[0] = (void*)valid[i];
        assert(nh_core_getSetting(&config, 3, "nh-core.threads") == NH_API_SUCCESS);
        assert(config.threads == expected[i]);
    }
    const char *invalid[] = {"-1", "65", "2147483648", "999999999999999999999", "", "abc", "1x", "1.5"};
    for (unsigned i = 0; i < sizeof(invalid) / sizeof(*invalid); ++i) {
        nh_core_Config config = {0};
        values[0] = (void*)invalid[i];
        assert(nh_core_getSetting(&config, 3, "nh-core.threads") != NH_API_SUCCESS);
        assert(config.threads == -1);
    }
    nh_core_Config config = {0};
    setting.size = 0;
    assert(nh_core_getSetting(&config, 3, "nh-core.threads") != NH_API_SUCCESS);
    assert(config.threads == -1);
    puts("thread configuration tests passed");
    return 0;
}
