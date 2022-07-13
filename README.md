# Odyssey Mario's Moveset
**Mod exclusively for the *Super Mario 64 PC Port*, developed by PeachyPeach.**

-----

## Build the game with *OMM Builder*

*OMM Builder* is a convenient Python script that makes the building process of the *Odyssey Mario's Moveset* mod simplier, while allowing some customization.<br>If you have never built the PC port of *Super Mario 64* before, please take a look at this [page](https://github.com/sm64pc/sm64ex/wiki/Compiling-on-Windows) or this [page](https://github.com/sm64pc/sm64ex/wiki/Compiling-on-Linux) first.<br>Make sure you have the following dependencies before starting: `MSYS2` (Windows only), `git`, `make`, `gcc`, `glew`, `SDL2` and `python3`.

```diff
- IMPORTANT NOTICE:
- Cloning the `sm64pc-omm` repository from PeachyPeachSM64's github is the only way to get the builder.
- Never download or execute the file `omm_builder.py` from any other source, even trusted ones, as it's
- likely to contain malicious code that can irremediably harm your computer or steal your personal data.
```

---

### Installing the builder:

- **Windows**:

  - Open an MSYS2 terminal. To do so, open the **Windows Start** menu, type `mingw` and select `MSYS2 MinGW 64-bit` or `MSYS2 MinGW 32-bit`, depending of your operating system and processor type. To see it, open the **Windows Settings**, go to **System**, and select **About**.

  - The first time, install the builder by cloning the *master* branch with this command:<br>`git clone https://github.com/PeachyPeachSM64/sm64pc-omm.git -b master`

  - Move to the created directory and open it in Windows Explorer by executing the following command:<br>`cd sm64pc-omm; explorer .`

  - If it's not already done, in Windows Explorer, click on the **View** tab, then check **File name extensions**. This option will allow you to rename files properly.

  - Place your copy of the **Super Mario 64 US ROM** inside it, next to the Python script `omm_builder.py`, and rename it `baserom.us.z64`.

- **Linux**:

  - Open a terminal.

  - The first time, install the builder by cloning the *master* branch with this command:<br>`git clone https://github.com/PeachyPeachSM64/sm64pc-omm.git -b master`

  - Move to the created directory by executing the following command:<br>`cd sm64pc-omm`

  - Place your copy of the **Super Mario 64 US ROM** inside it, next to the Python script `omm_builder.py`, and rename it `baserom.us.z64`.

---

### Building the game:

*OMM Builder* is a command-line tool with four main commands:

||||
|:-|:-|:-|
| Build | Compile the game with the version specified. | `python3 omm_builder.py [version] [build_speed] [args...]` |
| Run | Run the game with the version specified. An executable of the specified version must be compiled first. | `python3 omm_builder.py [version] run` |
| Clear | Clear the build directory of the version specified. | `python3 omm_builder.py [version] clear` |
| Reset | Reset the version directory without deleting it, but remove the compiled game. | `python3 omm_builder.py [version] reset` |
| Delete | Delete the version directory, including all its content. | `python3 omm_builder.py [version] delete` |

The `[version]` parameter must be one of the following:

|||
|:-|:-|
| `smex` | Super Mario 64 ex-nightly |
| `smms` | Super Mario 64 Moonshine |
| `xalo` | Super Mario 64 ex-alo |
| `sm74` | Super Mario 74 |
| `smsr` | Super Mario Star Road |
| `r96a` | Render96 ex-alpha |

The `[build_speed]` parameter must be one of the following:

|||
|:-|:-|
| `slow` | Build the game by compiling files one by one. |
| `fast` | Enable multi-processing to build the game faster. |
| `faster` | Build the game even faster (not recommended for weak PCs). |
| `fastest` | Use 100% of CPU to build the game as fast as possible. |

The `[args...]` parameters can be any of the following:

|||
|:-|:-|
| `DYNOS` | Download and install the latest version of DynOS. |
| `PATCHES` | Apply patches from the 'custom/patches' directory. |
| `DIRECT_X` | Replace SDL/OpenGL APIs by DirectX APIs. |
| `AUTO_RUN` | Start the game after building. |

For instance, if you want to build and play *Star Road* with 60 FPS and External data, run the command:<br>`python3 omm_builder.py smsr faster DIRECT_X AUTO_RUN`<br>Additionally, the builder is case-insensitive, meaning you can also write the previous command as:<br>`python3 omm_builder.py smsr faster directx autorun`

---

### Requirements

- To build *Super Mario 64 Moonshine*, you must extract the mod archive (`MOONSHINE_FINALUPDATE.rar`) into a directory named `moonshine`.

### Custom patches

- To build the game with custom patches, place a copy of your `.patch` files inside the `custom/patches` directory and run the command with the `PATCHES` option.
- Remember that not all patches or combination of patches are supported.

### Texture and sound packs

- Customize your game's textures and sounds by placing your packs `.zip` archives inside the `custom/res` directory.
- Texture packs must be `.zip` archives with a `gfx` directory inside them.
- Sound packs must be `.zip` archives with a `sound` directory inside them.

### Model packs (DynOS and *Render96* only)

- To be able to swap actors models in-game, copy your model packs directories inside the `custom/dynos/packs` directory and run the command with the `DYNOS` option.
- Model packs must be either directories of `.bin` files or filled with actors sub-directories, each sub-directory containing at least one `model.inc.c` and one `geo.inc.c` file as well as textures `.png` files.

### Musics, jingles and sounds (*Render96* only)

- Make your game more unique with custom musics, jingles and sounds! Put your audio data inside a `custom/dynos/audio` directory before running the command.
- *Render96* audio packs must be directories of sub-directories and `.txt` files, with each `.txt` file corresponding to one sub-directory.
- The sub-directories must be: `jingles`, `levels`, `sfx_mario`, `sfx_mario_peach`, `sfx_luigi`, `sfx_luigi_peach`, `sfx_wario`, `sfx_wario_peach`.
- The associated `.txt` files are: `jingle.txt`, `music.txt`, `sfx_mario.txt`, `sfx_mario_peach.txt`, `sfx_luigi.txt`, `sfx_luigi_peach.txt`, `sfx_wario.txt`, `sfx_wario_peach.txt`.

---

## Build the game with *sm64pcBuilder2*

Remember that this tool's only purpose is to build the *Odyssey Mario's Moveset* mod with the most common and useful features. It's not supposed (and will never) be as complete as **sm64pcBuilder2**.

If you want more customization or simply build the game the usual way, you can download the patch file of the latest version of OMM available in the section **Useful resources** below and add it as a custom patch in **sm64pcBuilder2**.

---

## Q&A

### The builder says `ERROR: The file 'baserom.us.z64' is missing.`, but it is here!

Your file is actually named `baserom.us.z64.z64`. It happened because Windows hides file extensions by default, and adds its extension back after renaming a file.<br>To prevent this, in Windows Explorer, click on the **View** tab and check **File name extensions**. Then remove the extra `.z64` from your file name.

### The builder is spitting out errors like `gcc: No such file or directory`. What does it mean?

You're missing dependencies. You need the following ones to properly build the game: `MSYS2` (Windows only), `git`, `make`, `gcc`, `glew`, `SDL2` and `python3`.<br>Go to this [page](https://github.com/sm64pc/sm64ex/wiki/Compiling-on-Windows) (Windows) or this [page](https://github.com/sm64pc/sm64ex/wiki/Compiling-on-Linux) (Linux) and make sure to follow the **Install dependencies** part.

### The game is built successfully, but the only thing I get when launching it is a black screen with the game over sound...

You probably built the game with a renderer not supported by your computer.<br>Delete your build and try again with the flag `DIRECT_X` to change the SDL/OpenGL API by the DirectX one.

### Where did my executable go? Do I have to always use the `run` command to play the game?

Once built, the game executable is located in the `build/us_pc` directory of the version you choose.<br>You don't need to use the `run` command every time, you can simply double-click on `sm64.us.f3dex2e.exe` like any other executable to start the game.<br>If you want to copy or move it, make sure to copy/move the entire `build/us_pc` directory, not only the executable file.

### Where is the save file located? How do I open/edit it?

Windows: `%appdata%\sm64ex\omm_save_file.txt`<br>Linux: `~/.local/share/sm64ex/omm_save_file.txt`<br>MacOS: `~/Library/Application Support/sm64ex/omm_save_file.txt`<br><br>The save file is a plain text file, so any text editor can open it. It is divided in multiple sections:
- `[xxxx:yy]`: The main game save data. `xxxx` is the version name, `yy` is the save slot.
  - Flags: 0 = locked and 1 = unlocked.
  - Courses: the first 7 numbers are the stars (1 = collected, 0 = not), the next number is the cannon (1 = open, 0 = closed) and the last number is the coin score.
- `[sparkly_stars]`: The Sparkly Stars save data. It cannot be edited directly.
- `[mario_colors]`, `[peach_colors]`: The custom palettes for playable characters.
  - Each `mario_custom` (`peach_custom`) entry consists of 6 (7) pairs of RGB values in hexadecimal form.
  - The first value of a pair is the ambient color, the second is the diffuse color.
  - For Mario, the colors are in the following order: *Cap*, *Hair*, *Skin*, *Gloves*, *Overalls*, *Shoes*.
  - For Peach, the colors are in the following order: *Skin*, *Hair*, *Dress (light tone)*, *Dress (dark tone)*, *Gloves*, *Earrings*, *Shoes*.
- `[settings]`: The saved options. It includes controls, shortcuts and extras.

### What is the options menu?

A PC port exclusivity, the options menu allows the player to configure their game without relying on code modifications, patches or external tools.<br>The options menu can be accessed by pausing the game, then pressing <kbd>RSHIFT</kbd> (keyboard) or <kbd>**R**</kbd> (controller).
- `Odyssey Mario's Moveset`:
  - `Character`: Mario, Peach (if unlocked), Luigi or Wario (Render96 only).
  - `Moveset`: Classic, Odyssey (3-Health), Odyssey (6-Health).
  - `Cap`: Classic, Cappy (no Capture), Cappy (with Captures).
  - `Stars`: Classic, Non-Stop.
  - `Power-ups`: Classic, Improved.
  - `Camera`: Classic, 8-Dir, 16-Dir.
  - `Sparkly Stars`:
    - `Mode`: Disabled, Normal, Hard.
    - `Assist`: If enabled, help the player in various ways.
    - `Show Hint at Level Entry`: If enabled, show the hint sign when entering a level if there's one.
  - `Cheats`:
    - `Unlimited Cappy Bounces`: Mario can bounce infinitely on Cappy.
    - `Cappy Stays Forever`: Cappy no longer returns to Mario after some time.
    - `Homing Attack Global Range`: Extend Cappy's homing attack range to the whole level.
    - `Mario Teleports to Cappy`: Press **(X)** again after throwing Cappy to teleport Mario to him.
    - `Cappy Can Collect Stars`: Cappy can interact and give stars and keys to Mario.
    - `Play As Cappy`: Throw Cappy to control him. Hold **(A)** to go up, **(B)** to go down and press **(X)** to return to Mario.
    - `Peach Endless Vibe Gauge`: Peach only. Vibes no longer depletes the Vibe gauge.
  - `Extras`:
    - `Mario Colors`: Select a color palette for Mario. The last 4 are customizable.
    - `Peach Colors`: Select a color palette for Peach. The last 4 are customizable.
    - `SMO Animations`: If enabled, replace some of the vanilla animations with animations from *Super Mario Odyssey*.
    - `Cappy and Tiara`: If enabled, replace Mario's cap by Cappy and Peach's crown by Tiara.
    - `Colored Stars`: If enabled, give each level stars a specific color, like moons in *Super Mario Odyssey*.
    - `Vanishing HUD`: If enabled, make the HUD progressively vanish when Mario is moving.
    - `Reveal Secrets`: If enabled, make the secrets visible.
    - `Red Coins Radar`: If enabled, locate and show the direction of the nearest red coin.
    - `Show Star Number`: If enabled, show for each star its number.
    - `Invisible Mode`: If enabled, make Mario, Cappy, Peach, Tiara and their effects invisible.
  - `Shortcuts`: Allow the player to bind up to three keys/buttons for a bunch of options.
- `Game` (Render96 only):
  - `Current Language`: Set the current language.<br>**Only English is naturally supported by OMM.** To support more languages, you must provide the language files `*.omm.json` yourself.
  - `Precache Textures`: Cache all textures when starting the game to avoid in-game freezes due to texture loading.
  - `Disable Billboards`: Don't force objects to face the camera. Must be enabled if the *Render96 Model Pack* is installed.
- `Camera`:
  - `Free Camera`/`Puppycam 2`: A modern game camera designed to be more comfortable.
  - `Analogue Camera`: Turn on or off the analogue camera.
  - `Mouse Look`: Allow the player to move the camera with their mouse.
  - `Invert X Axis`: Invert the horizontal rotation of the camera.
  - `Invert Y Axis`: Invert the vertical rotation of the camera.
  - `Camera X Sensitivity`: Control the horizontal movement speed of the camera.
  - `Camera Y Sensitivity`: Control the vertical movement speed of the camera.
  - `Camera Deceleration`: Control the movement deceleration of the camera.
- `Controls`:
  - Binds: Allow the player to bind up to three keys/buttons for each N64 button.
  - `Stick Deadzone`: The left-stick deadzone.
  - `Reset Controls`: Reset the controls back to default.
