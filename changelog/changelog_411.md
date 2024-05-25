# Version 4.1.1 (28/12/2020)

## Changes and fixes:
- Fixed a bug when Mario could die after a level exit if he triggered a death warp at the same time.
- OMM assets are no longer built-in, they are now stored inside the `dynos/gfx` and `dynos/audio` folders.<br>These assets can be replaced by custom ones, as long as filenames and textures dimensions remain the same.<br>External Data is not needed.
  
## OMM Bowser fights:
- General:
  - The overall difficulty has been reduced (most notably OMM Bowser 3).
  - Possessed flaming bob-omb can now withstand any of Bowser's fire attacks once.<br>It leaves a burn smoke above it, to show the player it got hit.<br>The second time it is damaged, it explodes and ejects Mario, as usual.
- Attacks:
  - The rain of fire (OMM Bowser 3 fight) has been removed.
  - The homing fire ball has been replaced by a regular fire ball, but...<br>If Mario is on ground, Bowser will try to predict his movement.<br>If Mario is airborne, Bowser will shoot a faster fire ball directly at him.<br>The fire ball leaves a burning trail on the ground, but quickly disappears.
- Graphics:
  - Added unique graphics for each one of OMM Bowser's attacks.
  - Attacks have been optimized to reduce at most any lag or slowdown.
  - Delays have been reduced and animations have been shortened to reduce at most the waiting time between actions.
  - The collision is left untouched, only the visuals are modified.
