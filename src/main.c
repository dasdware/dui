#include <stdio.h>

#include <raylib.h>

#include <dui/duix.h>

#define ARENA_IMPLEMENTATION
#include <arena.h>
#include <locale.h>

int main(void) {
    setlocale(LC_CTYPE, "");

    InitWindow(50 * 16, 50 * 9, "DUI");
    SetTargetFPS(60);

    Nob_String_Builder primary_text = {0};
    nob_sb_append_cstr(&primary_text, "Primary Edit");
    Nob_String_Builder secondary_text = {0};
    nob_sb_append_cstr(&secondary_text, "Secondary Edit");
    Nob_String_Builder default_text = {0};
    nob_sb_append_cstr(&default_text, "Default Edit");
    Nob_String_Builder disabled_text = {0};
    nob_sb_append_cstr(&disabled_text, "Disabled Edit");

    bool running = true;
    while (running && !WindowShouldClose()) {
        dui_main() {
            dui_screen(.padding = 3) {
                dui_stack(.gap = 2) {
                    dui_spaced(.gap = 2, .direction = DIRECTION_HORIZONTAL, .count = 4) {
                        dui_stack(.gap = 2) {
                            dui_label("Buttons");

                            dui_button(.caption = "Primary", .kind = DUI_PRIMARY);
                            dui_button(.caption = "Secondary", .kind = DUI_SECONDARY);
                            dui_button(.caption = "Default");
                            dui_button(.caption = "Disabled", .disabled = true);
                        }

                        dui_stack(.gap = 2) {
                            dui_label("Labels");
                            dui_label("Primary", .kind = DUI_PRIMARY);
                            dui_label("Secondary", .kind = DUI_SECONDARY);
                            dui_label("Default");
                        }

                        dui_stack(.gap = 2) {
                            dui_label("Checkboxes");
                            static bool primary_checked = true;
                            dui_checkbox(.caption = "Primary", .kind = DUI_PRIMARY, .checked = &primary_checked);
                            dui_checkbox(.caption = "Secondary", .kind = DUI_SECONDARY);
                            dui_checkbox(.caption = "Default");
                            dui_checkbox(.caption = "Disabled", .disabled = true);
                        }

                        dui_stack(.gap = 2) {
                            dui_label("Edits");

                            dui_edit(.kind = DUI_PRIMARY, .text = &primary_text);
                            dui_edit(.kind = DUI_SECONDARY, .text = &secondary_text);
                            dui_edit(.text = &default_text);
                            dui_edit(.disabled = true, .text = &disabled_text);
                        }
                    }

                    if (dui_button(.caption = "Exit", .kind = DUI_PRIMARY, .opposite = true)) {
                        running = false;
                    }
                }
            }
        }
    }
    CloseWindow();

    return 0;
}
