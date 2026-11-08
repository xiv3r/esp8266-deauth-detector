## What It Does
Scans all 13 WiFi channels and watches for **deauthentication (0xA0)** and **disassociation (0xC0)** frames. When the packet rate stays high, the LED turns on to indicate an attack.

## Hardware Required
- ESP8266 board (NodeMCU, Wemos D1 mini, etc.) with built-in LED on **GPIO 2**
- USB cable

## Setup Steps

1. **Download the firmware and flash, flash offset address is `0x0`**
   - https://github.com/xiv3r/esp8266-deauth-detector/raw/refs/heads/main/esp8266-firmware-0x0.bin

5. **Run**
   - The device starts scanning automatically. No configuration needed.

## How to Read the LED
- **LED OFF** — normal traffic, no attack detected.
- **LED ON** — deauth/disassociation flood detected on the channel being scanned.

> **Note:** On most ESP8266 boards the built-in LED is **active-low** (inverted), which is why `LED_INVERT` is set to `true`.

## Tuning

| Constant | Default | Meaning |
|----------|---------|---------|
| `CH_TIME` | `140` ms | Time spent on each channel |
| `PKT_RATE` | `5` | Packets per full scan cycle to count as "attack traffic" |
| `PKT_TIME` | `1` | Consecutive cycles needed to trigger the LED |
| `LED_INVERT` | `true` | Set to `false` if your LED is active-high |
| `channels[]` | 1–13 | Edit to restrict to fewer channels (faster scan) |

## Notes
- This is a **passive monitor** — it does not transmit or jam anything.
- Reception range is limited by the ESP8266's antenna (a few meters indoors).
- Only detects deauth/disassoc frame *types*, not which network they target.
- If you see no LED activity during a real attack, lower `PKT_RATE` or set `PKT_TIME` to `1`.
