#include "Window.h"

#include "Init.h"
#include "../../Window/Window.h"
#include "../../Window/Listener.h"
#include "../../Window/Event.h"

#include <android/input.h>
#include <android/keycodes.h>
#include <android/native_activity.h>
#include <android/configuration.h>
#include <android_native_app_glue.h>
#include <pthread.h>
#include <string.h>

static pthread_mutex_t NH_WSI_ANDROID_MUTEX = PTHREAD_MUTEX_INITIALIZER;
static struct android_app *NH_WSI_ANDROID_APP;
static void (*NH_WSI_ANDROID_PREVIOUS_COMMAND)(struct android_app *, int32_t);
static int32_t (*NH_WSI_ANDROID_PREVIOUS_INPUT)(struct android_app *, AInputEvent *);

static float nh_wsi_androidGetScale(struct android_app *app_p)
{
    int density = app_p && app_p->config ? AConfiguration_getDensity(app_p->config) : 0;
    return density > 0 && density != ACONFIGURATION_DENSITY_ANY ?
        (float)density / 160.0f : 1.0f;
}

static nh_api_Window *nh_wsi_getAndroidWindowForInput()
{
    for (int i = 0; i < NH_WSI_LISTENER.Windows.count; ++i) {
        nh_wsi_Window *Window_p = nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, i);
        if (Window_p && Window_p->type == NH_WSI_TYPE_ANDROID) {
            return (nh_api_Window*)Window_p;
        }
    }
    return NULL;
}

static void nh_wsi_updateAndroidWindows(bool clearWindow)
{
    for (int i = 0; i < NH_WSI_LISTENER.Windows.count; ++i) {
        nh_wsi_Window *Window_p = nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, i);
        if (!Window_p || Window_p->type != NH_WSI_TYPE_ANDROID) {
            continue;
        }

        pthread_mutex_lock(&NH_WSI_ANDROID_MUTEX);
        ANativeWindow *NativeWindow_p = clearWindow || !NH_WSI_ANDROID_APP ?
            NULL : NH_WSI_ANDROID_APP->window;
        if (NativeWindow_p != Window_p->Android.Handle) {
            if (NativeWindow_p) {
                ANativeWindow_acquire(NativeWindow_p);
            }
            if (Window_p->Android.Handle) {
                ANativeWindow_release(Window_p->Android.Handle);
            }
            Window_p->Android.Handle = NativeWindow_p;
            ++Window_p->Android.generation;
        }
        ANativeWindow *CurrentWindow_p = Window_p->Android.Handle;
        int width = CurrentWindow_p ? ANativeWindow_getWidth(CurrentWindow_p) : 0;
        int height = CurrentWindow_p ? ANativeWindow_getHeight(CurrentWindow_p) : 0;
        float scale = nh_wsi_androidGetScale(NH_WSI_ANDROID_APP);
        Window_p->scale = scale;
        pthread_mutex_unlock(&NH_WSI_ANDROID_MUTEX);

        if (clearWindow) {
            nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_FOCUS_OUT, 0, 0, 0, 0, 0, 0);
        } else {
            nh_wsi_sendWindowEvent(Window_p, NH_API_WINDOW_CONFIGURE, 0, 0,
                (int)(width / scale), (int)(height / scale), width, height);
        }
    }
}

static void nh_wsi_androidOnAppCommand(
    struct android_app *app_p, int32_t command)
{
    switch (command) {
        case APP_CMD_INIT_WINDOW:
        case APP_CMD_WINDOW_RESIZED:
        case APP_CMD_CONTENT_RECT_CHANGED:
        case APP_CMD_CONFIG_CHANGED:
            nh_wsi_updateAndroidWindows(false);
            break;
        case APP_CMD_TERM_WINDOW:
        case APP_CMD_DESTROY:
            nh_wsi_updateAndroidWindows(true);
            break;
        case APP_CMD_GAINED_FOCUS:
        case APP_CMD_LOST_FOCUS: {
            NH_API_WINDOW_E type = command == APP_CMD_GAINED_FOCUS ?
                NH_API_WINDOW_FOCUS_IN : NH_API_WINDOW_FOCUS_OUT;
            for (int i = 0; i < NH_WSI_LISTENER.Windows.count; ++i) {
                nh_wsi_Window *Window_p = nh_core_getFromLinkedList(&NH_WSI_LISTENER.Windows, i);
                if (Window_p && Window_p->type == NH_WSI_TYPE_ANDROID) {
                    nh_wsi_sendWindowEvent(Window_p, type, 0, 0, 0, 0, 0, 0);
                }
            }
            break;
        }
    }

