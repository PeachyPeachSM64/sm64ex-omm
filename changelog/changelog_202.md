# Version 2.0.2 (14/09/2020)

## Changes and fixes:
- Fixed a crash that happened when throwing Cappy 64 times in a row, without grabbing it or bouncing on it.
- Fixed a crash that happened when Cappy was out while Mario was teleporting through invisible warps.
- Removed Time Trials and Ghost Race from SMO, added it as a separate patch, Time Trials v2.0.
- Added SMO Options Menu as a separate `.txt` file, working with DynOS v0.2.<br>With DynOS patched, add the `.txt` file to your `res` folder.<br>Without DynOS patched, the default configuration used by the game is the following:
  - Mario's Odyssey moveset is enabled
  - Cappy throw is bound to the button 2 on a controller (corresponds to **(X)** button on XBox One controllers), or **[X]** key on keyboard
  - Cappy's behavior is set to `Capture`
  - Mario uses the 3-Health system from Odyssey
- Cappy now unloads if Mario has his cap in his hand or if he loses his cap (ukiki, big snowman, fwoosh).
