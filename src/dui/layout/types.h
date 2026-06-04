#ifndef DUI_LAYOUT_TYPES_H
#define DUI_LAYOUT_TYPES_H

#include <stdbool.h>

typedef enum {
    DUI_REMAINING,
    DUI_LEFT,
    DUI_TOP,
    DUI_RIGHT,
    DUI_BOTTOM,
} DUI_LayoutAnchor;

typedef struct {
    int size;
    int width;
    int height;
    int weight;
    bool remaining;
    bool opposite;
    DUI_LayoutAnchor anchor;
} DUI_LayoutData;

#define DUI_LAYOUT(...) .layout_data = CLITERAL(DUI_LayoutData) { __VA_ARGS__ }
#define DUI_L DUI_LAYOUT


#endif // DUI_LAYOUT_TYPES_H