    if (NH_WSI_ANDROID_PREVIOUS_COMMAND) {
        NH_WSI_ANDROID_PREVIOUS_COMMAND(app_p, command);
    }
}

static NH_API_MODIFIER_FLAG nh_wsi_androidModifiers(int32_t state)
{
    NH_API_MODIFIER_FLAG modifiers = 0;
    if (state & AMETA_SHIFT_ON) modifiers |= NH_API_MODIFIER_SHIFT;
    if (state & AMETA_CAPS_LOCK_ON) modifiers |= NH_API_MODIFIER_LOCK;
    if (state & AMETA_CTRL_ON) modifiers |= NH_API_MODIFIER_CONTROL;
    if (state & AMETA_ALT_ON) modifiers |= NH_API_MODIFIER_MOD1;
    if (state & AMETA_META_ON) modifiers |= NH_API_MODIFIER_MOD4;
    return modifiers;
}

static NH_API_KEY_E nh_wsi_androidKey(int32_t keyCode)
{
    switch (keyCode) {
        case AKEYCODE_DEL: return NH_API_KEY_BACKSPACE;
        case AKEYCODE_TAB: return NH_API_KEY_TAB;
        case AKEYCODE_ENTER: return NH_API_KEY_RETURN;
        case AKEYCODE_ESCAPE: case AKEYCODE_BACK: return NH_API_KEY_ESCAPE;
        case AKEYCODE_FORWARD_DEL: return NH_API_KEY_DELETE;
        case AKEYCODE_SHIFT_LEFT: return NH_API_KEY_SHIFT_L;
        case AKEYCODE_SHIFT_RIGHT: return NH_API_KEY_SHIFT_R;
        case AKEYCODE_CTRL_LEFT: return NH_API_KEY_CONTROL_L;
        case AKEYCODE_CTRL_RIGHT: return NH_API_KEY_CONTROL_R;
        case AKEYCODE_ALT_LEFT: return NH_API_KEY_ALT_L;
        case AKEYCODE_ALT_RIGHT: return NH_API_KEY_ALT_R;
        case AKEYCODE_META_LEFT: return NH_API_KEY_SUPER_L;
        case AKEYCODE_META_RIGHT: return NH_API_KEY_SUPER_R;
        case AKEYCODE_MOVE_HOME: return NH_API_KEY_HOME;
        case AKEYCODE_DPAD_LEFT: return NH_API_KEY_LEFT;
        case AKEYCODE_DPAD_UP: return NH_API_KEY_UP;
        case AKEYCODE_DPAD_RIGHT: return NH_API_KEY_RIGHT;
        case AKEYCODE_DPAD_DOWN: return NH_API_KEY_DOWN;
        case AKEYCODE_PAGE_UP: return NH_API_KEY_PAGE_UP;
        case AKEYCODE_PAGE_DOWN: return NH_API_KEY_PAGE_DOWN;
        case AKEYCODE_MOVE_END: return NH_API_KEY_END;
        case AKEYCODE_F1: return NH_API_KEY_F1;
        case AKEYCODE_F2: return NH_API_KEY_F2;
        case AKEYCODE_F3: return NH_API_KEY_F3;
        case AKEYCODE_F4: return NH_API_KEY_F4;
        case AKEYCODE_F5: return NH_API_KEY_F5;
        case AKEYCODE_F6: return NH_API_KEY_F6;
        case AKEYCODE_F7: return NH_API_KEY_F7;
        case AKEYCODE_F8: return NH_API_KEY_F8;
        case AKEYCODE_F9: return NH_API_KEY_F9;
        case AKEYCODE_F10: return NH_API_KEY_F10;
        case AKEYCODE_F11: return NH_API_KEY_F11;
        case AKEYCODE_F12: return NH_API_KEY_F12;
        default: return NH_API_KEY_NONE;
    }
}

