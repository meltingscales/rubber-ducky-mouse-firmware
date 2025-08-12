
#include <asf.h>
#include <usb_protocol_hid.h>
#include <udi_hid_kbd.h>

/* Simple xorshift32 for jitter; NOT cryptographically secure */
static uint32_t rng_state = 0xC0FFEE01u;
static inline uint32_t xorshift32(void) {
    uint32_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng_state = x;
    return x;
}

/* Return a small non-zero signed jitter in [-3, 3] */
static int8_t small_jitter(void) {
    int8_t v;
    do {
        v = (int8_t)((xorshift32() % 7) - 3); /* -3..+3 */
    } while (v == 0);
    return v;
}

static void ms_delay(uint32_t ms)
{
    // crude delay using udelay if available; ASF provides cpu_delay_ms for UC3
    cpu_delay_ms(ms, sysclk_get_cpu_hz());
}

int main(void)
{
    /* Basic clocks + board init */
    sysclk_init();
    irq_initialize_vectors();
    cpu_irq_enable();

    /* Seed RNG with USB frame number noise + a read of cycle counter */
    rng_state ^= (uint32_t)sysclk_get_cpu_hz();
    rng_state ^= (uint32_t)sysclk_get_pba_hz();

    /* Start USB device stack */
    udc_start();

    /* Wait until configured by host */
    while (!udc_enable()) {
        /* spin until USB configured */
    }

    for (;;) {
        /* Once per minute, press either CTRL or SHIFT (modifier only) */
        ms_delay(60000);

        /* Randomly choose between left CTRL and left SHIFT */
        bool use_ctrl = (xorshift32() & 1u) != 0u;
        uint8_t mod = use_ctrl ? HID_MODIFIER_LEFT_CTRL : HID_MODIFIER_LEFT_SHIFT;

        /* Press and release the chosen modifier */
        (void)udi_hid_kbd_modifier_down(mod);
        ms_delay(30);
        (void)udi_hid_kbd_modifier_up(mod);
    }
    // not reached
}
