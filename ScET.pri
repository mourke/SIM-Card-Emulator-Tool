HEADERS += Colors.h \
    Errors.h \
    FileManager.h \
    Version.h \
    Tracer/APDUSplitter.h \
    Tracer/APDUCommand.h \
    Tracer/SIMTraceUSB.h \
    Windows/MainWindow.h \
    Widgets/Buttons/TitleBarButton.h \
    ApplicationLayerDelegate.h \
    Widgets/AboutDialog.h \
    UpdateManager.h \
    Windows/FramelessMainWindow.h \
    Tracer/Tracer.h \
    Tracer/TracerManager.h \
    Widgets/Segment.h \
    Widgets/SegmentedControl.h \
    Widgets/Buttons/ToolBarButton.h \
    FontSizes.h \
    Windows/RemembersStateMainWindow.h
SOURCES += ApplicationLayerDelegate.cpp \
    Errors.cpp \
    FileManager.cpp \
    main.cpp \
    Tracer/APDUCommand.cpp \
    Tracer/APDUSplitter.cpp \
    Tracer/Tracer.cpp \
    Tracer/TracerManager.cpp \
    Widgets/Segment.cpp \
    Widgets/SegmentedControl.cpp \
    Widgets/Buttons/ToolBarButton.cpp \
    Widgets/Buttons/TitleBarButton.cpp \
    Windows/MainWindow.cpp \
    Windows/RemembersStateMainWindow.cpp
FORMS += Widgets/AboutDialog.ui \
    Windows/MainWindow.ui
TRANSLATIONS += Resources/Localisation/scet_es.ts \
    Resources/Localisation/scet_fr.ts
RESOURCES += Resources/ScET.qrc

win32 {
    SOURCES += Colors.cpp \
               Windows/FramelessMainWindow.cpp \
               Widgets/AboutDialog.cpp \
               singleapplication.cpp \
               singleapplication_p.cpp \
               UpdateManager.cpp
    HEADERS += singleapplication.h
}

mac {
    OBJECTIVE_SOURCES += Colors.mm \
                         Windows/FramelessMainWindow.mm \
                         Widgets/AboutDialog.mm \
                         CocoaInitializer.mm \
                         UpdateManager.mm
    HEADERS += OpenFileApplication.h \
               CocoaInitializer.h
    SOURCES += OpenFileApplication.cpp
}
