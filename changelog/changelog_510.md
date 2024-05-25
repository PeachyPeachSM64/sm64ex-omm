# Version 5.1.0 (28/04/2021)

## Bug fixes:
- Fixed a frame perfect softlock that happened when reading a sign as soon as the capture animation starts playing.
- Fixed a bug where Mario could capture objects while riding or holding something.
- Fixed a bug where Mario could bounce of Cappy while riding or holding something.
- Fixed a bug where Koopa could get his shell back if Mario was capturing the shell.
- Fixed a bug where the stationary bob-omb in BOB was not affected by gravity.
- Fixed a bug where uncapturing a Toad and capturing it again could spawn his star multiple times.
- Fixed a bug that prevented Mario from wall-sliding after jumping off a pole.
- Fixed a bug where Cappy used an incorrect Mario position to check their interaction, resulting in a 1-frame delay between the intended bounce and the actual bounce.
- Fixed a bug where Mario could do a Ground Cappy bounce on lava.

## Major changes:
- DynOS has been removed from OMM.
- Some functionalities have been kept, and moved to the regular options menu:
  - Warp to Level (level, area, star)
  - Return to Main Menu
  - OMM sub-menu:
    - The `Moveset`, `Cap`, `Stars`, `Power-ups`, `Camera`, `Buttons` and `Cheats` options are still the same.
    - Added a `Crystal Stars` option, allowing the player to set the Crystal Stars mode to `Disabled`, `Normal`, or `Easy`.<br>Selecting `Normal` or `Easy` requires a restart or a level change to apply.<br>You must defeat Bowser 3 with 120 stars at least once to be able to select `Normal` or `Easy` (so after the bad ending).
    - Added an `Extras` sub-menu with 3 new options:
      - `Cappy Eyes on Mario's Cap` displays Cappy eyes on Mario's cap, if enabled (doesn't work with all models, see below).
      - `Colored Stars` turns regular stars into colorful glowing stars, if enabled.
      - `3D Coins` turns 2D flat coins into 3D custom models, if enabled.

## Mario's Moveset:
- Added the Midair Spin: Press **(A)** during a fall to do a quick spin without losing height.<br>If Mario wears the Improved Vanish Cap, pressing **(A)** again right after a Midair Spin will trigger a regular Air Spin.
  
## Cappy:
- Pressing **(X)** + **(A)** does an up-throw instead of a regular throw.
- Mario can now bounce on Cappy during the falling part of a lava boost, like in Odyssey.
  
## Captures:
- After leaving a Goomba capture, Goombas stay stacked if **(Z)** is pressed, but break if Mario gets damaged.
- Attacking a Goomba stack with Mario will break the stack.
- You can stack up multiple Goomba stacks by jumping on top of them when possessing a Goomba.
- Modified the OMM Cam to be able to show the entire stack of Goombas if Mario is possessing a Goomba.
  
## Non-Stop mode:
- When Mario dies, he restarts from the area he was in, instead of the very start of the level.
- Collected stars no longer respawn after a death, as they stay collected until Mario leaves the level or select `Restart Level` on the pause menu.<br>If Time Trials is enabled, the entire level is reset after a death, and lost coins don't respawn.

## Crystal Stars:
- Added a coin arrow in HMC, DDD, WDW and COTMC to show where the star is.
- VCUTM: Reduced available time from 60 to 45 seconds.
- The bad ending now plays only after beating Bowser 3 for the first time with 120 stars, and as long as the Crystal Stars mode is enabled but Peach is not unlocked yet.
- The secret ending always plays after beating Bowser 4 and collecting the last Crystal Star, but the ending cake screen will show up only the first time, when Peach is about to be unlocked.
- Added Easy mode:<br>In Easy mode, the following changes make Crystal Stars easier to obtain:
  - The No-Cap condition is not checked after spawning a Star (Condition stars only), meaning that the player can use caps to reach the Star.
  - The Requirements (specific type of coins, no button press, etc...) are not checked after spawning a Star (Condition stars only).
  - Timers immediately stop after spawning a Star (Cap courses).
  - Bob-omb Buddies' cannons are available everywhere.