- `Display`:
  - `FPS`: Set the framerate to a value from 30 to 360. Does not affect game updates, they still run 30 times per second.
  - `Fullscreen`: Enable fullscreen mode.
  - `Vertical Sync`: Synchronize the framerate with the monitor's refresh rate. Enable it if you're experiencing tearing.
  - `Texture Filtering`: Set to nearest neighbor for blocky effects, or linear for smooth but blurry effects.
  - `Draw Distance`: Control the rendering distance of objects. Reduce it if you're experiencing slowdowns.
  - `HUD`: Show or hide the HUD.
- `Sound`:
  - `Master Volume`: The general volume setting.
  - `Music Volume`: The volume of background music.
  - `SFX Volume`: The volume of sound effects.
  - `ENV Volume`: The volume of environment sounds (birds chirping, water flowing, etc...).
- `Cheats`: A list of built-in modifications that gives Mario super-human powers, such as Moon-jumping, Infinite health, the ability to BLJ anywhere and much more.

### What are the default keyboard/controller controls?

| | Keyboard (qwerty) | Keyboard (azerty) | XBox One Controller | Switch Pro Controller |
|:-:|:-:|:-:|:-:|:-:|
| A Button | <kbd>L</kbd> | <kbd>L</kbd> | <kbd>**A**</kbd> | <kbd>**A**</kbd> |
| B Button | <kbd>,</kbd> | <kbd>;</kbd> | <kbd>**B**</kbd> | <kbd>**B**</kbd> |
| X Button | <kbd>K</kbd> | <kbd>K</kbd> | <kbd>**X**</kbd> | <kbd>**X**</kbd> |
| Y Button | <kbd>M</kbd> | <kbd>,</kbd> | <kbd>**Y**</kbd> | <kbd>**Y**</kbd> |
| Start Button | <kbd>SPACE</kbd> | <kbd>SPACE</kbd> | <kbd>**Start**</kbd> | <kbd>**+**</kbd> |
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

### How do I update *OMM Builder*/*Odyssey Mario's Moveset*?

If it detects a new version, the builder will ask you if you want to update it when you launch it.<br>As for the *Odyssey Mario's Moveset* mod, the files are automatically updated when running a Build command.

---

## Speedrunning

For speedrunners, OMM comes with a LiveSplit auto-splitter.<br>It works with every game that can be built with OMM.

An auto-splitter is a script, a small program, that runs through LiveSplit and does the splits automatically, so the player doesn't need to press a key or a button to trigger each one.<br>What it does in *Super Mario 64* is fairly simple:
- Starts the timer when the player selects a file,
- Splits each time a specific amount of stars is collected,
- Stops when the Grand Star is collected at the end of a run, or when the last split is triggered,
- Resets when the player returns to the main menu.

To install it and make it work, follow these steps:
- Your OMM version must be 6.1.0 or later.
- Open LiveSplit, and load your splits. Make sure they follow this format:
  - For star splits, you must indicate the amount of stars needed in brackets or parentheses like this: "\[20\]" or "(20)".
  - If a split has "Bowser" in its name but no star amount, it will be interpreted as a Bowser key split or a Grand Star split.
  - If the final split has no star amount, it is interpreted as a Grand Star split (i.e. after defeating the last Bowser).
- Right click, select `Save Splits As...` and save them as a `splits.lss` file next to the `sm64.us.f3dex2e.exe` executable.
- Right click again, and select `Edit Layout...`.
- Add a `Scriptable Auto Splitter`, double-click on it, and select `Browse...`.
- Navigate to the OMM builder directory and select the file `omm.asl`.
- Press `OK` twice, then start the game. The timer should start as soon as an **empty** save file is selected.

---

## Odyssey Mario's Moveset content

