# CLAUDE.md

Guidance for Claude Code when working in this repository.

## Project overview

**MYSTIC ARSENAL** is a first-person magic-shooter game being built for the **Kappa Cup Winter (カッパ杯ウィンター)** contest. It is a C++ / DirectX 12 Windows game built on **k2Engine**, Kawahara Gakuen's in-house teaching engine (河原学園内製エンジン).

Almost all of the ~25k files in the repo are the engine, third-party libraries and tools. **The game code is only about 20 files in `MYSTIC ARSENAL/Game/`.** That folder is where nearly all work happens.

## Repository layout

```
MYSTIC ARSENAL/
├── Game/                 ← THE GAME (edit here)
│   ├── Game.sln          ← open this solution in Visual Studio
│   ├── Game.vcxproj
│   ├── main.cpp          wWinMain: init engine, NewGO<Game>, run main loop
│   ├── stdafx.h/.cpp     precompiled header (includes k2EnginePreCompile.h)
│   ├── system/           window creation + K2Engine::CreateInstance (template code)
│   ├── Game.*            root scene object: spawns everything else
│   ├── Player.*          FPS player: CharacterController movement, arm model, shooting
│   ├── MainCamera.*      first-person camera (yaw/pitch from right stick)
│   ├── BackGround.*      stage model + static physics collision
│   ├── Magic.*           player projectile; hit-tests enemies
│   ├── Enemy.*           enemy with HP; AI is only stubbed so far
│   ├── EnemyMagic.*      empty subclass of Magic (placeholder)
│   └── Assets/           runtime data, loaded by relative path
│       ├── modelData/    .tkm models (+ .DDS textures), preset/ = engine presets
│       ├── animData/     .tka animations
│       ├── shader/       HLSL used by the engine at runtime (.fx/.h)
│       └── font/
├── k2Engine/             high-level engine: rendering engine, ModelRender,
│                         SpriteRender, FontRender, lights, post effects, LevelRender
├── k2EngineLow/          low-level engine: GameObject system, graphics (DX12),
│   │                     math, physics wrappers, HID (pad), sound, time, AI
│   └── ExEngine/         third-party: bulletPhysics, DirectXTK, effekseer, Photon
├── exlib/                prebuilt external libs (x64)
├── tools/                3ds Max plugin/scripts/shaders, Effekseer editor, k2SLEditor (level editor)
└── toolsProj/            source for tools: tkmExporter, tkmOptimizer, nvmExporter, k2SLEditorProj
```

Treat `k2Engine/`, `k2EngineLow/`, `exlib/`, `tools/` and `toolsProj/` as vendored engine code. Don't change them unless the task is explicitly about the engine.

## Building and running

- Windows only. Open `MYSTIC ARSENAL/Game/Game.sln` in Visual Studio (platform toolset **v145**, Windows 10 SDK). Build **x64** (`Debug`, `Release` or `Preview`). Win32 configurations exist but aren't used.
- The solution also builds `k2Engine`, `k2EngineLow`, `DirectXTK12` and the Bullet projects (`BulletCollision`, `BulletDynamics`, `LinearMath`). `Game` links `k2Engine.lib`.
- `Game.vcxproj` sets the include paths (`../k2Engine`, `../k2EngineLow`, `ExEngine/...`, Bullet, Effekseer, Photon) and library paths (`../exlib/$(Platform)/$(Configuration)`, DirectXTK, Effekseer).
- **The working directory must be `Game/`.** Asset paths such as `"Assets/modelData/stage/stage.tkm"` are relative, and the engine compiles shaders from `Assets/shader/` at runtime with `dxcompiler.dll` and `dxil.dll`, which sit in `Game/`.
- Command-line build (Developer PowerShell): `msbuild "MYSTIC ARSENAL\Game\Game.sln" /p:Configuration=Debug /p:Platform=x64`
- There are no automated tests and no linter. To verify a change, build it and run the game.
- Frame buffer is 1920×1080 (`FRAME_BUFFER_W/H` in `k2EngineLow/k2EngineLowPreCompile.h`).

## Engine programming model

Everything in the game is an `IGameObject` subclass managed by the engine's `GameObjectManager`.

```cpp
class Foo : public IGameObject {
public:
    bool Start() override;                  // called once before the first Update; return true when ready
    void Update() override;                 // called every frame
    void Render(RenderContext& rc) override; // draw: call m_modelRender.Draw(rc)
};
```

