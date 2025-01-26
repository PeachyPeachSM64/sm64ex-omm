# Version 8.1.0 (26/01/2025)

## Bug fixes:

- Fixed a crash when loading an old save file (from v7.1.0 or older). Such save files remain partially incompatible with the latest version and data loss can still happen (mostly Sparkly Stars).
- Fixed the rom assets MIO0 issues.
- Fixed a bug affecting Mario's movement when trying to capture an enemy and hitting a Sparkly Star block at the same time.
- Fixed a bug where the player model would not turn fully invisible and partially obstructing the view when entering first person view with the OMM camera.
- Fixed a bug that could give a wrong star number to fake or impossible stars.
- Fixed the 6-Health gauge animation.
- Fixed Rage Peach getting damaged sometimes when ground pounding a vulnerable enemy.
- Fixed the music distortion not resetting properly when returning to the main menu while a Vibe is active.
- Fixed the "no button press" condition in Sparkly Stars (PSS) being invalidated right after restarting the level.
- Fixed a frame-perfect bug that could softlock the player if they exit a level as soon as the Sparkly Stars hint dialog box pops up.
- Fixed Sparkly Bowser not emitting sparkles when talking. Additionally, Sparkly Bowser now reverts to regular Bowser after being defeated, indicating that he lost the power of the Sparkly Grand Star.
- Fixed the Sparkly Grand Star not emitting sparkles.
- Fixed Snapshot mode not working in *Super Mario 74* and *Super Mario Star Road*.

Render96:
- Fixed and improved the milk power-up. Make sure to set the `Power-ups` setting to `Improved`.

## Cappy:

- Cappy can target and trigger water level diamonds (WDW).

## Captures:

- Yoshi:
  - Can eat wooden poles and signposts.
  - Can trigger water level diamonds (WDW).

## Sparkly Stars:

- Crystal Stars:
  - Castle Lobby: Moved the Toad elsewhere. No longer requires the Classic Moveset and speedrun knowledge.

## Camera:

- OMM camera:
  - Added first person view underwater (both swimming and metal).
  - Sync the camera angles with the first person view when possible.
- Freecam:
  - Sensitivity: Bettercam and Puppycam 2 sensitivity speeds are now the same to avoid reconfiguring the camera settings when switching between the two versions.
  - Fixed broken camera axis inversion on Render96 Bettercam.
- Snapshot mode:
  - Press <kbd>**D-Up**</kbd> to reset the roll.
  - Press <kbd>**D-Down**</kbd> to toggle the help.

## New features:

- `Extras` **Mario Mode**:
  - When enabled, turns the current character into Mario, while keeping the moveset, abilities and animations.
- Local stats:
  - In addition to the global stats, each save file has now its own stats record.
- Stats board:
  - Press `Left`/`Right` to switch between local (in yellow color) and global (in blue color) stats.
  - The "Rewards" view has been enhanced.
  - Added a "Secrets" view that keeps track of the various secrets the player found.
- Data Management sub-menu:<br>Can only be accessed in the main menu, allows the player to delete permanent data:
  - Reset Mario palettes
  - Reset Peach palettes
  - Delete Sparkly Stars data
  - Delete global stats
  - Erase secrets

**sm64coopdx**:
- Added support for `SHIRT` color. Vanilla Mario's cap and shirt no longer use the same color and can be customized separately.
- Added support for `EMBLEM` color. The palette editor will show this color only for models that support it.
- Added [`Character Select`](https://github.com/Squishy6094/character-select-coop) compatibility:
  - The game will try to load Character Select mods placed in the `res/packs` and `dynos/packs` directories.
  - An entry in the `Model Packs` sub-menu will be created for each individual character found, labeled `(CS) <character_name>` and containing the character's model, caps, custom animations, voice clips and palette preset. **Other elements such as custom movesets or custom geo functions are not supported and will be ignored**.
  - *Character Select models can only replace Mario, so the `Mario Mode` must be enabled to use Character Select models with other characters (Peach, Luigi, Wario).*
  - Added a sub-menu to control settings related to Character Select:
    - `Palette Preset`, to enable or disable the palette preset.
    - `Animations`, to enable or disable the custom animations.
    - `Voice Clips`, to enable or disable the character voice clips.
    - `Voice Volume Factor %`, to control the volume of the character voice clips.

## Misc. changes:

- Added secrets. Try to find them all!
- Auto-enable Sparkly sparkles when collecting a Sparkly Grand Star for the first time.
- `Invisible Mode` now makes captures invisible and reduces the shadow visibility.
- The `Shadow Mario` cheat now works only on Mario and custom models replacing Mario, or any custom model if the `Mario Mode` is enabled.
- The individual capture count for each save file is now displayed on the file select screen.
- Palette editor:
  - Added a `Save` button in the bottom right corner.
  - Added `Copy`, `Paste`, `Darken` and `Lighten` options to colors.
