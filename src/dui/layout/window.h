#ifndef DUI_LAYOUT__WINDOW_H
#define DUI_LAYOUT__WINDOW_H

#include <dui/layout/layout.h>
#include <dui/element.h>

#define DUI_WINDOW_TYPE_ID DUI_ID("dui-window", 10, 2464886124)

typedef struct {
    DUI_Placement placement;

    Rectangle bounds;
    float transparency;
} DUI_WindowData;

typedef struct {
    DUI_LayoutElement layout_element;

    Rectangle bounds;
} DUI_WindowElement;

#define dui_window(...) dui_window_impl_(__COUNTER__, __VA_ARGS__)
#define dui_window_impl_(id, ...) dui_window_impl(id, __VA_ARGS__)
#define dui_window_impl(id, ...) \
    bool dui__guard_ ## id = true; \
    for (dui_begin_window_impl(id, CLITERAL(DUI_WindowData) { __VA_ARGS__ }); dui__guard_ ## id; dui_end_window(), dui__guard_ ## id = false)

#define dui_begin_window(...) dui_begin_window_impl(__COUNTER__, CLITERAL(DUI_WindowData) { __VA_ARGS__ })
void dui_begin_window_impl(int id, DUI_WindowData data);

#define dui_end_window dui_end_window_impl
void dui_end_window_impl();

#endif // DUI_LAYOUT__WINDOW_H
