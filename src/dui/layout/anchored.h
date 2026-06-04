#ifndef DUI_LAYOUT_ANCHORED_H
#define DUI_LAYOUT_ANCHORED_H

#include <dui/layout/layout.h>
#include <dui/element.h>

#define DUI_ANCHORED_TYPE_ID DUI_ID("dui-anchored", 12, 340178968)

typedef struct {
    DUI_LayoutData layout_data;
    int gap;
} DUI_AnchoredData;

typedef struct {
    DUI_LayoutElement layout_element;
    int gap;
    int inset_left;
    int inset_top;
    int inset_right;
    int inset_bottom;
} DUI_AnchoredElement;

#define dui_anchored(...) dui_anchored_impl_(__COUNTER__, __VA_ARGS__)
#define dui_anchored_impl_(id, ...) dui_anchored_impl(id, __VA_ARGS__)
#define dui_anchored_impl(id, ...) \
bool dui__guard_ ## id = true; \
for (dui_begin_anchored_impl(id, CLITERAL(DUI_AnchoredData) { __VA_ARGS__ }); dui__guard_ ## id; dui_end_anchored(), dui__guard_ ## id = false)

#define dui_begin_anchored(...) dui_begin_anchored_impl(__COUNTER__, CLITERAL(DUI_AnchoredData) { __VA_ARGS__ })
void dui_begin_anchored_impl(int id, DUI_AnchoredData data);

#define dui_end_anchored dui_end_layout

#endif // DUI_LAYOUT_ANCHORED_H
