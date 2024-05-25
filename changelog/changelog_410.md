# Version 4.1.0 (23/12/2020)

## Changes and fixes:
- Cappy can open treasure chests (JRB, DDD).
- Cappy can grab Unagi's tail star.
- Cappy can collect rings of coins and coins trails with a homing throw.
- OMM Mario's underwater ground pound can trigger underwater purple switches (HMC, WDW).
- Inside a course, the pause menu has 4 options when Non-Stop mode is enabled:
  - Continue
  - Restart Level
  - Exit Level
  - Return to Castle (same as `Exit Course`, brings Mario back to the center of the castle)
- Collecting 7 stars in a row auto-ejects Mario from the current course, like in Time Trials.
- Revamped HUD:
  - Custom icon for star counter, coin counter and O2 level.
  - Current power-up and its remaining time is now displayed below the coin counter.<br>This also include combined power-ups, as Metal Vanish cap for instance.
- Re-entering a completed course (7 stars collected) make 3 power-up boxes spawn at the start, one for each power-up (Non-Stop mode only).
- Odyssey Mario's Moveset options menu:
  - Some options have been moved, but stay basically the same as before. Here is the new structure:
    - `Mario` sub-menu:
      - `Mario's Moveset` (Choice: Classic, Odyssey)
      - `Mario's Health` (Choice: Classic, 3-Health, 6-Health)
      - `Mario's Stars` (Choice: Classic, Non-Stop)
      - `Mario's Power-ups` (Choice: Classic, Improved)
      - `After-effects` (Toggle)
    - `Cappy` sub-menu:
      - `Cappy's Behavior` (Choice: Disabled, No Capture, Capture)
      - `Cappy Throw Button` (Bind)
      - `Cappy Up Button` (Bind)
      - `Cappy Down Button` (Bind)
      - `Cappy Left Button` (Bind)
      - `Cappy Right Button` (Bind)
      - `Unlimited Cappy Jumps` (Toggle)
      - `Cappy Stays Forever` (Toggle)

## Improved Power-ups:
- The Wing, Vanish and Metal caps have been reworked to fit more their original design.
- Improved Wing cap:
  - Inspired by Galaxy's Red Star power-up, it allows Mario to move freely in the air, without any height limitation.
  - Hold **(B)** to brake and rotate in place. Release **(B)** to start flying again.
  - Duration has been increased from 45 to 60 seconds.
- Improved Vanish cap:
  - Inspired by the Vanish cap render, it allows Mario to move through grids and thin walls.
  - When Mario touches such a wall, he instantly vanishes only to reappear on the other side, while a teleport sound effect is played.
  - As long as Mario wears the Vanish cap, his gravity is reduced, allowing him to jump higher and fall slower, and Cappy ignores wall and ceiling collisions when thrown.
  - Duration has been increased from 20 to 40 seconds.
- Improved Metal cap:
  - Inspired by the in-game description itself, it allows Mario to be truly and fully invincible.
  - Nothing can damage or knockback Metal Mario. He's also immune to fall damage.
  - He's too heavy to be grabbed by Chuckyas, King Bob-omb or Heave-hos.
  - He's resistant to strong winds and water currents.
  - He can perfectly walk on lava and quicksands without being slowed down.
  - He can destroy enemies that try to squish him, like Whomps, Thwomps, Tox Boxes or Eyerock hands.
  - He's also able to break normally indestructible objects, like Mr. Blizzards, bowling balls or rolling rocks.
  - Duration has been increased from 20 to 30 seconds.
  
## Bowser fights:
- General changes:
  - Increased possessed flaming bob-omb explosions from 3 to 5.
  - Increased possessed flaming bob-omb movement speed.
  - Increased Bowser exhaustion duration.
- OMM Bowser 1 seemed to be too hard for a first boss, so some changes were made.
- OMM Bowser 2 is ready and replaces vanilla Bowser 2 when entering the Fire Sea arena.
  - Added a new attack to Bowser: the flamethrower.
- OMM Bowser 3 is ready and replaces vanilla Bowser 3 when entering the Sky arena.
  - Added a new attack to Bowser: the rain of fire.
  - The circular platform still turns into a star-shaped platform after the second hit.
  - OMM Bowser 3 is way harder than OMM Bowser 2 and vanilla Bowser 3, but fits more as a final boss.

## Speedrun Rules update:
- Because OMM Bowser fights are longer (and mandatory) than vanilla Bowser fights, submitted runs of OMM prior to this version (4.1.0) are now obsolete and invalid.
- Because Non-Stop mode and OMM Bowser fights are now fully implemented, submitted runs of OMM starting from this version (4.1.0) will always be valid, no matter the future updates of this mod.
- Because `Restart level` and `Exit level` are options from the pause menu, the usage of any DynOS option is now forbidden. Opening the DynOS menu during a run makes it invalid.
- Odyssey Mario's Moveset mandatory options:
  - `Mario's Moveset` must be set to `Odyssey`
  - `Mario's Health` must be set to `3-Health`
  - `Mario's Stars` must be set to `Non-Stop`
  - `Mario's Power-ups` must be set to `Improved`
  - `Cappy's Behavior` must be set to `Capture`
  - Both cheats `Unlimited Cappy Jumps` and `Cappy Stays Forever` must be set to `Disabled`
- Mandatory options, as well as key/button binds, must be set before the start of the run.
