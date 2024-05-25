# Frequently Asked Questions

## How do I build *Super Mario 64 Moonshine*?

To build ***Super Mario 64 Moonshine***, download the [Super Mario 64 Moonshine archive](https://www.mediafire.com/file/khy40tbd1rcve2p/MOONSHINE_FINALUPDATE.rar/file) and extract the mod archive content (`MOONSHINE_FINALUPDATE.rar`) into a `moonshine` directory.

## Can I use texture packs, models packs and other custom resources?

To build the game with custom resources (Patches, Texture packs, Sound packs, Model packs, Audio packs), put them into the `custom` directory.<br>
Upon running, *OMM Builder* will detect automatically the custom resources and their type.<br>Then, select them before building the game.

## The game is built successfully, but the only thing I get when launching it is a black screen...

You probably built the game with a renderer not supported by your computer.<br>
Delete your build and try again by changing the `Render API` option from `OpenGL 2.1` to `DirectX 11`.

## Where did my executable go?

- **Windows**: `repos/<game>/build/us_pc/sm64.us.f3dex2e.exe`
- **Linux**: `repos/<game>/build/us_pc/sm64.us.f3dex2e`

Once built, the game executable is located in the `build/us_pc` directory of the game you choose.<br>
If you want to copy or move it, make sure to copy/move the entire `build/us_pc` directory, not only the executable file.

## Why is the Texture pack I installed not visible in-game?

Make sure the Texture pack is in the `res` directory and is a valid `.zip` archive.<br>
If there is a `gfx.raw` file in the `res` directory, delete it, or change the in-game option `Texture Caching` from `Permanent` to `At Start-up`.

## Why is the Model pack I installed not visible in-game?

Make sure the Model pack is in the `res/packs` or `dynos/packs` directory and is a valid pack (directory of `.bin` files).<br>
In-game, open the `Model Packs` option menu and enable your model pack.

## Where is the save file located? How do I open/edit it?

- **Windows**: `%appdata%\sm64ex\omm_save_file.txt`
- **Linux**: `~/.local/share/sm64ex/omm_save_file.txt`

The save file is a plain text file, so any text editor can open it. It is divided in multiple sections:
- `[xxxx:yy]`: The main game save data. `xxxx` is the game name, `yy` is the save slot.
  - Flags: 0 = locked and 1 = unlocked.
  - Courses: the first 7 numbers are the stars (1 = collected, 0 = not), the next number is the cannon (1 = open, 0 = closed) and the last number is the coin score.
- `[sparkly_stars]`: The Sparkly Stars save data. It cannot be edited directly.
- `[mario_colors]`, `[peach_colors]`: The custom palettes for playable characters.
  - Each `mario_custom` (`peach_custom`) entry consists of 7 (8) pairs of RGB values in hexadecimal form + a palette name.
  - The first value of a pair is the ambient color, the second is the diffuse color.
  - For Mario, the colors are in the following order: *Cap*, *Hair*, *Skin*, *Gloves*, *Overalls*, *Shoes*, *Cappy Eyes*.
  - For Peach, the colors are in the following order: *Skin*, *Hair*, *Dress (light tone)*, *Dress (dark tone)*, *Gloves*, *Earrings*, *Shoes*, *Tiara Eyes*.
  - The palette name cannot have spaces, replace them by underscores.
- `[stats]`: Several in-game stats, such as number of objects collected, number of enemies defeated, distance travelled, etc...
- `[settings]`: The saved options. It includes controls, shortcuts and extras.

## What is the options menu?

A PC port exclusivity, the options menu allows the player to configure their game without relying on code modifications, patches or external tools.<br>
The options menu can be accessed by pausing the game, then pressing <kbd>RSHIFT</kbd> (keyboard) or <kbd>**R**</kbd> (controller).

See [Options](options.md) for the complete list of options.

## What are the default keyboard/controller controls?

| | Keyboard (qwerty) | Keyboard (azerty) | XBox One Controller | Switch Pro Controller |
|:-:|:-:|:-:|:-:|:-:|
| A Button | <kbd>L</kbd> | <kbd>L</kbd> | <kbd>**A**</kbd> | <kbd>**A**</kbd> |
| B Button | <kbd>,</kbd> | <kbd>;</kbd> | <kbd>**B**</kbd> | <kbd>**B**</kbd> |
| X Button | <kbd>K</kbd> | <kbd>K</kbd> | <kbd>**X**</kbd> | <kbd>**X**</kbd> |
| Y Button | <kbd>M</kbd> | <kbd>,</kbd> | <kbd>**Y**</kbd> | <kbd>**Y**</kbd> |
| Start Button | <kbd>SPACE</kbd> | <kbd>SPACE</kbd> | <kbd>**Start**</kbd> | <kbd>**+**</kbd> |
| Spin Button | <kbd>Mouse Wheel Button</kbd> | <kbd>Mouse Wheel Button</kbd> | <kbd>**LS-Button**</kbd> | <kbd>**LS-Button**</kbd> |
| L Trigger | <kbd>LSHIFT</kbd> | <kbd>LSHIFT</kbd> | <kbd>**LB**</kbd> | <kbd>**L**</kbd> |
| R Trigger | <kbd>RSHIFT</kbd> | <kbd>RSHIFT</kbd> | <kbd>**RB**</kbd> | <kbd>**R**</kbd> |
| Z Trigger | <kbd>O</kbd> | <kbd>O</kbd> | <kbd>**RT**</kbd> | <kbd>**ZR**</kbd> |
| C-Up | <kbd>&uarr;</kbd> | <kbd>&uarr;</kbd> | <kbd>**RS-Up**</kbd> | <kbd>**RS-Up**</kbd> |
| C-Down | <kbd>&darr;</kbd> | <kbd>&darr;</kbd> | <kbd>**RS-Down**</kbd> | <kbd>**RS-Down**</kbd> |
| C-Left | <kbd>&larr;</kbd> | <kbd>&larr;</kbd> | <kbd>**RS-Left**</kbd> | <kbd>**RS-Left**</kbd> |
| C-Right | <kbd>&rarr;</kbd> | <kbd>&rarr;</kbd> | <kbd>**RS-Right**</kbd> | <kbd>**RS-Right**</kbd> |
| D-Up | <kbd>-</kbd> | <kbd>)</kbd> | <kbd>**D-Up**</kbd> | <kbd>**D-Up**</kbd> |
| D-Down | <kbd>{</kbd> | <kbd>^</kbd> | <kbd>**D-Down**</kbd> | <kbd>**D-Down**</kbd> |
| D-Left | <kbd>P</kbd> | <kbd>P</kbd> | <kbd>**D-Left**</kbd> | <kbd>**D-Left**</kbd> |
| D-Right | <kbd>}</kbd> | <kbd>$</kbd> | <kbd>**D-Right**</kbd> | <kbd>**D-Right**</kbd> |
| Stick Up | <kbd>W</kbd> | <kbd>Z</kbd> | <kbd>**LS-Up**</kbd> | <kbd>**LS-Up**</kbd> |
| Stick Down | <kbd>S</kbd> | <kbd>S</kbd> | <kbd>**LS-Down**</kbd> | <kbd>**LS-Down**</kbd> |
| Stick Left | <kbd>A</kbd> | <kbd>Q</kbd> | <kbd>**LS-Left**</kbd> | <kbd>**LS-Left**</kbd> |
| Stick Right | <kbd>D</kbd> | <kbd>D</kbd> | <kbd>**LS-Right**</kbd> | <kbd>**LS-Right**</kbd> |

