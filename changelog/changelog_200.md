# Version 2.0.0 (08/09/2020)

## Changes and fixes:
- Mario's moveset
  - Fixed a wall slide glitch and the animation.
  - The moves Water plunge and Water dash now ignore water currents, but drains more O2.
  - While flying with the Wing cap, press **(X)** + **(A)** to make Cappy fly around Mario.
  - All the basic cheats are usable + `Super Responsive` works also airborne if SMO Mario is enabled.
- Captures
  - Increased Possessed-Goomba's hitbox height.<br>Now each Goomba inside the stack has its feet on the previous Goomba's head, instead of clipping through it.
  - Reduced the number of Possessed-Bobomb explosions to 3, but increased the explosion radius.<br>After each explosion, the bobomb bloats up and the third one destroys it.
  - Small boxes are now destructible by strong attacks, such as Possessed-Bobomb explosion and Possessed-Chomp bite.
  - Possessed-Koopa stays in his shell when **(B)** is held (even if he's jumping or not moving).
  - Mario is ejected from his possession if he's squished and doesn't take additional damage, but he still takes damage from a regular squish like the tox boxes' one in SSL.
- System
  - SMO Health options are now `Disabled`, `3-Health` (Odyssey default), `6-Health` (Odyssey Assist mode)
  - `SMO Mario`, `Cappy Capture` and `SMO 3-Health` are enabled by default
  - The SMO sound effects should play correctly in `External Resources` mode if they are included in a resource pack.<br>To insert SMO sounds in your `texture/sound pack`, patch the `00.json` and `00_sound_player.s` files.<br>Copy the sound folder inside your sm64 repo folder, and hit compile.<br>When done, grabs the `bank_sets`, `sequences.bin`, `sound_data.ctl` and `sound_data.tbl` files generated and add them in a `sound` folder to your pack.
- Patches
  - Included the `Exit to main menu` code (credits to Adya for the original patch).<br>To access it, pause the game, press **(R)** and select `Go Back to Main Menu`.
  - Included the `Return to Title after Ending` code (credits to GateGuy for the original patch).<br>After the Cake ending screen, press **(A)** to return to the main menu.
    
## New stuff:
- Time Trials
  - Included an enhanced version of the `Time Trials` code (credits to GateGuy for the original patch).<br>Time Trials doesn't require `Text Saves` anymore. Times are now saved on a separate `.txt` file inside your save folder.
  - The best recorded times for each course can be seen when opening the pause menu inside the Castle walls.<br>In the act selection menu, the best time is displayed above the selected star.
  - Additionally, the HUD shows a flashing *HI SCORE* if the player sets a new record when collecting a star or a key.
- Ghost Race
  - When establishing a new record, the path Mario took from the level entry to the star is saved as Ghost data.<br>Selecting this star on the act selection menu again will make Cosmic Mario spawn, allowing the player to `race` against his best time.
  - A ghost can be saved for every act of every course of the game, including the 3 Bowser fights, for a total of 118 ghosts per save file.
- Options
  - Time Trials has its own sub-menu, located below `Super Mario Odyssey`.
    - `Time Trials` can be enabled or disabled.
    - `Time Trial Ghost` can be set to `Show` or `Hide`.
  - As some stars are not selectable via the act selection menu, the following options specifies the Ghost to load when entering a level:
    - `Main Courses Ghost` can be set to `Selected Act Star` or `100 Coins Star`.
    - `Bowser Courses Ghost` can be set to `Bowser Fight` or `Red Coins Star`.
    - `PSS Ghost` can be set to `Star 1` or `Star 2`.
  - By default, the TT settings are `Enabled`, `Show`, `Selected Act Star`, `Bowser Fight` and `Star 1`.
  
## Whomp Fortress Captures:
These include Bullet Bill, Hoot, Whomp and Whomp King.
- Bullet Bill
  - The player enters the wing-cap flying Mario camera mode, and controls the Bullet with the left stick.
  - The Bullet Bill always moves at a constant speed, and nothing can slow it down. Hold **(B)** to go faster.
  - But colliding with a wall or a ceiling make the Bullet Bill explode, and Mario is ejected from it.<br>If **(B)** was held, it produces a powerful blast that can destroy enemies and some structures (the wall behind the Tower, the kickable board and both of the cannon walls).
  - Unlike Odyssey, the Bullet Bill doesn't self-destruct after a specific amount of time, allowing the player to freely move around the map.
- Hoot
  - This lil' bird is controllable like a regular enemy, but has infinite jumps and floats in the air.
  - Keep pressing **(A)** to flap Hoot's wings, making him go up.
  - Release **(A)** to float and slowly regain the ground.
- Whomp
  - Whomp is not really fast, and does not jump really high with **(A)**, but he's strong.
  - Press **(B)** to start a Body slam. When touching the ground, it emits a powerful shockwave that can eliminate most of the enemies.<br>The initial impact can even destroy some walls and boxes, and other Whomps, if you land on their back.
  - His back and the water seem to be his only weaknesses.
- Whomp King
  - First you try to step on him, now you want to capture it. It makes him so mad.
