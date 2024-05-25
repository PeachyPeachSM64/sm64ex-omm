# Version 4.0.0 (09/12/2020)

## Changes and fixes:
- Cappy is now always summoned when pressing the Cappy Throw button, even if Mario's throw animation is interrupted.
- Added the ground Cappy jump, a higher version of the Cappy jump that Mario performs when touching Cappy on ground.
- Grabbing trees and poles resets the Cappy jump.
- Hold B to climb on trees and poles faster.
- Mario can now properly ledge-grab out of a group pound jump, a Cappy jump and a ground Cappy jump.
- Mario now rolls on a straight line on sticky terrain.
- Ground pounding against a wall no longer results in a bonk.
- Landing on slippery slopes, Ground pounds and spin pounds no longer trigger the fall damage animation.
- Added minimum velocity required to start rolling out of a long jump/air dive.
- If the player hold **(Z)** out of a long jump or an air dive, Mario will instantly roll as he touches the ground, preventing any fall damage.
- Holding **(Z)** while rolling makes Mario roll longer and further.
- Collecting a star fills the O2 meter and restores Mario's health (Non-Stop mode only).
- Fixed the Whomp Fortress music cutoff (Non-Stop mode only).
- There is a small chance after a death for a lost coin to turn into a moneybag.
- Increased Goomba stack max count to 10 Goombas.
- Added the Leave Ending Screen patch by GateGuy, and delayed the `Thank you so much` voice line to not overlap with the credits music.
- As `Exit level` is now a DynOS feature, selecting `Exit course` on the pause screen brings Mario back again to the center of the Castle.
- HUD:
  - Outside the Bowser fights, the Castle and when the game is not paused, the star counter is replaced by 7 star icons,
    representing the 6 consecutive acts of a course and the 100 coins star.
  - There are three versions of the star icon: empty, full, dark.
    - Empty: not collected
    - Full: collected, but physically present (as a transparent star)
    - Dark: collected

## Colored Stars:
- Added a new star model, that replaces the old one and the transparent one.
- Every course has now his own colored stars:
  - Bob-omb Battlefield: Green
  - Whomp Fortress: Silver
  - Jolly Roger Bay: Pink
  - Cool, Cool Mountain: Cyan
  - Big Boo's Haunt: Purple
  - Hazy Maze Cave: Black
  - Lethal Lava Land: Red
  - Shifting Sand Land: Lime
  - Dire Dire Docks: Blue
  - Snowman's Land: White
  - Wet-Dry World: Blue-gray
  - Tall, Tall Mountain: Brown
  - Tiny-Huge Island: Gold
  - Tick Tock Clock: Light Orange
  - Rainbow Ride: Fuschia
  - Secret Stars: Yellow
  
## Cappy Eyes:
- Cappy eyes are now compatible with the following models:
  - Super Mario 64 Mario
  - High Definition Mario (by Arredondo)
  - Super Mario Odyssey Mario (by D4RW1N)
  - Mario Kart 8 Mario (by D4RW1N)
  - Luigi from L is Singleplayer 2401 (by LuigiFan1)
  - Render96 Mario v1.3 (by the Render96 team)
  - Render96 Mario v1.4.2 (by the Render96 team)
  - Render96 Mario v2.0 (by the Render96 team)
  - Render96 Luigi v1.4.2 (by the Render96 team)
  - Render96 Luigi v2.0 (by the Render96 team)
  - Render96 Wario (by AndresL64, LuigiNoodle)
  - Super Smash Bros Brawl Mario (by Opmox)
  - Hat Kid (by Napstio)
  - Bow Kid (by Napstio)
  
## Cool, Cool Mountain Captures:
- It's now the turn of the Cool, Cool Mountain entities to be possessed by Mario!
- Small Penguin:
  - Move with the Left Stick.
  - Press **(A)** to jump.
  - Press **(B)** to dive and start sliding, hold to keep sliding and gain speed, and release to progressively slow down.<br>The more slippery a surface is, the faster the small penguin will be able to slide on it.
  - Can interact with Tuxie's mother (mother penguin).
- Tuxie's mother:
  - She can be captured only before rescuing her child.
  - Do not capture her child after she gave you the star.
- Spindrift:
  - Move with the Left Stick.
  - Press **(A)** to jump. Press **(A)** again in the air to do a propeller jump.<br>Hold **(A)** to fall slower.
  - Press **(B)** on ground or in the air to perform a spin attack.
- Mr. Blizzard:
  - Move with the Left Stick. Like Chain Chomp, he moves by doing little hops.
  - Hold **(A)** to jump every time he touches the ground.
  - Press **(B)** to throw a snowball. The snowball can destroy most of enemies and objects, and can collect coins.<br>Hold **(B)** to make the snowball bigger before releasing it. Big snowballs are able to take down other Mr. Blizzards.
- Snowman
  - Its head is capturable only before being reunited with its body.
  - Its body can be captured on its pedestal, at the top of the mountain.
  - Snowman's body can move with the Left Stick, but cannot jump.<br>It gets bigger and faster as it rolls and defeats most of enemies on contact, but breaks and ejects Mario if it takes damage or lands from too high.

## Bonus:
- Toad is now capturable, and does not eject Mario after he finishes talking:
  - Walk with the Left Stick, hold **(B)** to run, press **(A)** to jump, and interact with doors to open them.
  - And because Peach gave him the Castle master key, he can open Star doors without the need of Power Stars, but can't open the doors locked by Bowser.
- Speaking of Bowser, he learnt some moves from his Odyssey counterpart, and is now waiting for Mario and Cappy to face him in the Dark World arena.
