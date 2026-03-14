#ifndef DUIX_H
#define DUIX_H

#include <dui/animation.h>
#include <dui/button.h>
#include <dui/checkbox.h>
#include <dui/colors.h>
#include <dui/context.h>
#include <dui/edit.h>
#include <dui/environment.h>
#include <dui/keyboard.h>
#include <dui/label.h>
#include <dui/layout.h>
#include <dui/text.h>
#include <dui/types.h>

#define dui_main() dui_main_impl_(__COUNTER__)
#define dui_main_impl_(id) dui_main_impl(id)
#define dui_main_impl(id) \
    bool dui__guard_ ## id = true; \
    for (dui_env_begin(); dui__guard_ ## id; dui_env_end(), dui__guard_ ## id = false)


#define dui_screen(...) dui_screen_impl_(__COUNTER__, __VA_ARGS__)
#define dui_screen_impl_(id, ...) dui_screen_impl(id, __VA_ARGS__)
#define dui_screen_impl(id, ...) \
    bool dui__guard_ ## id = true; \
    for (dui_lay_begin_screen_impl(id, CLITERAL(DUI_Layout_ScreenData) { __VA_ARGS__ }); dui__guard_ ## id; dui_lay_end(), dui__guard_ ## id = false)

#define dui_anchored(...) dui_anchored_impl_(__COUNTER__, __VA_ARGS__)
#define dui_anchored_impl_(id, ...) dui_anchored_impl(id, __VA_ARGS__)
#define dui_anchored_impl(id, ...) \
    bool dui__guard_ ## id = true; \
    for (dui_lay_begin_anchored_impl(id, CLITERAL(DUI_Layout_AnchoredData) { __VA_ARGS__ }); dui__guard_ ## id; dui_lay_end(), dui__guard_ ## id = false)

#define dui_stack(...) dui_stack_impl_(__COUNTER__, __VA_ARGS__)
#define dui_stack_impl_(id, ...) dui_stack_impl(id, __VA_ARGS__)
#define dui_stack_impl(id, ...) \
    bool dui__guard_ ## id = true; \
    for (dui_lay_begin_stack_impl(id, CLITERAL(DUI_Layout_StackData) { __VA_ARGS__ }); dui__guard_ ## id; dui_lay_end(), dui__guard_ ## id = false)

#define dui_spaced(...) dui_spaced_impl_(__COUNTER__, __VA_ARGS__)
#define dui_spaced_impl_(id, ...) dui_spaced_impl(id, __VA_ARGS__)
#define dui_spaced_impl(id, ...) \
    bool dui__guard_ ## id = true; \
    for (dui_lay_begin_spaced_impl(id, CLITERAL(DUI_Layout_SpacedData) { __VA_ARGS__ }); dui__guard_ ## id; dui_lay_end(), dui__guard_ ## id = false)

#endif // DUIX_H
