#import "FramelessMainWindow.h"
#import <AppKit/NSView.h>
#import <AppKit/NSWindow.h>

FramelessMainWindow::FramelessMainWindow(QWidget *parent) : RemembersStateMainWindow(parent) {
    auto windowId = this->winId();
    NSView *nativeView = reinterpret_cast<NSView *>(windowId);
    NSWindow *nativeWindow = [nativeView window];

    [nativeWindow setStyleMask:[nativeWindow styleMask] | NSWindowStyleMaskFullSizeContentView | NSWindowTitleHidden];
    [nativeWindow setTitlebarAppearsTransparent:YES];
    [nativeWindow setTitleVisibility:NSWindowTitleHidden];

    setContentsMargins(0, 25, 0, 0); // account for titlebar
}