- [Patch file](https://raw.githubusercontent.com/PeachyPeachSM64/sm64pc-omm/master/patch/omm.patch)
- [Coop LUA mod: OMM Moves](https://github.com/PeachyPeachSM64/sm64pc-omm/raw/master/coop/omm-moves.zip)
- [Coop LUA mod: OMM Cappy](https://github.com/PeachyPeachSM64/sm64pc-omm/raw/master/coop/omm-cappy.zip)
- [In-depth guide](https://docs.google.com/document/d/1IlhCxYGulxrnbvqbSuBMC1JgtBIEwoCcK3l-urVUADk/edit) (by **Cancel the Eat**)
- [Sparkly Stars guide](https://www.youtube.com/watch?v=xWHKPV-cbqI&list=PLFZ-DGZKGuUo3KuXfGoaP55RYiDXgxE8N) (by **Cancel the Eat**)
- [Pink Gold Stars guide](https://www.youtube.com/watch?v=sPhx7hDPLKs) (by **FastMario**)
- [Speedrun leaderboard](https://www.speedrun.com/omm) (by **JokerFactor**, **SwaGh**, **Mr.Needlemouse**)

---

## Useful resources

- [sm64pcBuilder2](https://sm64pc.info/sm64pcbuilder2/) (by **gunvalk**, **Filipianosol**, **GammaTendonNine**)
- [Super Mario 64 Moonshine archive](https://www.mediafire.com/file/khy40tbd1rcve2p/MOONSHINE_FINALUPDATE.rar/file) (by **TurnFlashed**, **s4ys**, **Fito**)
- [Super Mario Star Road HD Texture pack](https://github.com/aspieweeb759/Star-Road-HD) (by **sarah_taylor**)
- [Custom patches](https://sm64pc.info/downloads/patches/)
- [Model packs](https://sm64pc.info/downloads/model_pack/)
- [Texture packs](https://sm64pc.info/downloads/texture_pack/)
- [Sound packs](https://sm64pc.info/downloads/sound_pack/)

-----

## Version 7.1.0 (11/04/2022)

### OMM Builder:
- Fixed DynOS compilation on Linux-based systems.
- `r96a` now uses the repository `Render96ex-tester_rt64alpha`.<br>Note 1: You don't need an RTX card to build this branch.<br>Note 2: The new audio system in the `tester` branch is not supported yet.
- Removed options: `60_FPS` and `EXT_DATA`.
- Removed `rt64` and `saex` from compatible versions list.

### Bug fixes:
- Fixed a slowdown with Peach's model.
- Fixed a bug where Mario could long-jump out of a ground Cappy throw, even if he wasn't moving.
- Fixed the weird underwater Cappy throw momentum.
- Fixed a bug where cold water couldn't damage Mario when both Classic moveset and 1 HP mode were enabled.
- Fixed a bug where collecting coins underwater could kill Mario when both Classic moveset and 1 HP mode were enabled.
- Fixed a bug where collecting Castle stars could warp Mario back to the Castle grounds.
- Fixed a crash that could happen when starting a new game with Peach.
- Sparkly Stars:
  - Fixed a bug where Mario could capture Toads through walls with a well-placed Cappy throw.
  - Fixed a bug where Mario could go behind the TTC clock with a well-placed Cappy bounce.
- Star Road:
  - Fixed a wrong warp bug with the yellow pipe in the sewers.
  - Fixed the Grand Star hitbox.
- Render96:
  - Fixed the *Never stop playing as Peach* crash.

### GFX Engine:
- Built-in 60+ FPS patch, allowing to run the game from 30 to 360 FPS.
- Interpolated all HUD stuff, as well as dialog boxes, screen effects and timers.
- Frame interpolation is automatically disabled during states that doesn't need it, like the file select screen or when the game is paused.
- External Data is always enabled.

### OMM Bowser:
- Enraged mode: when Mario captures a flaming bob-omb, Bowser gets mad and enters Enraged mode.
  - In this state:
    - A burning aura is visible around Bowser,
    - The emergency jump is no longer triggered, so Bowser's combo is not reset if Mario gets too close to him.
  - During Enraged mode, Bowser cannot be damaged by the captured bob-omb's explosions.
  - Bowser leaves the Enraged mode if he's exhausted or if the bob-omb is no longer captured.
- Changes to Bowser's attacks:
  - Shockwaves move slightly faster, but the delay between each is reduced.
  - Most attacks have slightly reduced duration/cooldown.<br>As a result, **the boss fight is shorter, but the overall difficulty stays the same**.
- Changes to flaming bob-ombs:
  - They are summoned faster, and **can now be captured during their ascending state**.
    - If Mario captures a bob-omb during that state, Bowser enters Enraged mode instantly, and all other bombs are thrown at random locations on the battlefield.
    - This reduces the duration of the summoning and throwing action, but adds a difficulty as Mario has to avoid all remaining bombs at once.

### Color palettes
- Added color palettes for Mario and (playable) Peach to the `Extras` sub-menu.
- 28 built-in palettes are available and 4 customizable ones.
- Custom palettes can be edited in the save file, under `[mario_colors]` and `[peach_colors]`.
  - Each `mario_custom` (`peach_custom`) entry consists of 6 (7) pairs of RGB values in hexadecimal form.
  - The first value of a pair is the ambient color, the second is the diffuse color.
  - For Mario, the colors are in the following order: *Cap*, *Hair*, *Skin*, *Gloves*, *Overalls*, *Shoes*.
  - For Peach, the colors are in the following order: *Skin*, *Hair*, *Dress (light tone)*, *Dress (dark tone)*, *Gloves*, *Earrings*, *Shoes*.

### Odyssey Moveset:
- Mario can turn around faster underwater when standing still.
- The water punch can be canceled.
- The water dash no longer bonks into walls if Mario is not facing them.
- The player can redirect Mario after a water ground pound jump.
- Always perform the quick ledge climb action.
- The camera turns faster underwater.
- The camera moves below Mario when he's ceil-hanging.
- Cappy's homing attack range has been increased.

### Peach:
- Wearing an improved cap power-up allows Peach to launch Perry shockwaves, even if not at max health.
- Additionally, each power-up gives a Vibe effect to shockwaves:
  - The Wing cap gives Joy effect to shockwaves.
  - The Metal cap gives Rage effect to shockwaves.
  - The Vanish cap gives Gloom effect to shockwaves.

### Sparkly Stars
- Added the `Sparkly Stars` sub-menu:
  - `Mode`: Difficulty. *Disabled*, *Normal*, or *Hard*.
  - `Assist`: When enabled, helps the player by:
    - Preventing them from pressing not allowed buttons,
    - Preventing Cappy from capturing objects if captures are not allowed,
    - Preventing Peach from activating Vibes if Vibes are not allowed,
    - Displaying the remaining number of yellow/red/blue coins to collect, shards, boxes, mushroom 1-up, flames, etc...
  - `Show Hint at Level Entry`: When enabled, displays the hint sign text at level entry when there is one.
- Reduced SSL Pink Gold Star height.

### Other:
- The big Mario head (Goddard) is now skipped for all versions. The game goes directly from the title screen to the file select screen.
- Added level variant for BITS (120+ stars only):
  - Exit BBH or VCUTM, then enter BITS to access a Vanished variant of BITS, where the level geometry and surfaces are invisible (but collision is still there).
- Merged ex-nightly and ex-alo cheats list (does not overwrite Cheater in `r96a`):
  - `Moon Jump`
  - `God Mode`
  - `Invincible Player`
  - `Super Speed`
  - `Super Responsive Controls`
  - `No Fall Damage`
  - `Walk On Lava`
  - `Walk On Quicksand`
  - `Walk On Water`
  - `Walk On Poison Gas`
  - `Walk On Slope`
  - `Walk On Death Barrier`
  - `BLJ Anywhere`

### OMM ex-coop:
- OMM is now available as two lua mods for ex-coop:
  - `OMM (Moves)` **(v1.0.0)**:
    - Includes the wall slide, ground pound jump, underwater ground pound and jump, water dash, ground and air rolls, ground, air and midair spins.
    - Better ground and air handling, more action cancels.
    - Removes fall damage, game over, gives Mario short invulnerability after a burn and collecting a star restores health.
  - `OMM (Cappy)` **(v1.0.0)**:
    - Throw Cappy with (X), hold a D-pad button to do a directional throw.
    - Press a D-pad button after any throw to perform a homing attack.
    - Cappy can collect coins, 1-up mushrooms, secrets and interact with almost every object.
    - Collide with Cappy to bounce, or throw Cappy to other players to make them bounce or pop their bubble.
- To play with the mods, simply extract the `.zip` archives into your `sm64ex-coop-unstable/build/us_pc/mods/` directory. Then enable `OMM (Moves)` or/and `OMM (Cappy)` before hosting.
- Important note: **Do not use `OMM (Moves)` or `OMM (Cappy)` with `Shell Rush` or `Grand Theft Mario` if your coop version is not at least `unstable 24`.**

## Version 7.0.9 (01/02/2022)

### OMM Builder:
- Added *Saturn (sm64ex-nightly)* repository: `saex`. This repository shares save data with *Super Mario 64 ex-nightly* (`smex`).

### Bug fixes:
- Fixed a bug where captures triggered the regular star cutscene when touching a Sparkly star.
- Fixed a bug where the regular ground pound made Mario fall faster than a spin pound.
- Fixed a bug where some wooden poles didn't give coins after being fully pounded.
- Fixed a bug where water bombs could cause visual glitches when destroyed.
- Fixed a bug where some enemies didn't spawn Vibe sparkles after being defeated by a Perry or Vibe attack.
- Fixed a coin duplication glitch with Perry attacks.
- Fixed more music cuts (Whomp King defeated before starting the fight, Wiggler in Star Road).

### Changes:
- The instant spin can now be triggered by the mouse scroll button.
- Added more action cancels (`Moveset: Odyssey` only):
  - Wall slide:
    - Air spin **(spin)**
  - Air Cappy throw:
    - Mid-air spin **(A)**
    - Air spin **(spin)**
  - Ground Cappy throw:
    - Long jump **(Z)**+**(A)**
    - Roll **(Z)**+**(B)**
  - Turning around, Braking, Jump landings, Punching, Spinning, Idle, Crouching and Crawling:
    - Roll **(Z)**+**(B)**
  - Underwater ground pound jump:
    - Underwater ground pound **(Z)**
    - Water dash **(Z)**+**(B)**
  - Underwater Cappy throw:
    - Swim **(A)**
    - Punch **(B)**
- Reworked Peach spin attack to be less 'floaty'.
- Recovery hearts now fill the Vibe gauge as long as they spin, Peach no longer has to stand inside them.
- More enemies spawn Vibe sparkles and some enemies spawn more sparkles when defeated with Perry and Vibe attacks.
- *Super Mario 64 ex-alo* (`xalo`) now uses *Super Mario 64 ex-nightly* (`smex`) save data.
- *Super Mario Star Road* (`smsr`): The player can now access the Zero Life level, but the entrance is well hidden...

## Version 7.0.2 (17/01/2022)

### OMM Builder:
- Added a fix for the `undefined reference to 'gMusicData'` error.

### Bug fixes:
- Fixed a bug where Mario could grab classic Bowser during his jump in BITFS.
- Fixed a bug where Luigi and Wario couldn't use SMO animations.
- Fixed a bug where captured Motos could grab the moving bookshelf (and crash the game) in BBH.
- Fixed a bug where the text "Star Replica" in Star Road didn't show properly when collecting a Star replica with a capture.

### Changes:
- Sparkly Stars: Captures are not allowed in COTMC.
- Added an option `Sparkly Stars Hint` in `Extras` to toggle on/off the hint message at level entry.
- Peach uses different animations for some of her Perry attacks if `SMO Animations` is enabled.
- Captured Bully:
  - Increased charge duration from 2 to 3 seconds.
  - New ability: Press **(B)** while airborne to perform a powerful ground pound, similar to captured Whomp's attack.

## Version 7.0.0 (14/01/2022)

### Bug fixes:
- Fixed a bug where the red coin radar didn't show the red coins in Bowser levels.<br>Additionally, the red coin radar now indicates the red coin star when all red coins have been collected.
- Fixed a bug where Bob-ombs could explode when trying to grab them with Cappy, Tiara or Perry.
- Fixed a bug in Breezing Beach (`SM74EE`) where Mario couldn't reach the fifth star if `Moveset` was set to `Odyssey`.

### Hazy Maze Cave Captures
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

### Lethal Lava Land Captures
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

### Super Mario Odyssey Animations
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

### Moveset Changes:
- Mario can't capture the same object twice in a row if he left it airborne, until he lands.
- Hold **(X)** to prevent Cappy from capturing enemies, even if `Cap` is set to `Cappy - Capture`.
- Added **(Z)** and **(Z)**+**(B)** cancels for the underwater Cappy throw.
- Bob-omb capture: The third explosion doesn't destroy the Bob-omb immediately. Instead, the Bob-omb explodes and Mario is ejected only when he touches the ground.

### Other Changes:
- `Return to Castle` works now for `SMMS`, `SM74` and `SMSR`.
- Sparkly Stars hints are shown when entering a level. Some levels don't show anything, and respawning or restarting the level doesn't display the message again.
- OMM textures are precached when starting the game if `precache` is set to true (External Data/Render96 only).
- OMM texts and dialogs are loaded during start-up.<br>*Notes for Render96*:
    - Languages are supported, but only the file `AM_us.omm.json` is generated automatically. For more languages, you must provide the files yourself.
    - If the game crashes at launch, make sure the selected language is English. To do that, open `sm64config.txt` and set the `language` value to `0`.

## Version 6.1.2 (14/12/2021)

### OMM Builder:
- The `clear` command now deletes only the `build` directory.
- The `delete` command removes completely the version directory.

### Changes and fixes:
- Fixed a bug where the Boo blue coin couldn't be collected all the time with Cappy.
- Fixed a bug where the loading time of Render96 was longer than intended.
- Fixed a bug where the kickable board in WF couldn't be *attacked* by Cappy and Perry.
- The three parts of the TTM slide are now in a single area (instead of 3).

## Version 6.1.0 (11/12/2021)

### OMM Builder:
- The `star_road.patch` file is no longer needed to build *Super Mario Star Road*.
- Added *Render96 ex-alpha* repository.
- Added compatibility checks:
  - *Render96 ex-alpha*, *Super Mario 64 ex-alo* and *Super Mario Star Road* cannot be built with a version of OMM prior to 6.1.0.
  - *Super Mario 64 ex-alo*, *Super Mario 74* and *Super Mario Star Road* cannot be built with DynOS.
- Added the `DYNOS` option. When selected, download and install the latest version of DynOS with the selected repository, and copy the contents of the `custom/dynos/packs` directory to the executable directory.
- Added the `PATCHES` option. This option must be explicitly set to tell the builder to apply custom patches from the `custom/patches` directory before compiling.
- Modified the `EXT_DATA` option. This option must be explicitly set to tell the builder to compile with the flag `EXTERNAL_DATA=1`, and copy the contents of the `custom/res` directory to the executable directory.
- Added the LiveSplit auto-splitter file `omm.asl` for speedruns.

### Bug fixes:
- The star number is now hidden during the ending cutscene.
- Fixed an hitbox scaling bug on Perry.
- Fixed vanilla Bowser's shockwave interaction.

### Render96 compatibility (again):
- Besides Mario and Peach, the player can also select Luigi and Wario on the file select screen.<br>Remember that even if Luigi/Wario is selected, the player can play as him only if he has been unlocked in the chosen save file. Otherwise, the game will select Mario when entering Castle grounds.
- There are two additional ways to change the selected character in-game:
  - Through the `Odyssey Mario's Moveset` menu, option `Character`.
  - Go to the Fourth floor and enter a pipe: red pipe for Mario, green for Luigi and yellow for Wario.<br>To select Peach, enter your character's pipe (red if you play as Mario, green if Luigi or yellow if Wario).
- Each character has different properties and moves:
  - **Mario**: the most balanced character.
  - **Peach**: same properties as Mario, plus extra moves from *Super Princess Peach*. See [Version 6.0.0](https://github.com/PeachyPeachSM64/sm64pc-omm/tree/master#peach).
  - **Luigi**: runs faster, jumps higher and twirl after a back-flip, but slides on ground and moves slower in the air.
  - **Wario**: runs slower and jumps lower, but has some powerful moves from *Wario World* and gains speed faster in the air.
- Interactions with custom objects:
  - Cappy can collect Luigi keys, Wario coins and the *secret power-up* in WDW.
  - Gloom Peach can kill enemy Blarggs.
  - Rage Peach can knockback Motos.
- Cheater:<br>Most of Cheater's cheats work with OMM, but there are exceptions:
  - Cheats that have no effect when `Odyssey Moveset` is enabled:
    - `Super responsive controls`
    - `Auto wall-kicks`
  - Cheats that are completely disabled with OMM:
    - `Infinite lives`
    - `Play as`

### OMM with DynOS:
- Peach, Perry and Tiara models can now be converted into DynOS models, but they need a version of OMM >= 6.1.0 to work properly.
  - Peach's `model.inc.c` and `geo.inc.c` are in the `data/omm/peachy/peach` directory.
  - Perry's `model.inc.c` and `geo.inc.c` are in the `data/omm/peachy/perry` directory.
  - Tiara's `model.inc.c` and `geo.inc.c` are in the `data/omm/peachy/tiara` directory.
  - All textures are stored in the `omm/gfx/peach` directory.

### LiveSplit auto-splitter:
- The player no longer needs to enter their splits manually in a text file.
- To make the game read splits, save them from LiveSplit as a `splits.lss` file next to the `sm64.us.f3dex2e.exe` executable.
- See [Speedrunning](https://github.com/PeachyPeachSM64/sm64pc-omm/tree/master#speedrunning) for more details.

### Other changes:
- New Cappy cheats:
  - `Cappy Can Collect Stars`, and Bowser keys, and Sparkly stars.
  - `Play As Cappy`:
    - Press **(X)** to throw Cappy, and control him instead of Mario!
    - Move with the left stick, hold **(A)** to go up, hold **(B)** to go down.
    - The camera follows Cappy, but you can hold **(R)** to look at him from Mario's perspective.
    - Press **(X)** again or wait a few seconds to return to Mario.
- Better ceiling hanging:
  - Mario can hang on a ceiling with a jump, a double jump or a wall-jump.
  - The player no longer has to hold the **(A)** button, but has to press **(A)**, **(B)** or **(Z)** to leave the ceiling.
  - Mario moves faster, up to half his ground top speed.
- Captures are affected by the characters modifiers, but inverted:
  - This has no effect on Mario or Peach captures, but...
  - Luigi captures run slower and jump lower, but move faster in the air.
  - Wario captures run faster and jump higher, but move slower in the air.
- The following captures can now open doors:
  - Goomba
  - Koopa
  - Bob-omb
  - Spindrift

## Version 6.0.6 (20/11/2021)

### Changes and fixes:
- The `Red Coins Radar` and red coins counter are disabled if the red coins star is already collected.
- Goombas are now less likely to fall off ledges.
- Cappy:
  - Force Cappy to return to Mario during a spin throw or a flying throw if no target is found when attempting to do a homing throw.
  - The homing throw range has been increased for the flying throw, but it uses Mario's position as origin instead of Cappy's.
- Vibes:
  - Reduced the cooldown between two successive activations.
  - The player can now switch between Vibes without having to deactivate the current one first.
- `Odyssey Moveset` only:
  - Fixed glitchy ledge grabs (the ones that make Mario bonk).
  - Leaving a ledge by pressing **(Z)** still results in a soft bonk, but Mario regains control shortly after.
  - You can fire Bob-omb buddies' cannons with **(B)**.
  - Thwomp's collision changed to a cube (like Grindels), as it should be.

## Version 6.0.5 (05/11/2021)

### Bug fixes:
- Fixed a bug where spawning directly inside the JRB sunken ship activated the low visibility mode if Crystal Stars (Sparkly Stars - Hard mode) were enabled.
- Fixed a bug where Gloom Peach was sometimes not fully invincible to small flames.
- Fixed some graphical glitches on Peach and Perry with the 60 FPS interpolation.

### Changes:
- Collected red coins now display the correct number above them and play the correct sound effect (only noticeable for levels with more than 8 red coins: Veneno-Sphere, Crudelo-Sphere and Ztar Zanctuary in SM74EE).
- Additionally, the pause menu displays the number of collected red coins and the total number of red coins in the current level.
- Wooden posts now drop 5 yellow coins when fully pounded. No need to turn around them anymore.
- Added the `Red Coins Radar` in `Extras`. When enabled, it indicates the direction of the nearest red coin in the bottom-left corner of the screen.
- Added the `Show Star Number` toggle in `Extras`. When enabled, a floating number appears above every visible star to indicate which star of the current level it is.

## Version 6.0.3/6.0.4 (31/10/2021)

### Builder update:
- The builder will now ask to update if a new version is detected.

### Bug fixes (Star Road):
- Fixed a bug where Mario couldn't grab Bowser's tail.
- Fixed a bug where the ending star appeared already collected, making the 120 stars impossible to collect.

## Version 6.0.2 (28/10/2021)

### Builder update:
- The building process now generates a `logs.txt` file, similarly to **sm64pcBuilder2**.
- Star Road can now be built at any speed without stopping.
- When running the builder, if you get the error message `"Unable to retrieve current OMM version number."`, try to turn off your anti-virus before building again.
- To install Texture packs and Music packs, place the packs `.zip` archives inside a `res` directory before running the builder.
- Updated instructions.

### Changes and fixes:
- Fixed the music cuts that could happen after collecting a star in Non-Stop mode.
- Mario can break free after some time if he got shocked airborne (Odyssey Moveset only).
- Star Road Hard Mode is now available in every version. To activate it, press **(L)** on the file select screen.

### Super Mario 74:
- Debug is no longer needed to make the game work properly.
- Puppycam, 60 FPS and External Data are now fully functional with SM74.

## Version 6.0.1 (24/10/2021)
**From now on, you no longer have to update your version of OMM yourself.**<br>**However, if the changelog mentions a *Builder update* (like this one), you have to run the following command where the builder is to update it:**<br>`git checkout .; git pull -q`

### Builder update:
- The DirectX render API is now supported, build with the `DIRECT_X` option to replace the SDL/OpenGL API.
- Added an option `reset` to reset a repository without deleting it.
- The builder is now able to detect the installed version of OMM and cleaning it if it doesn't match the latest version. You don't have to update your version of OMM yourself.

### Bug fixes:
- Fixed a broken warp pipe in THI.
- Fixed a crash that happened when trying to enter rooms with Boos in BBH.
- Fixed a crash that could happen if some objects were loaded during time stop.
- Fixed BITDW Crystal Star missing coin condition.
- Fixed a bug where Tiara appeared in place of Peach's crown during some animations.
- Fixed a bug in Star Road where Peach could *collect* the Grand Star.

### Optimizations:
- Fixed a huge slowdown due to 3D coins and colored stars.<br>Colored stars have been optimized, 3D coins have been removed.
- Optimized surfaces and collisions.<br>Due to how surfaces work now, the surface cucking glitch is fixed for both movesets.
- Optimized object interactions.
- Optimized some of Peach's objects that could have potentially caused slowdowns (Rage shockwave, Joy tornados, sparkles).
- Removed unused content.

### Controls:
Controls have been redesigned to be less confusing and to prevent annoying softlocks:
- Default configuration is given by the table below:

| | Keyboard (qwerty) | Keyboard (azerty) | XBox One Controller | Switch Pro Controller |
|:-:|:-:|:-:|:-:|:-:|
| A Button | <kbd>L</kbd> | <kbd>L</kbd> | <kbd>**A**</kbd> | <kbd>**A**</kbd> |
| B Button | <kbd>,</kbd> | <kbd>;</kbd> | <kbd>**B**</kbd> | <kbd>**B**</kbd> |
| X Button | <kbd>K</kbd> | <kbd>K</kbd> | <kbd>**X**</kbd> | <kbd>**X**</kbd> |
| Y Button | <kbd>M</kbd> | <kbd>,</kbd> | <kbd>**Y**</kbd> | <kbd>**Y**</kbd> |
| Start Button | <kbd>SPACE</kbd> | <kbd>SPACE</kbd> | <kbd>**Start**</kbd> | <kbd>**+**</kbd> |
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

- All binds have been moved and are now in the `Controls` sub-menu, and are saved inside the OMM save file.
- To reset all binds to their default values (see table above), press the `Reset Controls` button.
- If the one of the **(A)**, **(R)** or **(Start)** button has no bind, its binds are reset to default.<br>This way, the player can always access the `Controls` sub-menu and press the `Reset Controls` button.
- Additionally, the player cannot bind a keyboard key or a controller button that is already assigned to the **(A)** button.<br>If another button has the same bind as the **(A)** button, that bind is removed from that button.

### Other:
- Moved the Vibe trigger to the **(Y)** button.
- Reduced Joy flying action (**(A)** button) max duration from 6 to 5 seconds.
- Added the puppy icon for `xalo` and `smsr` when Puppycam is enabled and selected.
- Added a "Correct!" sound effect when the 'complete save file' code is input correctly on the file select screen.
- Added a failsafe to the OMM save file:
  - When saving, the game creates a back-up, then write the savefile.
  - If the savefile is missing or gets corrupted/erased, the back-up will replace it.

## Version 6.0.0 (17/10/2021)

### Bug fixes:
- Fixed a bug where blue coins dropped by Boos acted like yellow coins.
- Fixed a bug where Mario couldn't leave the JRB ship through the warp pipe if Moveset was set to Classic.
- Fixed a bug where Mario could die when capturing an object near an out of bounds area.
- Fixed a bug where Mario couldn't capture objects while climbing a ledge, holding a pole, or hanging from a ceiling.
- Fixed a bug where Mario couldn't always capture a Whomp.
- Fixed a bug where Mario could perform underwater Odyssey moves while being Metal.
- Fixed a bug where the Vanish cap wasn't working properly with Vanish cap walls.
- Fixed a bug where restarting the level didn't restart the freeze timer.

### Peach:
- Peach now has some unique moves, inspired by her moveset from *Super Princess Peach*.<br>Except for Vibes, her new moves can be used only when the option `Moveset` is set to `Odyssey`.
- **Float**:
  - Replaces the mid-air spin.
  - Hold **(A)** during a fall to hover for a short time.
- **Glide**:
  - Press and hold **(A)** after a float to open Perry and start gliding.
  - Peach can glide when caught in vertical wind to go even higher!
- **Perry**:
  - Punches and Kicks are replaced by sword-like slashes and circular attacks, increasing Peach's attack range.
  - When running, Peach can execute a double slash without losing speed by simply pressing **(B)**.
  - The increased range also acts as a grab, if possible, and occurs before the damage.
  - When Peach's health is full, every attack launches a shockwave.
- **Vibes**:
  - There are four of them: *Joy*, *Rage*, *Gloom* and *Calm*.<br>Each Vibe gives Peach additional moves and effects.
  - Vibes consume Vibe points when active and deactivate at 0.<br>Vibe points are refilled over time when no Vibe is active, but Peach can fill the Vibe gauge faster by attacking enemies with Perry or by standing inside a recovery heart.
  - When a Vibe is active, Perry's attacks always launch a shockwave, with an extra effect for each type of Vibe.
  - Like Cappy directional throws, hold a D-pad button, then press **(L)** to activate a Vibe.
    - Hold **(D-up)** for *Joy*,
    - Hold **(D-right)** for *Rage*,
    - Hold **(D-left)** for *Gloom*,
    - Hold **(D-down)** for *Calm*.
  - However, there are some restrictions:
    - While a Vibe is active, Peach cannot grab or use caps. Also, activating a Vibe cancels the current cap power-up.
	- Captures still occur, but they deactivate Vibes, and Vibes cannot be used while possessing an enemy.
	- Vibes cannot stack. The active Vibe must be deactivated first to be able to use another one.
	- Vibes cannot be activated during an intangible or invulnerable state, such as taking damage or being knockbacked.
	- Vibes cannot be used underwater, and deactivate if Peach touches water, except *Calm*.
  - *Joy*:<br>Peach spins endlessly, creating a tornado around her.<br>In that state, she can only move, fly by holding **(A)** or attack by pressing **(B)**.<br>The tornado can repel enemies, attract nearby coins and lifts Peach upwards. As a result, she never makes direct contact with the floor, making her invulnerable to lava and quicksand.
    - Perry attacks are replaced by gusts. Like the tornado, they can attract coins, but damage enemies instead of simply repelling them.
	- Her happiness is such that the background music becomes faster and her voice gets higher.
  - *Rage*:<br>Peach becomes furious, making her stronger.<br>She is surrounded by a burning aura that damages enemies on contact, her ground pounds emit a fire shockwave on landing, can destroy some solid objects and defeat most of bosses in a single hit, but she's also slower and her jumps are lower.
	- Perry attacks are also stronger and launch larger, but slower fiery shockwaves.
	- Because of her menacing stance, the background music is slower and her voice is deeper.
  - *Gloom*:<br>Peach is so sad she can't stop crying...<br>She moves faster, jumps higher and her tears can extinguish most of the flames.<br>She can also run on slopes and on water.
    - Perry attacks produce blue shockwaves. They move faster and go through enemies, but are smaller than regular ones.
	- Peach's sadness slows down the music and her voice.
  - *Calm*:<br>Peach regenerates herself with positive thoughts.<br>She regains health and O2 over time, and becomes immune to the effects of frozen water.<br>*Calm* cannot be used if Peach's health and O2 are already at max.
    - Perry attacks release green shockwaves that heal Peach if they damage an enemy.
	- Peach's positivity makes the music and her voice slightly pitched up.

### Sparkly Stars:
- Crystal Stars have been redesigned.<br>They are now called Sparkly Stars, and come with two different versions, the Pink Gold Stars and the (new) Crystal Stars.
- **Pink Gold Stars**:
  - Correspond to the *Normal* difficulty.
  - Collecting them all is the condition to unlock Peach.
  - To unlock this mode, the player needs 120 Power Stars and defeat Bowser 3.
  - Each Star is similar to the old *Crystal Stars Easy mode*, with some changes:
    - Lethal Lava Land: The lava floor stops before the top, where the 5th Power star is.
	- Shifting Sand Land: The Star is now located above the top of the pyramid, and needs a crazy box to be reached.
	- Dire Dire Docks: The Star is now located above the Bowser door in the second area.
	- Tiny-Huge Island: The Star is now in Tiny Island, and needs a stack of 10 mini-goombas.
	- Castle Inside/Castle Upstairs: Star shards are easier to locate and collect.
	- Bowser 4: No more life-up and infinite Flaming Bob-omb, but the difficulty of this fight has been lowered down.
- **Crystal Stars**:
  - Correspond to the *Hard* difficulty.
  - Collecting them all unlocks the `Crystal Stars Reward` option.
  - To unlock this mode, the player must have completed the Sparkly Stars *Normal* difficulty, i.e. collected all 30 Pink Gold Stars and unlocked Peach.
  - The overall difficulty is above the old *Crystal Stars Normal mode*:
    - Whomp Fortress: The Star is now located under the fortress, and the player has only 1 minute and 50 seconds to spawn and collect it.
	- Jolly Roger Bay: The field of view is greatly reduced.
	- Cool, Cool Mountain: The Star is now located high above the mountaintop, and needs a spindrift to be reached.
	- Big Boo's Haunt: The player has only 2 minutes and 15 seconds to spawn and collect the Star.
	- Hazy Maze Cave: The player must extinguish all red flames in under 3 minutes.
	- Lethal Lava Land: The Star shards are harder to collect, and the lava rises faster.
	- Shifting Sand Land: The Star is revealed only after collecting all coins inside the pyramid, and the player starts with only 15 seconds.
	- Dire Dire Docks: The Star is now located in bounds, high above the back of the docks.
	- Snowman's Land: The Star is now located high above the head of the big snowman, and needs a spindrift to be reached.
	- Wet-Dry World: The player needs to bring a Chuckya in a specific place to spawn the Star.
	- Tall, Tall Mountain: The player needs to do a really long jump to reach the Star.
	- Tiny-Huge Island: A stack of 10 huge goombas is required to make the Star spawn.
	- Tick Tock Clock: No timer, no boxes, but Super Mario Maker 2 style.
	- Rainbow Ride: No **(A)**/**(B)**/**(Z)** press, but all 9 1-up mushrooms are required to make the Star spawn.
	- Bowser in the Dark World: In addition to the coins requirement, the player needs to stack all 7 goombas and has only 1 minute and 45 seconds to spawn and collect the Star.
	- Bowser in the Fire Sea: 6 secrets must be collected instead of 1-up mushrooms.
	- Bowser in the Sky: No **(A)**/**(B)**/**(Z)** press, all red coins, but no yellow coin, no blue coin, no capture and no 1-up mushroom.
	- Tower of the Wing Cap: Available time reduced to 35 seconds.
	- Vanish Cap under the Moat: Available time reduced to 40 seconds.
	- Cavern of the Metal Cap: Available time reduced to 30 seconds.
	- Secret Aquarium: Cappy/Tiara cannot be used.
	- Castle Inside/Castle Upstairs: Toads are hidden and harder to reach.
	- Castle Basement: That rabbit is suspicious.
	- Castle Courtyard: Totally Odyssey.
	- Bowser 4: Good luck.
- Collecting all 30 Stars in a difficulty allows the player to use caps, cannons, Vibes and cheats in that difficulty.
- Icons are added to the HUD when the player needs to collect a specific amount of items:
  - Remaining boxes in TTC,
  - Remaining 1-up mushrooms in BITFS and RR,
  - Remaining flames in HMC.
- The collected Sparkly Stars can be seen in the pause menu and on the file select screen, after clicking on a Star icon.
- Each difficulty has now its proper timer, which starts after breaking the Sparkly Star block for the first time and ends after collecting the Bowser 4 Sparkly Star.
- This timer is displayed in the pause menu and on the file select screen after selecting a Star mode.
- On the Sparkly Stars score screen, hold **(B)** for 3 seconds to reset the selected difficulty timer and Stars, without clearing the *completed* flag.

### Odyssey Moveset:
- Mario becomes invulnerable for a short time after being burn (lava boost excluded) and after capturing or leaving a capture.
- When burned airborne (not lava boost), Mario now regains control after some time.
- Mario's hitbox is shorten when rolling/air-rolling.
- Mario can now perform a Cappy bounce directly after a lava boost, if Cappy was thrown (Mario cannot throw Cappy during a lava boost).
- 1-up mushrooms now fill completely the O2 gauge.
- The object grab has been improved:
  - The grab radius is extended to a small area in front of Mario, making grabs easier to perform.
  - Grabs can now be performed during the two punches of a punch sequence, not only during a few frames of the first punch.

### Captures and Objects:
- Captures movement is more similar to Mario's movement:
  - Captures progressively accelerate and decelerate instead of reaching instantly max or min speed.
  - Same handling system as Mario (low speed = tight turn, high speed = large turn).
  - Captures jumps higher if **(A)** is held (**(A)** press = small jump, **(A)** held = high jump).
- Some buffs and nerfs:
  - Goomba:
    - Increased movement speed.
  - Koopa (shelled/unshelled):
    - Increased movement speed.
  - Koopa shell:
    - Reduced base speed, you must hold **(B)** to reach max speed.
  - Bob-omb:
    - Increased movement speed.
	- Explosions affect destructible surfaces, like Thwomps and Whomps.
  - Bob-omb Buddy:
    - Capturable underwater.
  - Chain chomp:
    - Increased movement speed.
  - Bullet bill:
    - Increased movement speed.
    - Explodes after a short time (10 seconds, 5 seconds if **(B)** is held).
  - Whomp:
    - Increased movement speed.
    - Properly immune to fire when body slamming.
    - Collect its remaining coins when captured.
  - Hoot:
    - Increased movement speed.
    - Can jump up to 6 times before landing (no more infinite jumps).
  - Baby penguin:
    - Increased movement speed (belly slide speed stays unchanged).
  - Spindrift:
    - Increased movement speed and jump height.
  - Mr. Blizzard:
    - Increased movement speed.
  - Boo:
    - Can jump up to 6 times before landing (no more infinite jumps).
    - No longer immune to lava and quicksands.
  - Scuttlebug:
    - Increased movement speed.
  - Crazy box:
    - Increased movement speed.
    - No longer immune to lava, quicksands and strong winds.
    - Hold **(A)** for jumps instead of hops, hold **(A)** + **(B)** for big jumps (the Crazy box breaks after 3 big jumps).
  - Mr. I:
    - Increased movement speed.
    - No longer immune to lava and quicksands.
- Non-Courtyard Boos always drop blue coins.
- The blue coins switch respawns until there is no blue coin left.
- Cappy explains how to beat OMM Bowser during the Bowser 1 fight after some time.
- Yoshi's gift (100 lives) has been replaced by something more surprising!

### Engine:
- Extended bounds is now part of OMM:
  - Horizontal bounds are 4x bigger.
  - Vertical bounds remain unchanged.
  - Surface pools are now allocated dynamically, allowing for an infinite amount of surfaces!
- Recoded the surface load/collision engine. This one is more robust than the vanilla one, and fixes most of custom levels collision issues.
- Collision loading distance has been removed, removing at the same time collision oddities like the Bullet Bill launcher in Whomp Fortress.
- OMM Camera:
  - First person mode allows a full 360 degrees turn, and extends the pitch limit to 89 degrees (almost vertical).
  - For levels with multiple rooms (Big Boo's Haunt, Hazy Maze Cave), the rendered room is the one where the camera is pointing at, not only Mario's room.
  - Bowser fights have now a proper OMM camera mode.
- Save files:
  - OMM has now its proper save file and file format.
  - This format is similar to `TEXT_SAVES`, but it stores all save data of every version of the game in one single file, as well as settings related to OMM and Sparkly Stars save data.
  - It also includes a cheat to instantly complete a save file and unlock Peach: On the file select screen, move the cursor to a save file button, then press the following buttons in that order:<br>**(C-up)**, **(C-up)**, **(C-down)**, **(C-down)**, **(C-left)**, **(C-right)**, **(C-left)**, **(C-right)**, **(Z)**, **(R)**, **(A)**.
  
### HUD and Menus:
- The HUD has been reworked. Icons are smaller, and progressively become transparent when Mario is moving.
- The O2 gauge is no longer a number, but an actual gauge like the one in Odyssey.
- The star select screen got a revamp, inspired by the Sunshine one. You can select the level with **(Up)** and **(Down)** and the star with **(Left)** and **(Right)**.
- Pause menus are better looking.
- Options menu: `Odyssey Mario's Moveset` is now looking like this:
  - `Character`: `Mario`, `Peach` (You need to change or restart the level to play as the selected character)
  - `Moveset`: `Classic`, `Odyssey - 3-Health`, `Odyssey - 6-Health`
  - `Cap`: `Classic`, `Cappy - No Capture`, `Cappy - Capture`
  - `Stars`: `Classic`, `Non-Stop`
  - `Power-ups`: `Classic`, `Improved`
  - `Camera`: `Classic`, `8-Directions`, `16-Directions`
  - `Sparkly Stars`: `Disabled`, `Normal Mode`, `Hard Mode`
  - `Buttons`: Various buttons binds.
  - `Cheats`: Same cheats as previous versions, plus a new one, `Endless Vibe Gauge`, that make Peach never run out of Vibe points.
  - `Extras`: A bunch of graphical options that can be enabled/disabled at will.
    - `Cappy Eyes on Mario's Cap`: Display Cappy eyes on Mario's cap. If disabled, turn Tiara into Peach's regular crown.
	- `Colored Stars`: Replace vanilla stars by glowing colored stars.
	- `3D Coins`: Replace 2D coins by custom 3D coins.
	- `Vanishing HUD`: Make the HUD transparent when Mario is moving.
	- `Reveal Secrets`: Show the secrets.
	- `Invisible Mode`: Make Mario/Peach, Cappy/Tiara and their effects invisible, except for their shadows.
	- `Crystal Stars Reward`: Only available after collecting all 30 Crystal Stars.
  - `Shortcuts`<br>A new feature that allows the player to bind options to keyboard keys.<br>Pressing a shortcut changes the value of an option without the need of opening the options menu.
  
### Super Mario 64 Moonshine
- Goombones and Bees are interactable.
- Goombones can be captured and act as Goombas.

### Super Mario Star Road
- Hard mode is compatible with the Odyssey moveset.
- All levels are compatible with Non-Stop mode.
- Custom behaviors are interactable.
- Ground shy guys can be captured and act as Goombas.

### Super Mario 74
- Added a `Swap Version` button in options menu.
- The `Warp to Level` sub-menu can also be used to swap versions.
- The OMM savefile keeps track of both versions for each file.

### Miscellaneous changes:
- The game is always built with the flags `TEXTURE_FIX`, `EXT_OPTIONS_MENU`, `NODRAWINGDISTANCE` and `BETTERCAMERA`.
- The following models are now DynOS-replaceable (for star models, the `*` character must be replaced by a number):
  - Colored stars (opaque): `omm_geo_star_*_opaque`
  - Colored stars (transparent): `omm_geo_star_*_transparent`
  - Yellow 3D coin: `omm_geo_coin_yellow` and `omm_geo_coin_yellow_no_shadow`
  - Red 3D coin: `omm_geo_coin_red` and `omm_geo_coin_red_no_shadow`
  - Blue 3D coin: `omm_geo_coin_blue` and `omm_geo_coin_blue_no_shadow`
  - Red flame: `omm_geo_fire_smoke_red`
  - Blue flame: `omm_geo_fire_smoke_blue`
  - Metal Mario sparkle: `omm_geo_metal_sparkle`
  - Vanish Mario mist: `omm_geo_vanish_mist`
  - Break particle: `omm_geo_break_particle`
  - Snowball: `omm_geo_snowball`
  - Mr. I beam: `omm_geo_mr_i_beam`
  - Captured Bob-omb explosion: `omm_geo_explosion`
  - Captured Whomp shockwave: `omm_geo_shockwave_whomp`
  - Captured Spindrift shockwave: `omm_geo_shockwave_spindrift`
  - Rising lava: `omm_geo_rising_lava`
  - OMM Bowser fire shockwave: `omm_geo_shockwave_fire`
  - OMM Bowser mine: `omm_geo_bowser_mine`
  - OMM Bowser flame: `omm_geo_bowser_flame`
  - OMM Bowser fireball: `omm_geo_bowser_fireball`
  - OMM Bowser fireball trail: `omm_geo_bowser_fireball_flame`
  - Flaming Bob-omb: `omm_geo_flaming_bobomb`
  - Flaming Bob-omb aura: `omm_geo_flaming_bobomb_aura`
  - Flaming Bob-omb explosion: `omm_geo_flaming_bobomb_explosion`
  - Sparkly star (opaque): `omm_geo_sparkly_star_*_opaque`
  - Sparkly star (transparent): `omm_geo_sparkly_star_*_transparent`
  - Sparkly star sparkle: `omm_geo_sparkly_star_*_sparkle`
  - Sparkly star block: `omm_geo_sparkly_star_*_block`
  - Sparkly star signpost: `omm_geo_sparkly_star_*_hint`
  - Peach: `omm_geo_peach`
  - Peach's crown: `omm_geo_peach_crown`
  - Pink Gold Peach's crown: `omm_geo_peach_crown_metal`
  - Perry: `omm_geo_perry`
  - Perry trail: `omm_geo_perry_trail`
  - Perry shockwave: `omm_geo_perry_shockwave`
  - Vibe aura (pink): `omm_geo_peach_vibe_aura`
  - Vibe sparkle (pink): `omm_geo_peach_vibe_sparkle`
  - Joy tornado: `omm_geo_peach_vibe_joy_tornado`
  - Joy gust: `omm_geo_peach_vibe_joy_gust`
  - Rage aura: `omm_geo_peach_vibe_rage_aura`
  - Rage shockwave: `omm_geo_peach_vibe_rage_shockwave`
  - Gloom aura: `omm_geo_peach_vibe_gloom_aura`
  - Gloom tears: `omm_geo_peach_vibe_gloom_tear`
  - Gloom droplets: `omm_geo_peach_vibe_gloom_tear_small`
  - Calm aura: `omm_geo_peach_vibe_calm_aura`
  - Calm sparkle: `omm_geo_peach_vibe_calm_sparkle`
  - Character select button (Mario): `omm_geo_menu_button_mario`
  - Character select button (Peach): `omm_geo_menu_button_peach`
  - Character select button (Luigi): `omm_geo_menu_button_luigi`
  - Character select button (Wario): `omm_geo_menu_button_wario`

## Version 5.1.2 (02/05/2021)

### Bug fixes:
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

### Changes:
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

## Version 5.1.0 (28/04/2021)

### Bug fixes:
- Fixed a frame perfect softlock that happened when reading a sign as soon as the capture animation starts playing.
- Fixed a bug where Mario could capture objects while riding or holding something.
- Fixed a bug where Mario could bounce of Cappy while riding or holding something.
- Fixed a bug where Koopa could get his shell back if Mario was capturing the shell.
- Fixed a bug where the stationary bob-omb in BOB was not affected by gravity.
- Fixed a bug where uncapturing a Toad and capturing it again could spawn his star multiple times.
- Fixed a bug that prevented Mario from wall-sliding after jumping off a pole.
- Fixed a bug where Cappy used an incorrect Mario position to check their interaction, resulting in a 1-frame delay between the intended bounce and the actual bounce.
- Fixed a bug where Mario could do a Ground Cappy bounce on lava.

### Major changes:
- DynOS has been removed from OMM.
- Some functionalities have been kept, and moved to the regular options menu:
  - Warp to Level (level, area, star)
  - Return to Main Menu
  - OMM sub-menu:
    - The `Moveset`, `Cap`, `Stars`, `Power-ups`, `Camera`, `Buttons` and `Cheats` options are still the same.
    - Added a `Crystal Stars` option, allowing the player to set the Crystal Stars mode to `Disabled`, `Normal`, or `Easy`.<br>Selecting `Normal` or `Easy` requires a restart or a level change to apply.<br>You must defeat Bowser 3 with 120 stars at least once to be able to select `Normal` or `Easy` (so after the bad ending).
    - Added an `Extras` sub-menu with 3 new options:
      - `Cappy Eyes on Mario's Cap` displays Cappy eyes on Mario's cap, if enabled (doesn't work with all models, see below).
      - `Colored Stars` turns regular stars into colorful glowing stars, if enabled.
      - `3D Coins` turns 2D flat coins into 3D custom models, if enabled.

### Mario's Moveset:
- Added the Midair Spin: Press **(A)** during a fall to do a quick spin without losing height.<br>If Mario wears the Improved Vanish Cap, pressing **(A)** again right after a Midair Spin will trigger a regular Air Spin.
  
### Cappy:
- Pressing **(X)** + **(A)** does an up-throw instead of a regular throw.
- Mario can now bounce on Cappy during the falling part of a lava boost, like in Odyssey.
  
### Captures:
- After leaving a Goomba capture, Goombas stay stacked if **(Z)** is pressed, but break if Mario gets damaged.
- Attacking a Goomba stack with Mario will break the stack.
- You can stack up multiple Goomba stacks by jumping on top of them when possessing a Goomba.
- Modified the OMM Cam to be able to show the entire stack of Goombas if Mario is possessing a Goomba.
  
### Non-Stop mode:
- When Mario dies, he restarts from the area he was in, instead of the very start of the level.
- Collected stars no longer respawn after a death, as they stay collected until Mario leaves the level or select `Restart Level` on the pause menu.<br>If Time Trials is enabled, the entire level is reset after a death, and lost coins don't respawn.

### Crystal Stars:
- Added a coin arrow in HMC, DDD, WDW and COTMC to show where the star is.
- VCUTM: Reduced available time from 60 to 45 seconds.
- The bad ending now plays only after beating Bowser 3 for the first time with 120 stars, and as long as the Crystal Stars mode is enabled but Peach is not unlocked yet.
- The secret ending always plays after beating Bowser 4 and collecting the last Crystal Star, but the ending cake screen will show up only the first time, when Peach is about to be unlocked.
- Added Easy mode:<br>In Easy mode, the following changes make Crystal Stars easier to obtain:
  - The No-Cap condition is not checked after spawning a Star (Condition stars only), meaning that the player can use caps to reach the Star.
  - The Requirements (specific type of coins, no button press, etc...) are not checked after spawning a Star (Condition stars only).
  - Timers immediately stop after spawning a Star (Cap courses).
  - Bob-omb Buddies' cannons are available everywhere.
- Additionally, Easy mode comes with changes specific to each Crystal Star:
  - BOB: Cannons can be used to collect the secrets.
  - WF: Only 90 coins are needed (Thwomp coins can be skipped).
  - JRB: Only 80 coins are needed (you can miss up to 4 blue coins).
  - CCM: Same area, but the star is in bounds and easier to reach.
  - BBH: No change.
  - HMC: Same area, but the star is in bounds and easier to reach.
  - LLL: Same area, but the star spawns above the 5th power star and requires only wall-jumps to be reached.
  - SSL: The star is slightly lower.
  - DDD: The star is in front of the wall, instead of behind.
  - SL: The snowman's big hop is higher, spindrifts are not required.
  - WDW: Moved to above the 1-up mushrooms tunnel.
  - TTM: Same place, but lower.
  - THI: Same place, but lower.
  - TTC: Increased available time from 45 to 60 seconds, and the star spawns lower.
  - RR: No A press only.
  - BITDW: Only the 8 red coins are needed.
  - BITFS: Reduced the number of needed 1-up mushrooms from 7 to 6, and the star spawns right after the bridge leading to Bowser 2.
  - BITS: No A press only, and the star spawns in front of the pipe instead of above.
  - PSS: Increased available time from 22 to 24 seconds.
  - TOTWC: Same duration (40 seconds), but the timer stops after collecting all coins.
  - VCUTM: Same duration (45 seconds), but the timer stops after touching all 5 corners.
  - COTMC: Same duration (50 seconds), but the star spawns lower.
  - WMOTR: Only the 8 red coins are needed.
  - SA: Only the 8 red coins are needed.
  - GROUNDS: Same place, but lower.
  - INSIDE: The out of bounds star is replaced by 8 shards to find and collect.
  - BASEMENT: The hint is more explicit.
  - 2ND FLOOR: The out of bounds star is replaced by 8 shards to find and collect.
  - COURTYARD: The hint is more explicit.
  - BOWSER 4: Flaming bob-ombs can't explode after taking damage, and Bowser drops a 1-up mushroom each time he gets hurt.
- Updated anti-cheat system:
  - Now works as intended, with no false positive.
  - When a cheat is detected, the anti-cheat acts instantly, instead of at level exit.
  - New Bowser quotes!
- If cheats were enabled before the Crystal Star mode, prevents the latter from enabling via the options menu.<br>However, breaking the Crystal Star block on Castle Grounds sets the mode to `Normal` and turns off cheats.

### Render96-alpha changes:
- All text used by OMM is stored in `AM_us.omm.json` files, and only the English version is done for now.<br>Feel free to copy this file and translate it into your native language.
- In dialogs, the word `Mario` is correctly replaced by `Luigi` or `Wario`, depending on the selected character.<br>Course names and Star names are not affected by this change.
- Added Odyssey death sounds for Luigi and Wario.
- Added Luigi and Wario to the character select menu.<br>Luigi and Wario appear only if they have been unlocked in at least one save file.<br>Loading a save file checks if the selected character has been unlocked in that file, and change the selected character to Mario if not.
- Added Peach to the character swap room (fourth floor).<br>To select Peach, jump into the selected character's pipe (red pipe for Mario, green pipe for Luigi, yellow pipe for Wario).<br>You must have unlocked Peach first for this functionality to work.
- Tweaked some of Wario's moves, to be able to carry on Odyssey moves:
  - Wario's pile driver acts as a ground pound, so Wario can perform a ground pound jump by pressing **(A)** out of it.
  - Wario can throw Cappy during his special triple jump.
  - Wario can roll by pressing **(Z)** during his shoulder bash.
  - Wario can stun an entire Goomba stack by jumping on it, but grabbing the bottom Goomba will break the stack.
- Some Odyssey moves are enhanced by Luigi's hidden modifier:
  - Luigi's jumps are higher, including Cappy bounces.
  - Luigi's water dash is faster.
  - Luigi flies faster with the Improved Wing Cap.
- Added bad cake ending screens for Luigi and Wario.
- Wario's shoulder bash loses its invulnerability against OMM Bowser 4 attacks (Normal mode only).
- Milk makes Cappy grow.

### Minor changes:
- Infinite health cheat now fills the O2 gauge and makes Mario immune to frozen water.
- Stage variants only appear after collecting all 120 stars.
- Instant quicksands are now scorching sands (Odyssey moveset only):
  - They no longer kill Mario instantly, but rather act as lava.
  - Metal Mario can still walk on this type of sand.
  - Deep quicksands are not affected and remain the same.
- Reworked Cappy eyes code.<br>For now, they appear only on the following models:
  - Vanilla Mario
  - Odyssey Mario
  - Moonshine Mario
  - Low-Poly Promo Mario/Luigi/Wario
  - Render96 2.2 Mario/Luigi/Wario
  - VaniLla96 Mario/Luigi/Wario

## Version 5.0.2 (10/04/2021)

### Bug fixes:
- Fixed the infinite Cappy jump glitch.
- Fixed the infinite stairs bypass glitch.

## Version 5.0.1 (03/04/2021)

### Bug fixes:
- Fixed the bad/secret endings.

## Version 5.0.0 (01/04/2021)

### Bug fixes:
- Fixed a bug where OMM Bowser could get hit at the start of his jump action, resulting in a premature knockback action that could lead to a crash.
- Fixed a bug where OMM Bowser was unable to hit Mario with his flamethrower.
- Fixed a bug where the sinking into quicksands effect were applied twice per frame.
- Fixed a bug where Mario couldn't open a door already opened by a possessed Toad.
- Fixed a bug where selecting Restart Level during a Bowser fight restarted the whole level instead of the fight.
- Fixed a bug where Mario was stunned after falling while riding a shell, thus losing the shell.
- Fixed a rendering crash in VCUTM.

### Mario's Moveset:
- OMM Health has been merged with OMM Moveset.<br>Mario's Moveset choices are now: `Classic`, `Odyssey (3-Health)`, `Odyssey (6-Health)`.
- Mario no longer loses a life when dying.
- Reduced the water dash O2 consumption.
- Mario now progressively loses health instead of dying instantly when its O2 meter reaches 0 (but still dies instantly in poison gas).
- Increased HP consumption in cold water for Classic Moveset to 1 segment per second.
- Dive recoveries now carry into a freefall after the rollout, allowing Mario to perform aerial moves instead of falling to his death.
- Mario can wall-slide after a jump kick.
- Better stick rotation detection: Spin jumps should be easier to perform.
- Fwoosh and Big Snowman can no longer blow off Mario's cap (Mario still receives the knockback).
  
### Cappy:
- Increased Cappy throw distance with the Vanish cap, due to the lower gravity.
- You can now perform a Cappy homing attack with spin throw variants.
  - Spin throw: Left and Right buttons move Cappy outside of its circle.
  - Flying throw: Left and Right buttons move Cappy outside of its circle, Up button moves Cappy forward, Down button moves Cappy backwards.
- Cappy jumps are now called Cappy bounces, because a Cappy bounce is not a jump.
- Added Cappy throw strength: hold the throw button to launch Cappy far away or slightly press it to launch it near Mario.<br>If the left stick is held in any direction, always perform a maximum distance Cappy throw.<br>This change affects only the regular Cappy throw, not up, down and spin variants.
- New Cappy cheats: `Global Homing Attack Range` and `Mario Teleports To Cappy`.
- Cappy's homing attack ignore collisions if `Global Homing Attack Range` is enabled.
- Cappy's homing attack can be canceled by pressing the throw button **(X)**.
- Cappy's homing attack targets coins first.
- Cappy is now strong enough to break exclamation boxes.
  
### Captures:
- Increased captured Bob-omb jump height, and explosions **(B)** trigger a second jump if performed airborne.
- Captured Bob-ombs now respawn correctly after being destroyed.
- Captured Bullet Bill explosions now always destroy objects, even if the Bullet Bill is not moving fast (**(B)** held).
- Captured Bullet Bills and Whomps can now destroy Thwomps with their attacks.
- Jumping on bullies with a capture knockbacks them.
- King Bob-omb, King Whomp, Tuxie's mother and the Snowman's head are no longer capturable.
- Koopa Shells (without Koopas) are now capturable, and are slightly faster than a shelled Koopa.
  
### Improved Power-ups:
- Improved Metal Mario can no longer be eaten by Bubba.
- Improved Metal Mario is immune to lava damage, but still does the lava boost.
- Improved Metal Mario can now destroy the Mad Piano.
- Improved Wing Cap: Press **(B)** twice while airborne to start flying (only if Mario is not diving).
- Reverted initial cap durations in Cap courses to their default values (Improved power-ups still have their custom values when they are collected from an exclamation box).

### Non-Stop mode:
- Added glowing stars, with optional Render96 star textures (SM64 star textures are selected by default).
- Because of warp pipes, checkpoints are no longer disabled.
- Koopa the Quick no longer cares about King Bob-omb in BOB.<br>The player can now start the race before defeating King Bob-omb.
- The following changes are applied now to every stage, not only the main courses:
  - Mario always does the Odyssey star dance, unless he's about to exit the current stage (see below).
  - Mario automatically exits the current stage he's in if all stars have been collected, except in the Castle or in Bowser stages.
  - Collecting a key after a Bowser fight forces Mario to leave the stage.
  - The Odyssey death sequence is always played when Mario's health reaches 0, except in the Castle.
- Lost coins no longer turn into money bags.
- World objects (warp pipes, sets of power-up boxes...) always spawn, even if the Non-Stop mode is disabled.
  
### Enhanced Camera:
- Fixed a crash that could randomly happen when using the OMM camera.
- The OMM camera can be enabled anywhere and at anytime, except inside the Castle and during Bowser fights.
- More modes: 5 distances, 8-Directions and 16-Directions, press **(R)** to change mode without opening the DynOS menu.
- Hold **(R)** while standing still to enter the first person camera mode (like C-up mode, but Mario is invisible).
- The Camera status is now correctly displayed on the HUD.
  
### Physics changes:
- Collisions: The following changes are applied if the Moveset is set to Odyssey:
  - Treats PUs as out of bounds.
  - Fixes the unreferenced wall glitch, and returns the closest wall Mario is looking at.
  - Fixes surface cucking, always returns the lowest ceiling and the highest floor.
  - Checks floor on a square instead of a point, to prevent objects from falling inside small gaps.
  - Increases number of cells checked for more precise wall collisions.
- Steps: The following changes are applied if the Moveset is set to Odyssey:
  - Increases number of sub-steps from 4/4/1 to 16/16/16, for more precise surface detection.
  - Fixes exposed ceilings, they no longer extends infinitely upwards, but up to half Mario's height.
  - Out of bounds does not make Mario bonk, but prevents him from moving horizontally.
  - Ceilings does not make Mario bonk, but prevents him from moving horizontally.
  - Fixes glitchy ledge grabs.
  - Fixes jumps on moving platforms.
  - Fixes underwater up/downwarps.
  - Fixes whirlpool attractive strength.
    
### Stage variants:
- Exit CCM or SL and enter SA to change the water to cold water.
- Exit DDD or WDW and enter JRB to increase the water level, resulting in an entirely flooded JRB.<br>The ship and Goombas don't spawn inside flooded JRB, but the jet stream and its star do.
    
### Some QoL:
- From now on, OMM uses its own save file (only if `TEXT_SAVES` is not enabled).
- Added a built-in model pack `Vanilla Mario With Cappy`.
- Added a version number on the title screen and the main menu (bottom-right corner).
- Added a sanity check for graph nodes: Corrupted graph nodes are less likely to crash the game when processed.
- Added a sanity check for music sequences: Corrupted sequences are less likely to crash the game when processed.
- The HUD shows the correct amount of Stars available in the current level (mainly for PC-ported rom-hacks).
- Because the life counter is now useless, mushrooms 1up fully restore Mario's health when collected.
- Mips always spawns in the basement after collecting the 120 stars.
  
### Big Boo's Haunt captures:
- Boo:
  - The Boo's coin is automatically collected when captured.
  - Press **(B)** to turn transparent and go through walls, as well as gaining a nice speed boost.
  - Press **(A)** repeatedly to move up. Release **(A)** to fall slowly.
  - Capturing a Ghost Hunt Boo or a Merry Go Round Boo marks it as eliminated, making the Big Boo spawn faster.
  - Capturing the Courtyard Big Boo (the one with the cage) triggers the warp to BBH.
- Scuttlebug:
  - Jump with **(A)**, run by holding **(B)**.
  - Captured Scuttlebugs can open doors.
  - Captured Scuttlebugs can stick to walls by holding **(B)**.
    On a wall, you can move left and right as long as **(B)** is held, and jump from it by pressing **(A)**.
- Mad Piano:
  - Can move, but cannot jump or attack.
  - Press **(A)** or **(B)** to play a song.<br>The song lasts as long as Mario is inside BBH, and pressing different buttons has different effects:
    - **(A)**: Play the next song on the list.
    - **(B)**: Play a random song.
    - **(A)** + **(B)**: Restore the BBH music.
- Crazy box:
  - Constantly bounces, even if not moving.
  - Hold **(A)** to bounce higher.
  - Hold **(B)** to do a super bounce, each super bounce is higher than the previous one, but the box breaks after 3 super bounces.
  - Leaving the Crazy Box resets the super bounce counter.
- Mr. I:
  - Can move, jump, and has low gravity.
  - Because it floats, captured Mr. I is immune to ground hazards (lava, quicksands).
  - Can shoot a projectile by pressing **(B)** (hold **(B)** to shoot faster and further), and can open doors.
  
### Post-game content:
- Increased number of collectable Stars from 120 to 150.
- These 30 new stars, called Crystal Stars, appear after collecting the 120 Power Stars, and defeating Bowser 3.
- They are incredibly hard to find and collect, but gathering them all unlocks something really worth...
- Added two new endings, the bad ending and the secret ending.

## Version 4.1.4 (25/01/2021)

### Changes and fixes:
- JRB/DDD: Fixed a bug where the whirlpools were more attractive/repulsive than intended.
- TTM: Fixed the cage monkey path and its dialog trigger distance.
- Fixed some issues with the new 8-dir cam:
  - The camera is now available in BBH, but only outside of the mansion.
  - The camera can now ignore metal grids, fences, transparent walls and vanish cap walls collisions.
  - The camera can slightly clip into a wall if it's too close to Mario.
  - The camera is now disabled during Bowser fights.
- Added a `Param Select` option in the `Warp To Level` sub-menu:
  - You can now choose to spawn Bowser's submarine and/or the poles before warping to DDD.
  - You can now set the water level before warping to WDW.
  - You can now select the island you want to spawn in and choose if the top has been drained or not before warping to THI.
  - You can now select the clock speed before warping to TTC.

## Version 4.1.3 (16/01/2021)

### Changes and fixes:
- Fixed a bug where the player could pause the game during the star dance animation.
- BBH: Fixed a bug where defeating the big Mr. I could spawn a blue coin instead of the `Eye to eye in the secret room` star.
- CCM: Fixed a bug where the game could softlock while rolling towards the snowman's body.
- CCM: Fixed a bug where the snowman's body could respawn more than once.
- Increased Cappy's homing attack max distance.
- Air bonks turn into a freefall after 1 second for backward knockbacks, and after 1.5 second for forward knockbacks.
- Air roll turns into a freefall as soon as Mario's forward velocity goes under 45, only if **(Z)** is not held.
- Added an enhanced version of the 8-directions camera. This camera can replace the vanilla camera inside courses by setting `Mario's Camera` to `8-Directions` from the `Mario` submenu.

## Version 4.1.2 (10/01/2021)

### Changes and fixes:
- Fixed a crash that happens when collecting a Castle star after completing a course (7 stars).
- Sideflips are easier to perform.
- Mario can now grab stars while possessing an enemy.
  
### Better collision v2:
- Out of Bounds and Ceiling collisions don't make Mario bonk, but prevent him from moving horizontally.
- Ceilings no longer extend infinitely upwards, but up to half Mario's height (80) (partial fix to exposed ceilings).
- Recovering from a bonk/knockback is faster.
- Redesigned air knockback to be more Odyssey-like.
- Jump kicks no longer push Mario out of a wall.
- Jump kicks can now extend into ledge grabs.

### Speedrun:
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

## Version 4.1.1 (28/12/2020)

### Changes and fixes:
- Fixed a bug when Mario could die after a level exit if he triggered a death warp at the same time.
- OMM assets are no longer built-in, they are now stored inside the `dynos/gfx` and `dynos/audio` folders.<br>These assets can be replaced by custom ones, as long as filenames and textures dimensions remain the same.<br>External Data is not needed.
  
### OMM Bowser fights:
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

## Version 4.1.0 (23/12/2020)

### Changes and fixes:
- Cappy can open treasure chests (JRB, DDD).
- Cappy can grab Unagi's tail star.
- Cappy can collect rings of coins and coins trails with a homing throw.
- OMM Mario's underwater ground pound can trigger underwater purple switches (HMC, WDW).
- Inside a course, the pause menu has 4 options when Non-Stop mode is enabled:
  - Continue
  - Restart Level
  - Exit Level
  - Return to Castle (same as `Exit Course`, brings Mario back to the center of the castle)
- Collecting 7 stars in a row auto-ejects Mario from the current course, like in Time Trials.
- Revamped HUD:
  - Custom icon for star counter, coin counter and O2 level.
  - Current power-up and its remaining time is now displayed below the coin counter.<br>This also include combined power-ups, as Metal Vanish cap for instance.
- Re-entering a completed course (7 stars collected) make 3 power-up boxes spawn at the start, one for each power-up (Non-Stop mode only).
- Odyssey Mario's Moveset options menu:
  - Some options have been moved, but stay basically the same as before. Here is the new structure:
    - `Mario` sub-menu:
      - `Mario's Moveset` (Choice: Classic, Odyssey)
      - `Mario's Health` (Choice: Classic, 3-Health, 6-Health)
      - `Mario's Stars` (Choice: Classic, Non-Stop)
      - `Mario's Power-ups` (Choice: Classic, Improved)
      - `After-effects` (Toggle)
    - `Cappy` sub-menu:
      - `Cappy's Behavior` (Choice: Disabled, No Capture, Capture)
      - `Cappy Throw Button` (Bind)
      - `Cappy Up Button` (Bind)
      - `Cappy Down Button` (Bind)
      - `Cappy Left Button` (Bind)
      - `Cappy Right Button` (Bind)
      - `Unlimited Cappy Jumps` (Toggle)
      - `Cappy Stays Forever` (Toggle)

### Improved Power-ups:
- The Wing, Vanish and Metal caps have been reworked to fit more their original design.
- Improved Wing cap:
  - Inspired by Galaxy's Red Star power-up, it allows Mario to move freely in the air, without any height limitation.
  - Hold **(B)** to brake and rotate in place. Release **(B)** to start flying again.
  - Duration has been increased from 45 to 60 seconds.
- Improved Vanish cap:
  - Inspired by the Vanish cap render, it allows Mario to move through grids and thin walls.
  - When Mario touches such a wall, he instantly vanishes only to reappear on the other side, while a teleport sound effect is played.
  - As long as Mario wears the Vanish cap, his gravity is reduced, allowing him to jump higher and fall slower, and Cappy ignores wall and ceiling collisions when thrown.
  - Duration has been increased from 20 to 40 seconds.
- Improved Metal cap:
  - Inspired by the in-game description itself, it allows Mario to be truly and fully invincible.
  - Nothing can damage or knockback Metal Mario. He's also immune to fall damage.
  - He's too heavy to be grabbed by Chuckyas, King Bob-omb or Heave-hos.
  - He's resistant to strong winds and water currents.
  - He can perfectly walk on lava and quicksands without being slowed down.
  - He can destroy enemies that try to squish him, like Whomps, Thwomps, Tox Boxes or Eyerock hands.
  - He's also able to break normally indestructible objects, like Mr. Blizzards, bowling balls or rolling rocks.
  - Duration has been increased from 20 to 30 seconds.
  
### Bowser fights:
- General changes:
  - Increased possessed flaming bob-omb explosions from 3 to 5.
  - Increased possessed flaming bob-omb movement speed.
  - Increased Bowser exhaustion duration.
- OMM Bowser 1 seemed to be too hard for a first boss, so some changes were made.
- OMM Bowser 2 is ready and replaces vanilla Bowser 2 when entering the Fire Sea arena.
  - Added a new attack to Bowser: the flamethrower.
- OMM Bowser 3 is ready and replaces vanilla Bowser 3 when entering the Sky arena.
  - Added a new attack to Bowser: the rain of fire.
  - The circular platform still turns into a star-shaped platform after the second hit.
  - OMM Bowser 3 is way harder than OMM Bowser 2 and vanilla Bowser 3, but fits more as a final boss.

### Speedrun Rules update:
- Because OMM Bowser fights are longer (and mandatory) than vanilla Bowser fights, submitted runs of OMM prior to this version (4.1.0) are now obsolete and invalid.
- Because Non-Stop mode and OMM Bowser fights are now fully implemented, submitted runs of OMM starting from this version (4.1.0) will always be valid, no matter the future updates of this mod.
- Because `Restart level` and `Exit level` are options from the pause menu, the usage of any DynOS option is now forbidden. Opening the DynOS menu during a run makes it invalid.
- Odyssey Mario's Moveset mandatory options:
  - `Mario's Moveset` must be set to `Odyssey`
  - `Mario's Health` must be set to `3-Health`
  - `Mario's Stars` must be set to `Non-Stop`
  - `Mario's Power-ups` must be set to `Improved`
  - `Cappy's Behavior` must be set to `Capture`
  - Both cheats `Unlimited Cappy Jumps` and `Cappy Stays Forever` must be set to `Disabled`
- Mandatory options, as well as key/button binds, must be set before the start of the run.

## Version 4.0.0 (09/12/2020)

### Changes and fixes:
- Cappy is now always summoned when pressing the Cappy Throw button, even if Mario's throw animation is interrupted.
- Added the ground Cappy jump, a higher version of the Cappy jump that Mario performs when touching Cappy on ground.
- Grabbing trees and poles resets the Cappy jump.
- Hold B to climb on trees and poles faster.
- Mario can now properly ledge-grab out of a group pound jump, a Cappy jump and a ground Cappy jump.
- Mario now rolls on a straight line on sticky terrain.
- Ground pounding against a wall no longer results in a bonk.
- Landing on slippery slopes, Ground pounds and spin pounds no longer trigger the fall damage animation.
- Added minimum velocity required to start rolling out of a long jump/air dive.
- If the player hold **(Z)** out of a long jump or an air dive, Mario will instantly roll as he touches the ground, preventing any fall damage.
- Holding **(Z)** while rolling makes Mario roll longer and further.
- Collecting a star fills the O2 meter and restores Mario's health (Non-Stop mode only).
- Fixed the Whomp Fortress music cutoff (Non-Stop mode only).
- There is a small chance after a death for a lost coin to turn into a moneybag.
- Increased Goomba stack max count to 10 Goombas.
- Added the Leave Ending Screen patch by GateGuy, and delayed the `Thank you so much` voice line to not overlap with the credits music.
- As `Exit level` is now a DynOS feature, selecting `Exit course` on the pause screen brings Mario back again to the center of the Castle.
- HUD:
  - Outside the Bowser fights, the Castle and when the game is not paused, the star counter is replaced by 7 star icons,
    representing the 6 consecutive acts of a course and the 100 coins star.
  - There are three versions of the star icon: empty, full, dark.
    - Empty: not collected
    - Full: collected, but physically present (as a transparent star)
    - Dark: collected

### Colored Stars:
- Added a new star model, that replaces the old one and the transparent one.
- Every course has now his own colored stars:
  - Bob-omb Battlefield: Green
  - Whomp Fortress: Silver
  - Jolly Roger Bay: Pink
  - Cool, Cool Mountain: Cyan
  - Big Boo's Haunt: Purple
  - Hazy Maze Cave: Black
  - Lethal Lava Land: Red
  - Shifting Sand Land: Lime
  - Dire Dire Docks: Blue
  - Snowman's Land: White
  - Wet-Dry World: Blue-gray
  - Tall, Tall Mountain: Brown
  - Tiny-Huge Island: Gold
  - Tick Tock Clock: Light Orange
  - Rainbow Ride: Fuschia
  - Secret Stars: Yellow
  
### Cappy Eyes:
- Cappy eyes are now compatible with the following models:
  - Super Mario 64 Mario
  - High Definition Mario (by Arredondo)
  - Super Mario Odyssey Mario (by D4RW1N)
  - Mario Kart 8 Mario (by D4RW1N)
  - Luigi from L is Singleplayer 2401 (by LuigiFan1)
  - Render96 Mario v1.3 (by the Render96 team)
  - Render96 Mario v1.4.2 (by the Render96 team)
  - Render96 Mario v2.0 (by the Render96 team)
  - Render96 Luigi v1.4.2 (by the Render96 team)
  - Render96 Luigi v2.0 (by the Render96 team)
  - Render96 Wario (by AndresL64, LuigiNoodle)
  - Super Smash Bros Brawl Mario (by Opmox)
  - Hat Kid (by Napstio)
  - Bow Kid (by Napstio)
  
### Cool, Cool Mountain Captures:
- It's now the turn of the Cool, Cool Mountain entities to be possessed by Mario!
- Small Penguin:
  - Move with the Left Stick.
  - Press **(A)** to jump.
  - Press **(B)** to dive and start sliding, hold to keep sliding and gain speed, and release to progressively slow down.<br>The more slippery a surface is, the faster the small penguin will be able to slide on it.
  - Can interact with Tuxie's mother (mother penguin).
- Tuxie's mother:
  - She can be captured only before rescuing her child.
  - Do not capture her child after she gave you the star.
- Spindrift:
  - Move with the Left Stick.
  - Press **(A)** to jump. Press **(A)** again in the air to do a propeller jump.<br>Hold **(A)** to fall slower.
  - Press **(B)** on ground or in the air to perform a spin attack.
- Mr. Blizzard:
  - Move with the Left Stick. Like Chain Chomp, he moves by doing little hops.
  - Hold **(A)** to jump every time he touches the ground.
  - Press **(B)** to throw a snowball. The snowball can destroy most of enemies and objects, and can collect coins.<br>Hold **(B)** to make the snowball bigger before releasing it. Big snowballs are able to take down other Mr. Blizzards.
- Snowman
  - Its head is capturable only before being reunited with its body.
  - Its body can be captured on its pedestal, at the top of the mountain.
  - Snowman's body can move with the Left Stick, but cannot jump.<br>It gets bigger and faster as it rolls and defeats most of enemies on contact, but breaks and ejects Mario if it takes damage or lands from too high.

### Bonus:
- Toad is now capturable, and does not eject Mario after he finishes talking:
  - Walk with the Left Stick, hold **(B)** to run, press **(A)** to jump, and interact with doors to open them.
  - And because Peach gave him the Castle master key, he can open Star doors without the need of Power Stars, but can't open the doors locked by Bowser.
- Speaking of Bowser, he learnt some moves from his Odyssey counterpart, and is now waiting for Mario and Cappy to face him in the Dark World arena.

## Version 3.1.0 (03/11/2020)

### Changes and fixes:
- Removed turning while backflipping.
- Mario enters the roll state out of a dive or a long jump by holding **(Z)**.
- Mario keeps most of his momentum after throwing Cappy on ground if the joystick is held.
- Non-Stop mode no longer gives Mario infinite lives if Health System is set to Classic.
- The player is no longer redirected to the Game Over screen if he dies when having 0 lives if Health System is set to 3-Health or 6-Health.
- Mario can now properly jump when standing on a moving platform.
- Fixed Dire Dire Docks Manta Ray's water rings hitbox.

### Cappy:
- Reworked Cappy wall collisions.
- Cappy is now properly frozen during Time Stop.
- Cappy's hitbox and model are larger than Mario's cap when thrown.
- Cappy throws are slightly faster than before to match Odyssey's ones.
- Cappy can no longer be thrown again if he's already out, but Mario can force him to return prematurely by pressing **(X)** again.
- Up-throw, Down-throw and Homing throws have been added to Cappy's moveset:
  - Trigger an Up-throw by holding D-pad up and pressing **(X)**.
  - Trigger a Down-throw by holding D-pad down and pressing **(X)**, or by pressing **(X)** after landing from a ground pound.
  - Trigger a spin throw by holding D-pad left or right and pressing **(X)**, or by pressing **(X)** while spinning.
  - Trigger a Homing throw by pressing a D-pad direction while Cappy's out:
    - Cappy will target the nearest interactable object, then will return to Mario.
    - If no target is found, Cappy will move in the direction of the button pressed, then will return to Mario.
- Cappy has now his own sub-menu:
  - `Cappy's behavior` can be set to `Disabled`, `No Capture` or `Capture`.
  - `Cappy Throw Button` is **[X]** key or **(X)** button by default.
  - `Cappy Up Button` is **[Num8]** key or **D-Pad Up** button by default.
  - `Cappy Down Button` is **[Num2]** key or **D-Pad Down** button by default.
  - `Cappy Left Button` is **[Num4]** key or **D-Pad Left** button by default.
  - `Cappy Right Button` is **[Num6]** key or **D-Pad Right** button by default.
  - `Unlimited Cappy Jumps` allows the player to do as many consecutive Cappy jumps as he wants.
  - `Cappy Stays Forever` is back, and force Cappy to never return to Mario, unless the player presses **(X)**.

### True Non-Stop mode:
- Non-Stop mode has been redesigned to make all 7 stars available in every course.
- When entering a painting, Mario is directly warped to the course, skipping entirely the act selection screen.
- The in-course pause menu has been changed to show which stars have been collected and which ones are not.
- Some courses have been altered to make all 7 stars spawn in one go:
  - Bob-omb Battlefield:
    - Bob-omb King always starts at the top of the mountain, ready for a fight.
    - Koopa the Quick also spawns, but cannot start a race until Bob-omb King is defeated.
  - Whomp Fortress:
    - Whomp King always starts at the top of his fortress, ready for a fight.
    - The Tower appears once the Whomp King star is collected.
  - Jolly Roger Bay:
    - Only one Eel spawns, and it's always the *Can the Eel Come Out to Play?* star one.
    - The ship always starts sunken.
    - After collecting the *Plunder in the Sunken Ship* star, a warp pipe appears at the bottom of the sunken ship.
    - The sunken ship then emerges, and a warp pipe appears, leading to its inside.
    - The whirlpool and the *Through the Jet Stream* star appear once the ship is fully emerged.
  - Cool, Cool Mountain:
    - The racing penguin is always here, waiting for a race against Mario.
    - The Snowman's body always spawns separated from its head, looking for Mario to escort him.
  - Big Boo's Haunt:
    - All 3 versions of the Big Boo can be fought. The first one only appears after defeating all 5 *Ghost Hunt* Boos.
    - The Boos inside the manor are always *Ghost Hunt* Boos, and trigger a dialog when defeated.
  - Lethal Lava Land:
    - The green shell box always spawns.
    - A one-way warp pipe has been added inside the Volcano to allow Mario to return to the main area.
  - Shifting Sand Land:
    - The two versions of Klepto (the star one and the hat-stealer one) are here.
    - A one-way warp pipe has been added inside the Pyramid to allow Mario to return to the main area.
  - Dire Dire Docks:
    - The Manta Ray always spawns, even if Bowser's sub is here.
    - Red coins poles always spawn, even if Bowser's sub is here.
    - A one-way warp pipe has been added inside the *Collect the Caps* star cage to allow Mario to escape the cage.
  - Wet Dry World:
    - A one-way warp pipe has been added at the top the underwater city to allow Mario to return to the main area.
  - Tall Tall Mountain:
    - The mountaintop star always spawns.
    - Mountaintop Ukiki and his cage always spawn as well.
  - Tiny-Huge Island:
    - Koopa the Quick always wait for a race in the Huge version of the island.
    - A one-way warp pipe has been added inside Wiggler's room to allow Mario to return to the main area.<br>This pipe only appears after defeating Wiggler.
- Non-Stop mode is completely disabled if the game is compiled with the Time Trials patch.

## Version 3.0.1 (18/10/2020)

### Changes and fixes:
- Fixed a bug that locked Mario in an infinite loop of death if Non-Stop mode was deactivated
- Fixed a bug that made Mario start any savefile with only 4 segments of health if SMO-Health was deactivated
- Fixed a bug that prevented the SM64 power meter from being displayed if SMO-Health was deactivated
- Fixed a bug that drew Cappy's eyes over wings on Render96 Mario's wing cap

## Version 3.0.0 (16/10/2020)

### Changes and fixes:
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

### Mario's Moveset:
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
  
### Super Metal Mario:
- Underwater Metal Mario's moveset has been greatly enhanced. Here is the list of the moves he can perform:<br>Idle, Walk, Crouch, Crawl, Jump, Double jump, Triple jump, Backflip, Sideflip, Long Jump, Freefall, Wall slide, Wall jump, Punch (all 3 variants), Jump kick, Dive, Ground pound, Ground pound jump, Cappy throws, Cappy jump, and all Spin actions.
- The Metal cap duration has been increased from 20 to 30 seconds.
- Metal Mario is now truly invincible, as he cannot die as long as he's wearing his metal cap.<br>Note that he's still vulnerable to instant death effects, such as quicksand or death barrier.
  
### Star Collection Mode:
- In main courses, Mario does not leave the course after collecting a star.<br>Instead, he does a victory dance while the star jingle is playing, and, similarly to Odyssey, a text box appears below him, showing the message *YOU GOT A STAR* with the name of the course and the star he got.
- The game auto-saves after collecting a star. Furthermore, it will ask the player if he wants to leave the course after he collects all the available stars (100 coins star included).
- As long as Star Collection Mode is active, the option `Exit course` always brings Mario back in front of the course painting instead of the center of the Castle. This works for every stage, not only the 15 main courses.
- Dying drops Mario at the beginning of the course again, instead of returning to the Castle.<br>The death sequences have been edited to look more Odyssey-like.<br>There are seven of them: regular, drowning, falling, frozen, eaten by Bubba, quicksanded, squished.
- When Mario dies, the course is completely reset and he loses all his coins, but (at most) 20 of them drop on the floor and can be collected again after his death. This makes a death less frustrating and punishing, specifically when trying to collect a 100 coins star. This works for every stage, not only the 15 main courses.
  
### Odyssey Options Menu:
- DynOS v0.4 is included with this patch. You don't need to patch it.
- Custom options
  - `Mario's Moveset` can be set to `Classic` or `Odyssey`.
  - `Cappy's Behavior` can be set to `Disabled`, `No Capture` or `Capture`
  - `Cappy Throw Button` allows you to bind your keys/buttons used to throw Cappy.
  - `Health System` can be set to `Classic` (SM64), `3-Health` (Odyssey normal mode) or `6-Health` (Odyssey assist mode).
  - `Star Collection` can be set to `Classic` or `Non-Stop`.
  - `Underwater Metal Mario` can be set to `Classic` or `Super`.
  
### Last but not least, Cappy:
- Cappy now has eyes, but only with SM64 Mario, HD Mario, Render96 Mario and Render96 Luigi. They show up with regular Mario, Vanish-cap Mario and Winged-cap Mario, but not Metal Mario.

## Version 2.0.2 (14/09/2020)

### Changes and fixes:
- Fixed a crash that happened when throwing Cappy 64 times in a row, without grabbing it or bouncing on it.
- Fixed a crash that happened when Cappy was out while Mario was teleporting through invisible warps.
- Removed Time Trials and Ghost Race from SMO, added it as a separate patch, Time Trials v2.0.
- Added SMO Options Menu as a separate `.txt` file, working with DynOS v0.2.<br>With DynOS patched, add the `.txt` file to your `res` folder.<br>Without DynOS patched, the default configuration used by the game is the following:
  - Mario's Odyssey moveset is enabled
  - Cappy throw is bound to the button 2 on a controller (corresponds to **(X)** button on XBox One controllers), or **[X]** key on keyboard
  - Cappy's behavior is set to `Capture`
  - Mario uses the 3-Health system from Odyssey
- Cappy now unloads if Mario has his cap in his hand or if he loses his cap (ukiki, big snowman, fwoosh).

## Version 2.0.1 (10/09/2020)

### Changes and fixes:
- Time Trials
  - Fixed a bug when generated ghost data would produce abnormally large .txt files under certain circumstances.<br>The new time trials data files use a different file format, so older files are incompatible with 2.0.1.
  
## Version 2.0.0 (08/09/2020)

### Changes and fixes:
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
    
### New stuff:
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
  
### Whomp Fortress Captures:
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
    
## Version 1.5.8 (28/08/2020)

### Hotfixes:
- SMO HUD
  - Hide the coin counter inside the castle walls
  - Properly shows collected keys (below star count) with the SGI v1.4.2 pack

## Version 1.5.7 (27/08/2020)

### Changes and fixes:
- Roll speed has been slightly reduced.
- Mario has more air control: he can turn progressively while airborne, and turns instantly when throwing Cappy.
- Fall height has been increased and Mario no longer takes fall damage, but he's unable to move for 1.5 second when landing from a high place.
- Grounded Mario's controls are more responsive (similar but not as extreme as `Super-responsive controls` cheat).
- Cappy returns to Mario after 4 seconds instead of vanishing instantly. Same when he hits an attackable enemy (bully, skeeter...).
- Mario can throw Cappy after a Cappy jump, but the latter is intangible for him (like in Odyssey).
- Mostly code refactoring (for compatibility purposes).

### New stuff:
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

## Version 1.4.2 (22/08/2020)

### Changes and fixes:
- Now compatible with SGI v1.4.2 (Credits to DorfDork).

## Version 1.0.1 (14/08/2020)

### Changes and fixes:
- Fixed a crash that occured when trying to capture an enemy, if a custom sound pack was loaded.

## Version 1.0.0 (12/08/2020)

### Changes and fixes:
- Mario takes off his cap when Cappy is thrown. (Thanks to s4ys)
- Mario's wall slide is faster.
- Metal Mario can punch and jump kick under water.
- Mario turns while throwing Cappy, making him able to change direction airborne.
- Cappy's throw is now bound to a new button (not the B button anymore).<br>You'll have to go to the Options menu, Controls sub-menu and bind keys for the `X button`.
- Cappy can be thrown in a way that it orbits around Mario by pressing **(X)** and **(A)** simultaneously.
- A sub-menu `Super Mario Odyssey` is available in the Options menu.<br>You can choose from here Mario's moveset and if you want to enable or disable Cappy.<br>By default, Mario's moveset is set to `64 moveset` and Cappy is disabled.
- Two new cheats have been added to the *secret* Cheats sub-menu:<br>`Unlimited Cappy jumps` and `Cappy stays forever`. (Thanks to s4ys for the idea)

### New stuff:
You wanted it, here it is. Now, Mario can capture and possess his enemies! (Well, currently, a few ones)
- In the `Super Mario Odyssey` sub-menu, set Cappy to `Capture`.<br>Throw Cappy to an enemy, and it will try to possess it. Each enemy has a unique way to move and act.<br>For this version, only the enemies present in Bob-omb Battlefield are capturable.<br>This includes Goomba, Koopa, Bob-omb, Bob-omb buddy, Bob-omb King and Chain Chomp.<br>Press **(Z)** while possessing an enemy to leave it. Mario automatically leaves his possession if he takes damage.
- Goomba
  - This little boy can walk, run by holding **(B)** and jump by pressing **(A)**.
  - If you jump on another goomba, you can create a Goomba stack.<br>Jump on other ones to make your stack grow. (with a limit of 8 Goombas)
- Koopa (shelled)
  - This one can walk, jump by pressing **(A)**, and shell-slide by holding **(B)**.
  - Shell-sliding has the same properties as Mario while he rides a shell: knockback enemies, travel on lava...
  - Koopa the Quick is not capturable.
- Koopa (unshelled)
  - The other one can't shell-slide, but can run (for his life).
  - He can get his shell back if he touches it.
- Bob-omb
  - Bob-omb can't run, but launches a destructive explosion when pressing **(B)**.
  - His explosion destroys enemies and brick blocks.
- Bob-omb buddy
  - Mario can't move him around, but reads his dialog without having to talk to him.
  - Mario is automatically ejected when the dialog ends.
  - The cannon variant is also capturable.
- Bob-omb king
  - Just try to capture him.
- Chain chomp
  - While possessing Chain chomp, Mario can move, do big hops by holding **(A)** and perform a bite attack by pressing **(B)**.
  - He's restricted to his chain attached to the wooden post, but can break free if he tries to bite while his chain is fully stretched.
  - When freed, this massive iron ball is invulnerable and knockbacks enemies on contact, and can even break brick blocks and some walls by biting them.
  - Note: Freed chain chomp has a new behavior when Mario leaves it: he will start to chase Mario if he is too close to him.

## Version 0.11.0 (27/07/2020)

### Changes and fixes:
- Mario wall-slides automatically when touching a wall, making wall jumps easier.<br>Additionally, press **(Z)** on a wall to stop wall-sliding.
- Breathing underwater does not drain Mario's power anymore.<br>He has now a separate oxygen meter, visible under Mario's lives counter.

### New actions:
- On the ground, press **(Z)** + **(B)** to do a roll. Press **(Z)** + **(B)** again to keep rolling.
- In the air, press **(Z)** to start a ground pound, then press **(B)** before impact to do an air dive.
- After landing from a ground pound, press **(A)** to do a high jump, or **(Z)** to start rolling.
- Underwater, press **(Z)** to plunge, **(Z)** + **(B)** to do a water dash.<br>These actions make Mario move really fast, but they drain his oxygen meter.

### Cap actions:
- On the ground, in the air or underwater, press **(B)** to throw Mario's cap in front of him.<br>His cap collects coins and secrets and attacks enemies like Goombas or Koopas.
- Touch the cap to bounce on it. If Mario touches it from the ground, he can throw it once more in the air.<br>Underwater, Mario can't interact with his cap, but Metal Mario can.
- For the moment, there is no capture mechanic. Instead, the cap gives to Mario any grabbable object it encounters on its path (Bob-ombs, corkboxes, lil' penguin...)
  
