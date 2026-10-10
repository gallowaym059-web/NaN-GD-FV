# Frame Window Visualizer

A frame-window tracking, labeling and precision toolkit for **Geometry Dash** (Geode).

## What it does

- **Record** your own inputs while you play (a built-in bot recorder).
- **Analyze** the recording into labeled **frame windows** (each input becomes a
  window with a `+/-` tolerance).
- **Replay** the recording while the windows are drawn and labeled live.
- **Visualize** every window as a colored marker that follows the player, plus a
  live HUD showing the current physics frame and how many windows were hit.

## Usage

1. Pause a level and press the **clock icon** on the right side of the pause menu.
2. Use **Record** to start capturing inputs, play your route, then **Stop**.
3. Press **Analyze** to turn the recorded inputs into frame windows.
4. Press **Play** to replay them with markers/labels, or just watch the HUD live.
5. **Import / Export** windows to a JSON file in the mod's save directory.
6. **Add Frame** lets you drop a manual window at any frame number.

## Notes

- Windows are matched against the physics tick counter, so this pairs well with
  high-TPS / click-between-frames setups.
- This mod stores no data outside your own save directory.

See `README.md` for the full feature list and build instructions.
