#ifndef DUI_ENVIRONMENT_H
#define DUI_ENVIRONMENT_H

#include <stdbool.h>
#include <arena.h>

#include <dui/colors.h>
#include <dui/context.h>

typedef struct
{
    const DUI_Palette* palette;
    DUI_Shade background_shade;
    DUI_Shade text_shade;
    DUI_Shade disabled_text_shade;
    DUI_Shade label_shade;
    DUI_Shade normal_shade;
    DUI_Shade hover_shade;
    DUI_Shade down_shade;
    DUI_Shade focus_frame_shade;
} DUI_PaletteShades;

typedef enum
{
    ENV_DEFAULT,
    ENV_PRIMARY,
    ENV_SECONDARY,
} DUI_EnvironmentColorKind;

typedef enum
{
    ENV_SHADE_BACKGROUND,
    ENV_SHADE_TEXT,
    ENV_SHADE_DISABLED_TEXT,
    ENV_SHADE_LABEL,
    ENV_SHADE_NORMAL,
    ENV_SHADE_HOVER,
    ENV_SHADE_DOWN,
    ENV_SHADE_FOCUS_FRAME,
} DUI_EnvironmentColorShade;

typedef struct
{
    Arena memory;
    bool initialized;

    DUI_ContextElement root_context;
    DUI_ContextElement* context_stack_top;

    DUI_PaletteShades primary_shades;
    DUI_PaletteShades secondary_shades;

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

Color dui_env_color(DUI_EnvironmentColorKind kind, DUI_EnvironmentColorShade shade);

int dui_env_next_element_kind();

#endif // DUI_ENVIRONMENT_H
