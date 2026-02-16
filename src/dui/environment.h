#ifndef DUI_ENVIRONMENT_H
#define DUI_ENVIRONMENT_H

#include <stdbool.h>
#include <arena.h>

#include <dui/colors.h>
#include <dui/context.h>

typedef struct
{
    Arena memory;
    bool initialized;

    DUI_ContextElement root_context;
    DUI_ContextElement* context_stack_top;

    int focus_frame_offset;
    int focus_frame_width;

    Font font;
    int font_height;
    float font_spacing;

    DUI_Element* focused_element;
} DUI_Environment;

DUI_Environment *dui_env();
void dui_env_begin();
void dui_env_end();

int dui_env_spacing(float n);

void dui_env_load_font(const char* file_name, int size, float spacing);

void dui_env_clear_background();
void dui_env_draw_focus_frame(Rectangle bounds, Color color);

#define dui_env_focus(element) dui_env_focus_impl((DUI_Element*) (element))
void dui_env_focus_impl(DUI_Element* element);

#define dui_env_has_focus(element) dui_env_has_focus_impl((DUI_Element*) (element))
bool dui_env_has_focus_impl(const DUI_Element* element);

int dui_env_next_element_kind();

#endif // DUI_ENVIRONMENT_H
