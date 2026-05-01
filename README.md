# asobi-unreal-demo

Minimal UE5 demo project for the [Asobi Unreal SDK](https://github.com/widgrensit/asobi-unreal).

A player auto-registers, queues for the `demo` matchmaker, drops into a match, and moves a cube around with WASD. Other players in the same match render as remote cubes, their positions driven by `match.state` updates from the asobi server.

## Prerequisites

- Unreal Engine 5.4+ (tested on 5.4–5.7)
- A running [`sdk_demo_backend`](https://github.com/widgrensit/sdk_demo_backend) (see below)

## Setup

1. **Bring up a backend.** One command, no Erlang or rebar3 required:

   ```bash
   git clone https://github.com/widgrensit/sdk_demo_backend
   cd sdk_demo_backend && docker compose up -d
   ```

   Server listens on `http://localhost:8084`.

2. **Clone this repo with submodules:**

   ```bash
   git clone --recursive https://github.com/widgrensit/asobi-unreal-demo.git
   ```

   If you already cloned without `--recursive`:

   ```bash
   git submodule update --init --recursive
   ```

3. **Generate project files:** right-click `AsobiUnrealDemo.uproject` → *Generate Visual Studio project files* (or the Unix equivalent).

4. **Open `AsobiUnrealDemo.uproject` in UE5.** First open will compile the `AsobiSDK` plugin.

5. **Create a level:** the repo doesn't ship a `.umap`. In the editor, *File → New Level → Basic*, save it under `Content/Maps/ArenaMap`, then *Edit → Project Settings → Maps & Modes → Game Default Map*: set to `ArenaMap`.

6. **Press Play.** The HUD will show `LoggingIn → Connecting → Queuing → InMatch`. Press WASD to move; if a second client is queued, it'll appear as a remote cube.

## Controls

- **WASD** — move

## Project layout

| File | Purpose |
|---|---|
| `AsobiDemoGameInstance` | Owns SDK objects, handles auth → WS connect → matchmaker |
| `AsobiDemoGameMode` | Listens to `OnMatchState`, spawns and updates pawns; registers `AsobiDemoHUD` |
| `AsobiDemoPawn` | Local player WASD + 30 Hz `match.input`; remote pawns lerp to `match.state` positions; ships a top-down spring-arm + camera so Play just works |
| `AsobiDemoHUD` | Status overlay |

## License

Apache 2.0
