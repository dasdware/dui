#include <dui/environment.h>
#include <dui/label.h>
#include <dui/text.h>

void dui_label_impl(const char* caption, const DUI_LabelData data)
{
    const DUI_Environment* env = dui_env();
    const Rectangle bounds = dui_lay_rectangle(.height = env->font_height + 2 * dui_env_spacing(1)).bounds;
    dui_text(caption, bounds, .vertical_alignment = ALIGN_CENTER, .color = DUI_LABEL_TEXT_COLOR(data.kind));
}
