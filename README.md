# NaN-GD-FV
A **Geode** mod for Geometry Dash 2.2081 that records your inputs, analyzes them
into labeled **frame windows**, and replays them with live in-game visualization.

Inspired by frame-window / NaNDL-precision tooling.

## Features

### Bot recorder
- Hooks `PlayerObject::pushButton` / `PlayerObject::releaseButton` and stores
  every input together with the physics frame it happened on.
- Works for both players (1P / 2P).

### Frame analysis
- `Analyze` turns the recorded input list into labeled frame windows:
  each press becomes a window centered on its frame with a configurable
  `+/-` tolerance, a label and a color.

### Playback with labels
- `Play` re-dispatches the recorded inputs on the exact frames they were
  captured on, while the visualizer draws and labels every window.

### Visualization
- Colored markers spawn on the player when a window's frame is reached and fade
  out automatically.
- A live HUD shows the current physics frame, the total window count and how
  many windows have been hit this attempt.

### Editor
- Open it from the pause-menu clock button.
- Add / delete / enable / disable windows, record, analyze, play, clear,
  and import / export the window list as JSON.

## Project layout

```
src/
├── main.cpp                    Entry point ($on_mod Loaded)
├── FrameData.hpp               FrameWindow type + matjson (de)serialization
├── State.hpp / State.cpp       Global window list, persistence, file IO
├── BotController.hpp / .cpp    Input recorder, playback, analyzer
├── VisualizerNode.hpp / .cpp   Markers + HUD
├── ui/FrameWindowLayer.*       Pause-menu editor popup
└── hooks/
    ├── PlayLayerHook.cpp        Frame tick, playback dispatch, reset
    ├── PauseLayerHook.cpp       Editor button
    └── PlayerObjectHook.cpp     Input capture
```

## Building

GitHub Actions is already configured — push to any branch and download the
combined `.geode` from the workflow artifacts.

Or build locally with the [Geode CLI](https://github.com/geode-sdk/cli):

```bash
geode build
```

## File format

Exported/imported JSON lives in the mod save directory as
`frame-windows.json`:

```json
{
  "windows": [
    { "frame": 1280, "window": 2, "player": 1, "enabled": true, "label": "F1280 P1", "r": 1.0, "g": 1.0, "b": 1.0, "a": 1.0 }
  ]
}
```

## Credits

- Author: **bedrock**
- Built on the [Geode SDK](https://geode-sdk.org).
