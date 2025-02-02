# Version 8.0.0 (06/11/2024)

## Bug fixes:

- Fixed 100 coins star that could spawn higher than normal on some stages.
- Fixed cap modifier allowing Mario to wear a cap that is not unlocked yet, unlimited duration if cap modifier cheat enabled, and cap modifier is now available in the Castle levels if all stars of a game have been collected.
- Mario now leaves his capture if he loses his cap due to Klepto.
- Underwater shell is now properly thrown when pressing <kbd>**B**</kbd>.
- Hoot can be properly grabbed again.
- Capture no longer occurs when the enemy is out of bounds.
- Held, ridden and captured objects no longer despawn if Mario changes area due to an instant warp (DDD, WDW).
- Fixed a bug that prevented Mario from getting out of the snow quicker by pressing <kbd>**A**</kbd> repeatedly.
- The last explosion of a flaming bob-omb now properly affects Bowser.
- Fixed a bug that prevented Mario from performing a cap throw during the first 6 frames of a roll.
- Fixed the Peach/Bowser painting fading when approaching BITDW.
- Peach's rage shockwave now properly spawn at Peach's height.
- Cappy can no longer collect coins when Mario is in a death action.
- Fixed various mother penguin bugs.
- Fixed hitbox/surface collision detection.
- Cappy can now properly target and interact with Unagi's tail.
- Sparkly sparkles now follows Mario's model rather than his physical position.
- Throwing Cappy after a shell jump now properly gives upwards velocity.
- Added missing sound effects and particles for some moves.
- Fixed Mario being grabbed:
  - Fixed Astral projection glitch.
  - Mario can now be grabbed and released underwater.
  - Fixed all softlocks related to being grabbed.
- The Sparkly Stars bad ending can no longer be triggered from a "no-save" file.
- Fixed Whomp being stuck after slamming in a small body of water.
- Mr. Blizzard can no longer be attacked while hidden in the snow.
- Fixed ground pound/spin pound actions not transitioning into metal underwater versions when entering water as Metal Mario while holding <kbd>**Z**</kbd>.
- Giant Goombas can now be ground pounded by Metal Mario.
- Vanilla Bowser can no longer fall out of the ring after being defeated, to prevent the defeated animation from being canceled.
- Mario now properly looks at Bowser during the Bowser fight intro.
- The Peach ending cutscene Grand star now uses the OMM model if the colored stars are enabled.
- Fixed Vanish cap step not working properly underwater.
- Fixed God Mode cheat preventing Mario from triggering the Yoshi "surprise".
- Captures no properly trigger the PSS start and end timer events.
- Mario's wallbox no longer interferes with small captures hitboxes.
- Fixed a top-of-pole jump offset.
- Fixed the underwater metal dive not being able to be redirected after a spin pound.
- Fixed an inconsistency with JRB star 1 and Non-Stop mode.
- Fixed a spin softlock bug.
- Wall slide now properly resets the air combo if canceled by an air spin.
- Fixed a bug that allowed OMM Bowser to get mad if Mario captured any object, not only flaming bob-ombs.
- Fixed a bug that allowed grab actions being carried between save files.
- The shocked from high fall action no longer makes Mario invulnerable.
- Fixed a rare softlock that happened if Mario gets squished and pushed at the same time.

Render96:
- Fixed "Sfx Mute" option not muting Peach/OMM sound effects.
- Fixed missing red overlay during Chaos mode "Bowser Time!".
- Fixed "Inverted Screen" effect.
- Fixed "Rainbow" effect.
- Fixed "Slow Time" effect.
- Fixed softlock with "Deadly Objects".
- Fixed the weird fade-in and the distorded music when collecting a star.
- The Luigi key in WF is now hidden if the Whomp tower is not visible yet.

## Odyssey Moveset:

- Greatly improved underwater moveset:
  - Handling is better, Mario turns faster, depends on current speed.
  - Most of lag time has been removed, each action can cancel into another one almost instantly, including when diving into water or throwing Cappy.
  - Increased top speed for most of actions (most notably breaststroke and flutterkick).
  - Water punch and water throw have faster animations.
  - Rebirth water ground pound: hold <kbd>**Z**</kbd> again to regain falling speed.
  - Water jumps:
    - Easier to perform: hold <kbd>**A**</kbd> + stick down at surface.
    - Retain most of Mario's swimming speed, but they have no handling (turn into freefall after some time).
	- Can transition into a wall slide.
    - Perform a Super water jump if <kbd>**A**</kbd> is pressed within a 3-frame window and given enough speed from the moment Mario surfaces.
