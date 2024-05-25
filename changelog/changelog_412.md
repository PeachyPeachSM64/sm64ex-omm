# Version 4.1.2 (10/01/2021)

## Changes and fixes:
- Fixed a crash that happens when collecting a Castle star after completing a course (7 stars).
- Sideflips are easier to perform.
- Mario can now grab stars while possessing an enemy.
  
## Better collision v2:
- Out of Bounds and Ceiling collisions don't make Mario bonk, but prevent him from moving horizontally.
- Ceilings no longer extend infinitely upwards, but up to half Mario's height (80) (partial fix to exposed ceilings).
- Recovering from a bonk/knockback is faster.
- Redesigned air knockback to be more Odyssey-like.
- Jump kicks no longer push Mario out of a wall.
- Jump kicks can now extend into ledge grabs.

## Speedrun:
Added a LiveSplit Auto-Splitter script: `omm.4.1.2.asl`.<br>If you don't use LiveSplit or prefer manual splits, ignore this.<br>If you don't know how yo use LiveSplit scripts, please read the LiveSplit documentation.
- Features:
  - This script does all the splits for you on OMM runs.
  - Starts when a file is selected.
  - Splits after catching some stars (see below).
  - Stops when the Grand Star is collected.
  - Resets when the `Return to Main Menu` option is selected, or after the cake ending scene.
- Instructions:
  - Open `omm.4.1.2.asl` in a text editor.
  - Edit the `vars.starSplits` array to match your splits (each number is a star count).
  - For instance, if your splits are: BOB (5), WF (8), JRB (12)<br>Write: `vars.starSplits = new byte[] { 5, 8, 12 };`
  - For 70 star runs, the array must end with 70
  - For 120 star runs, the array must end with 120
  - Make sure your exe is named `sm64.us.f3dex2e.exe`
