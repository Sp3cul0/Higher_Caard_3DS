# 🎮 Higher: The Last Dream — 3DS Visit Card

> An interactive business card for the Nintendo 3DS, showcasing the game **Higher: The Last Dream** by **Sp3culo**.

---

## 📸 Overview

This homebrew 3DS application serves as a digital business card / demo showcase for the game *Higher: The Last Dream*. It features:

- Animated logo with touch-triggered spin effect on the **top screen**
- Blinking prompt and social links (Steam & Discord) on the **bottom screen**
- Full **stereoscopic 3D** support via the 3D slider
- Smooth **scene fade transitions**
- **Audio** playback on launch

---

## 📁 Project Structure

```
Higher_Caard/
├── source/
│   ├── main.cpp                    # Entry point, render loop
│   ├── scenes/
│   │   ├── MainMenuTop.hpp         # Top screen scene (logo, touch spin)
│   │   └── MainMenuBot.hpp         # Bottom screen scene (Steam & Discord)
│   └── scripts/
│       ├── SceneManager.hpp        # Scene lifecycle & fade system
│       ├── Renderer.hpp            # citro2d drawing wrapper
│       ├── AudioManager.hpp        # ndsp WAV audio playback
│       ├── Coroutine.hpp           # Frame-based timer utility
│       ├── Animation.hpp           # Sprite animation helper
│       └── UIAlign.hpp             # Screen alignment constants
├── romfs/
│   ├── assets/                     # .t3x sprite sheets (logo, backgrounds, icons)
│   └── fonts/                      # .bcfnt custom font
├── icon.png                        # Home menu icon (48x48)
├── banner.bin                      # Home menu banner
├── app.rsf                         # 3DS app metadata (title, region, etc.)
├── Makefile                        # devkitARM build system
└── generate/                       # Asset generation scripts
```

---

## 🛠️ Building

### Prerequisites

- [devkitARM](https://devkitpro.org/wiki/Getting_Started) with 3DS support
- `libctru`, `citro2d`, `citro3d`
- devkitPro packages: `3ds-dev`

Install via devkitPro pacman:
```bash
dkp-pacman -S 3ds-dev
```

### Compile

```bash
make
```

This generates:
- `Higher_Caard.3dsx` — for use with [Homebrew Launcher](https://smealum.github.io/3ds/)
- `Higher_Caard.cia` — installable via FBI (requires `makerom` and a valid `app.rsf`)

---

## 🖼️ Assets

All assets live in `romfs/assets/` as `.t3x` sprite sheets (compiled with `tex3ds`).

| Key              | File                    | Usage                        |
|------------------|-------------------------|------------------------------|
| `background_top` | `output_top.t3x`        | Top screen background        |
| `background_bot` | `output_bottom.t3x`     | Bottom screen background     |
| `menu`           | `logo.t3x`              | Game logo (animated spin)    |
| `steam`          | `steam.t3x`             | Steam icon                   |
| `discord`        | `discord.t3x`           | Discord icon                 |
| `level`          | `bglevel_1.t3x`         | Level background (reserved)  |

### Converting Images

Images must be pre-processed before converting to `.t3x`. Use ImageMagick to swap BGR channels and rotate for the 3DS framebuffer format:

```bash
convert input.png \
  -channel B -separate input.png \
  -channel G -separate input.png \
  -channel R -separate \
  -channel RGB -combine \
  -rotate 90 output.rgb
```

Then rename `output.rgb` → `output.bin` and place it in the `data/` folder, or convert to `.t3x` using `tex3ds`.

> ⚠️ The 3DS top/bottom screens are portrait displays rotated 90°, and use BGR888 color order — hence the channel swap and rotation.

---

## 🔊 Audio

Audio is handled via `ndsp` (Nintendo DSP). Sounds must be in uncompressed **PCM WAV** format and placed in `romfs/`.

Load and play a sound:
```cpp
audioManager.load("menu", "romfs:/audio/menu.wav");
audioManager.play("menu", /* loop = */ false);
```

---

## 🌐 Features

### Stereoscopic 3D
The top screen renders twice (left eye / right eye) with a configurable pixel offset driven by the 3D slider:
```cpp
float offset = osGet3DSliderState() * 6.0f;
sceneManagerTop.setEyeOffset(-offset); // left eye
sceneManagerTop.setEyeOffset(+offset); // right eye
```

### Touch Interaction
Touching the touchscreen triggers a full 360° logo spin on the top screen with ease-out deceleration.

### Scene System
Scenes extend the `Scene` base class and implement `update()` and `draw()`. Transition between scenes with a fade-to-black:
```cpp
goToTop("MainMenuTop");
goToBot("MainMenuBot");
```

---

## 👥 Credits

| Role         | Person      |
|--------------|-------------|
| Development  | Sp3culo     |
| Development  | Le_Sims     |

---

## 🔗 Links

- 🎮 **Steam Page** — coming soon
- 💬 **Discord Community** — join via the in-app QR / link on the bottom screen

---

## 📄 License

This project is a promotional homebrew tool for *Higher: The Last Dream*. All game assets are © their respective owners.
