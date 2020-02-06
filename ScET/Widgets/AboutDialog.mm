#include "AboutDialog.h"
#include <QApplication>
#import <AppKit/NSView.h>
#import <AppKit/NSWindow.h>
#import <AppKit/NSButton.h>
#import <AppKit/NSColor.h>
#import <QDebug>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    ui.setupUi(this);

    ui.appVersion->setText(ui.appVersion->text() + " " + QApplication::applicationVersion());

    auto windowId = this->winId();
    NSView *nativeView = reinterpret_cast<NSView *>(windowId);
    NSWindow *nativeWindow = [nativeView window];

    [[nativeWindow standardWindowButton:NSWindowZoomButton] setHidden:YES];
    [nativeWindow setTitleVisibility:NSWindowTitleHidden];
}
