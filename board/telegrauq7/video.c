
/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 * Fabio Estevam <fabio.estevam@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <linux/list.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/arch/mx6-pins.h>
#include <linux/fb.h>
#include <ipu_pixfmt.h>

#define MX53LOCO_LCD_POWER		IMX_GPIO_NR(3, 24)

static struct fb_videomode const claa_wvga = {
    .name		= "CLAA07LC0ACW",
    .refresh	= 57,
    .xres		= 800,
    .yres		= 480,
    .pixclock	= 37037,
    .left_margin	= 40,
    .right_margin	= 60,
    .upper_margin	= 10,
    .lower_margin	= 10,
    .hsync_len	= 20,
    .vsync_len	= 10,
    .sync		= 0,
    .vmode		= FB_VMODE_NONINTERLACED
};

static struct fb_videomode const seiko_wvga = {
    .name		= "Seiko-43WVF1G",
    .refresh	= 60,
    .xres		= 800,
    .yres		= 480,
    .pixclock	= 29851, /* picosecond (33.5 MHz) */
    .left_margin	= 89,
    .right_margin	= 164,
    .upper_margin	= 23,
    .lower_margin	= 10,
    .hsync_len	= 10,
    .vsync_len	= 10,
    .sync		= 0,
};

void setup_iomux_lcd(void)
{
    static const iomux_v3_cfg_t lcd_pads[] = {
        MX6_PAD_DI0_DISP_CLK__LCD_CLK,
        MX6_PAD_DI0_PIN15__LCD_ENABLE,
        MX6_PAD_DI0_PIN2__LCD_HSYNC,
        MX6_PAD_DI0_PIN3__LCD_VSYNC,
        MX6_PAD_DISP0_DAT0__LCD_DATA00,
        MX6_PAD_DISP0_DAT1__LCD_DATA01,
        MX6_PAD_DISP0_DAT2__LCD_DATA02,
        MX6_PAD_DISP0_DAT3__LCD_DATA03,
        MX6_PAD_DISP0_DAT4__LCD_DATA04,
        MX6_PAD_DISP0_DAT5__LCD_DATA05,
        MX6_PAD_DISP0_DAT6__LCD_DATA06,
        MX6_PAD_DISP0_DAT7__LCD_DATA07,
        MX6_PAD_DISP0_DAT8__LCD_DATA08,
        MX6_PAD_DISP0_DAT9__LCD_DATA09,
        MX6_PAD_DISP0_DAT10__LCD_DATA10,
        MX6_PAD_DISP0_DAT11__LCD_DATA11,
        MX6_PAD_DISP0_DAT12__LCD_DATA12,
        MX6_PAD_DISP0_DAT13__LCD_DATA13,
        MX6_PAD_DISP0_DAT14__LCD_DATA14,
        MX6_PAD_DISP0_DAT15__LCD_DATA15,
        MX6_PAD_DISP0_DAT16__LCD_DATA16,
        MX6_PAD_DISP0_DAT17__LCD_DATA17,
        MX6_PAD_DISP0_DAT18__LCD_DATA18,
        MX6_PAD_DISP0_DAT19__LCD_DATA19,
        MX6_PAD_DISP0_DAT20__LCD_DATA20,
        MX6_PAD_DISP0_DAT21__LCD_DATA21,
        MX6_PAD_DISP0_DAT22__LCD_DATA22,
        MX6_PAD_DISP0_DAT23__LCD_DATA23,
    };

    imx_iomux_v3_setup_multiple_pads(lcd_pads, ARRAY_SIZE(lcd_pads));

    /* Turn on GPIO backlight */
    //imx_iomux_v3_setup_pad(MX53_PAD_EIM_D24__GPIO3_24);
    //gpio_direction_output(MX53LOCO_LCD_POWER, 1);

    /* Turn on display contrast */
    //imx_iomux_v3_setup_pad(MX53_PAD_GPIO_1__GPIO1_1);
    //gpio_direction_output(IMX_GPIO_NR(1, 1), 1);
}

int board_video_skip(void)
{
    int ret;
    char const *e = getenv("panel");

    if (e) {
        if (strcmp(e, "seiko") == 0) {
            ret = ipuv3_fb_init(&seiko_wvga, 0, IPU_PIX_FMT_RGB24);
            if (ret)
                printf("Seiko cannot be configured: %d\n", ret);
            return ret;
        }
    }

    /*
     * 'panel' env variable not found or has different value than 'seiko'
     *  Defaulting to claa lcd.
     */
    ret = ipuv3_fb_init(&claa_wvga, 0, IPU_PIX_FMT_RGB565);
    if (ret)
        printf("CLAA cannot be configured: %d\n", ret);
    return ret;
}
