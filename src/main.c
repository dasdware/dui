#include <stdio.h>

#include <raylib.h>
#include <dui/environment.h>
#include <dui/button.h>

#define ARENA_IMPLEMENTATION
#include <arena.h>

#include <dui/layout.h>
#include <dui/keyboard.h>



int main(void)
{
    InitWindow(800, 600, "DUI");
    SetTargetFPS(60);

    DUI_Environment* env = dui_env();
    env->primary_shades.palette = &PALETTE_SKY;

    while (!WindowShouldClose())
    {
        dui_env_begin();

        dui_lay_begin_screen(.padding = 3);
            dui_lay_begin_stack(.gap = 2);
                if (dui_button(.caption = "Exit", .kind = ENV_PRIMARY, .opposite = true))
                {
                    break;
                }

                for (int i = 0; i < 10; ++i)
                {
                    if (dui_button(.caption = TextFormat("Click me #%d!", i), .opposite = (i % 2), .disabled = (i % 3 == 1)))
                    {
                        printf("Button %d clicked.\n", i);
                    }
                }

                dui_lay_begin_spaced(.count = 3, .remaining = true, .direction = DIRECTION_HORIZONTAL, .gap = 2);
                    for (int i = 0; i < 3; ++i)
                    {
                        dui_button(.caption = TextFormat("Inner click click me #%d!", i), .kind = ENV_PRIMARY);
                    }
                dui_lay_end();
            dui_lay_end();
        dui_lay_end();

        dui_env_end();
    }
    CloseWindow();

    return 0;
}
