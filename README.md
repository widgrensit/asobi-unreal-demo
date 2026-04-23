# asobi-unreal-demo

Minimal UE5 demo project for the [Asobi Unreal SDK](https://github.com/widgrensit/asobi-unreal).

A player auto-registers, joins the `arena` matchmaker, drops into a match, and moves a cube around with WASD. Other players in the same match render as remote cubes, their positions driven by `match.state` updates from the asobi server.

## Prerequisites

- Unreal Engine 5.7+
- Asobi backend running locally (see [widgrensit/asobi](https://github.com/widgrensit/asobi))
- Arena game mode configured on the backend (`arena` → `asobi_arena`)

## Setup

1. Clone this repo with submodules:

   ```bash
   git clone --recursive https://github.com/widgrensit/asobi-unreal-demo.git
   ```

   If you already cloned without `--recursive`:

   ```bash
   git submodule update --init --recursive
   ```

2. Right-click `AsobiUnrealDemo.uproject` → **Generate Visual Studio project files** (or the Unix equivalent).

3. Open `AsobiUnrealDemo.uproject` in UE5. It will compile the `AsobiSDK` plugin on first open.

4. In the editor, create an empty level at `Content/Maps/ArenaMap.umap`, add a floor/lighting, and save.

5. Start the asobi backend:

   ```bash
   cd /path/to/asobi
   docker compose up -d
   rebar3 shell
   ```

6. Press **Play** in UE5 — you should see the HUD cycle through `Registering → Queuing → In match as <id>`.

## Controls

- **WASD** — move

## Project layout

| File | Purpose |
|---|---|
| `AsobiDemoGameInstance` | Owns SDK objects, handles auth → WS connect → matchmaker |
| `AsobiDemoGameMode` | Listens to `OnMatchState`, spawns and updates pawns |
| `AsobiDemoPawn` | Local player WASD + 30 Hz `match.input`; remote pawns lerp to `match.state` positions |
| `AsobiDemoHUD` | Status overlay |

## License

Apache 2.0
