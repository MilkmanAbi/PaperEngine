# PaperEngine Architecture

## System Overview

```
┌─────────────────────────────────────────────────────────────┐
│                         Application                         │
│                     (Your Game Code)                        │
└────────────────────────────┬────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────┐
│                       PaperEngine                           │
│                      (Core Systems)                         │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐    │
│  │   Scene      │  │   Entity     │  │  Component   │    │
│  │  Manager     │◄─┤   System     │◄─┤   System     │    │
│  └──────────────┘  └──────────────┘  └──────────────┘    │
│         │                  │                  │            │
│         └──────────────────┼──────────────────┘            │
│                            │                               │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐    │
│  │   Renderer   │  │    Input     │  │   Physics    │    │
│  │   (SDL2)     │  │   Manager    │  │    World     │    │
│  └──────────────┘  └──────────────┘  └──────────────┘    │
│         │                  │                  │            │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐    │
│  │   Resource   │  │    Audio     │  │   Timing     │    │
│  │   Manager    │  │   Manager    │  │   System     │    │
│  └──────────────┘  └──────────────┘  └──────────────┘    │
│                                                             │
└─────────────────────────────────────────────────────────────┘
                             │
                             ▼
┌─────────────────────────────────────────────────────────────┐
│                      SDL2 Library                           │
│         (Graphics, Audio, Input, Windowing)                 │
└─────────────────────────────────────────────────────────────┘
```

## Component Hierarchy

```
Component (Base Class)
├── Transform (Built-in)
├── SpriteRenderer
├── Animator
├── Rigidbody
├── Collider (Abstract)
│   ├── BoxCollider
│   └── CircleCollider
├── ParticleSystem
├── Camera
└── [Your Custom Components]
```

## Game Loop Flow

```
┌─────────────────────────────────────────────────────┐
│ 1. Process Events                                   │
│    - SDL_PollEvent()                                │
│    - Input system update                            │
└────────────────┬────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────┐
│ 2. Update Physics                                   │
│    - Integrate velocity                             │
│    - Check collisions                               │
│    - Resolve collisions                             │
│    - Integrate position                             │
└────────────────┬────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────┐
│ 3. Update Scene                                     │
│    - Update active scene                            │
│    - Update all entities                            │
│    - Call component update()                        │
└────────────────┬────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────┐
│ 4. Render                                           │
│    - Begin frame                                    │
│    - Sort render queue by layer                    │
│    - Execute render commands                        │
│    - End frame                                      │
│    - Present to screen                              │
└────────────────┬────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────┐
│ 5. Frame Timing                                     │
│    - Calculate delta time                           │
│    - Cap frame rate if needed                       │
│    - Update FPS counter                             │
└─────────────────────────────────────────────────────┘
```

## Entity Component System (ECS)

```
Scene
  └─ Entities[]
       └─ Entity
            ├─ Transform (always present)
            │    ├─ position
            │    ├─ rotation
            │    ├─ scale
            │    └─ layer
            │
            └─ Components{}
                 ├─ SpriteRenderer
                 ├─ Rigidbody
                 ├─ Collider
                 └─ Custom Components
```

## Rendering Pipeline

```
Game Objects
     │
     ▼
┌─────────────────────┐
│ Gather render data  │  → Components push to render queue
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Sort by layer/depth │  → Background to foreground
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Apply camera trans. │  → Position, zoom, shake
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Draw shadows first  │  → If enabled on sprite
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Draw sprites        │  → With tint, effects, rotation
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Draw particles      │  → Additive/Alpha blending
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│ Draw UI             │  → Top layer
└──────────┬──────────┘
           │
           ▼
      Present to screen
```

## Physics System

```
┌─────────────────────────────────────────────────────┐
│ Physics Update                                      │
├─────────────────────────────────────────────────────┤
│                                                     │
│  1. Apply Forces                                   │
│     - Gravity                                      │
│     - User forces                                  │
│     - Drag                                         │
│                                                     │
│  2. Integrate Velocity                             │
│     velocity += acceleration * dt                  │
│                                                     │
│  3. Collision Detection                            │
│     - Broad phase (spatial partitioning)           │
│     - Narrow phase (shape intersection)            │
│                                                     │
│  4. Collision Resolution                           │
│     - Calculate penetration                        │
│     - Calculate impulse                            │
│     - Apply impulse                                │
│     - Separate objects                             │
│                                                     │
│  5. Integrate Position                             │
│     position += velocity * dt                      │
│                                                     │
└─────────────────────────────────────────────────────┘
```

## Memory Management

- **Entities**: Owned by Scene (unique_ptr)
- **Components**: Owned by Entity (unique_ptr)
- **Resources**: Cached by ResourceManager (unique_ptr)
- **Scenes**: Managed by SceneManager (unique_ptr)

## Threading Model

Current: **Single-threaded**
- All updates on main thread
- Deterministic execution order

Future considerations:
- Job system for parallel entity updates
- Async resource loading
- Background audio streaming

## File Structure

```
PaperEngine/
├── include/          # Public API headers
│   ├── PaperEngine.h
│   ├── Entity.h
│   ├── Scene.h
│   ├── Components.h
│   ├── Physics.h
│   ├── Renderer.h
│   └── ...
├── src/              # Implementation files
│   ├── PaperEngine.cpp
│   ├── Entity.cpp
│   └── ...
├── examples/         # Example games
├── docs/             # Documentation
├── assets/           # Game assets
└── CMakeLists.txt    # Build configuration
```