- Underwater Metal Mario:
  - Movement speed is greater, most actions have less lag.
  - Underwater air bonk is no longer infinite and turns into freefall after some time.
- Some jumps changes:
  - Ground pound jump now peaks slightly higher.
  - Triple jump is now affected by forward velocity: As a reference, a running triple jump now goes as high as a spin jump.
- Top-of-pole action can be canceled by pressing <kbd>**Z**</kbd>.
- Peach's Calm Vibe now stays active underwater and in poison gas, unless God Mode is enabled.

## Cappy:

- Cappy throws are faster and Cappy can be recalled sooner.
- Cappy homing attacks from spin throws now use Mario as origin and has increased radius.
- When Mario is swimming, he can now bounce on Cappy if he's at the water surface.
- Cappy can now reveal hidden Moneybags.

## Enemies:

- Removed the need to touch the checkpoint in CCM to lead the big snowman ball to its head.
- Mother penguin gets angry if Mario grabs or captures her baby after giving him the star.
- The Ukiki cage in TTM can be destroyed by some strong attacks.
- Moneybags are always tangible unless they fade-in/out, but their fading duration is shorter.
- Heave-Hos are now visible and tangible underwater.
- Heave-Hos can be destroyed by a Metal Mario ground pound.
- Grabbing and throwing a Toad or Mips forces them to spawn their star.

## Captures:

- Captures can now trigger warps and reappear after a warp:
  - Instant warps (DDD, WDW)
  - Surface warps (various holes and entrances)
  - Fading warps
  - Warp doors (Basement and Upstairs doors still need the corresponding key to be opened)
  - Warp pipes
  - Painting warps
- First person camera is now available when controlling a capture. Hold <kbd>**R**</kbd> to enter first person mode.
- Goomba:
  - Can no longer mix different Goomba sizes when stacking Goomba-like objects.
- Chain Chomp:
  - Hops are treated as ground pounds.
- Whomp:
  - Body slam attack is treated as a ground pound.
- <kbd>New!</kbd> Thwomp, Grindels:
  - Bounce when moving, hold <kbd>**A**</kbd> to bounce higher.
  - Jump and ground pound with <kbd>**B**</kbd>. If the left stick is held, slightly move in the direction of the stick during the jump part. Produces a powerful shockwave on landing.
- Bullet Bill:
  - Hold <kbd>**A**</kbd> to slow down and stop.
- Big snowball:
  - Attacks are treated as ground pounds.
- Small penguin:
  - Press <kbd>**B**</kbd> to start sliding, press <kbd>**B**</kbd> again on ground to stop.
- Mad piano:
  - Hops are treated as ground pounds.
- Dorrie:
  - Can swim peacefully on a still lake, but sometimes dreams of being able to fly above clouds...
- Bully:
  - Ground pound attack is now properly treated as a ground pound.
  - Capturing the last bully alive of a bully trio makes the Big Bully appear.
- Motos:
  - Move and jump, above and under water.
  - Can charge its throw.
- <kbd>New!</kbd> Fly guy:
  - Faster than a spindrift, but jumps slightly lower.
  - Spindrift moveset, but throw bouncing fireballs with <kbd>**B**</kbd> instead of attacking with a spin.
- <kbd>New!</kbd> Pokey:
  - Move and jump.
  - Throw lowest body part with <kbd>**B**</kbd>, reducing its height but increasing its move speed and jump height.
  - Body parts grow again after some time.
- <kbd>New!</kbd> Amp:
  - Move and jump, can float for some time if A is held after a jump, like Peach.
  - Produces an electric field that always attacks nearest objects and attracts coins.
  - Hold <kbd>**B**</kbd> to charge and release a powerful electric discharge.
- <kbd>New!</kbd> Fire spitter:
  - Move and jump, both above and under water.
  - Press <kbd>**B**</kbd> to throw a fireball in a straight line.
  - Hold <kbd>**B**</kbd> to produce a powerful flamethrower.
- <kbd>New!</kbd> Bub:
  - Move horizontally, but you can go up by holding <kbd>**A**</kbd> and down by holding <kbd>**B**</kbd>.
  - By pressing <kbd>**A**</kbd> + <kbd>**B**</kbd>, perform a fast spin attack that attracts nearby coins.
