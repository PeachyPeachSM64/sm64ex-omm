# Version 3.0.0 (16/10/2020)

## Changes and fixes:
- Better ground/air movement
  - To mimic Odyssey's mechanics, Mario's face yaw (his forward direction) is more or less responsive to change depending on his forward velocity.<br>If he's standing still (horizontally), he can quickly change direction, grounded and airborne. But the faster he is, the harder he's to control.
  - Any Cappy throw will instantly change Mario's direction, regardless of his forward speed.
  - Furthermore, some actions set at their end Mario's direction to the joystick direction.
- Better wall-slide
  - Mario starts wall-sliding only if he's falling, so a wall slide will never interrupt a jump.
  - Mario progressively slides on the wall, instead of falling at a constant speed.
  - Mario cannot grab the same wall or another wall with a similar angle after a wall jump, until he touches the ground or water.
  - Mario bonks if he was long-jumping or air diving.
  - Mario's wall jump direction follows the reflection angle, but can be altered by the left stick while wall-jumping.
- Improved wall/floor/ceiling collision system
  - Wall collision:
    - Increased the search radius from 1 to 9 cells, and removed the radius limit.
    - Collisions are not checked anymore for PUs, as they are treated as out of bounds.
  - Ceiling collision:
    - Always return the lowest ceiling found, instead of the first one (no more ceiling-cucking).
    - Collisions are not checked anymore for PUs, as they are treated as out of bounds.
  - Floor collision:
    - Five points are checked instead of one for each floor, to prevent objects from going through small gaps.<br>This fix applies only for flat and pseudo-flat floors, to prevent Mario from walking mid-air while moving on steep floors (SSL pillars).
    - Always return the highest floor found, instead of the first one (no more floor-cucking).
    - Collisions are not checked anymore for PUs, as they are treated as out of bounds.
  - Water level and Gas level:
    - Changed < to <=, removing edge-case bugs (z = -58).
  - Mario Ground step:
    - Increased the number of sub-steps from 4 to 16.
    - Walls are more resistant to hyper-speed glitches (BLJ).
  - Mario Air step:
    - Increased the number of sub-steps from 4 to 16.
    - Fixed the glitchy ledge grab (no more WF cannon-less).
    - Prevents Mario from going through thin floors with enough speed.
    - Fixed a bug that prevented Mario from jumping from a raising platform.
  - Mario Water step:
    - Increased the number of sub-steps from 1 to 16.
    - Better steep floors and ceilings collision.
    - Mario can't move more than he should, removing all up/downwarps.
- Freezing effect in cold water
  - Freezing water no longer drains more O2, but progressively freezes Mario.
  - The freezing effect intensifies during 5 seconds, after that Mario takes damage every 5 seconds.
  - The freezing timer decreases slowly while airborne, but decreases quickly when Mario is on ground.

## Mario's Moveset:
- More Odyssey-style roll
  - Start rolling by pressing **(Z)** and **(B)**, then keep pressing **(B)** to gain speed. Press **(A)** to stop the roll and do a long-jump.<br>Mario goes full speed if he starts a roll after a ground pound land or a spin pound land.<br>Mario can slightly change his direction with the control stick while rolling.
  - Mario slowly decelerates on flat/pseudo-flat ground. For reference, the first bridge of BoB is the limit of what is a pseudo-flat ground (< 20°).<br>Mario rapidly loses speed and handling when moving uphill, but goes even faster when moving downhill on a steep slope or a slide.<br>He comes to a full stop and bonks when touching a wall.<br>The roll carries on airborne, but is harder to control, and continues when Mario comes back to the ground.
- Underwater ground pound jump
  - When Mario touches the floor underwater after a ground pound, immediately press **(A)** to do a high jump.
- Improved water dash
  - Mario can move in all directions, and keep dashing as long as **(B)** is held, but this move drains O2 really fast.
- Spin actions
  - Do a full circle with the left stick to start spinning.
  - While Mario is spinning, press **(A)** to do a spin jump. This jump is the highest one Mario can do, even higher than a triple jump.
  - Do a full circle while airborne to do an air spin. Mario stops falling for a few moment while performing an air spin.
  - Press **(Z)** to turn Mario's ground pound into a spin pound if he's spinning or twirling in the air.
  - Cappy spin throws are now triggered by spin + **(X)** instead of **(X)** + **(A)**.
- Animations
  - Ground pound jump: more accurate to Odyssey.
  - Roll: better rolling animation.
  - Jump kick: Mario will do a little spin instead of a jump kick if Cappy is not on his head.
  
## Super Metal Mario:
- Underwater Metal Mario's moveset has been greatly enhanced. Here is the list of the moves he can perform:<br>Idle, Walk, Crouch, Crawl, Jump, Double jump, Triple jump, Backflip, Sideflip, Long Jump, Freefall, Wall slide, Wall jump, Punch (all 3 variants), Jump kick, Dive, Ground pound, Ground pound jump, Cappy throws, Cappy jump, and all Spin actions.
- The Metal cap duration has been increased from 20 to 30 seconds.
- Metal Mario is now truly invincible, as he cannot die as long as he's wearing his metal cap.<br>Note that he's still vulnerable to instant death effects, such as quicksand or death barrier.
  
## Star Collection Mode:
- In main courses, Mario does not leave the course after collecting a star.<br>Instead, he does a victory dance while the star jingle is playing, and, similarly to Odyssey, a text box appears below him, showing the message *YOU GOT A STAR* with the name of the course and the star he got.
- The game auto-saves after collecting a star. Furthermore, it will ask the player if he wants to leave the course after he collects all the available stars (100 coins star included).
- As long as Star Collection Mode is active, the option `Exit course` always brings Mario back in front of the course painting instead of the center of the Castle. This works for every stage, not only the 15 main courses.
- Dying drops Mario at the beginning of the course again, instead of returning to the Castle.<br>The death sequences have been edited to look more Odyssey-like.<br>There are seven of them: regular, drowning, falling, frozen, eaten by Bubba, quicksanded, squished.
- When Mario dies, the course is completely reset and he loses all his coins, but (at most) 20 of them drop on the floor and can be collected again after his death. This makes a death less frustrating and punishing, specifically when trying to collect a 100 coins star. This works for every stage, not only the 15 main courses.
  
## Odyssey Options Menu:
- DynOS v0.4 is included with this patch. You don't need to patch it.
- Custom options
  - `Mario's Moveset` can be set to `Classic` or `Odyssey`.
  - `Cappy's Behavior` can be set to `Disabled`, `No Capture` or `Capture`
  - `Cappy Throw Button` allows you to bind your keys/buttons used to throw Cappy.
  - `Health System` can be set to `Classic` (SM64), `3-Health` (Odyssey normal mode) or `6-Health` (Odyssey assist mode).
  - `Star Collection` can be set to `Classic` or `Non-Stop`.
  - `Underwater Metal Mario` can be set to `Classic` or `Super`.
  
## Last but not least, Cappy:
- Cappy now has eyes, but only with SM64 Mario, HD Mario, Render96 Mario and Render96 Luigi. They show up with regular Mario, Vanish-cap Mario and Winged-cap Mario, but not Metal Mario.
