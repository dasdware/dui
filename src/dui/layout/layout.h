#ifndef DUI_LAYOUT_LAYOUT_H
#define DUI_LAYOUT_LAYOUT_H

#include <dui/element.h>
#include <dui/theme.h>

#define DUI_PADDING(bounds, left, top, right, bottom) \
    CLITERAL(Rectangle) {                             \
        .x = (bounds).x + DUI_SPACING(left),                     \
        .y = (bounds).y + DUI_SPACING(top),                      \
        .width = (bounds).width - DUI_SPACING(left) - DUI_SPACING(right),   \
        .height = (bounds).height - DUI_SPACING(top) - DUI_SPACING(bottom), \
    }

#define DUI_PADDING_ALL(bounds, size) DUI_PADDING(bounds, size, size, size, size)

typedef enum {
    DUI_VERTICAL,
    DUI_HORIZONTAL,
} DUI_Direction;

typedef struct {
    Rectangle bounds;
    bool tabOrderBack;
} DUI_BoundsData;

typedef struct DUI_LayoutElement DUI_LayoutElement;
typedef DUI_BoundsData (*DUI_LayoutCallback)(void* element, DUI_LayoutData data);
typedef void (*DUI_LayoutResetCallback)(void* element);

struct DUI_LayoutElement {
    DUI_Element element;
    DUI_LayoutElement* parent_layout_element;
    DUI_LayoutCallback callback;
    DUI_LayoutResetCallback reset_callback;
};

#define dui_next_bounds(...) dui_next_bounds_impl(CLITERAL(DUI_LayoutData) { __VA_ARGS__ })
DUI_BoundsData dui_next_bounds_impl(DUI_LayoutData layout_data);

#define dui_begin_layout(type, id, data, element) dui_begin_layout_impl(type, id, data, sizeof(*element), (void**)&element)
void dui_begin_layout_impl(DUI_Id type, int id, DUI_LayoutData data, size_t size, void** element);

void dui_end_layout();

DUI_LayoutData dui_forward_layout(int preferred_width, int preferred_height, DUI_LayoutData data);

#endif // DUI_LAYOUT_LAYOUT_H
