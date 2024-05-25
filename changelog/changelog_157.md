# Version 1.5.7 (27/08/2020)

## Changes and fixes:
- Roll speed has been slightly reduced.
- Mario has more air control: he can turn progressively while airborne, and turns instantly when throwing Cappy.
- Fall height has been increased and Mario no longer takes fall damage, but he's unable to move for 1.5 second when landing from a high place.
- Grounded Mario's controls are more responsive (similar but not as extreme as `Super-responsive controls` cheat).
- Cappy returns to Mario after 4 seconds instead of vanishing instantly. Same when he hits an attackable enemy (bully, skeeter...).
- Mario can throw Cappy after a Cappy jump, but the latter is intangible for him (like in Odyssey).
- Mostly code refactoring (for compatibility purposes).

## New stuff:
- The 3-health system from Odyssey has been implemented.
  - Every damage Mario takes reduces his health points by 1.<br>Collecting 10 coins or touching a spinning heart recovers 1 health.<br>Burn damage is applied only on frame 1, but Mario still does his burning animation.
  - Mario can still die, but can't game-over, as this system provides him infinite lives.
  - Collecting a 1-up mushroom fully heals Mario and increases his max health points from 3 to 6.<br>But if he falls under 3 health, he can't get back to 6 health until he collects a 1-up mushroom again.
  - The life-up effect is carried through levels.
- The HUD has been updated to match with this system.
  - The lives counter is replaced by the star counter, and the coin counter is moved below the star one.<br>Underwater, the O2 level is shown below the coin counter.
  - On the top-right now stands the health gauge from Odyssey.
  - The timer (displayed during PSS or Koopa the Quick race) is moved to the bottom-right.
- The SMO options sub-menu now has three categories.
  - You can enable or disable SMO Mario's moveset anytime.<br>SMO Mario's moveset includes new moves, wall slide, air control, responsive ground control, fall height and fall damage changes.
  - You can choose Cappy's mode from `Disabled`, `No Capture` or `Capture`.<br>This way, Cappy (and captures) can be used even if SMO Mario's moveset is disabled.
  - You can enable or disable the SMO health system.<br>Enabling sets Mario's HP from 0 to 3, according to SM64 Mario's current health, even if it's full. Mario can only get to 6 HP if he collects a 1-up mushroom.<br>Disabling makes Mario lose his life-up effect.
- Last, but not least, 3 sound effects from Odyssey have been added.
  - The sound when Mario collects a Life-Up heart is played when collecting a 1-up mushroom.
  - The sound when Mario takes damage.
  - The sound when Mario regains 1 health point.
