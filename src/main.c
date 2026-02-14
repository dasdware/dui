#include <stdio.h>

#include <raylib.h>
#include <dui/environment.h>
#include <dui/button.h>

#define ARENA_IMPLEMENTATION
#include <arena.h>

#include <dui/layout.h>
#include <dui/label.h>

int main(void)
{
    InitWindow(800, 600, "DUI");
    SetTargetFPS(60);

    DUI_Environment* env = dui_env();
    env->primary_shades.palette = &PALETTE_SKY;

    while (!WindowShouldClose())
    {
        dui_env_begin();
        {
            dui_lay_begin_screen(.padding = 3);
            {
                dui_lay_begin_stack(.gap = 2);
                {
                    dui_label("Buttons");

                    dui_button(.caption = "Secondary");
                    dui_button(.caption = "Secondary (disabled)", .disabled = true);
                    dui_button(.caption = "Primary", .kind = ENV_PRIMARY);
                    dui_button(.caption = "Primary (disabled)", .kind = ENV_PRIMARY, .disabled = true);

                    dui_label("Labels");
                    dui_label("Secondary");
                    dui_label("Primary", .kind = ENV_PRIMARY);


                    if (dui_button(.caption = "Exit", .kind = ENV_PRIMARY, .opposite = true))
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