- <kbd>New!</kbd> Sushi Shark:
  - Underwater Bullet Bill, but without a time limit. Hold <kbd>**B**</kbd> to go faster, <kbd>**A**</kbd> to stop.
- <kbd>New!</kbd> Moneybag:
  - Move and jump.
  - Hold <kbd>**B**</kbd> to charge and release a spinning coin that can hit enemies, bounces on walls and attract nearby coins, but consume 1 coin. At max charge, launch a stronger, faster, wider blue coin but consume 5 coins.
  - If the projectile hits an enemy, it releases coins equal to the number that was consumed to launch it (only once per enemy hit).
  - All coins collected as the Moneybag capture are worth 2x, except lost coins.
- <kbd>New!</kbd> Skeeter:
  - Walk on ground, slide on water and jump.
  - Hold <kbd>**B**</kbd> to run on ground or slide faster on water.
- <kbd>New!</kbd> Heave-Ho:
  - Move and jump, above and under water.
  - Grab objects by pressing <kbd>**B**</kbd>.
  - Hold and release <kbd>**B**</kbd> to throw the grabbed object.
- <kbd>New!</kbd> Chuckya:
  - Same as Motos and Heave-Ho, but move speed and jump are greater.
  - While Motos throws are more horizontal and Heave-Ho throws are more vertical, Chuckya throws have a good balance between horizontal and vertical distances.
- <kbd>New!</kbd> Ukiki:
  - Move and jump.
  - Hold <kbd>**B**</kbd> to run faster, or climb and move on walls.
  - The TTM cage Ukiki can open the cage and free the star by simply touching it.
- <kbd>New!</kbd> Lakitu:
  - Move horizontally, hold <kbd>**A**</kbd> to go up, release to go down.
  - Lakitu can move anywhere, but disappears after some time.
  - Floats above the ground, making it immune to ground hazards.
  - Can throw Spiny eggs by pressing <kbd>**B**</kbd>.
- <kbd>New!</kbd> Spiny:
  - *Rolling around at the speed of sound*
  - Moves really fast, jumps really high and has a double jump.
  - Hold <kbd>**B**</kbd> on ground to charge and release a spin dash.
  - Press <kbd>**B**</kbd> airborne to perform a spin pound and bounce on ground.
- <kbd>New!</kbd> Mips:
  - Move and jump, can be controlled after his dialog.
  - One of the fastest captures, hold <kbd>**B**</kbd> to run.
- <kbd>New!</kbd> Bee (SMSR, SMMS):
  - Spindrift moveset.
- <kbd>New!</kbd> Yoshi:
  - Can move and jump, both above and under water.
  - Hold <kbd>**A**</kbd> mid-air to flutter jump.
  - Press <kbd>**B**</kbd> to stick Yoshi's tongue out, pulling in any object it touches. When reaching Yoshi's mouth, the object is either swallowed, destroyed or kept in his mouth.
  - When an object is in Yoshi's mouth, press <kbd>**B**</kbd> again to throw it, or press <kbd>**X**</kbd> to swallow it and turn it into an egg.
  - When Yoshi has nothing in his mouth, hold <kbd>**X**</kbd> to charge and release an egg.
  - The egg can ricochet and target the nearest enemy in range, until there is no more enemy available.
  - When the egg breaks, spawn 1 coin for each enemy it has defeated.
  - Yoshi is the only capture that gets additional moves when Mario wears a power-up cap:
    - Wing cap:
      - Yoshi grows wings on his back, allowing him to fly indefinitely.
      - In this form, Yoshi cannot swallow enemies, but throw fireballs instead.
    - Vanish cap:
      - Yoshi can go through walls and ignore most of enemies attacks.
      - Gains a speed boost and becomes immune to fire.
    - Metal cap:
      - Becomes invulnerable and immune to most of hazards.
      - Cannot be grabbed and attacks objects that touch him.
  - When Yoshi has a specific kind of enemy in his mouth, he gains an extra ability:
    - Amp: Attract coins.
    - Lakitu, Mips: Speed boost.
    - Boo: Can move through walls, but he's still vulnerable to attacks.
    - Goomba: Can spin jump with the spin button.
    - Moneybag: Doubles the value of collected coins.
    - Bowling balls, Bullies: Landing is always treated as a ground pound.
- The Yoshi capture becomes available after fulfilling some requirements:
  - All other captures must be registered.
  - After registering 47 captures, Yoshi will spawn on the Castle roof, even if Mario doesn't have 120 stars. That Yoshi must be captured at least once.
