#import "../src/lib/nh-wsi/Platforms/IOS/Window.m"

#include <assert.h>

static int configureEvents;

void nh_wsi_sendWindowEvent(nh_wsi_Window *window, NH_API_WINDOW_E type,
    int x, int y, int width, int height, int pixelWidth, int pixelHeight)
{
    (void)window; (void)x; (void)y;
    assert(type == NH_API_WINDOW_CONFIGURE);
    assert(width == 400 && height == 800);
    assert(pixelWidth == 800 && pixelHeight == 1600);
    ++configureEvents;
}

void nh_wsi_sendKeyboardEvent(nh_wsi_Window *window, NH_ENCODING_UTF32 codepoint,
    NH_API_KEY_E special, NH_API_TRIGGER_E trigger, NH_API_MODIFIER_FLAG state)
{
    (void)window; (void)codepoint; (void)special; (void)trigger; (void)state;
}

void nh_wsi_sendMouseEvent(nh_wsi_Window *window, int x, int y,
    NH_API_TRIGGER_E trigger, NH_API_MOUSE_E type)
{
    (void)window; (void)x; (void)y; (void)trigger; (void)type;
}

NH_API_RESULT nh_wsi_setClipboard(char *text, int size, bool selfOwned)
{
    (void)text; (void)size; (void)selfOwned;
    return NH_API_SUCCESS;
}

@interface KeyboardTestView : UIView
@property(nonatomic, strong) UIWindow *attachedWindow;
@end
@implementation KeyboardTestView
- (UIWindow *)window { return self.attachedWindow; }
- (CGRect)convertRect:(CGRect)rect fromView:(UIView *)view { (void)view; return rect; }
@end

static void postFrame(NSString *name, CGRect frame)
{
    [[NSNotificationCenter defaultCenter] postNotificationName:name object:nil
        userInfo:@{UIKeyboardFrameEndUserInfoKey: [NSValue valueWithCGRect:frame]}];
}

int main(void)
{
    @autoreleasepool {
        nh_wsi_Window window = {0};
        NHCustomViewController *controller = [NHCustomViewController new];
        controller.Window_p = &window;
        KeyboardTestView *view = [[KeyboardTestView alloc] initWithFrame:CGRectMake(0, 0, 400, 800)];
        view.attachedWindow = [UIWindow new];
        view.contentScaleFactor = 2;
        controller.view = view;

        postFrame(UIKeyboardWillChangeFrameNotification, CGRectMake(0, 500, 400, 300));
        assert(window.keyboardInsetBottom == 600 && configureEvents == 1);
        postFrame(UIKeyboardWillChangeFrameNotification, CGRectMake(0, 400, 400, 400));
        assert(window.keyboardInsetBottom == 800 && configureEvents == 2);
        postFrame(UIKeyboardWillChangeFrameNotification, CGRectMake(40, 500, 300, 300));
        assert(window.keyboardInsetBottom == 0 && configureEvents == 3);
        postFrame(UIKeyboardWillChangeFrameNotification, CGRectMake(0, 500, 400, 300));
        assert(window.keyboardInsetBottom == 600 && configureEvents == 4);
        postFrame(UIKeyboardWillHideNotification, CGRectMake(0, 500, 400, 300));
        assert(window.keyboardInsetBottom == 0 && configureEvents == 5);
        postFrame(UIKeyboardWillHideNotification, CGRectZero);
        assert(window.keyboardInsetBottom == 0 && configureEvents == 5);
    }
    return 0;
}
