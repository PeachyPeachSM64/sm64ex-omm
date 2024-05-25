# Version 6.0.1 (24/10/2021)
**From now on, you no longer have to update your version of OMM yourself.**<br>**However, if the changelog mentions a *Builder update* (like this one), you have to run the following command where the builder is to update it:**<br>`git checkout .; git pull -q`

## Builder update:
- The DirectX render API is now supported, build with the `DIRECT_X` option to replace the SDL/OpenGL API.
- Added an option `reset` to reset a repository without deleting it.
- The builder is now able to detect the installed version of OMM and cleaning it if it doesn't match the latest version. You don't have to update your version of OMM yourself.

## Bug fixes:
- Fixed a broken warp pipe in THI.
- Fixed a crash that happened when trying to enter rooms with Boos in BBH.
- Fixed a crash that could happen if some objects were loaded during time stop.
- Fixed BITDW Crystal Star missing coin condition.
- Fixed a bug where Tiara appeared in place of Peach's crown during some animations.
- Fixed a bug in Star Road where Peach could *collect* the Grand Star.

## Optimizations:
- Fixed a huge slowdown due to 3D coins and colored stars.<br>Colored stars have been optimized, 3D coins have been removed.
- Optimized surfaces and collisions.<br>Due to how surfaces work now, the surface cucking glitch is fixed for both movesets.
- Optimized object interactions.
- Optimized some of Peach's objects that could have potentially caused slowdowns (Rage shockwave, Joy tornados, sparkles).
- Removed unused content.

## Controls:
Controls have been redesigned to be less confusing and to prevent annoying softlocks:
- Default configuration is given by the table below:

| | Keyboard (qwerty) | Keyboard (azerty) | XBox One Controller | Switch Pro Controller |
|:-:|:-:|:-:|:-:|:-:|
| A Button | <kbd>L</kbd> | <kbd>L</kbd> | <kbd>**A**</kbd> | <kbd>**A**</kbd> |
| B Button | <kbd>,</kbd> | <kbd>;</kbd> | <kbd>**B**</kbd> | <kbd>**B**</kbd> |
| X Button | <kbd>K</kbd> | <kbd>K</kbd> | <kbd>**X**</kbd> | <kbd>**X**</kbd> |
| Y Button | <kbd>M</kbd> | <kbd>,</kbd> | <kbd>**Y**</kbd> | <kbd>**Y**</kbd> |
| Start Button | <kbd>SPACE</kbd> | <kbd>SPACE</kbd> | <kbd>**Start**</kbd> | <kbd>**+**</kbd> |
| L Trigger | <kbd>LSHIFT</kbd> | <kbd>LSHIFT</kbd> | <kbd>**LB**</kbd> | <kbd>**L**</kbd> |
| R Trigger | <kbd>RSHIFT</kbd> | <kbd>RSHIFT</kbd> | <kbd>**RB**</kbd> | <kbd>**R**</kbd> |
| Z Trigger | <kbd>O</kbd> | <kbd>O</kbd> | <kbd>**RT**</kbd> | <kbd>**ZR**</kbd> |
| C-Up | <kbd>&uarr;</kbd> | <kbd>&uarr;</kbd> | <kbd>**RS-Up**</kbd> | <kbd>**RS-Up**</kbd> |
| C-Down | <kbd>&darr;</kbd> | <kbd>&darr;</kbd> | <kbd>**RS-Down**</kbd> | <kbd>**RS-Down**</kbd> |
| C-Left | <kbd>&larr;</kbd> | <kbd>&larr;</kbd> | <kbd>**RS-Left**</kbd> | <kbd>**RS-Left**</kbd> |
| C-Right | <kbd>&rarr;</kbd> | <kbd>&rarr;</kbd> | <kbd>**RS-Right**</kbd> | <kbd>**RS-Right**</kbd> |
| D-Up | <kbd>-</kbd> | <kbd>)</kbd> | <kbd>**D-Up**</kbd> | <kbd>**D-Up**</kbd> |
| D-Down | <kbd>{</kbd> | <kbd>^</kbd> | <kbd>**D-Down**</kbd> | <kbd>**D-Down**</kbd> |
| D-Left | <kbd>P</kbd> | <kbd>P</kbd> | <kbd>**D-Left**</kbd> | <kbd>**D-Left**</kbd> |
| D-Right | <kbd>}</kbd> | <kbd>$</kbd> | <kbd>**D-Right**</kbd> | <kbd>**D-Right**</kbd> |
| Stick Up | <kbd>W</kbd> | <kbd>Z</kbd> | <kbd>**LS-Up**</kbd> | <kbd>**LS-Up**</kbd> |
| Stick Down | <kbd>S</kbd> | <kbd>S</kbd> | <kbd>**LS-Down**</kbd> | <kbd>**LS-Down**</kbd> |
| Stick Left | <kbd>A</kbd> | <kbd>Q</kbd> | <kbd>**LS-Left**</kbd> | <kbd>**LS-Left**</kbd> |
| Stick Right | <kbd>D</kbd> | <kbd>D</kbd> | <kbd>**LS-Right**</kbd> | <kbd>**LS-Right**</kbd> |

- All binds have been moved and are now in the `Controls` sub-menu, and are saved inside the OMM save file.
- To reset all binds to their default values (see table above), press the `Reset Controls` button.
- If the one of the **(A)**, **(R)** or **(Start)** button has no bind, its binds are reset to default.<br>This way, the player can always access the `Controls` sub-menu and press the `Reset Controls` button.
- Additionally, the player cannot bind a keyboard key or a controller button that is already assigned to the **(A)** button.<br>If another button has the same bind as the **(A)** button, that bind is removed from that button.

## Other:
- Moved the Vibe trigger to the **(Y)** button.
- Reduced Joy flying action (**(A)** button) max duration from 6 to 5 seconds.
- Added the puppy icon for `xalo` and `smsr` when Puppycam is enabled and selected.
- Added a "Correct!" sound effect when the 'complete save file' code is input correctly on the file select screen.
- Added a failsafe to the OMM save file:
  - When saving, the game creates a back-up, then write the savefile.
  - If the savefile is missing or gets corrupted/erased, the back-up will replace it.