- When unlocked:
  - Yoshi can be summoned in any stage by pressing <kbd>**L**</kbd> + <kbd>**Y**</kbd>.
  - Yoshi cannot be summoned during OMM Bowser fights, and cannot be used during Sparkly Stars (unless the corresponding mode is already completed).

## Sparkly Stars:

- During a Sparkly Star coins or red coins challenge, regular star cutscene don't play.
- As long as Sparkly Stars are enabled, Toads and Mips cannot trigger warps.
- Unless mode is completed, captures cannot be taken from a level to another.
- Unless mode is completed, captures other than flaming bob-ombs cannot enter Bowser 4 fights and cannot be used during the fight.
- Due to the evolution of the moveset and the addition of new captures, some Sparkly Stars have been reworked:
  - Pink-Gold Stars:
    - THI: Only Tiny Goomba captures are allowed.
    - RR: No capture allowed.
    - BITFS: Star has been moved.
    - VCUTM: No capture allowed.
  - Crystal Stars:
    - BOB: Star height greatly increased.
    - WF: Timer reduced from 1:40 to 1:20.
    - CCM: Star height greatly increased.
    - BBH: Timer reduced from 1:50 to 1:40.
    - LLL: Star height slightly increased.
    - SSL: Start time reduced from 15 seconds to 5 seconds, time per coin reduced from 1.5 second to 1 second, required coins increased from 62 to 70 coins, but captures are now allowed.
    - DDD: New Star challenge.
    - SL: Star height slightly increased.
    - WDW: New Star challenge.
    - THI: Star height slightly increased, only Huge Goomba captures are allowed.
    - RR: Timer reduced from 1:35 to 1:30, no capture allowed.
    - BITDW: Timer reduced from 1:40 to 1:15.
    - BITFS: Star has been moved.
    - BITS: Timer reduced from 1:50 to 1:40.
    - VCUTM: No capture allowed.
    - Castle Grounds: Star height greatly increased.
    - Castle Lobby: Number of Toads reduced from 2 to 1.
    - Castle Upstairs: Number of Toads reduced from 2 to 1.
    - Castle Courtyard: Star height greatly increased.

## New features:

- The game executable and its resources no longer contain ROM assets, these assets are extracted in-game.
- Dialog boxes can be skipped by pressing <kbd>**B**</kbd>. Skipping a Yes/No dialog box always select the first option (Yes).
- Automatic dialogs (Bowser intro, star milestones, level entry text) are always skipped if Non-Stop mode is enabled. Note that Sparkly Stars automatic dialogs are NOT skipped, but can be closed by pressing <kbd>**B**</kbd>.
- Added star display outside of the main menu, hold <kbd>**L**</kbd> when the game is paused and hold <kbd>**A**</kbd> to show coin scores.
- Added Free Rotation camera mode for the OMM camera.
- Added Snapshot mode in the `Camera` sub-menu to freely move the camera while the game is paused.
  - <kbd>**L-stick**</kbd>: Move horizontally.
  - <kbd>**R-stick**</kbd>: Rotate the camera.
  - <kbd>**A**</kbd>: Go up.
  - <kbd>**B**</kbd>: Go down.
  - <kbd>**D-pad**</kbd>: Change the roll.
  - <kbd>**Z**</kbd>/<kbd>**Start**</kbd>: Exit the Snapshot mode.
- Added built-in model loader with compatibility with sm64coopdx DynOS models.
- DynOS packs can be loaded from `.zip` archives.
- Added built-in Time Trials.
- `Warp to Level` can now only warps to available levels.
- Added `Warp to Castle`.
- On the main menu, Press <kbd>**Start**</kbd> on "Play" to resume the last save file directly without opening the file select screen.
- Added ability to launch another game from the options menu, if there is another exe in the same directory.
- Added a loading screen.

## Misc. changes:

- Palette editor reworked, only a mouse is needed to fully customize a character palette.
- Moved Sparkly Stars unlockable options in the sparkly stars sub-menu.
- "Fixed" Mirror room: All objects now have their reflection in the mirror, including particles and shadows.
- Added "Captures" and "Rewards" to the stats board, to keep track of the registered captures and the unlocked features.
- Fixed skyboxes, skybox textures from texture packs now work correctly.
- Fixed Improved Wing cap wing trail interpolation, trails now appear during the jumbo star cutscene.
- No longer locks Mario and the camera during the Chain Chomp cutscene in BOB.
