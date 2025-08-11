# TODO this is broken until ASF 3.45.0.88 and earlier can be obtained  >:(

# Rubber Ducky Mark I (AT32UC3B1256) — HID Mouse Firmware (random jiggle every 10s)

This is a **minimal ASF-based** firmware that enumerates as a USB HID **mouse** and nudges the cursor by a small **random** amount every **10 seconds**. It targets the classic Rubber Ducky **Mark I** (AT32UC3B1256, Atmel DFU `03eb:2ff6`).

> ⚠️ Note: Building is now only supported via Docker. You must provide the legacy Atmel Software Framework (ASF) and avr32-gcc toolchain as described in the Dockerfile.

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
   The firmware hex will be in `build/ducky_mouse.hex`.
4. Flash (DFU mode: hold button, plug in):
   ```sh
   dfu-programmer at32uc3b1256 erase
   dfu-programmer at32uc3b1256 flash --suppress-bootloader-mem build/ducky_mouse.hex
   dfu-programmer at32uc3b1256 reset
   ```

For details on how to provide the toolchain and ASF for the Docker build, see the comments in the `Dockerfile`.

## What it does
- Enumerates as **HID Mouse** (relative movement).
- Every **10 seconds**, sends a small random (dx, dy) in range `[-3, +3]` (non-zero), simulating a subtle jiggle.

## File layout
- `src/main.c` — system init + random jiggle loop using ASF's HID mouse API.
- `src/ui.c` — minimal UI callbacks required by ASF's UDI HID mouse class.
- `src/usb_descriptors.h` — HID report descriptor (mouse).
- `conf/conf_board.h` — board stub.
- `conf/conf_clock.h` — clock config (12 MHz crystal -> PLL for USB).
- `conf/conf_usb.h` — select HID mouse class, endpoints.
- `Makefile` — simple build that pulls in ASF mouse class.
- `Dockerfile` — **stub** showing expected layout if you want to containerize the toolchain.

## Notes
- This uses **relative** mouse reports (classic cursor moves). If you want **absolute** reports (tablet-like), adjust the descriptor in `usb_descriptors.h` and ASF config (less common).
- The random source uses an `xorshift32` seeded from a basic timer tick and USB frame counter — good enough for jitter, not for crypto.
- If you want **composite keyboard+mouse**, you can switch the UDI in `conf_usb.h` to the composite template and add HID keyboard class (requires extra glue). This scaffold keeps it mouse-only for clarity.

## Troubleshooting
- If it enumerates but doesn't move, ensure your OS allows HID mouse input from the device and the endpoint isn't stalled.
- LED behavior: this scaffold doesn't drive LEDs. Add your own in `ui.c` if your board has one.
- Stuck in DFU: unplug, hold the tiny button, replug. Erase and flash again.

## License
Public domain / CC0 for the scaffold code here. ASF components remain under their respective licenses.
