# Version 7.1.0 (11/04/2022)

## *OMM Builder*:
- Fixed DynOS compilation on Linux-based systems.
- `r96a` now uses the repository `Render96ex-tester_rt64alpha`.<br>Note 1: You don't need an RTX card to build this branch.<br>Note 2: The new audio system in the `tester` branch is not supported yet.
- Removed options: `60_FPS` and `EXT_DATA`.
- Removed `rt64` and `saex` from compatible versions list.

## Bug fixes:
- Fixed a slowdown with Peach's model.
- Fixed a bug where Mario could long-jump out of a ground Cappy throw, even if he wasn't moving.
- Fixed the weird underwater Cappy throw momentum.
- Fixed a bug where cold water couldn't damage Mario when both Classic moveset and 1 HP mode were enabled.
- Fixed a bug where collecting coins underwater could kill Mario when both Classic moveset and 1 HP mode were enabled.
- Fixed a bug where collecting Castle stars could warp Mario back to the Castle grounds.
- Fixed a crash that could happen when starting a new game with Peach.
- Sparkly Stars:
  - Fixed a bug where Mario could capture Toads through walls with a well-placed Cappy throw.
  - Fixed a bug where Mario could go behind the TTC clock with a well-placed Cappy bounce.
- Star Road:
  - Fixed a wrong warp bug with the yellow pipe in the sewers.
  - Fixed the Grand Star hitbox.
- Render96:
  - Fixed the *Never stop playing as Peach* crash.

## GFX Engine:
- Built-in 60+ FPS patch, allowing to run the game from 30 to 360 FPS.
- Interpolated all HUD stuff, as well as dialog boxes, screen effects and timers.
- Frame interpolation is automatically disabled during states that doesn't need it, like the file select screen or when the game is paused.
- External Data is always enabled.

## OMM Bowser:
- Enraged mode: when Mario captures a flaming bob-omb, Bowser gets mad and enters Enraged mode.
  - In this state:
    - A burning aura is visible around Bowser,
    - The emergency jump is no longer triggered, so Bowser's combo is not reset if Mario gets too close to him.
  - During Enraged mode, Bowser cannot be damaged by the captured bob-omb's explosions.
  - Bowser leaves the Enraged mode if he's exhausted or if the bob-omb is no longer captured.
- Changes to Bowser's attacks:
  - Shockwaves move slightly faster, but the delay between each is reduced.
  - Most attacks have slightly reduced duration/cooldown.<br>As a result, **the boss fight is shorter, but the overall difficulty stays the same**.
- Changes to flaming bob-ombs:
  - They are summoned faster, and **can now be captured during their ascending state**.
    - If Mario captures a bob-omb during that state, Bowser enters Enraged mode instantly, and all other bombs are thrown at random locations on the battlefield.
    - This reduces the duration of the summoning and throwing action, but adds a difficulty as Mario has to avoid all remaining bombs at once.

## Color palettes
- Added color palettes for Mario and (playable) Peach to the `Extras` sub-menu.
- 28 built-in palettes are available and 4 customizable ones.
- Custom palettes can be edited in the save file, under `[mario_colors]` and `[peach_colors]`.
  - Each `mario_custom` (`peach_custom`) entry consists of 6 (7) pairs of RGB values in hexadecimal form.
  - The first value of a pair is the ambient color, the second is the diffuse color.
  - For Mario, the colors are in the following order: *Cap*, *Hair*, *Skin*, *Gloves*, *Overalls*, *Shoes*.
  - For Peach, the colors are in the following order: *Skin*, *Hair*, *Dress (light tone)*, *Dress (dark tone)*, *Gloves*, *Earrings*, *Shoes*.

## Odyssey Moveset:
- Mario can turn around faster underwater when standing still.
- The water punch can be canceled.
- The water dash no longer bonks into walls if Mario is not facing them.
- The player can redirect Mario after a water ground pound jump.
- Always perform the quick ledge climb action.
- The camera turns faster underwater.
- The camera moves below Mario when he's ceil-hanging.
- Cappy's homing attack range has been increased.

## Peach:
- Wearing an improved cap power-up allows Peach to launch Perry shockwaves, even if not at max health.
- Additionally, each power-up gives a Vibe effect to shockwaves:
  - The Wing cap gives Joy effect to shockwaves.
  - The Metal cap gives Rage effect to shockwaves.
  - The Vanish cap gives Gloom effect to shockwaves.

## Sparkly Stars
- Added the `Sparkly Stars` sub-menu:
  - `Mode`: Difficulty. *Disabled*, *Normal*, or *Hard*.
  - `Assist`: When enabled, helps the player by:
    - Preventing them from pressing not allowed buttons,
    - Preventing Cappy from capturing objects if captures are not allowed,
    - Preventing Peach from activating Vibes if Vibes are not allowed,
    - Displaying the remaining number of yellow/red/blue coins to collect, shards, boxes, mushroom 1-up, flames, etc...
  - `Show Hint at Level Entry`: When enabled, displays the hint sign text at level entry when there is one.
- Reduced SSL Pink Gold Star height.

## Other:
- The big Mario head (Goddard) is now skipped for all versions. The game goes directly from the title screen to the file select screen.
- Added level variant for BITS (120+ stars only):
  - Exit BBH or VCUTM, then enter BITS to access a Vanished variant of BITS, where the level geometry and surfaces are invisible (but collision is still there).
- Merged ex-nightly and ex-alo cheats list (does not overwrite Cheater in `r96a`):
  - `Moon Jump`
  - `God Mode`
  - `Invincible Player`
  - `Super Speed`
  - `Super Responsive Controls`
  - `No Fall Damage`
  - `Walk On Lava`
  - `Walk On Quicksand`
  - `Walk On Water`
  - `Walk On Poison Gas`
  - `Walk On Slope`
  - `Walk On Death Barrier`
  - `BLJ Anywhere`