## How do I update *OMM Builder*/*Odyssey Mario's Moveset*?

If it detects a new version, the builder will ask you if you want to update it when you launch it.<br>
As for the *Odyssey Mario's Moveset* mod, the files are automatically updated when building a game.

## What about speedrunning? Is it compatible with LiveSplit?

[Speedrunning leaderboard](https://www.speedrun.com/omm)

For speedrunners, *Odyssey Mario's Moveset* comes with a LiveSplit auto-splitter.<br>
It works with every game that can be built with *Odyssey Mario's Moveset*.

An auto-splitter is a script, a small program, that runs through LiveSplit and does the splits automatically, so the player doesn't need to press a key or a button to trigger each one.<br>
What it does in *Super Mario 64* is fairly simple:
- Starts the timer when the player selects a file,
- Splits each time a specific amount of stars is collected,
- Stops when the Grand Star is collected at the end of a run, or when the last split is triggered,
- Resets when the player returns to the main menu.

To install it and make it work, follow these steps:
- Your *Odyssey Mario's Moveset* version must be **6.1.0** or later.
- Open LiveSplit, and load your splits. Make sure they follow this format:
  - For star splits, you must indicate the amount of stars needed in brackets:
    - Use square brackets to split upon star collection: `[20]`.
    - Use parentheses to delay the split until the level exit: `(20)`.
  - If a split has `Bowser` in its name but no star amount, it is interpreted as a Bowser key split or a Grand Star split and the split is triggered on key/star collection.
  - If the final split has no star amount, it is interpreted as a Grand Star split (i.e. after defeating the last Bowser) and immediately stops the timer.
- Right click, select `Save Splits As...` and save them as a `splits.lss` file next to the `sm64.us.f3dex2e.exe` executable.
- Right click again, and select `Edit Layout...`.
- Add a `Scriptable Auto Splitter`, double-click on it, and select `Browse...`.
- Navigate to the *OMM Builder* directory and select the file `omm.asl`.
- Press `OK` twice, then start the game. The timer should start as soon as an **empty** save file is selected.
