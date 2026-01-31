#ifndef DUI_COLORS_H
#define DUI_COLORS_H

#include <raylib.h>

#define DUI_COLOR(hex) CLITERAL(Color){ (hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF, 0xFF }

typedef struct
{
    Color _50;
    Color _100;
    Color _200;
    Color _300;
    Color _400;
    Color _500;
    Color _600;
    Color _700;
    Color _800;
    Color _900;
    Color _950;
} DUI_Palette;

extern const DUI_Palette PALETTE_SLATE;
extern const DUI_Palette PALETTE_GRAY;
extern const DUI_Palette PALETTE_ZINC;
extern const DUI_Palette PALETTE_NEUTRAL;
extern const DUI_Palette PALETTE_STONE;
extern const DUI_Palette PALETTE_RED;
extern const DUI_Palette PALETTE_ORANGE;
extern const DUI_Palette PALETTE_AMBER;
extern const DUI_Palette PALETTE_YELLOW;
extern const DUI_Palette PALETTE_LIME;
extern const DUI_Palette PALETTE_GREEN;
extern const DUI_Palette PALETTE_EMERALD;
extern const DUI_Palette PALETTE_TEAL;
extern const DUI_Palette PALETTE_CYAN;
extern const DUI_Palette PALETTE_SKY;
extern const DUI_Palette PALETTE_BLUE;
extern const DUI_Palette PALETTE_INDIGO;
extern const DUI_Palette PALETTE_VIOLET;
extern const DUI_Palette PALETTE_PURPLE;
extern const DUI_Palette PALETTE_FUCHSIA;
extern const DUI_Palette PALETTE_PINK;
extern const DUI_Palette PALETTE_ROSE;

typedef enum
{
    SHADE_50,
    SHADE_100,
    SHADE_200,
    SHADE_300,
    SHADE_400,
    SHADE_500,
    SHADE_600,
    SHADE_700,
    SHADE_800,
    SHADE_900,
    SHADE_950,
    COUNT_SHADES,
} DUI_Shade;

Color dui_palette_color(const DUI_Palette* palette, const DUI_Shade shade);

#endif //DUI_COLORS_H
