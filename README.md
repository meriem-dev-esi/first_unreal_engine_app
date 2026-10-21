# 🥊🔥 first_unreal_engine_app — 3D Hybrid Brawler & Tactical Shooter

A high-intensity 3D Arena Combat game developed in **Unreal Engine 5.7** combining **fluid Melee Martial Arts combos**, **Tactical Ballistic Gunplay**, **Bullet Time Slow-Motion**, **Parkour Movement**, **Wave-Based Survival**, and **Interactive Environmental Physics**.

---

## 🌟 Game Highlights & Deep Gameplay Systems

### 1. ⚔️ Hybrid Combat Engine (Melee + Ballistic Gunplay)
- **Melee Combo Strings**: Multi-hit light punch/kick combos with hit-confirm cancels and hitstop impact effects.
- **Heavy / Charged Knockdown Attacks**: Charge up strikes to break enemy guard and trigger physics ragdoll knockdowns.
- **Super Ground Slam (Ultimate `Q`)**: Radial seismic shockwave unleashing persistent ground fire pools that launch enemies into the air and set them ablaze with burning Damage-Over-Time.
- **Cluster Fire Bombs (`AClusterFireProjectile`)**: High-impact mortar shells that detonate on contact and scatter 4 secondary incendiary bomblets in a wide radial starburst.
- **Burning Status Effect (`UBurningStatusComponent`)**: Targets caught in fire pools or cluster blasts suffer continuous burn ticks (15–30 DPS) and visual fire effects.
- **Lingering Fire Hazard Pools (`AFireFieldHazard`)**: Persistent fiery napalm pools (8s duration) that ignite anyone walking through them.

### 2. ⏳ Bullet Time & Tactical Focus (`UTacticalTimeDilationComponent`)
- **Cinematic Slow-Motion**: Slows down the entire game world to **0.25x speed** while keeping the player character moving at full speed to weave between incoming bullets, aim headshots, and dodge cluster explosions.
- **Focus Energy Bar**: Consumes Focus Energy while active (20 energy/s) and automatically regenerates when deactivated.
- **Instant Activation**: Triggered with `E` or `Middle Mouse Click`.

### 3. 🎒 4-Slot Weapon Arsenal & Tactical Reload (`UCombatInventoryComponent`)
- **Slot 1 (Primary)**: Assault Rifle (30-round mag, 650 RPM, high-velocity suppression).
- **Slot 2 (Secondary)**: Tactical Pistol (15-round mag, semi-auto, high accuracy).
- **Slot 3 (Heavy)**: Cluster Fire Launcher (4-round heavy mortar with incendiary starbursts).
- **Slot 4 (Melee)**: Martial Arts Brawling (Unarmed combo strings with infinite stamina combo flow).
- **Tactical Reloading (`R`)**: Auto-reloads when magazines run empty or on manual command with reserve ammo tracking.
- **Quick Switching**: Switch via number keys `1`, `2`, `3`, `4` or Mouse Scroll Wheel.

### 4. 🤸 Combat Parkour & Acrobatic Mobility (`UCombatParkourMovementComponent`)
- **Double Jump**: Press `Spacebar` while airborne to perform an acrobatic vertical boost.
- **Air Dash**: Press `Left Shift` in mid-air to blast forward with a kinetic burst of velocity.
- **Combat Slide**: Press `C` / `Ctrl` while running to slide under fire with reduced friction while continuing to shoot!

### 5. 📡 360° Tactical Radar Scanner (`UCombatRadarComponent`)
- **Real-Time Threat Detection**: Continuously sweeps a **3,500 unit radius** around the player.
- **Threat Categorization**: Identifies Shooter NPCs, Melee Brawlers, Elite Bosses, dropped Powerups, and explosive hazard barrels.
- **Minimap / HUD Feeds**: Dispatches real-time ping data with world coordinates and threat distances for on-screen HUD markers.

### 6. 🌊 Wave Survival System (`AWaveSurvivalGameMode`)
- **Dynamic Wave Progression**: Survive escalating waves (Waves 1 through 10) featuring diverse enemy compositions.
- **Wave Scaling**: Enemies gain progressively increased health, aggression, and movement speed each round.
- **Boss Rounds**: Every 5th wave spawns **Annihilator Prime** — an elite boss with 1.5x scale, massive health pool, and ground-pound abilities.
- **Intermission & Pacing**: Tactical 5-second breathers between waves to scavenge for health and reposition.

