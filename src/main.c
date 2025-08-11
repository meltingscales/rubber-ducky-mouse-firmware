
#include <asf.h>
#include <udi_hid_mouse.h>

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
        /* Every ~10 seconds, nudge mouse by small random dx/dy */
        ms_delay(10000);
        int8_t dx = small_jitter();
        int8_t dy = small_jitter();
        /* ASF mouse move: dx, dy, scroll (0) */
        udi_hid_mouse_moveX(dx);
        udi_hid_mouse_moveY(dy);
        // Alternatively: udi_hid_mouse_move(dx, dy, 0);
    }
    // not reached
}
