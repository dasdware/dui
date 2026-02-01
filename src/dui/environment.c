#include <math.h>
#include <raylib.h>
#include <dui/environment.h>
#include <dui/keyboard.h>

DUI_Environment* dui_env()
{
    static DUI_Environment environment = {0};
    if (!environment.initialized)
    {
        environment.context_stack_top = &environment.root_context;

        environment.primary_shades.palette = &PALETTE_SKY;
        environment.primary_shades.background_shade = SHADE_950;
        environment.primary_shades.text_shade = SHADE_50;
        environment.primary_shades.disabled_text_shade = SHADE_500;
        environment.primary_shades.normal_shade = SHADE_700;
        environment.primary_shades.hover_shade = SHADE_900;
        environment.primary_shades.down_shade = SHADE_950;
        environment.primary_shades.focus_frame_shade = SHADE_200;

        environment.secondary_shades.palette = &PALETTE_STONE;
        environment.secondary_shades.background_shade = SHADE_950;
        environment.secondary_shades.text_shade = SHADE_50;
        environment.secondary_shades.disabled_text_shade = SHADE_400;
        environment.secondary_shades.normal_shade = SHADE_600;
        environment.secondary_shades.hover_shade = SHADE_700;
        environment.secondary_shades.down_shade = SHADE_800;
        environment.secondary_shades.focus_frame_shade = SHADE_200;

        environment.focus_frame_offset = 0;
        environment.focus_frame_width = 2;

        environment.font_height = 20;
        environment.font_spacing = 0;
        environment.font = LoadFontEx("./resources/fonts/open-sans.regular.ttf", environment.font_height, NULL, 0);

        environment.initialized = true;
    }
    return &environment;
}

int dui_env_spacing(const float n)
{
    const DUI_Environment* env = dui_env();
    return round(n / 4 * env->font_height);
}

void dui_env_load_font(const char* file_name, const int size, const float spacing)
{
    DUI_Environment* env = dui_env();
    UnloadFont(env->font);

    env->font_height = size;
    env->font_spacing = spacing;
    env->font = LoadFontEx(file_name, env->font_height, NULL, 0);
}

void dui_env_clear_background()
{
    ClearBackground(dui_env_color(ENV_SECONDARY, ENV_SHADE_BACKGROUND));
}

void dui_env_draw_focus_frame(const Rectangle bounds, const Color color)
{
    const DUI_Environment* env = dui_env();

    const Rectangle focus_frame_bounds = {
        bounds.x - env->focus_frame_offset - env->focus_frame_width,
        bounds.y - env->focus_frame_offset - env->focus_frame_width,
        bounds.width + 2 * (env->focus_frame_offset + env->focus_frame_width),
        bounds.height + 2 * (env->focus_frame_offset + env->focus_frame_width),
    };
    DrawRectangleLinesEx(focus_frame_bounds, env->focus_frame_width, color);
}

void dui_env_focus_impl(DUI_Element* element)
{
    dui_env()->focused_element = element;
}

bool dui_env_has_focus_impl(const DUI_Element* element)
{
    return dui_env()->focused_element == element;
}

void dui_env_begin()
{
    BeginDrawing();
    dui_env_clear_background();

    dui_env()->root_context.indices.count = 0;
}

void dui_env_end()
{
    EndDrawing();

    dui_handle_global_keys();
}

Color dui_env_color(DUI_EnvironmentColorKind kind, DUI_EnvironmentColorShade shade)
{
    const DUI_Environment* env = dui_env();
    const DUI_PaletteShades* shades = NULL;

    switch (kind)
    {
    case ENV_PRIMARY:
        shades = &env->primary_shades;
        break;
    case ENV_SECONDARY:
    case ENV_DEFAULT:
    default:
        shades = &env->secondary_shades;
        break;
    }

    switch (shade)
    {
    case ENV_SHADE_TEXT:
        return dui_palette_color(shades->palette, shades->text_shade);
    case ENV_SHADE_DISABLED_TEXT:
        return dui_palette_color(shades->palette, shades->disabled_text_shade);
    case ENV_SHADE_NORMAL:
        return dui_palette_color(shades->palette, shades->normal_shade);
    case ENV_SHADE_HOVER:
        return dui_palette_color(shades->palette, shades->hover_shade);
    case ENV_SHADE_DOWN:
        return dui_palette_color(shades->palette, shades->down_shade);
    case ENV_SHADE_FOCUS_FRAME:
        return dui_palette_color(shades->palette, shades->focus_frame_shade);
    case ENV_SHADE_BACKGROUND:
    default:
        return dui_palette_color(shades->palette, shades->background_shade);
    }
}

int dui_env_next_element_kind()
{
    static int current_element_kind = 0;
    current_element_kind += 1;
    return current_element_kind;
}
