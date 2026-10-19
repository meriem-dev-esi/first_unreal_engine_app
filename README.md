# 🥊🔥 first_unreal_engine_app — 3D Hybrid Brawler & Tactical Shooter

A high-intensity 3D Arena Combat game developed in **Unreal Engine 5.7** combining **fluid Melee Martial Arts combos**, **Tactical Ballistic Gunplay**, **Wave-Based Enemy Survival**, and **Interactive Environmental Physics**.

---

## 🌟 Game Highlights & Deep Gameplay Systems

### 1. ⚔️ Hybrid Combat Engine (Melee + Ballistic Gunplay)
- **Melee Combo Strings**: Multi-hit light punch/kick combos with hit-confirm cancels and hitstop impact effects.
- **Heavy / Charged Knockdown Attacks**: Charge up strikes to break enemy guard and trigger physics ragdoll knockdowns.
- **Super Ground Slam (Ultimate)**: Radial shockwave (800 unit blast radius) dealing 150 area damage and launching surrounding enemies into the air.
- **Tactical Firearms**:
  - **Tactical Pistol**: Fast semi-auto sidearm with rapid reload.
  - **Assault Rifle**: High-velocity automatic rifle for continuous suppression.
  - **Grenade Launcher**: Arc-projectile explosive weapon triggering high-yield AoE blast VFX and camera shake.
  - **Cluster Fire Bombs (`AClusterFireProjectile`)**: High-impact mortar shells that detonate on contact and scatter 4 secondary incendiary bomblets in a wide radial starburst.
  - **Hellfire Ground Slam (`SuperAttack`)**: Radial seismic shockwave unleashing persistent ground fire pools that launch enemies into the air and set them ablaze with burning Damage-Over-Time.
  - **Burning Status Effect (`UBurningStatusComponent`)**: Targets caught in fire pools or cluster blasts suffer continuous burn ticks (15–30 DPS) and visual fire effects.
  - **Lingering Fire Hazard Pools (`AFireFieldHazard`)**: Persistent fiery napalm pools (8s duration) that ignite anyone walking through them.

### 2. 🌊 Wave Survival System (`AWaveSurvivalGameMode`)
- **Dynamic Wave Progression**: Survive escalating waves (Waves 1 through 10) featuring diverse enemy compositions.
- **Wave Scaling**: Enemies gain progressively increased health, aggression, and movement speed each round.
- **Boss Rounds**: Every 5th wave spawns **Annihilator Prime** — an elite boss with 1.5x scale, massive health pool, and ground-pound abilities.
- **Intermission & Pacing**: Tactical 5-second breathers between waves to scavenge for health and reposition.

### 3. 🎯 Combo Score & Style Ranking System
- **Combo Multipliers**: Fast consecutive eliminations build your combo multiplier:
  - 1–4 Kills: **x1.0 Multiplier**
  - 5–9 Kills: **x1.5 Multiplier**
  - 10–14 Kills: **x2.0 Multiplier**
  - 15+ Kills: **x3.0 Multiplier (Berserk Mode)**
- **Combat Ranks**: Earn ranks based on total combat performance:
  - **Rank D**: Beginner
  - **Rank C**: Fighter
  - **Rank B**: Warrior
  - **Rank A**: Champion
  - **Rank S**: Legendary
  - **Rank SSS**: Godlike

### 4. 🛢️ Interactive Physics Hazards & Arena Verticality
- **Explosive Barrels (`AExplosiveBarrelActor`)**: Detonate when shot with bullets, struck with heavy punches, or caught in grenade blast. Deals 120 radial damage and applies 2500 force physics impulse to launch debris and ragdolls.
- **Jump Launch Pads (`BP_JumpPad`)**: Propel your character to high vantage platforms for aerial dive-punches and sniper angles.
- **Environmental Lava Pits (`BP_Combat_LavaFloor`)**: Knock enemies off arena ledges into hazard zones for instant environmental finishes.

