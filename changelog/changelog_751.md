# Version 7.5.1 (25/02/2023)

## Bug fixes:
- Fixed the "unlocking key door" action.
- Fixed secrets count, graphics and sound.
- Fixed the ending skip.
- Fixed the "Bully stuck in lava" glitch.
- Wiggler no longer reduces the background volume if Mario spawns below it.
- If the first star of Whomp's Fortress is collected with a Capture, Mario will leave it for the Non-Stop cutscene to play properly.
- Captures are no longer glitched when going through an instant warp (SSL, DDD, WDW).
- Captures can now properly climb stairs.
- Ground pounding at the top of flooded JRB doesn't refill O2 anymore.
- OMM camera:
  - Fixed transition between slide states and non-slide states.
  - Fixed the weird zooms that could happen in Bowser 3.
  - Camera inverted axes have now an effect on the main camera, the first person mode and inside a cannon.
- Vanilla/Better camera:
  - Fixed camera position with underwater captures.

## Captures:
- All automatic and Boss dialogs are skipped when interacting with a Capture.
- Some captures have been buffed to make them less useless:
- Goomba:
  - Added ability to spin jump.
  - Dust particles can be seen to indicate that Goomba's next jump is a spin jump.
- Koopa:
  - Buffed a little to match the Koopa-less shell capture.
- Underwater Koopa shell:
  - Same as a regular Koopa shell, but can move underwater instead of sliding on water.
- Chain Chomp:
  - Faster, but heavier.
- Hoot:
  - Faster and fly higher.
- Whomp:
  - Moves a bit faster, jumps higher and its attacks are faster.
- Spindrift:
  - Slightly faster.
- Mr. Blizzard:
  - No longer locked in place when holding **(B)**.
  - Snowballs charge faster, and fully charged snowballs are bigger.
  - The throw animation no longer prevents the player to move or attack.
  - Flat ceilings no longer destroy snowballs.
- Boo:
  - Slightly faster.
- Mad Piano:
  - Reduced wall hitbox.
  - Can now destroy doors on contact.
- Mr. I:
  - Slightly faster, jumps higher.
  - No longer locked in place when holding **(B)**.
  - Eye beams charge faster, and fully charged beams are bigger.
  - Flat floors and ceilings no longer destroy beams.
- Jumping Box:
  - Can bounce once on lava if **(A)** is held. Land on a safe floor to restore the bounce.
- Swoop:
  - Fly a bit higher.
- Snufit:
  - No longer locked in place when holding **(B)**.
  - Flat floors and ceilings no longer destroy balls.
- Monty Mole:
  - Moves faster.
  - Rocks can be charged.
  - The throw animation no longer prevents the player to move or attack.
  - Flat ceilings no longer destroy rocks.
  - Added new ability: "hole teleportation". Press **(A)** to hide in ground, select a hole with the left stick, then press **(B)**.
- Bully:
  - Can jump and ground pound during the charge attack.
- Motos:
  - A bit faster, jumps higher.
  - Hold and throw animations are faster.
- Blargg:
  - Moves faster, fire breath is faster.
- Friendly Blargg:
  - Moves faster.
- Toad:
  - Runs faster than Mario.

## Enemies and objects:
- Some enemies play their death animation faster: Mr. Blizzard, Piranha Plants, Fire Piranha Plants, Mr. I.
- Hoot's dialog when waking up has been removed.
- All 5 Fire Piranha Plants can be spawned at the same time.
- Removed delay between Wiggler's unsquishing and its dialog.
- Captured Chain Chomp and Mad Piano can now destroy most of enemies.
- Spawned stars can be grabbed a bit earlier.
- Clams no longer deal damage (only knockback) and don't interact with captures.

## Odyssey Moveset:
- Unrestricted cannon angles and controllable with the mouse if mouse camera is enabled.
- Vertical wind: Mario goes into freefall state after some time if he's no longer above a vertical wind surface.
- Better underwater shell ride: faster, infinite, cancellable with **(B)** or **(Z)** and the shell no longer disappears.
- Cappy can now grab objects if Mario is underwater (but not Metal).
- Health meter is updated only when the pause menu is closed, meaning cycling through the Moveset options doesn't reduce health anymore.
- There is now a 3-frame window where the player can cancel a high fall landing by pressing **(A)**.

## Changes:
- Brand new main menu, looking more Odyssey-like.
- Added a stats board that keeps track of most of the player's actions. The stats board is available in every game, and can be found somewhere in the Castle grounds.
- Added a button bind for the spin ability. Defaults to left-stick button and mouse wheel button.
- Added a new cheat: "Shadow Mario".<br>Turns Mario into Shadow Mario and replaces his attacks by Shadow Mario's brush attacks.<br>Peach needs to be unlocked to unlock this cheat.
- Built-in model loader: You can load DynOS models without DynOS installed by putting your model packs in the `dynos/packs` or `res/packs` directory.
- LiveSplit Auto-splitter can now trigger on level exit:
  - In star splits, use square brackets to split upon star collection or parentheses to delay the split until the level exit.
- Crystal Stars:
  - WF: Reduced timer from 1'45" to 1'40".
  - BBH: Reduced timer from 2'10" to 1'50".
- ***Super Mario 74: Extreme Edition*** has now its own pattern of star colors.
