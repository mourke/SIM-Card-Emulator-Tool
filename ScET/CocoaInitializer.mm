#include "CocoaInitializer.h"
#include <Foundation/NSAutoreleasePool.h>

class CocoaInitializer::CocoaInitializerPrivate {
    public:
        NSAutoreleasePool *autoReleasePool;
};

CocoaInitializer::CocoaInitializer() {
    p = new CocoaInitializerPrivate();
    p->autoReleasePool = [[NSAutoreleasePool alloc] init];
}

CocoaInitializer::~CocoaInitializer() {
    [p->autoReleasePool release];
    delete p;
}
