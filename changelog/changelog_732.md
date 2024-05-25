# Version 7.3.2 (18/09/2022)

## *OMM Builder*:
- New game available: ***Super Mario 64: The Green Stars***.<br>Main changes compared to the original game:
  - The Grand Star after defeating Bowser 3 warps the player to the unused level with the 131st star.<br>*Notes for speedrunning:*
    - Like ***Super Mario Star Road***, *Any%* runs end when touching the Grand Star.
    - The 131st star is required only in *131 Star* runs.
  - *Course 9 - Sandy Seaside Bay* music has been replaced by ***Super Mario Star Road*** *Course 4 - Chuckya Harbor* music.
- The following custom resources formats are now supported: `.7z`, `.gz`, `.tar`, `.rar`.
- Updated the `Time Trials` patch to `v2.5` to be compatible with all available games and added it to the `custom` directory.
- Updated the `DynOS` patch to `v1.2` to be compatible with all available games.

## Bug fixes:
- Bob-omb freeze when thrown near a ledge.
- Koopa the Quick/Racing penguin path.
- Find floor on slanted walls resulting in out-of-bounds.
- Hitboxes being weird when scale is negative.
- Incorrect dialog font height.
- Cappy going through some enemies even when Capture is enabled.
- *Bad ending* custcene (Toads dialog).
- Moon Jump cheat with captures.

## Changes:
- Stand on a blue coin switch to display the number and location of hidden blue coins.
- Boxes that appear when pressing a purple switch are now indestructible.
- Eyerok's eyes (hands) can be hit by Cappy, Tiara and Peach attacks.
- Pause menu: Courses that have not been visited (0 star collected) are now displayed as `???`.
- Odyssey Moveset: Deadly quicksand invincibility frames are reduced from 2 seconds to 1 second.
- Koopa shells are no longer destroyed when Mario leaves them. Instead, they become intangible for 0.5 second after leaving them.
- Captured Bob-omb explosions can damage King Bob-omb.
- Pink-Gold Stars: JRB: Increased number of required coins from 80 to 101.
- Star colors now match better with the levels environment for all non-Vanilla games.
- ***Super Mario 64***: Power-up boxes that appear after collecting all 7 stars of a course have been removed.
  - Instead, the player unlocks the ability to wear any cap after collecting all stars of a course, **and that for almost all courses (not Castle and Bowser fights) and all games**.
  - To wear a cap, hold **(L)** and press a **D-pad** button:
    - **D-pad Up**: Wing cap
    - **D-pad Left**: Vanish cap
    - **D-pad Right**: Metal cap
    - **D-pad Down**: Remove all caps
  - Additionally, a new cheat has been added, `Cap Modifier`.<br>When enabled, give the player the ability to wear any cap anywhere, anytime.
- ***Super Mario 64 Moonshine***: Added coins to *Sweet Sweet Rush* to be able to collect the 100 coins moon in one go.
- ***Super Mario 74***: Select a save file with **(Start)** instead of **(A)** to swap directly to *Extreme Edition*.