### 5. 💊 Collectible Power-ups & Field Loot (`ACombatPowerupActor`)
Defeated enemies have a 25% chance to spawn rotating 3D pickups:
- 🟢 **Health Medkit**: Instantly restores +50 HP.
- 🔵 **Armor Shield**: Provides damage reduction overshield.
- 🔴 **Double Damage**: Grants 2x damage output for 15 seconds.
- ⚡ **Speed Surge**: +50% movement speed for hyper-mobility.
- ⭐ **Instant Super**: Resets Ultimate Ground Slam cooldown instantly.

---

## 🗺️ Playable Levels

| Level Map | Description | How to Play |
| :--- | :--- | :--- |
| **`Lvl_ArenaShooter`** | Full 3D combat arena featuring weapon pickups, AI snipers, platform verticality, and full damage/ragdoll physics. | Default startup map or double-click in `Content/Variant_Shooter/` |
| **`Lvl_Combat`** | Dedicated martial arts brawling dojo featuring damageable boxes, dummy targets, lava hazards, and combo brawlers. | Double-click in `Content/Variant_Combat/` |
| **`Lvl_ThirdPerson`** | Open arena sandbox level for custom placement and testing. | Double-click in `Content/ThirdPerson/` |

---

## 🕹️ Controls

| Action | Keyboard / Mouse | Gamepad |
| :--- | :--- | :--- |
| **Move** | `W` `A` `S` `D` | Left Stick |
| **Look / Aim** | Mouse | Right Stick |
| **Fire Weapon / Light Punch** | **Left Mouse Button (LMB)** | Right Trigger (`RT`) |
| **Aim Down Sights / Heavy Attack** | **Right Mouse Button (RMB)** | Left Trigger (`LT`) |
| **Super Ground Slam (Ultimate)** | `Q` | `Y` / `Triangle` |
| **Jump / Launch Pad Bounce** | `Spacebar` | `A` / `Cross` |
| **Sprint / Dodge Roll** | `Left Shift` | `B` / `Circle` |
| **Lock-On Target** | `Middle Mouse Button` / `Tab` | Right Stick Click (`R3`) |
| **Guard / Block** | `F` | Left Bumper (`LB`) |

---

## 🏗️ Repository Architecture

```
first_unreal_engine_app/
├── Config/
│   ├── DefaultEngine.ini             # Maps, CoreRedirects, RHI & Substrate rendering
│   ├── DefaultGame.ini               # Project identity & description
│   └── DefaultInput.ini              # Enhanced Input action mappings
├── Content/
│   ├── Variant_Shooter/              # Guns (Rifle, Pistol, Launcher), Projectiles, Shooter AI
│   ├── Variant_Combat/               # Combo animations, Enemy brawlers, Camera shakes, VFX
│   ├── LevelPrototyping/             # Jump pads, wobble targets, destructibles
│   └── UI/                           # Dynamic health bars and combat HUD
├── _Source_Cpp_Reference/
│   └── first_unreal_engine_app/
│       ├── FighterTypes.h            # Combat states, Wave states, Powerups, Ranks & Structs
│       ├── FighterCharacter.h/.cpp   # Melee, Stamina, Health, Ultimate, Target Lock
│       ├── WaveSurvivalGameMode.h/.cpp# Dynamic wave spawning, scaling, boss rounds, scoring
│       ├── ExplosiveBarrelActor.h/.cpp# Destructible explosive barrel with physics impulse
│       └── CombatPowerupActor.h/.cpp # Rotating collectible powerups (Health, Damage, Speed)
└── first_unreal_engine_app.uproject
```

---

## 📦 Version Control & Assets
- Full Git LFS tracking for `.uasset`, `.umap`, and audio files.
- Main branch: [meriem-dev-esi/first_unreal_engine_app](https://github.com/meriem-dev-esi/first_unreal_engine_app).
