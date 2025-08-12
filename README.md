# Rubber Ducky Mark I (AT32UC3B1256) — HID Keyboard Firmware (random CTRL/SHIFT once per minute)

This is a **minimal ASF-based** firmware that enumerates as a USB HID **keyboard** and, once per minute, presses and releases either **CTRL** or **SHIFT** at random. It targets the classic Rubber Ducky **Mark I** (AT32UC3B1256, Atmel DFU `03eb:2ff6`).

> ⚠️ Note: Building is now only supported via Docker. You must provide the legacy Atmel Software Framework (ASF) and avr32-gcc toolchain as described in the Dockerfile.

This container fetches and uses ASF version **3.52.0.113** and the AVR32 GNU Toolchain **3.4.2** automatically.

## Quick Start (Docker Only)

1. Prepare your ASF and avr32 toolchain directories (see Dockerfile comments).
2. Build the Docker image:
   ```sh
   docker build -t ducky-avr32 .
   ```
3. Build the firmware using Docker:
   ```sh
   docker run --rm -v $(pwd):/work ducky-avr32 make
   ```
   The firmware hex will be in `build/ducky_keyboard.hex`.
4. Flash (DFU mode: hold button, plug in):
   ```sh
   dfu-programmer at32uc3b1256 erase
   dfu-programmer at32uc3b1256 flash --suppress-bootloader-mem build/ducky_keyboard.hex
   dfu-programmer at32uc3b1256 reset
   ```

For details on how to provide the toolchain and ASF for the Docker build, see the comments in the `Dockerfile`.

## What it does
- Enumerates as **HID Keyboard**.
- Every **60 seconds**, presses and releases one modifier: either **Left CTRL** or **Left SHIFT**, chosen at random.

## File layout
- `src/main.c` — system init + once-per-minute random CTRL/SHIFT using ASF's HID keyboard API.
- `src/ui.c` — minimal UI callbacks required by ASF's UDI HID keyboard class.
- `src/usb_descriptors.h` — not used (keyboard report is provided by ASF `udi_hid_kbd`).
- `conf/conf_board.h` — board stub.
- `conf/conf_clock.h` — clock config (12 MHz crystal -> PLL for USB).
- `conf/conf_usb.h` — select HID keyboard class.
- `Makefile` — Docker wrapper that builds inside an image.
- `Dockerfile` — **stub** showing expected layout if you want to containerize the toolchain.

## Notes
- The random source uses an `xorshift32` seeded from clocks — fine for this purpose, not for crypto.
- If you want **composite keyboard+mouse**, switch the UDI in `conf_usb.h` to a composite and add both classes (requires extra glue).

## Troubleshooting
- If it enumerates but doesn't move, ensure your OS allows HID mouse input from the device and the endpoint isn't stalled.
- LED behavior: this scaffold doesn't drive LEDs. Add your own in `ui.c` if your board has one.
- Stuck in DFU: unplug, hold the tiny button, replug. Erase and flash again.

## License
Public domain / CC0 for the scaffold code here. ASF components remain under their respective licenses.
