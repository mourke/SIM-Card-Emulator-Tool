include($$PWD/../ScET.pri)

TEMPLATE = app
TARGET = ScET

QT += core gui widgets svg
win32 {
    QT += winextras network
}
CONFIG += console
DEFINES += QAPPLICATION_CLASS=QApplication
INCLUDEPATH += $$PWD/../Frameworks/include \
               $$PWD/GeneratedFiles \
               . \
               ./Widgets
DEPENDPATH += $$PWD/../Frameworks/include \
              $$PWD/GeneratedFiles \
              .
PRE_TARGETDEPS += $$PWD/../Frameworks/lib/libusb-1.0.a
LIBS += -L$$PWD/../Frameworks/lib/ -lusb-1.0

mac {
    QMAKE_LFLAGS += -F$$PWD/../Frameworks
    QMAKE_CXXFLAGS += -F$$PWD/../Frameworks
    FRAMEWORKS.files = $$files($$PWD/../Frameworks/*.framework, true)
    FRAMEWORKS.path = Contents/Frameworks
    QMAKE_BUNDLE_DATA += FRAMEWORKS
    LIBS += -framework Sparkle

    LIBS += -framework AppKit -framework Foundation
    ICON += Resources/ScET.icns
    QMAKE_INFO_PLIST += Resources/Info.plist
    FILE_ICON.files = Resources/FileIcon.icns
    FILE_ICON.path = Contents/Resources
    QMAKE_BUNDLE_DATA += FILE_ICON
}

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../x64/Debug
    MOC_DIR += $$PWD/GeneratedFiles/Debug
    OBJECTS_DIR += $$PWD/x64/Debug
} else {
    DESTDIR = $$PWD/../x64/Release
    MOC_DIR += $$PWD/GeneratedFiles/Release
    OBJECTS_DIR += $$PWD/x64/Release
}

UI_DIR += $$PWD/GeneratedFiles
RCC_DIR += $$PWD/GeneratedFiles

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

