#ifndef DUI_THEME_H
#define DUI_THEME_H

#include <math.h>

// Font
#ifndef DUI_FONT_FILE
#define DUI_FONT_FILE "./resources/fonts/open-sans.regular.ttf"
#endif

#ifndef DUI_FONT_SIZE
#define DUI_FONT_SIZE 20
#endif

#ifndef DUI_FONT_SPACING
#define DUI_FONT_SPACING 0
#endif

#define DUI_SPACING(n) round(((n) * DUI_FONT_SIZE) / 4)

#endif // DUI_THEME_H
