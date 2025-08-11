
#pragma once
/* Clock config for UC3B0/B1.
 * Rubber Ducky has a 12 MHz crystal.
 * We'll configure PLL to get 48 MHz for USB.
 */
#define CONFIG_SYSCLK_SOURCE        SYSCLK_SRC_PLL0
#define CONFIG_PLL0_SOURCE          PLL_SRC_OSC0
#define CONFIG_PLL0_MUL             8      /* 12MHz * 8 = 96MHz */
#define CONFIG_PLL0_DIV             2      /* 96/2 = 48MHz for USB */
#define CONFIG_USBCLK_SOURCE        USBCLK_SRC_PLL0
#define CONFIG_USBCLK_DIV           1
