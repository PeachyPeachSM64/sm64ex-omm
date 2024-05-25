# Version 6.1.2 (14/12/2021)

## *OMM Builder*:
- The `clear` command now deletes only the `build` directory.
- The `delete` command removes completely the version directory.

## Changes and fixes:
- Fixed a bug where the Boo blue coin couldn't be collected all the time with Cappy.
- Fixed a bug where the loading time of Render96 was longer than intended.
- Fixed a bug where the kickable board in WF couldn't be *attacked* by Cappy and Perry.
- The three parts of the TTM slide are now in a single area (instead of 3).

# Version 6.1.0 (11/12/2021)

## *OMM Builder*:
- The `star_road.patch` file is no longer needed to build ***Super Mario Star Road***.
- Added ***Render96*** repository.
- Added compatibility checks:
  - ***Render96***, ***Super Mario 64 ex-alo*** and ***Super Mario Star Road*** cannot be built with a version of OMM prior to **6.1.0**.
  - ***Super Mario 64 ex-alo***, ***Super Mario 74*** and ***Super Mario Star Road*** cannot be built with DynOS.
- Added the `DYNOS` option. When selected, download and install the latest version of DynOS with the selected repository, and copy the contents of the `custom/dynos/packs` directory to the executable directory.
- Added the `PATCHES` option. This option must be explicitly set to tell the builder to apply custom patches from the `custom/patches` directory before compiling.
- Modified the `EXT_DATA` option. This option must be explicitly set to tell the builder to compile with the flag `EXTERNAL_DATA=1`, and copy the contents of the `custom/res` directory to the executable directory.
- Added the LiveSplit auto-splitter file `omm.asl` for speedruns.

## Bug fixes:
- The star number is now hidden during the ending cutscene.
- Fixed an hitbox scaling bug on Perry.
- Fixed vanilla Bowser's shockwave interaction.

## Render96 compatibility (again):
- Besides Mario and Peach, the player can also select Luigi and Wario on the file select screen.<br>Remember that even if Luigi/Wario is selected, the player can play as him only if he has been unlocked in the chosen save file. Otherwise, the game will select Mario when entering Castle grounds.
- There are two additional ways to change the selected character in-game:
  - Through the `Odyssey Mario's Moveset` menu, option `Character`.
  - Go to the Fourth floor and enter a pipe: red pipe for Mario, green for Luigi and yellow for Wario.<br>To select Peach, enter your character's pipe (red if you play as Mario, green if Luigi or yellow if Wario).
- Each character has different properties and moves:
  - **Mario**: the most balanced character.
  - **Peach**: same properties as Mario, plus extra moves from *Super Princess Peach*. See [Version 6.0.0](https://github.com/PeachyPeachSM64/sm64ex-omm/tree/master#peach).
  - **Luigi**: runs faster, jumps higher and twirl after a back-flip, but slides on ground and moves slower in the air.
  - **Wario**: runs slower and jumps lower, but has some powerful moves from *Wario World* and gains speed faster in the air.
- Interactions with custom objects:
  - Cappy can collect Luigi keys, Wario coins and the *secret power-up* in WDW.
  - Gloom Peach can kill enemy Blarggs.
  - Rage Peach can knockback Motos.
- Cheater:<br>Most of Cheater's cheats work with OMM, but there are exceptions:
  - Cheats that have no effect when `Odyssey Moveset` is enabled:
    - `Super responsive controls`
    - `Auto wall-kicks`
  - Cheats that are completely disabled with OMM:
    - `Infinite lives`
    - `Play as`

## OMM with DynOS:
- Peach, Perry and Tiara models can now be converted into DynOS models, but they need a version of OMM >= **6.1.0** to work properly.
  - Peach's `model.inc.c` and `geo.inc.c` are in the `data/omm/peachy/peach` directory.
  - Perry's `model.inc.c` and `geo.inc.c` are in the `data/omm/peachy/perry` directory.
  - Tiara's `model.inc.c` and `geo.inc.c` are in the `data/omm/peachy/tiara` directory.
  - All textures are stored in the `omm/gfx/peach` directory.

## LiveSplit auto-splitter:
- The player no longer needs to enter their splits manually in a text file.
- To make the game read splits, save them from LiveSplit as a `splits.lss` file next to the `sm64.us.f3dex2e.exe` executable.
- See [Speedrunning](https://github.com/PeachyPeachSM64/sm64ex-omm/tree/master#speedrunning) for more details.

## Other changes:
- New Cappy cheats:
  - `Cappy Can Collect Stars`, and Bowser keys, and Sparkly stars.
  - `Play As Cappy`:
    - Press **(X)** to throw Cappy, and control him instead of Mario!
    - Move with the left stick, hold **(A)** to go up, hold **(B)** to go down.
    - The camera follows Cappy, but you can hold **(R)** to look at him from Mario's perspective.
    - Press **(X)** again or wait a few seconds to return to Mario.
- Better ceiling hanging:
  - Mario can hang on a ceiling with a jump, a double jump or a wall-jump.
  - The player no longer has to hold the **(A)** button, but has to press **(A)**, **(B)** or **(Z)** to leave the ceiling.
  - Mario moves faster, up to half his ground top speed.
- Captures are affected by the characters modifiers, but inverted:
  - This has no effect on Mario or Peach captures, but...
  - Luigi captures run slower and jump lower, but move faster in the air.
  - Wario captures run faster and jump higher, but move slower in the air.
- The following captures can now open doors:
  - Goomba
  - Koopa
  - Bob-omb
  - Spindrift