### 7. 🎯 Combo Score & Style Ranking System
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

### 8. 🛢️ Interactive Physics Hazards & Nature
- **Explosive Barrels (`AExplosiveBarrelActor`)**: Detonate when shot or punched, dealing 120 radial damage and applying 2500 force physics impulse.
- **Realistic 3D Foliage Trees (`HillTree_02`)**: High-fidelity oak trees with branch shaders and wind movement.
- **Lit Green Terrain & Lumen Lighting**: Vibrant emerald arena ground with indirect diffuse illumination and bloom.
- **Jump Launch Pads (`BP_JumpPad`)**: Propel your character to high vantage platforms for aerial dive-punches and sniper angles.
- **Environmental Lava Pits (`BP_Combat_LavaFloor`)**: Knock enemies off arena ledges into hazard zones for instant finishes.

### 9. 💊 Collectible Power-ups & Field Loot (`ACombatPowerupActor`)
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
| **Bullet Time Slow-Motion** | `E` / `Middle Mouse Button` | Left Stick Click (`L3`) |
| **Super Ground Slam (Ultimate)** | `Q` | `Y` / `Triangle` |
| **Jump / Double Jump** | `Spacebar` *(tap twice in air)* | `A` / `Cross` |
| **Air Dash / Sprint** | `Left Shift` | `B` / `Circle` |
| **Combat Slide** | `C` / `Ctrl` *(while moving)* | Right Stick Click (`R3`) |
| **Reload Weapon** | `R` | `X` / `Square` |
| **Weapon Slot 1 (Rifle)** | `1` | D-Pad Up |
| **Weapon Slot 2 (Pistol)** | `2` | D-Pad Right |
| **Weapon Slot 3 (Cluster)** | `3` | D-Pad Down |
| **Cycle Weapon** | Mouse Scroll Wheel Up / Down | D-Pad Left |
| **Guard / Block** | `F` | Left Bumper (`LB`) |
| **Lock-On Target** | `Tab` | Right Stick Click (`R3`) |

---

## 🏗️ Repository Architecture

```
first_unreal_engine_app/
├── Config/
│   ├── DefaultEngine.ini             # Maps, CoreRedirects, Lumen & Substrate rendering
│   ├── DefaultGame.ini               # Project identity & description
│   └── DefaultInput.ini              # Tactical actions (Bullet Time, Slide, Weapon Slots)
├── Content/
│   ├── Foliage/                      # Realistic 3D Trees (HillTree_02), branches, leaves
│   ├── Materials/                    # Lit green terrain, ProcGrid materials
│   ├── Variant_Shooter/              # Guns (Rifle, Pistol, Launcher), Projectiles, Shooter AI
│   ├── Variant_Combat/               # Combo animations, Enemy brawlers, Camera shakes, VFX
│   ├── LevelPrototyping/             # Jump pads, wobble targets, destructibles
│   └── UI/                           # Dynamic health bars and combat HUD
├── _Source_Cpp_Reference/
│   └── first_unreal_engine_app/
│       ├── FighterTypes.h            # Weapon slots, Radar types, Wave states, Ranks
│       ├── FighterCharacter.h/.cpp   # Melee, Stamina, Health, Ultimate, Target Lock
│       ├── TacticalTimeDilationComponent.h/.cpp # Bullet time slow-motion & focus energy
│       ├── CombatInventoryComponent.h/.cpp      # 4-slot weapon arsenal & tactical reload
│       ├── CombatParkourMovementComponent.h/.cpp# Double jump, air dash, combat slide
│       ├── CombatRadarComponent.h/.cpp          # 360-degree tactical threat radar scanner
│       ├── WaveSurvivalGameMode.h/.cpp          # Wave progression, difficulty scaling, bosses
│       ├── BurningStatusComponent.h/.cpp        # Burn damage-over-time status effects
│       ├── ClusterFireProjectile.h/.cpp         # Multi-stage cluster firebomb mortars
│       ├── FireFieldHazard.h/.cpp               # Persistent napalm fire pools
│       ├── ExplosiveBarrelActor.h/.cpp          # Destructible explosive barrel
│       └── CombatPowerupActor.h/.cpp            # Rotating collectible powerups
└── first_unreal_engine_app.uproject
```

---

## 📦 Version Control & Assets
- Full Git LFS tracking for `.uasset`, `.umap`, and audio files.
- Main branch: [meriem-dev-esi/first_unreal_engine_app](https://github.com/meriem-dev-esi/first_unreal_engine_app).
