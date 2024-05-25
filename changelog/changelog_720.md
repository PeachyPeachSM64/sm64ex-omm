# Version 7.2.0 (21/08/2022)

## *OMM Builder*:
- Brand new version! See **[Building the game](https://github.com/PeachyPeachSM64/sm64ex-omm/tree/master#building-the-game)**.
- Main changes:
  - `r96a` is now `r96x` and uses the `Render96ex-tester` branch.
  - All custom resources (Patches, Texture packs, Sound packs, Model packs and Audio packs) can be put into a `custom` directory, without distinction.
  - Start-up script for Windows (`omm_builder.bat`) and Linux (`omm_builder.sh`).
  - Built-in HD Texture pack for OMM. Use it with another HD Texture pack like Render96 or Redrawn.

## Bug fixes:
- Odyssey Moveset - 3-Health: Collecting a 1-up now properly stops the KtQ/PSS timer during the life-up transition.
- Fixed a bug where Mario would die if switching from Classic to Odyssey Health if his health was low enough.
- Fixed a bug where blue coins from blue coin switch could be collected multiple times.
- Fixed a bug where caps and Vibes could be used during the Bowser 4 fights.
- Fixed triple jump/wall kick animation.
- Fixed Time Trials crashes and compatibility.
- Fixed a crash on Linux machines (thanks **TheBoiJoshua**).
- Render96:
  - Fixed the language loading crash.
- Star Road:
  - Fixed crashes when entering levels with Non-Stop disabled.
  - Entering Chuckya Harbor from the secret entrance now skips the star select screen.

## Odyssey Moveset:
- Improved shell ride:
  - Increased top speed,
  - Jump slightly higher,
  - Better handling, both ground and airborne,
  - Pressing **(X)** triggers a Cappy spin throw,
  - No more interaction with trees and poles.
- Super Cappy bounce: Increased the window from frame-perfect to 3 frames.
- Almost all moves have now action cancels.
- Mario can kick, throw Cappy or spin during an air roll is **(Z)** is not held.
- Increased water punch grab range.
- Extend a water ground pound by holding **(Z)**, but drains the air meter faster.
- Implemented underwater Metal hold actions.
- Fixed fake pole grabs.
- Top-of-pole transition is faster.
- Mario can throw crazy boxes by pressing **(B)** between bounces.
- Balanced tiny Goombas and huge Goombas move speed and jump height.
- Re-balanced physics properties per character:

| | Mario | Peach | Luigi | Wario | Peach (Joy) | Peach (Rage) | Peach (Gloom) | Peach (Calm) |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
| Ground speed | | | ➕ | ➖ | | ➖➖ | ➕➕ | |
| Air speed | | | ➖ | ➕ | | ➖➖ | ➕➕ | |
| Swimming speed | | | ➕ | ➖ | | | | |
| Jumping height | | ➖ | ➕ | ➖ | | ➖➖ | ➕➕ | ➖ |
| Gravity | | ➖ | ➖ | ➕ | | ➕➕ | ➖ | ➖ |
| Sliding accel | | | ➖ | ➕ | | ➕➕ | | |

*Multipliers are inverted for captures.*

## Sparkly Stars:
- Some Pink-Gold Stars and Crystal Stars have been reworked.<br>The goal of this rework is to provide more fun and less infuriating challenges without lowering the difficulty.<br>Therefore, most of the "no button press" challenges have been replaced.
- Pink-Gold Stars:
  - Along with Peach, completing Normal mode now unlocks the `Sparkly Stars Reward` option in the `Extras` sub-menu.
  - Changes:
    - Shifting Sand Land:
      - The Star height has been lowered.
    - Snowman's Land:
      - Captured snowmen no longer receive a height boost in Normal mode.
    - Wet-Dry World:
      - The Star is higher and now requires a cannon shot.
    - Rainbow Ride:
      - **New**: Collect 6 secrets in order by climbing poles from lowest to highest.
    - Bowser in the Dark World:
      - The Goomba capture is forced, even if Cap is set to Classic or No Capture.
    - Bowser in the Sky:
      - **New**: Collect at least 75 coins. Start with 1 minute, but each coin collected adds 1 second to the timer.
    - Princess Secret Slide:
      - Pressing **(L)** or **(R)** no longer invalidates the Star.
- Crystal Stars:
  - Completing Hard mode now unlocks a new ability for Peach: **Perry's Charged Shot**. Hold **(B)** to charge energy and release to unleash a stronger version of a Perry shockwave.
  - Changes:
    - Whomp Fortress:
      - Reduced timer from 1'50 to 1'45.
    - Big Boo's Haunt:
      - Reduced timer from 2'15 to 2'10.
    - Hazy Maze Cave:
      - Reduced timer from 3'00 to 2'30.
    - Snowman's Land:
      - **New**: Collect at least 80 coins, but only when riding a shell.
    - Tick-Tock Clock:
      - You only have 35 seconds to reach the Star after leaving the floor.
    - Rainbow Ride:
      - Collect all 9 1-up mushrooms in 1'35. No more button restriction.
    - Bowser in the Dark World:
      - Reduced timer from 1'45 to 1'40.
      - The Goomba capture is forced, even if Cap is set to Classic or No Capture.
    - Bowser in the Fire Sea:
      - Mario cannot have more than 3 Health (4 with Classic moveset).
    - Bowser in the Sky:
      - **New**: Destroy all enemies with a captured bob-omb. This special bob-omb has infinite explosions, but you only have 1'50.
      - The Bob-omb capture is forced, even if Cap is set to Classic or No Capture.
    - Princess Secret Slide:
      - Pressing **(L)** or **(R)** no longer invalidates the Star.
- Bowser 4 and 5 are now *Pink-Gold Bowser* and *Crystal Bowser*.

## Enhancements:
- Better frame interpolation and framerate modes:
  - Fixed: 30, 60
  - Auto: Sets the framerate to the monitor's refresh rate
  - Unlimited: No sleep between frames, the game constantly draws content
- Faster external texture data loading.
- Warping, restarting, exiting or returning to Castle now heals Mario to full health.
- Exclamation boxes with shells now respawn after some time.
- Fixed some enemies' hitbox (notably Bullet Bill's).
- Press **(Start)** during the ending cutscene to skip it and return to the file select screen.
- Select an empty save file with **(Start)** instead of **(A)** to skip the intro.

## Palette Editor
Customize in-game your Mario and Peach color palettes!<br>To access the Palette Editor, select the Mario/Peach button on file select screen with **(Start)**.<br>Controls:
- **D-Pad Left**/**Right** to change the current palette
- **D-Pad Up**/**Down** to change the current color
- **L-Stick** to choose a color or **Mouse Left-Click** to select a color (OpenGL only)
- Hold **(Z)** to change the brightness
- **(X)** to switch between cap and capless
- **(Y)** to switch between Mario and Peach
- Hold **(R)** to zoom in, **(L)** to zoom out
- **R-Stick** to change the camera angles
- **(B)** to change the background
- **(A)** to manually enter a color code
  - Enter an hexadecimal color code with keys <kbd>0</kbd>-<kbd>9</kbd>/<kbd>A</kbd>-<kbd>F</kbd> or
  - Press <kbd>V</kbd> to paste a color code from the clipboard (Windows only)
- **(Start)** to save and return to the file select screen.

## Playtest:
Special thanks to all people who playtested this version before its release and contributed to spot errors, fixing bugs and making it better:
- **JokerFactor**
- **Wookis**
- **chani**
- **Mr.Needlemouse**
- **FROST**
- **SonicDarkHedgeboy**
- **Cancel the Eat**
- **FastMario**
- **Petch**
