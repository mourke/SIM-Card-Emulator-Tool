#include "Colors.h"
#import <AppKit/NSColor.h>
#import <AppKit/NSColorSpace.h>

QColor accentColor() {
    QColor systemAccentColor;

    if (@available(macOS 10.14, *)) {
        NSColor *color = [[NSColor controlAccentColor] colorUsingColorSpace:[NSColorSpace deviceRGBColorSpace]];
        systemAccentColor = QColor(static_cast<int>(255.0 * color.redComponent), static_cast<int>(255.0 * color.greenComponent), static_cast<int>(255.0 * color.blueComponent), static_cast<int>(255.0 * color.alphaComponent));
    }

    return systemAccentColor;
}
