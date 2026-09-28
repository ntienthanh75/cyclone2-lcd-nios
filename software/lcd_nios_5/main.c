/*
 * Nios LCD flower demo
 * Creator: NGUYEN Tien Thanh
 */
#include "alt_types.h"
#include <io.h>
#include <system.h>
#include "altera_avalon_pio_regs.h"
#include "LCD32.h"

static void fill_circle(int center_x, int center_y, int radius, alt_u16 color)
{
    int x;
    int y;
    int radius_squared = radius * radius;

    for (y = -radius; y <= radius; y++)
    {
        for (x = -radius; x <= radius; x++)
        {
            if ((x * x) + (y * y) <= radius_squared)
            {
                LCD_SetPoint((alt_u16)(center_x + x),
                             (alt_u16)(center_y + y), color);
            }
        }
    }
}

static void draw_flower(void)
{
    LCD_Clear(Black);

    /* Stem and leaves. */
    LCD_DrawLine(160, 140, 160, 225, Green);
    LCD_DrawLine(160, 185, 125, 165, Green);
    LCD_DrawLine(160, 205, 198, 180, Green);
    LCD_DrawLine(125, 165, 145, 170, Green);
    LCD_DrawLine(198, 180, 175, 190, Green);

    /* Five petals and the yellow center. */
    fill_circle(160, 78, 30, Magenta);
    fill_circle(130, 106, 30, Red);
    fill_circle(138, 143, 30, Magenta);
    fill_circle(190, 106, 30, Red);
    fill_circle(182, 143, 30, Magenta);
    fill_circle(160, 118, 22, Yellow);

    GUI_Text(112, 12, (alt_u8 *)"Nios flower", White, Black);
}

int main(void)
{
    /* Backlight is active-high on BL_P and active-low on BL_N. */
    IOWR_ALTERA_AVALON_PIO_DATA(BL_P_BASE, 1);
    IOWR_ALTERA_AVALON_PIO_DATA(BL_N_BASE, 0);

    LCD_Initializtion();
    draw_flower();

    while (1)
    {
        /* Keep the flower displayed until the next Nios download or reset. */
    }

    return 0;
}
