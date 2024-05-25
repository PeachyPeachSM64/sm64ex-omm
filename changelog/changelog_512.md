# Version 5.1.2 (02/05/2021)

## Bug fixes:
- Fixed a bug where the midair spin prevented Mario from doing a triple jump if it was performed after a double jump.
- Fixed a bug where Mario could move normally inside flooded JRB, as if the level was completely dry.
- Fixed a bug where Mario could break out of a capture by using cheats (sm64ex-alo only).
- Fixed a bug where Mario could fight Bowser 4 without the Crystal Stars mode enabled.
- Fixed a bug where Mario could interact with Cappy when he was not supposed to.
- Fixed a bug where some functionalities (Non-Stop mode, stage variants) were no longer available after watching the ending cutscene.
- Fixed a bug where the ending text (Peach speaking to Mario) was sometimes replaced by garbage data (sm64ex-nightly only).
- Fixed a bug where two 1up-mushrooms in BITFS didn't spawn during Non-Stop mode if Bowser 2 was not defeated.
- Fixed a bug where huge Goombas didn't stay stacked after leaving them with **(Z)**.
- Fixed a softlock triggered by pausing the game during the transition to the Bowser 4 arena.
- Fixed buttons conflicts with CHEATER's Time Stop.<br>If you want to use the top-right button (**(X)** on Switch Pro-cons, **(Y)** on XBox controllers or **(Triangle)** on PS controllers), make sure to clear the Time Stop buttons first (or assign them to another values).

## Changes:
- Huge Goombas are easier to stack.
- Mario can now perform a Midair spin after a Jump-kick.
- Steep jumps are now regular jumps on non-slippery steep surfaces (Odyssey Moveset only).
- Wario can now perform his shoulder bash and his triple jump underwater with the Metal cap.
- Render96 2.2 Wario's W doesn't go through Cappy eyes anymore.
- Each playable character has now its own power-up icons, with Cappy variants.
- Redesigned instant quicksands (Odyssey Moveset only):
  - They no longer insta-kill (pre v5.1.0) or burn (post v5.1.0) Mario.
  - Mario quickly sinks into quicksands, taking damage every 2 seconds.
  - If Mario's health reaches 0 or if the sand completely covers him, the quicksand death sequence is triggered.
  - If the Infinite Health cheat is enabled, Mario doesn't take any damage, but the sinking effect is still applied to him.
- Improved Metal Mario is completely immune to both of the effects.
- The anti-cheat is not triggered anymore after collecting all 30 Crystal Stars.
