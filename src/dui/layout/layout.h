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

#define DUI_CENTER(bounds, w, h)                      \
    CLITERAL(Rectangle) {                             \
        .x = (bounds).x + ((bounds).width - (w)) / 2, \
        .y = (bounds).y + ((bounds).height - (h)) / 2, \
        .width = (w),                                 \
        .height = (h),                                \
    }

typedef enum {
    DUI_VERTICAL,
    DUI_HORIZONTAL,
} DUI_Direction;

typedef struct {
    Rectangle bounds;
    bool tabOrderBack;
} DUI_BoundsData;

typedef struct DUI_LayoutElement DUI_LayoutElement;
typedef DUI_BoundsData (*DUI_LayoutCallback)(void* element, DUI_Placement placement);
typedef void (*DUI_LayoutResetCallback)(void* element);

struct DUI_LayoutElement {
    DUI_Element element;
    DUI_LayoutElement* parent_layout_element;
    DUI_LayoutCallback callback;
    DUI_LayoutResetCallback reset_callback;
};

#define dui_next_bounds(...) dui_next_bounds_impl(CLITERAL(DUI_Placement) { __VA_ARGS__ })
DUI_BoundsData dui_next_bounds_impl(DUI_Placement placement);

#define dui_begin_layout(type, id, data, element) dui_begin_layout_impl(type, id, data, sizeof(*element), (void**)&element)
void dui_begin_layout_impl(DUI_Id type, int id, DUI_Placement placement, size_t size, void** element);

#define dui_begin_inactive_layout(type, id, element) dui_begin_inactive_layout_impl(type, id, sizeof(*element), (void**)&element)
void dui_begin_inactive_layout_impl(DUI_Id type, int id, size_t size, void** element);

void dui_end_layout();

DUI_Placement dui_forward_placement(int preferred_width, int preferred_height, DUI_Placement placement);

#endif // DUI_LAYOUT_LAYOUT_H
