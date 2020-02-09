#ifndef FONT_SIZES_H
#define FONT_SIZES_H

#include <qsystemdetection.h>

#if defined(Q_OS_MAC)
#define FONT_SIZE_LARGE 16
#define FONT_SIZE_CONTROL 14
#define FONT_SIZE_MEDIUM 13
#define FONT_SIZE_NORMAL 12
#elif defined(Q_OS_WIN)
#define FONT_SIZE_LARGE 13
#define FONT_SIZE_CONTROL 11
#define FONT_SIZE_MEDIUM 10
#define FONT_SIZE_NORMAL 8
#endif

#endif // FONT_SIZES_H
