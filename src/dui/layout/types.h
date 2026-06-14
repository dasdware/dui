#ifndef DUI_LAYOUT_TYPES_H
#define DUI_LAYOUT_TYPES_H

#include <stdbool.h>

typedef enum {
    DUI_REMAINING,
    DUI_LEFT,
    DUI_TOP,
    DUI_RIGHT,
    DUI_BOTTOM,
} DUI_Anchor;

typedef struct {
    int size;
    int width;
    int height;
    int weight;
    bool remaining;
    bool opposite;
    DUI_Anchor anchor;
} DUI_Placement;

#define DUI_PLACEMENT(...) .placement = CLITERAL(DUI_Placement) { __VA_ARGS__ }
#define DUI_P DUI_PLACEMENT


#endif // DUI_LAYOUT_TYPES_H
