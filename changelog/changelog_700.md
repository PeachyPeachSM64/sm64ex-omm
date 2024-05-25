# Version 7.0.0 (14/01/2022)

## Bug fixes:
- Fixed a bug where the red coin radar didn't show the red coins in Bowser levels.<br>Additionally, the red coin radar now indicates the red coin star when all red coins have been collected.
- Fixed a bug where Bob-ombs could explode when trying to grab them with Cappy, Tiara or Perry.
- Fixed a bug in Breezing Beach (`SM74EE`) where Mario couldn't reach the fifth star if `Moveset` was set to `Odyssey`.

## Hazy Maze Cave Captures
- **Swoop**
  - Like Boo and Hoot, Swoop can jump with **(A)** multiple times in the air.
  - Hold **(B)** to move faster.
  - Hold **(A)** when hitting a ceiling to hang on it.
- **Snufit**
  - Press **(A)** to jump, **(B)** to shoot metal balls.
  - Hold **(B)** for at least 0.5 second to lock the Snufit in place and start to rapid-fire.
- **Monty Mole**
  - Once captured, can move anywhere on the ground.
  - Press **(B)** to throw a rock.
  - Monty Mole can't jump, but holding **(A)** allows it to hide in the ground.

## Lethal Lava Land Captures
- **Small Bully/Chill Bully**
  - Moves at a moderate pace, can open doors and can jump by pressing **(A)**.
  - But when pressing **(B)**, the Bully starts to accelerate to charge like Wario's shoulder bash.<br>In this state, the Bully becomes semi-invulnerable and attacks everything it touches, but can't jump.
- **Friendly Blargg** (Render96 only)
  - Similar to the Koopa shell, moves fast and even faster when holding **(B)**.
  - However, it can't jump, and can move only on lava.
- **Enemy Blargg** (Render96 only)
  - Can't jump and can move only on lava, but invincible.
  - Press **(B)** to throw a fireball that can burn almost anything.
- **Motos** (Render96 only)
  - Motos are rather slow and their jumps (**(A)**) are low, but nothing can hurt them (except lava) and they can open doors.
  - Near an object, press **(B)** to grab it. Once grabbed, Motos can move it with ease. Press **(B)** again to throw the held object.<br>More than 50 different objects can be grabbed and throw by a captured Motos!

## Super Mario Odyssey Animations
Credits to **Sharen**, who recreated the following animations in Super Mario 64:
- Cappy vault
- Cappy throw
- Cappy up-throw
- Cappy down-throw
- Cappy spin throw
- Rainbow spin
- Default death
- Drowning
- Frozen to death
- Star dance (Moon get)
These animations can be toggled on/off with the option `Super Mario Odyssey Animations`, in the `Extras` sub-menu of the OMM menu.

## Moveset Changes:
- Mario can't capture the same object twice in a row if he left it airborne, until he lands.
- Hold **(X)** to prevent Cappy from capturing enemies, even if `Cap` is set to `Cappy - Capture`.
- Added **(Z)** and **(Z)**+**(B)** cancels for the underwater Cappy throw.
- Bob-omb capture: The third explosion doesn't destroy the Bob-omb immediately. Instead, the Bob-omb explodes and Mario is ejected only when he touches the ground.

## Other Changes:
- `Return to Castle` works now for `SMMS`, `SM74` and `SMSR`.
- Sparkly Stars hints are shown when entering a level. Some levels don't show anything, and respawning or restarting the level doesn't display the message again.
- OMM textures are precached when starting the game if `precache` is set to true (External Data/Render96 only).
- OMM texts and dialogs are loaded during start-up.<br>*Notes for Render96*:
    - Languages are supported, but only the file `AM_us.omm.json` is generated automatically. For more languages, you must provide the files yourself.
    - If the game crashes at launch, make sure the selected language is English. To do that, open `sm64config.txt` and set the `language` value to `0`.
