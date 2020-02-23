#ifndef COCOA_INITIALIZER_H
#define COCOA_INITIALIZER_H

class CocoaInitializer {

public:
    CocoaInitializer();
    ~CocoaInitializer();

private:
    class CocoaInitializerPrivate;
    CocoaInitializerPrivate *p;
};

#endif // COCOA_INITIALIZER_H
