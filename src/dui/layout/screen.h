#ifndef DUI_LAYOUT_SCREEN_H
#define DUI_LAYOUT_SCREEN_H

#include <dui/layout/layout.h>
#include <dui/element.h>

#define DUI_SCREEN_TYPE_ID DUI_ID("dui-screen", 10, 2301373524)

typedef struct {
    DUI_Placement placement;
    int padding;
} DUI_ScreenData;

typedef struct {
    DUI_LayoutElement layout_element;
    int padding;
} DUI_ScreenElement;

#define dui_screen(...) dui_screen_impl_(__COUNTER__, __VA_ARGS__)
#define dui_screen_impl_(id, ...) dui_screen_impl(id, __VA_ARGS__)
#define dui_screen_impl(id, ...) \
    bool dui__guard_ ## id = true; \
    for (dui_begin_screen_impl(id, CLITERAL(DUI_ScreenData) { __VA_ARGS__ }); dui__guard_ ## id; dui_end_screen(), dui__guard_ ## id = false)

#define dui_begin_screen(...) dui_begin_screen_impl(__COUNTER__, CLITERAL(DUI_ScreenData) { __VA_ARGS__ })
void dui_begin_screen_impl(int id, DUI_ScreenData data);

#define dui_end_screen dui_end_layout

#endif // DUI_LAYOUT_SCREEN_H
