
#include <asf.h>
/* Minimal UI callbacks. You can add LED feedback here if desired. */
void ui_init(void) {}
void ui_powerdown(void) {}
void ui_wakeup(void) {}
void ui_process(uint16_t framenumber) { (void)framenumber; }
