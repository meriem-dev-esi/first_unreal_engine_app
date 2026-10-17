# first_unreal_engine_app

A 3D Third-Person Melee Arena Brawler fighting game prototype created in **Unreal Engine 5.7** using **C++ and Blueprints**.

## 🎮 Game Features

- **Dynamic Melee Combat**: Light attack combo strings and heavy knockdown attacks with stamina costs.
- **Defensive Tactics**: Active blocking with damage mitigation (75% damage reduction) and guard break on stamina depletion.
- **Lock-On Target Tracking**: Souls-like target lock-on system that orients player attacks and camera toward the nearest combatant.
- **Stamina & Health System**: Automatic stamina regeneration during idle periods, stamina penalties for attacks and blocking.
- **Match & Round System**: Round countdown timer (90s), best-of-three round tracking, timeout decision by HP advantage.
- **Blueprint Integration**: All C++ combat actions, delegates, and attributes are exposed for Blueprint animation montages, VFX, and UI health bars.

---

## 🏗️ Project Architecture

```
first_unreal_engine_app/
├── Source/
│   └── first_unreal_engine_app/
│       ├── FighterTypes.h            # Combat state enums (Idle, Attacking, Blocking, HitStun, Dead)
│       ├── FighterCharacter.h/.cpp   # Core combat pawn (Combos, Health, Stamina, Lock-on, Hitboxes)
│       ├── FighterGameModeBase.h/.cpp# Match controller (Round timer, round wins, match state)
│       ├── first_unreal_engine_app.Build.cs
│       └── first_unreal_engine_app.Target.cs
├── Config/
│   ├── DefaultEngine.ini
│   └── DefaultGame.ini
├── .gitignore                        # Optimized for Unreal Engine (excludes Binaries, Saved, Intermediate)
├── .gitattributes                   # Git LFS configured for .uasset, .umap, and audio
└── first_unreal_engine_app.uproject
```

---

## 🚀 Getting Started

### 1. Requirements
- **Unreal Engine 5.7**
- **Visual Studio 2022 / Visual Studio Build Tools** (with *Game Development with C++*)
- **Git & Git LFS**

### 2. Opening the Project
1. Right-click `first_unreal_engine_app.uproject` and select **Generate Visual Studio project files**.
2. Open `first_unreal_engine_app.uproject` directly in **Unreal Engine 5.7** or open `first_unreal_engine_app.sln` in Visual Studio and build `Development Editor`.
3. In the Unreal Editor Content Browser, create a Blueprint derived from `FighterCharacter` (e.g. `BP_FighterCharacter`) and assign your animation montages:
   - `LightAttackMontages`
   - `HeavyAttackMontages`
   - `HitReactMontage`
   - `DodgeMontage`
   - `DeathMontage`

---

## 🕹️ Controls (Default Mapping)

| Action | Keyboard / Mouse | Gamepad |
| :--- | :--- | :--- |
| **Move** | `W` `A` `S` `D` | Left Stick |
| **Look / Aim** | Mouse | Right Stick |
| **Light Attack** | Left Click | Right Bumper (RB) |
| **Heavy Attack** | Right Click | Right Trigger (RT) |
| **Block** | Left Shift | Left Bumper (LB) |
| **Dodge** | Space | `A` / Cross Button |
| **Lock-On Target** | Middle Mouse / `Tab` | Right Stick Click (R3) |

---

## 📦 Git Setup & Push

```bash
git init
git add .
git commit -m "feat: complete 3D melee arena brawler C++ foundation"
git branch -M main
git remote add origin https://github.com/meriem-dev-esi/first_unreal_engine_app.git
git push -u origin main
```
