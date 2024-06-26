# Options

- `Odyssey Mario's Moveset`:
  - `Character`: Mario, Peach (if unlocked), Luigi or Wario (***Render96*** only).
  - `Moveset`: Classic, Odyssey (3-Health), Odyssey (6-Health), Odyssey (1-Health).
  - `Cap`: Classic, Cappy (No Capture), Cappy (Capture-Press), Cappy (Capture-Hold).
  - `Stars`: Classic, Non-Stop.
  - `Power-ups`: Classic, Improved.
  - `Camera`: Classic, 8-Dir, 16-Dir.
  - `Sparkly Stars`:
    - `Mode`: Disabled, Normal, Hard.
    - `Show Hint at Level Entry`: Always, Only If Not Collected, Never.
  - `Cheats`:
    - `Unlimited Cappy Bounces`: Mario can bounce infinitely on Cappy.
    - `Cappy Stays Forever`: Cappy no longer returns to Mario after some time.
    - `Homing Attack Global Range`: Extend Cappy's homing attack range to the whole level.
    - `Mario Teleports to Cappy`: Press **(X)** again after throwing Cappy to teleport Mario to him.
    - `Cappy Can Collect Stars`: Cappy can interact and give stars and keys to Mario.
    - `Play As Cappy`: Throw Cappy to control him. Hold **(A)** to go up, **(B)** to go down and press **(X)** to return to Mario.
    - `Peach Endless Vibe Gauge`: Peach only. Vibes no longer depletes the Vibe gauge.
    - `Shadow Mario`: Turns Mario into Shadow Mario.
  - `Extras`:
    - `Mario Colors`: Select a color palette for Mario. The last 4 are customizable.
    - `Peach Colors`: Select a color palette for Peach. The last 4 are customizable.
    - `Super Mario Odyssey Animations`: If enabled, replace some of the vanilla animations with animations from *Super Mario Odyssey*.
    - `Cappy and Tiara`: If enabled, replace Mario's cap by Cappy and Peach's crown by Tiara.
    - `Colored Stars`: If enabled, give each level stars a specific color, like moons in *Super Mario Odyssey*.
    - `Vanishing HUD`: If enabled, make the HUD progressively vanish when Mario is moving.
    - `Reveal Secrets`: If enabled, make the secrets visible.
    - `Red Coins Radar`: If enabled, locate and show the direction of the nearest red coin.
    - `Show Star Number`: If enabled, show for each star its number.
    - `Invisible Mode`: If enabled, make Mario, Cappy, Peach, Tiara and their effects invisible.
  - `Shortcuts`: Allow the player to bind up to three keys/buttons for a bunch of options.
- `Game` (***Render96*** only):
  - `Current Language`: Set the current language.
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
  - `Frame Rate`: Set the framerate. Does not affect game updates, they still run 30 times per second.
    - `30`: No frame interpolation.
    - `60`: Enable frame interpolation to make the game render at a constant 60 FPS.
    - `Auto`: Synchronize the framerate with the monitor's refresh rate.
    - `Unlimited`: Render as much frames as possible.
  - `Show FPS`: If enabled, show the actual framerate in the bottom-left corner of the screen.
  - `Fullscreen`: Enable fullscreen mode.
  - `Vertical Sync`: Synchronize the render rate with the monitor's refresh rate. Enable it if you're experiencing tearing.
  - `Texture Caching`: Preload and cache all textures when starting the game to avoid in-game freezes due to texture loading.
    - `Disabled`: Never preload textures.
    - `At Start-up`: Load all textures present in the `res` directory every time the game starts.
    - `Permanent`: Load all textures once and cache them permanently.
  - `Texture Filtering`: Set to nearest neighbor for blocky effects, or linear for smooth but blurry effects.
  - `Draw Distance`: Control the rendering distance of objects. Reduce it if you're experiencing slowdowns.
  - `HUD`: Show or hide the HUD.
- `Sound`:
  - `Master Volume`: The general volume setting.
  - `Music Volume`: The volume of background music.
  - `SFX Volume`: The volume of sound effects.
  - `ENV Volume`: The volume of environment sounds (birds chirping, water flowing, etc...).
- `Cheats`: A list of built-in modifications that gives Mario super-human powers, such as Moon-jumping, Infinite health, the ability to BLJ anywhere and much more.