- Additionally, Easy mode comes with changes specific to each Crystal Star:
  - BOB: Cannons can be used to collect the secrets.
  - WF: Only 90 coins are needed (Thwomp coins can be skipped).
  - JRB: Only 80 coins are needed (you can miss up to 4 blue coins).
  - CCM: Same area, but the star is in bounds and easier to reach.
  - BBH: No change.
  - HMC: Same area, but the star is in bounds and easier to reach.
  - LLL: Same area, but the star spawns above the 5th power star and requires only wall-jumps to be reached.
  - SSL: The star is slightly lower.
  - DDD: The star is in front of the wall, instead of behind.
  - SL: The snowman's big hop is higher, spindrifts are not required.
  - WDW: Moved to above the 1-up mushrooms tunnel.
  - TTM: Same place, but lower.
  - THI: Same place, but lower.
  - TTC: Increased available time from 45 to 60 seconds, and the star spawns lower.
  - RR: No A press only.
  - BITDW: Only the 8 red coins are needed.
  - BITFS: Reduced the number of needed 1-up mushrooms from 7 to 6, and the star spawns right after the bridge leading to Bowser 2.
  - BITS: No A press only, and the star spawns in front of the pipe instead of above.
  - PSS: Increased available time from 22 to 24 seconds.
  - TOTWC: Same duration (40 seconds), but the timer stops after collecting all coins.
  - VCUTM: Same duration (45 seconds), but the timer stops after touching all 5 corners.
  - COTMC: Same duration (50 seconds), but the star spawns lower.
  - WMOTR: Only the 8 red coins are needed.
  - SA: Only the 8 red coins are needed.
  - GROUNDS: Same place, but lower.
  - INSIDE: The out of bounds star is replaced by 8 shards to find and collect.
  - BASEMENT: The hint is more explicit.
  - 2ND FLOOR: The out of bounds star is replaced by 8 shards to find and collect.
  - COURTYARD: The hint is more explicit.
  - BOWSER 4: Flaming bob-ombs can't explode after taking damage, and Bowser drops a 1-up mushroom each time he gets hurt.
- Updated anti-cheat system:
  - Now works as intended, with no false positive.
  - When a cheat is detected, the anti-cheat acts instantly, instead of at level exit.
  - New Bowser quotes!
- If cheats were enabled before the Crystal Star mode, prevents the latter from enabling via the options menu.<br>However, breaking the Crystal Star block on Castle Grounds sets the mode to `Normal` and turns off cheats.

## Render96-alpha changes:
- All text used by OMM is stored in `AM_us.omm.json` files, and only the English version is done for now.<br>Feel free to copy this file and translate it into your native language.
- In dialogs, the word `Mario` is correctly replaced by `Luigi` or `Wario`, depending on the selected character.<br>Course names and Star names are not affected by this change.
- Added Odyssey death sounds for Luigi and Wario.
- Added Luigi and Wario to the character select menu.<br>Luigi and Wario appear only if they have been unlocked in at least one save file.<br>Loading a save file checks if the selected character has been unlocked in that file, and change the selected character to Mario if not.
- Added Peach to the character swap room (fourth floor).<br>To select Peach, jump into the selected character's pipe (red pipe for Mario, green pipe for Luigi, yellow pipe for Wario).<br>You must have unlocked Peach first for this functionality to work.
- Tweaked some of Wario's moves, to be able to carry on Odyssey moves:
  - Wario's pile driver acts as a ground pound, so Wario can perform a ground pound jump by pressing **(A)** out of it.
  - Wario can throw Cappy during his special triple jump.
  - Wario can roll by pressing **(Z)** during his shoulder bash.
  - Wario can stun an entire Goomba stack by jumping on it, but grabbing the bottom Goomba will break the stack.
- Some Odyssey moves are enhanced by Luigi's hidden modifier:
  - Luigi's jumps are higher, including Cappy bounces.
  - Luigi's water dash is faster.
  - Luigi flies faster with the Improved Wing Cap.
- Added bad cake ending screens for Luigi and Wario.
- Wario's shoulder bash loses its invulnerability against OMM Bowser 4 attacks (Normal mode only).
- Milk makes Cappy grow.

## Minor changes:
- Infinite health cheat now fills the O2 gauge and makes Mario immune to frozen water.
- Stage variants only appear after collecting all 120 stars.
- Instant quicksands are now scorching sands (Odyssey moveset only):
  - They no longer kill Mario instantly, but rather act as lava.
  - Metal Mario can still walk on this type of sand.
  - Deep quicksands are not affected and remain the same.
- Reworked Cappy eyes code.<br>For now, they appear only on the following models:
  - Vanilla Mario
  - Odyssey Mario
  - Moonshine Mario
  - Low-Poly Promo Mario/Luigi/Wario
  - Render96 2.2 Mario/Luigi/Wario
  - VaniLla96 Mario/Luigi/Wario
