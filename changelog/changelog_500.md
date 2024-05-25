# Version 5.0.0 (01/04/2021)

## Bug fixes:
- Fixed a bug where OMM Bowser could get hit at the start of his jump action, resulting in a premature knockback action that could lead to a crash.
- Fixed a bug where OMM Bowser was unable to hit Mario with his flamethrower.
- Fixed a bug where the sinking into quicksands effect were applied twice per frame.
- Fixed a bug where Mario couldn't open a door already opened by a possessed Toad.
- Fixed a bug where selecting Restart Level during a Bowser fight restarted the whole level instead of the fight.
- Fixed a bug where Mario was stunned after falling while riding a shell, thus losing the shell.
- Fixed a rendering crash in VCUTM.

## Mario's Moveset:
- OMM Health has been merged with OMM Moveset.<br>Mario's Moveset choices are now: `Classic`, `Odyssey (3-Health)`, `Odyssey (6-Health)`.
- Mario no longer loses a life when dying.
- Reduced the water dash O2 consumption.
- Mario now progressively loses health instead of dying instantly when its O2 meter reaches 0 (but still dies instantly in poison gas).
- Increased HP consumption in cold water for Classic Moveset to 1 segment per second.
- Dive recoveries now carry into a freefall after the rollout, allowing Mario to perform aerial moves instead of falling to his death.
- Mario can wall-slide after a jump kick.
- Better stick rotation detection: Spin jumps should be easier to perform.
- Fwoosh and Big Snowman can no longer blow off Mario's cap (Mario still receives the knockback).
  
## Cappy:
- Increased Cappy throw distance with the Vanish cap, due to the lower gravity.
- You can now perform a Cappy homing attack with spin throw variants.
  - Spin throw: Left and Right buttons move Cappy outside of its circle.
  - Flying throw: Left and Right buttons move Cappy outside of its circle, Up button moves Cappy forward, Down button moves Cappy backwards.
- Cappy jumps are now called Cappy bounces, because a Cappy bounce is not a jump.
- Added Cappy throw strength: hold the throw button to launch Cappy far away or slightly press it to launch it near Mario.<br>If the left stick is held in any direction, always perform a maximum distance Cappy throw.<br>This change affects only the regular Cappy throw, not up, down and spin variants.
- New Cappy cheats: `Global Homing Attack Range` and `Mario Teleports To Cappy`.
- Cappy's homing attack ignore collisions if `Global Homing Attack Range` is enabled.
- Cappy's homing attack can be canceled by pressing the throw button **(X)**.
- Cappy's homing attack targets coins first.
- Cappy is now strong enough to break exclamation boxes.
  
## Captures:
- Increased captured Bob-omb jump height, and explosions **(B)** trigger a second jump if performed airborne.
- Captured Bob-ombs now respawn correctly after being destroyed.
- Captured Bullet Bill explosions now always destroy objects, even if the Bullet Bill is not moving fast (**(B)** held).
- Captured Bullet Bills and Whomps can now destroy Thwomps with their attacks.
- Jumping on bullies with a capture knockbacks them.
- King Bob-omb, King Whomp, Tuxie's mother and the Snowman's head are no longer capturable.
- Koopa Shells (without Koopas) are now capturable, and are slightly faster than a shelled Koopa.
  
## Improved Power-ups:
- Improved Metal Mario can no longer be eaten by Bubba.
- Improved Metal Mario is immune to lava damage, but still does the lava boost.
- Improved Metal Mario can now destroy the Mad Piano.
- Improved Wing Cap: Press **(B)** twice while airborne to start flying (only if Mario is not diving).
- Reverted initial cap durations in Cap courses to their default values (Improved power-ups still have their custom values when they are collected from an exclamation box).

## Non-Stop mode:
- Added glowing stars, with optional Render96 star textures (SM64 star textures are selected by default).
- Because of warp pipes, checkpoints are no longer disabled.
- Koopa the Quick no longer cares about King Bob-omb in BOB.<br>The player can now start the race before defeating King Bob-omb.
- The following changes are applied now to every stage, not only the main courses:
  - Mario always does the Odyssey star dance, unless he's about to exit the current stage (see below).
  - Mario automatically exits the current stage he's in if all stars have been collected, except in the Castle or in Bowser stages.
  - Collecting a key after a Bowser fight forces Mario to leave the stage.
  - The Odyssey death sequence is always played when Mario's health reaches 0, except in the Castle.
- Lost coins no longer turn into money bags.
- World objects (warp pipes, sets of power-up boxes...) always spawn, even if the Non-Stop mode is disabled.
  
