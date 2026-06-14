//
// Created by thomas on 04.06.2026.
//

#ifndef DUI_LAYOUT_SPACED_H
#define DUI_LAYOUT_SPACED_H

#include <dui/layout/layout.h>
#include <dui/element.h>

#define DUI_SPACED_TYPE_ID DUI_ID("dui-spaced", 10, 2316177380)

typedef struct {
    DUI_Placement placement;
    DUI_Direction direction;
    int count;
    int gap;
} DUI_SpacedData;

typedef struct {
    DUI_LayoutElement layout_element;
    DUI_Direction direction;
    int count;
    int gap;
    int cursor;
} DUI_SpacedElement;

#define dui_spaced(...) dui_spaced_impl_(__COUNTER__, __VA_ARGS__)
#define dui_spaced_impl_(id, ...) dui_spaced_impl(id, __VA_ARGS__)
#define dui_spaced_impl(id, ...) \
    bool dui__guard_ ## id = true; \
    for (dui_begin_spaced_impl(id, CLITERAL(DUI_SpacedData) { __VA_ARGS__ }); dui__guard_ ## id; dui_end_spaced(), dui__guard_ ## id = false)

#define dui_begin_spaced(...) dui_begin_spaced_impl(__COUNTER__, CLITERAL(DUI_SpacedData) { __VA_ARGS__ })
void dui_begin_spaced_impl(int id, DUI_SpacedData data);

#define dui_end_spaced dui_end_layout


#endif // DUI_LAYOUT_SPACED_H
