#include <stdio.h>

#include <raylib.h>
#include <dui/environment.h>
#include <dui/button.h>
#include <dui/checkbox.h>

#define ARENA_IMPLEMENTATION
#include <arena.h>

#include <dui/layout.h>
#include <dui/label.h>

int main(void)
{
    InitWindow(50*16, 50*9, "DUI");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        dui_env_begin();
        {
            dui_lay_begin_screen(.padding = 3);
            {
                dui_lay_begin_stack(.gap = 2);
                {
                    dui_lay_begin_stack(.gap = 2, .direction = DIRECTION_HORIZONTAL, .item_size = 200);
                    {
                        dui_lay_begin_stack(.gap = 2);
                        {
                            dui_label("Buttons");

                            dui_button(.caption = "Primary", .kind = DUI_PRIMARY);
                            dui_button(.caption = "Secondary", .kind = DUI_SECONDARY);
                            dui_button(.caption = "Default");
                            dui_button(.caption = "Disabled", .disabled = true);
                        }
                        dui_lay_end();

                        dui_lay_begin_stack(.gap = 2);
                        {
                            dui_label("Labels");
                            dui_label("Primary", .kind = DUI_PRIMARY);
                            dui_label("Secondary", .kind = DUI_SECONDARY);
                            dui_label("Default");
                        }
                        dui_lay_end();

                        dui_lay_begin_stack(.gap = 2);
                        {
                            dui_label("Checkboxes");
                            static bool primary_checked = true;
                            dui_checkbox(.caption = "Primary", .kind = DUI_PRIMARY, .checked = &primary_checked);
                            dui_checkbox(.caption = "Secondary", .kind = DUI_SECONDARY);
                            dui_checkbox(.caption = "Default");
                            dui_checkbox(.caption = "Disabled", .disabled = true);
                        }
                        dui_lay_end();
                    }
                    dui_lay_end();

                    if (dui_button(.caption = "Exit", .kind = DUI_PRIMARY, .opposite = true))
                    {
                        break;
                    }
                }
                dui_lay_end();
            }
            dui_lay_end();
        }
        dui_env_end();
    }
    CloseWindow();

    return 0;
}