static NH_ENCODING_UTF32 nh_wsi_androidCodepoint(int32_t keyCode, int32_t metaState)
{
    bool shift = (metaState & AMETA_SHIFT_ON) != 0;
    if (keyCode >= AKEYCODE_A && keyCode <= AKEYCODE_Z) {
        bool uppercase = shift != ((metaState & AMETA_CAPS_LOCK_ON) != 0);
        return (NH_ENCODING_UTF32)((uppercase ? 'A' : 'a') + keyCode - AKEYCODE_A);
    }
    if (keyCode >= AKEYCODE_0 && keyCode <= AKEYCODE_9) {
        static const char shiftedDigits[] = ")!@#$%^&*(";
        int digit = keyCode - AKEYCODE_0;
        return (NH_ENCODING_UTF32)(shift ? shiftedDigits[digit] : '0' + digit);
    }
    switch (keyCode) {
        case AKEYCODE_SPACE: return ' ';
        case AKEYCODE_COMMA: return shift ? '<' : ',';
        case AKEYCODE_PERIOD: return shift ? '>' : '.';
        case AKEYCODE_MINUS: return shift ? '_' : '-';
        case AKEYCODE_EQUALS: return shift ? '+' : '=';
        case AKEYCODE_APOSTROPHE: return shift ? '"' : '\'';
        case AKEYCODE_SLASH: return shift ? '?' : '/';
        case AKEYCODE_SEMICOLON: return shift ? ':' : ';';
        case AKEYCODE_LEFT_BRACKET: return shift ? '{' : '[';
        case AKEYCODE_RIGHT_BRACKET: return shift ? '}' : ']';
        case AKEYCODE_BACKSLASH: return shift ? '|' : '\\';
        case AKEYCODE_GRAVE: return shift ? '~' : '`';
        default: return 0;
    }
}

static int32_t nh_wsi_androidOnInputEvent(
    struct android_app *app_p, AInputEvent *event_p)
{
    nh_api_Window *Window_p = nh_wsi_getAndroidWindowForInput();
    int handled = 0;

    if (Window_p && AInputEvent_getType(event_p) == AINPUT_EVENT_TYPE_MOTION) {
        int32_t action = AMotionEvent_getAction(event_p);
        int32_t actionType = action & AMOTION_EVENT_ACTION_MASK;
        size_t pointerIndex = (size_t)((action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
            AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT);
        size_t pointerCount = AMotionEvent_getPointerCount(event_p);
        if (pointerCount > 0) {
            if (pointerIndex >= pointerCount) {
                pointerIndex = 0;
            }
            int x = (int)AMotionEvent_getX(event_p, pointerIndex);
            int y = (int)AMotionEvent_getY(event_p, pointerIndex);
            switch (actionType) {
                case AMOTION_EVENT_ACTION_DOWN:
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                    nh_wsi_sendMouseEvent((nh_wsi_Window*)Window_p, x, y, NH_API_TRIGGER_PRESS, NH_API_MOUSE_LEFT);
                    handled = 1;
                    break;
                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_POINTER_UP:
                    nh_wsi_sendMouseEvent((nh_wsi_Window*)Window_p, x, y, NH_API_TRIGGER_RELEASE, NH_API_MOUSE_LEFT);
                    handled = 1;
                    break;
                case AMOTION_EVENT_ACTION_CANCEL:
                    nh_wsi_sendMouseEvent((nh_wsi_Window*)Window_p, x, y, NH_API_TRIGGER_CANCEL, NH_API_MOUSE_LEFT);
                    handled = 1;
                    break;
                case AMOTION_EVENT_ACTION_MOVE:
                case AMOTION_EVENT_ACTION_HOVER_MOVE:
                    nh_wsi_sendMouseEvent((nh_wsi_Window*)Window_p, x, y, NH_API_TRIGGER_MOVE, NH_API_MOUSE_MOVE);
                    handled = 1;
                    break;
                case AMOTION_EVENT_ACTION_SCROLL: {
                    float vertical = AMotionEvent_getAxisValue(event_p, AMOTION_EVENT_AXIS_VSCROLL, pointerIndex);
                    float horizontal = AMotionEvent_getAxisValue(event_p, AMOTION_EVENT_AXIS_HSCROLL, pointerIndex);
                    if (vertical != 0.0f) {
                        nh_wsi_sendMouseEvent((nh_wsi_Window*)Window_p, x, y,
                            vertical > 0.0f ? NH_API_TRIGGER_UP : NH_API_TRIGGER_DOWN, NH_API_MOUSE_SCROLL);
                    }
                    if (horizontal != 0.0f) {
                        nh_wsi_sendMouseEvent((nh_wsi_Window*)Window_p, x, y,
                            horizontal > 0.0f ? NH_API_TRIGGER_RIGHT : NH_API_TRIGGER_LEFT, NH_API_MOUSE_SCROLL);
                    }
                    handled = vertical != 0.0f || horizontal != 0.0f;
                    break;
                }
            }
        }
    } else if (Window_p && AInputEvent_getType(event_p) == AINPUT_EVENT_TYPE_KEY) {
        int32_t action = AKeyEvent_getAction(event_p);
        if (action == AKEY_EVENT_ACTION_DOWN || action == AKEY_EVENT_ACTION_UP) {
            int32_t keyCode = AKeyEvent_getKeyCode(event_p);
            int32_t metaState = AKeyEvent_getMetaState(event_p);
            NH_API_KEY_E special = nh_wsi_androidKey(keyCode);
            NH_ENCODING_UTF32 codepoint = special == NH_API_KEY_NONE ?
                nh_wsi_androidCodepoint(keyCode, metaState) : 0;
            if (special != NH_API_KEY_NONE || codepoint != 0) {
                nh_wsi_sendKeyboardEvent((nh_wsi_Window*)Window_p, codepoint, special,
                    action == AKEY_EVENT_ACTION_DOWN ? NH_API_TRIGGER_PRESS : NH_API_TRIGGER_RELEASE,
                    nh_wsi_androidModifiers(metaState));
                handled = 1;
            }
        }
    }

    int32_t previousResult = NH_WSI_ANDROID_PREVIOUS_INPUT ?
        NH_WSI_ANDROID_PREVIOUS_INPUT(app_p, event_p) : 0;
    return previousResult || handled;
}

NH_API_RESULT nh_wsi_setAndroidApp(
    struct android_app *app_p)
{
    if (!app_p) {
        return NH_API_ERROR_BAD_STATE;
    }