## Enhanced Camera:
- Fixed a crash that could randomly happen when using the OMM camera.
- The OMM camera can be enabled anywhere and at anytime, except inside the Castle and during Bowser fights.
- More modes: 5 distances, 8-Directions and 16-Directions, press **(R)** to change mode without opening the DynOS menu.
- Hold **(R)** while standing still to enter the first person camera mode (like C-up mode, but Mario is invisible).
- The Camera status is now correctly displayed on the HUD.
  
## Physics changes:
- Collisions: The following changes are applied if the Moveset is set to Odyssey:
  - Treats PUs as out of bounds.
  - Fixes the unreferenced wall glitch, and returns the closest wall Mario is looking at.
  - Fixes surface cucking, always returns the lowest ceiling and the highest floor.
  - Checks floor on a square instead of a point, to prevent objects from falling inside small gaps.
  - Increases number of cells checked for more precise wall collisions.
- Steps: The following changes are applied if the Moveset is set to Odyssey:
  - Increases number of sub-steps from 4/4/1 to 16/16/16, for more precise surface detection.
  - Fixes exposed ceilings, they no longer extends infinitely upwards, but up to half Mario's height.
  - Out of bounds does not make Mario bonk, but prevents him from moving horizontally.
  - Ceilings does not make Mario bonk, but prevents him from moving horizontally.
  - Fixes glitchy ledge grabs.
  - Fixes jumps on moving platforms.
  - Fixes underwater up/downwarps.
  - Fixes whirlpool attractive strength.
    
## Stage variants:
- Exit CCM or SL and enter SA to change the water to cold water.
- Exit DDD or WDW and enter JRB to increase the water level, resulting in an entirely flooded JRB.<br>The ship and Goombas don't spawn inside flooded JRB, but the jet stream and its star do.
    
## Some QoL:
- From now on, OMM uses its own save file (only if `TEXT_SAVES` is not enabled).
- Added a built-in model pack `Vanilla Mario With Cappy`.
- Added a version number on the title screen and the main menu (bottom-right corner).
- Added a sanity check for graph nodes: Corrupted graph nodes are less likely to crash the game when processed.
- Added a sanity check for music sequences: Corrupted sequences are less likely to crash the game when processed.
- The HUD shows the correct amount of Stars available in the current level (mainly for PC-ported rom-hacks).
- Because the life counter is now useless, mushrooms 1up fully restore Mario's health when collected.
- Mips always spawns in the basement after collecting the 120 stars.
  
## Big Boo's Haunt captures:
- Boo:
  - The Boo's coin is automatically collected when captured.
  - Press **(B)** to turn transparent and go through walls, as well as gaining a nice speed boost.
  - Press **(A)** repeatedly to move up. Release **(A)** to fall slowly.
  - Capturing a Ghost Hunt Boo or a Merry Go Round Boo marks it as eliminated, making the Big Boo spawn faster.
  - Capturing the Courtyard Big Boo (the one with the cage) triggers the warp to BBH.
- Scuttlebug:
  - Jump with **(A)**, run by holding **(B)**.
  - Captured Scuttlebugs can open doors.
  - Captured Scuttlebugs can stick to walls by holding **(B)**.
    On a wall, you can move left and right as long as **(B)** is held, and jump from it by pressing **(A)**.
- Mad Piano:
  - Can move, but cannot jump or attack.
  - Press **(A)** or **(B)** to play a song.<br>The song lasts as long as Mario is inside BBH, and pressing different buttons has different effects:
    - **(A)**: Play the next song on the list.
    - **(B)**: Play a random song.
    - **(A)** + **(B)**: Restore the BBH music.
- Crazy box:
  - Constantly bounces, even if not moving.
  - Hold **(A)** to bounce higher.
  - Hold **(B)** to do a super bounce, each super bounce is higher than the previous one, but the box breaks after 3 super bounces.
  - Leaving the Crazy Box resets the super bounce counter.
- Mr. I:
  - Can move, jump, and has low gravity.
  - Because it floats, captured Mr. I is immune to ground hazards (lava, quicksands).
  - Can shoot a projectile by pressing **(B)** (hold **(B)** to shoot faster and further), and can open doors.
  
## Post-game content:
- Increased number of collectable Stars from 120 to 150.
- These 30 new stars, called Crystal Stars, appear after collecting the 120 Power Stars, and defeating Bowser 3.
- They are incredibly hard to find and collect, but gathering them all unlocks something really worth...
- Added two new endings, the bad ending and the secret ending.
