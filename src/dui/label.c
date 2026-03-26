#include <dui/label.h>
#include <dui/text.h>
#include <dui/theme.h>

void dui_label_impl(const char* caption, const DUI_LabelData data) {
    const Rectangle bounds = dui_lay_rectangle(.height = DUI_FONT_SIZE + 2 * DUI_SPACING(1)).bounds;
    dui_text(caption, bounds, .vertical_alignment = ALIGN_CENTER, .color = DUI_LABEL_TEXT_COLOR(data.kind));
}