    pthread_mutex_lock(&NH_WSI_ANDROID_MUTEX);
    if (NH_WSI_ANDROID_APP && NH_WSI_ANDROID_APP != app_p) {
        pthread_mutex_unlock(&NH_WSI_ANDROID_MUTEX);
        return NH_API_ERROR_BAD_STATE;
    }
    if (NH_WSI_ANDROID_APP == app_p) {
        bool callbacksInstalled = app_p->onAppCmd == nh_wsi_androidOnAppCommand &&
            app_p->onInputEvent == nh_wsi_androidOnInputEvent;
        pthread_mutex_unlock(&NH_WSI_ANDROID_MUTEX);
        return callbacksInstalled ? NH_API_SUCCESS : NH_API_ERROR_BAD_STATE;
    }

    NH_WSI_ANDROID_APP = app_p;
    NH_WSI_ANDROID_PREVIOUS_COMMAND = app_p->onAppCmd;
    NH_WSI_ANDROID_PREVIOUS_INPUT = app_p->onInputEvent;
    app_p->onAppCmd = nh_wsi_androidOnAppCommand;
    app_p->onInputEvent = nh_wsi_androidOnInputEvent;
    pthread_mutex_unlock(&NH_WSI_ANDROID_MUTEX);

    if (app_p->window) {
        nh_wsi_updateAndroidWindows(false);
    }
    return NH_API_SUCCESS;
}

void nh_wsi_terminateAndroid()
{
    pthread_mutex_lock(&NH_WSI_ANDROID_MUTEX);
    if (NH_WSI_ANDROID_APP) {
        if (NH_WSI_ANDROID_APP->onAppCmd == nh_wsi_androidOnAppCommand) {
            NH_WSI_ANDROID_APP->onAppCmd = NH_WSI_ANDROID_PREVIOUS_COMMAND;
        }
        if (NH_WSI_ANDROID_APP->onInputEvent == nh_wsi_androidOnInputEvent) {
            NH_WSI_ANDROID_APP->onInputEvent = NH_WSI_ANDROID_PREVIOUS_INPUT;
        }
    }
    NH_WSI_ANDROID_APP = NULL;
    NH_WSI_ANDROID_PREVIOUS_COMMAND = NULL;
    NH_WSI_ANDROID_PREVIOUS_INPUT = NULL;
    pthread_mutex_unlock(&NH_WSI_ANDROID_MUTEX);
}

ANativeWindow *nh_wsi_acquireAndroidNativeWindow(
    nh_wsi_Window *Window_p, uint32_t *generation_p)
{
    if (!Window_p || Window_p->type != NH_WSI_TYPE_ANDROID) {
        return NULL;
    }

    pthread_mutex_lock(&NH_WSI_ANDROID_MUTEX);
    ANativeWindow *NativeWindow_p = Window_p->Android.Handle;
    if (NativeWindow_p) {
        ANativeWindow_acquire(NativeWindow_p);
    }
    if (generation_p) {
        *generation_p = Window_p->Android.generation;
    }
    pthread_mutex_unlock(&NH_WSI_ANDROID_MUTEX);
    return NativeWindow_p;
}

void nh_wsi_releaseAndroidNativeWindow(
    ANativeWindow *Window_p)
{
    if (Window_p) {
        ANativeWindow_release(Window_p);
    }
}

struct android_app *nh_wsi_getAndroidApp()
{
    pthread_mutex_lock(&NH_WSI_ANDROID_MUTEX);
    struct android_app *app_p = NH_WSI_ANDROID_APP;
    pthread_mutex_unlock(&NH_WSI_ANDROID_MUTEX);
    return app_p;
}

NH_API_RESULT nh_wsi_createAndroidWindow(
    nh_wsi_Window *Window_p, nh_wsi_WindowConfig Config,
    nh_gfx_SurfaceRequirements *Requirements_p)
{
    (void)Config;
    (void)Requirements_p;
    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->window) {
        return NH_API_ERROR_BAD_STATE;
    }
    pthread_mutex_lock(&NH_WSI_ANDROID_MUTEX);
    ANativeWindow *NativeWindow_p = app_p->window;
    ANativeWindow_acquire(NativeWindow_p);
    Window_p->Android.Handle = NativeWindow_p;
    Window_p->Android.generation = 1;
    Window_p->scale = nh_wsi_androidGetScale(app_p);
    pthread_mutex_unlock(&NH_WSI_ANDROID_MUTEX);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_destroyAndroidWindow(
    nh_wsi_AndroidWindow *Window_p)
{
    pthread_mutex_lock(&NH_WSI_ANDROID_MUTEX);
    if (Window_p->Handle) {
        ANativeWindow_release(Window_p->Handle);
        Window_p->Handle = NULL;
    }
    ++Window_p->generation;
    pthread_mutex_unlock(&NH_WSI_ANDROID_MUTEX);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_moveAndroidWindow(nh_wsi_AndroidWindow *Window_p)
{
    (void)Window_p;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_getAndroidWindowSize(
    nh_wsi_Window *Window_p, int *width_p, int *height_p)
{
    if (!Window_p || !width_p || !height_p) {
        return NH_API_ERROR_BAD_STATE;
    }
    ANativeWindow *NativeWindow_p = nh_wsi_acquireAndroidNativeWindow(Window_p, NULL);
    if (!NativeWindow_p) {
        return NH_API_ERROR_BAD_STATE;
    }
    *width_p = ANativeWindow_getWidth(NativeWindow_p);
    *height_p = ANativeWindow_getHeight(NativeWindow_p);
    nh_wsi_releaseAndroidNativeWindow(NativeWindow_p);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_showAndroidKeyboard(nh_wsi_AndroidWindow *Window_p)
{
    (void)Window_p;
    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->activity) {
        return NH_API_ERROR_BAD_STATE;
    }
    ANativeActivity_showSoftInput(app_p->activity, ANATIVEACTIVITY_SHOW_SOFT_INPUT_IMPLICIT);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_hideAndroidKeyboard(nh_wsi_AndroidWindow *Window_p)
{
    (void)Window_p;
    struct android_app *app_p = nh_wsi_getAndroidApp();
    if (!app_p || !app_p->activity) {
        return NH_API_ERROR_BAD_STATE;
    }
    ANativeActivity_hideSoftInput(app_p->activity, ANATIVEACTIVITY_HIDE_SOFT_INPUT_NOT_ALWAYS);
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_getAndroidInput(nh_wsi_Window *Window_p, bool *idle_p)
{
    (void)Window_p;
    if (!idle_p) {
        return NH_API_ERROR_BAD_STATE;
    }
    *idle_p = true;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidWindowDecorated(nh_wsi_AndroidWindow *Window_p, bool decorated)
{
    (void)Window_p;
    (void)decorated;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidWindowState(nh_wsi_AndroidWindow *Window_p, bool *state_p)
{
    (void)Window_p;
    (void)state_p;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidWindowType(nh_wsi_AndroidWindow *Window_p, NH_WSI_WINDOW_TYPE_E type)
{
    (void)Window_p;
    (void)type;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidWindowTitle(nh_wsi_AndroidWindow *Window_p, const char *title_p)
{
    (void)Window_p;
    (void)title_p;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_wsi_setAndroidMouseCursor(nh_wsi_AndroidWindow *Window_p, NH_WSI_CURSOR_E cursor)
{
    (void)Window_p;
    (void)cursor;
    return NH_API_SUCCESS;
}