- **Create:** `NewGO<T>(priority, "name")`. Priority is 0–254, and lower runs first. Every object here uses priority 0.
- **Destroy:** `DeleteGO(obj)`. Deletion is deferred: the object is marked dead and removed later. Every object made with `NewGO` must eventually be passed to `DeleteGO`.
- **Find:** `FindGO<T>("name")` returns the first match. `FindGOs<T>("name")` returns all matches. `QueryGOs<T>("name", [&](T* go){ ...; return true; /* false = stop */ })` iterates over matches. The name string is the lookup key, so keep names consistent: `"game"`, `"player"`, `"mainCamera"`, `"backGround"`, `"enemy"`, `"magic"`.
- **Globals:** `g_pad[0]` (gamepad: `GetLStickXF`, `GetRStickYF`, `IsTrigger(enButtonB)`, `SetVibration`), `g_camera3D`, `g_gameTime->GetFrameDeltaTime()`, `g_renderingEngine`, `g_sceneLight`, `g_k2Engine`.
- **Common components:**
  - `ModelRender`: `Init("Assets/modelData/x.tkm", animClips, numClips, upAxis, ...)`, then `SetPosition`/`SetRotation`/`SetScale`, `Update()`, and `Draw(rc)` inside `Render`.
  - `AnimationClip`: `Load("Assets/animData/x.tka")`, `SetLoopFlag`.
  - `CharacterController`: `Init(radius, height, pos)`. `Execute(moveSpeed, dt)` returns the new position. `IsOnGround()`.
  - `PhysicsStaticObject`: `CreateFromModel(model, worldMatrix)` builds level collision.
  - Also available: `SpriteRender`, `FontRender`, `SkyCube`, `LevelRender`, `SpotLight`/`PointLight`, `SpringCamera`.
- `stdafx.h` includes `k2EnginePreCompile.h` and pulls in `using namespace nsK2EngineLow; using namespace nsK2Engine;`. Every `.cpp` must start with `#include "stdafx.h"`.
- Math types: `Vector3` (with `Vector3::AxisZ` etc.), `Quaternion` (`SetRotationDegY`, `Multiply`, `Apply`), `Matrix`.

## Current game flow

1. `main.cpp` calls `InitGame`, then `NewGO<Game>(0, "game")`, then loops on `K2Engine::Execute()`.
2. `Game::Start` spawns `BackGround`, `Player`, `MainCamera` and 3 `Enemy` objects at x = 100, 500, 900.
3. `MainCamera` reads the right stick for yaw/pitch (pitch clamped to ±80°). It puts the camera at the player position + 150 on Y, then calls `Player::SetArmTransform` so the arm model (`PlayerArm.tkm`) follows the view.
4. `Player` moves relative to the camera with the left stick (speed 300) and applies gravity when not on the ground. **B** fires a `Magic` from the camera position + 80 along the camera's forward direction.
5. `Magic` flies at 1500 units/s. It does a sphere-distance check (< 60) against every `"enemy"`, calls `Enemy::Damage(1)` on a hit, and deletes itself on a hit or after 3 s.
6. `Enemy` has 3 HP and calls `DeleteGO(this)` at 0 HP.

### Work in progress / known gaps

- `Enemy` declares `Move`, `Gravity`, `Attack`, `Shot`, `UpdateState`, `Turn` and `GetTOPlayerXZ`, plus state and timer members, but **none of them are implemented yet**. Implement them before calling them, or you'll get linker errors.
- `EnemyMagic` is an empty subclass of `Magic`. `Magic`'s members are `private`, so subclasses can't reach them yet.
- `Player` animations are commented out, and `Player::Rotation()` is empty. The `else` branch in `Player::Update` (`characterController.IsOnGround() == true;`) is a no-op comparison.
- `Player`'s members are effectively public (`//private:` is commented out). `MainCamera` reads `m_player->position` directly.
- `Game` has unused members (`m_modelRender`, `m_pos`).
- `main.cpp` vibrates the pad when **A** is pressed (a debug leftover from the template).

## Conventions

- Class per file pair (`Foo.h` / `Foo.cpp`), with `#pragma once`. Forward-declare classes in headers and include their headers in the `.cpp`.
- When you add a new source file, register it in **both** `Game.vcxproj` and `Game.vcxproj.filters`. Otherwise Visual Studio won't compile it.
- Naming is mixed. Newer code uses the `m_` prefix for members (`Enemy`, `Magic`, `MainCamera`), while `Player` and `BackGround` use bare names. Prefer `m_` for new code.
- Comments are in Japanese. **Encoding varies:** engine and template files (`system/`, `main.cpp`, most of `k2Engine*`) are **Shift-JIS (CP932)**, and newer game files are UTF-8. Keep each file's existing encoding when you edit it, so Japanese text doesn't get garbled. Write new files in UTF-8 (with BOM, which MSVC handles reliably).
- Units are engine units (roughly cm). The frame time is fixed at `1.0f / 60.0f` for `CharacterController` and `g_gameTime->GetFrameDeltaTime()` elsewhere.

## Assets

- Models are `.tkm`, skeletons are `.tks`, animations are `.tka`. They are exported from 3ds Max with the plugin and scripts in `tools/3dsMax*`, or from FBX through `toolsProj/tkmExporter`. Textures are `.DDS` files next to the model.
- `Assets/modelData/preset/` holds engine-required defaults (null maps, sky cube maps, volume-light meshes). Don't delete them.
- `Assets/shader/` is used by the engine at runtime. Change it only for rendering work.

## Repo notes

- `.gitignore` excludes `*.dll`, `*.exe`, `*.pdb`, `*.obj`, `*.lib`, `.vs`, etc. **`dxcompiler.dll`/`dxil.dll` and `exlib/*.lib` match these patterns.** If they're missing after a fresh clone, they have to come from the engine distribution.
- `Game/x64/` and `Game/Game/x64/` are build outputs. `MainPlayer.obj` in there is stale and has no matching source.
