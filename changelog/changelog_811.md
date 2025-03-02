# Version 8.1.1 (02/03/2025)

## Bug fixes:

- Fixed DDD cage clip and other wall clips that were accidentally patched in v8.
- Switching between Classic and Odyssey movesets no longer cancels burn damage.
- Odyssey Moveset: Mario can no longer accelerate airborne against level boundaries (out-of-bounds walls).
- Mario no longer brings capture through warps if the `Cappy` setting is not set to `Capture`.
- Peach attacks can now be canceled into a roll with <kbd>**Z**</kbd>+<kbd>**B**</kbd>.
- Fixed flaming bob-ombs sometimes stuck in the air during OMM Bowser fights.
- Fixed Yoshi being a registerable capture in Moonshine.
- Fixed Cappy being unloaded when changing area through an instant warp (SSL, DDD, WDW).
- Fixed Castle basement Pink-gold Star not spawning if the Toads on the pillars aren't the basement ones.
- Fixed the save file cheat code erasing Pink-gold Stars data if Peach wasn't unlocked yet.
- Fixed misplaced hand effects for custom models (Wing cap hand glows, Perry, hanging actions).
- Fixed custom animations with non-Mario objects.
- Fixed inconsistent hitboxes between player models (Yoshi tongue, Rage vibe aura, Perry attacks).
- Fixed instant warp smooth interpolation (endless stairs, secret room corridor).

## Time Trials:

- Time Trials for Sparkly Stars.
- Individual times for each playable character.

## HUD:

- Sparkly Stars HUD:
  - Highlight the current Sparkly Star state and shows restrictions if any.
  - Cap timer is also highlighted in Cap courses.
- New option in `Display`: `HUD Size`, to control the base size of the HUD elements.

## Builder:

- Updated the **Render96** model packs version from `v3.2` to `v3.25`.
- Updated the **OMM-HD** texture pack version from `v8.0.0` to `v8.1.1`.
- Added `linuxmint` as a supported operating system.
