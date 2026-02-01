#include <dui/environment.h>
#include <dui/text.h>

Vector2 dui_text_measure(const char* text)
{
    if (text == NULL)
    {
        return CLITERAL(Vector2){ 0, 0 };
    }

    const DUI_Environment* env = dui_env();
    return MeasureTextEx(env->font, text, env->font_height, env->font_spacing);
}

void dui_text_impl(const char* text, const Rectangle bounds, const DUI_TextData data)
{
    const DUI_Environment* env = dui_env();
    const Vector2 text_size = dui_text_measure(text);

    int x;
    switch (data.horizontal_alignment)
    {
    case ALIGN_CENTER:
        x = bounds.x + (bounds.width - text_size.x) / 2;
        break;
    case ALIGN_END:
        x = bounds.x + bounds.width - text_size.x;
        break;
    case ALIGN_DEFAULT:
    case ALIGN_START:
    default:
        x = bounds.x;
        break;
    }

    int y;
    switch (data.vertical_alignment)
    {
    case ALIGN_CENTER:
        y = bounds.y + (bounds.height - text_size.y) / 2;
        break;
    case ALIGN_END:
        y = bounds.y + bounds.height - text_size.y;
        break;
    case ALIGN_DEFAULT:
    case ALIGN_START:
    default:
        y = bounds.y;
        break;
    }

    DrawTextEx(env->font, text, CLITERAL(Vector2) {x + data.offset_x, y + data.offset_y}, env->font_height, env->font_spacing, data.color);
}
