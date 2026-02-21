# PaperEngine 🎨📄

**Version 0.0.1 - A specialized C++ game engine for paper-aesthetic games**

PaperEngine is built for creating games with a distinctive paper-craft visual style—layered 2D graphics, stylized lighting, hand-drawn animation, and smooth-as-butter 60 FPS performance.

## ✨ Features (v0.0.1)

### Core Systems (IMPLEMENTED)
- **Advanced Entity Component System (ECS)** - Flexible, modular game object architecture
- **Layered 2D Renderer** - Paper-sheet style depth layering with shadow support
- **2D Physics Engine** - Optimized collision detection and rigid body dynamics
- **Scene Management** - Easy scene transitions and organization
- **Resource Management** - Efficient asset loading and caching
- **Audio System** - Sound effects and music playback
- **Input System** - Comprehensive keyboard and mouse handling

### Advanced Features (STUB IMPLEMENTATIONS - See ROADMAP.md)
- 🔹 **Layered parallax with depth** - Multi-plane scrolling with automatic depth scaling
- 🔹 **Paper fold/bend shaders** - OpenGL/Vulkan-based deformation effects
- 🔹 **Soft drop shadows** - Gaussian-blurred, multi-pass shadow rendering
- 🔹 **Super tight frame pacing** - 60 FPS lock with <1ms jitter, motion interpolation
- 🔹 **Hand-drawn animation pipeline** - Frame wobble, onion skinning, state machines
- 🔹 **Stylized lighting** - Rim lights, cel shading, paper-specific illumination
- 🔹 **Rotoscoping/3D backgrounds** - Video playback, pre-rendered 3D, perspective grids

### Paper-Specific Features
- **Layer-Based Rendering** - Simulate stacked paper sheets
- **Shadow Effects** - Automatic drop shadows for depth perception
- **Paper Color Palette** - Built-in paper-themed colors (paper, cardboard, parchment, ink)
- **Paper Effects** - Crumple, tear, and shadow effects (v0.0.1: basic support)
- **Particle Systems** - Confetti, torn paper, dust effects

### Built-in Components
- `SpriteRenderer` - Render textures with paper effects
- `Animator` - Frame-based sprite animation
- `Rigidbody` - Physics simulation (Static, Kinematic, Dynamic)
- `BoxCollider` & `CircleCollider` - Collision shapes
- `ParticleSystem` - Paper-themed particle effects
- `Camera` - Camera control with shake effects

## ⚠️ v0.0.1 NOTICE

**This is version 0.0.1** - The core engine is functional, but advanced features (parallax, shaders, lighting, 3D backgrounds, frame pacing) have **stub implementations only**.

**See `ROADMAP.md` for the full implementation plan!**

These stubs provide the architecture and API surface for:
- Multi-layer parallax scrolling
- Paper fold/bend shaders (OpenGL/Vulkan)  
- Soft Gaussian shadows
- 60 FPS frame pacing with <1ms jitter
- Hand-drawn animation with wobble
- Stylized lighting (rim, cel, ambient)
- 3D background rendering and rotoscoping

All the headers are in place. The classes are defined. The APIs are ready. Now it's time to fill in the implementations! 🚀

## 🚀 Quick Start

### Prerequisites
- C++17 or later compiler
- CMake 3.15+
- SDL2 development libraries

### Building

```bash
# Clone the repository
git clone https://github.com/yourusername/PaperEngine.git
cd PaperEngine

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .

# Run example
./PaperExample
```

### Basic Example

```cpp
#include "PaperEngine.h"
#include "Scene.h"
#include "Components.h"

using namespace Paper;

class MyScene : public Scene {
public:
    void onEnter() override {
        // Create a player entity
        auto player = createEntity("Player");
        player->transform()->position = Vector2(400, 300);
        player->transform()->layer = 2; // Front layer
        
        // Add sprite renderer
        auto sprite = player->addComponent<SpriteRenderer>();
        sprite->texture = /* your texture */;
        sprite->castShadow = true;
        
        // Add physics
        auto rb = player->addComponent<Rigidbody>();
        auto collider = player->addComponent<BoxCollider>(32, 48);
    }
    
    void update(float deltaTime) override {
        Scene::update(deltaTime);
        
        // Game logic here
        auto input = Engine::getInstance()->getInput();
        if (input->isKeyDown(Key::Space)) {
            // Do something
        }
    }
};

int main() {
    Engine engine;
    
    Engine::Config config;
    config.windowTitle = "My Paper Game";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    
    engine.initialize(config);
    
    auto scene = std::make_unique<MyScene>();
    engine.getSceneManager()->addScene("main", std::move(scene));
    engine.getSceneManager()->setActiveScene("main");
    
    engine.run();
    return 0;
}
```

## 📚 Architecture

### Engine Structure
```
PaperEngine
├── Core Engine Loop (timing, updates, rendering)
├── Renderer (layered 2D graphics with SDL2)
├── Scene Manager (scene transitions)
├── Resource Manager (texture/audio caching)
├── Input Manager (keyboard/mouse)
├── Physics World (2D collision/rigid bodies)
└── Audio Manager (sound/music)
```

### Entity Component System
- **Entity**: Game object container
- **Component**: Modular behavior attachment
- **Transform**: Every entity has position, rotation, scale, and layer
- **Scene**: Collection of entities with lifecycle management

### Rendering Layers
PaperEngine uses an integer-based layer system for z-ordering:
- Lower numbers render first (background)
- Higher numbers render last (foreground)
- Example: Background=-1, Middleground=0, Player=1, UI=2

## 🎮 Example Projects

### Platformer
Classic paper-cutout style platformer with jumping, moving platforms, and collectibles.

### Puzzle Game
Slide paper pieces to solve puzzles with layered graphics.

### Adventure Game
Top-down adventure with paper-craft characters and environments.

## 🛠️ Advanced Features

### Custom Components

```cpp
class HealthComponent : public Component {
public:
    int health = 100;
    
    void takeDamage(int amount) {
        health -= amount;
        if (health <= 0) {
            getEntity()->destroy();
        }
    }
    
    void update(float deltaTime) override {
        // Component update logic
    }
};

// Usage
auto health = entity->addComponent<HealthComponent>();
health->takeDamage(25);
```

### Particle Effects

```cpp
// Confetti explosion
auto particles = entity->addComponent<ParticleSystem>();
particles->setConfettiMode();
particles->emit(50);

// Torn paper effect
particles->setTornPaperMode();
particles->startVelocity = Vector2(0, -100);
```

### Physics Callbacks

```cpp
// Collision detection (in physics update loop)
void onCollision(Entity* other) {
    if (other->getName() == "Enemy") {
        // Handle collision
    }
}
```

## 🎨 Paper Aesthetic Guidelines

To achieve the best paper-like look:

1. **Use Layers Effectively** - Separate foreground, middleground, and background
2. **Enable Shadows** - Adds depth perception
3. **Paper Color Palette** - Use `Color::paper()`, `Color::cardboard()`, etc.
4. **Slight Rotations** - Give static objects small random tilts (±5-15°)
5. **Crumple/Tear Effects** - Add texture to sprites with `paperCrumple` and `paperTear`
6. **Particle Systems** - Use confetti and torn paper particles for visual flair

## 📖 API Reference

### Engine Class
- `initialize(config)` - Initialize engine
- `run()` - Start main loop
- `shutdown()` - Cleanup
- `quit()` - Exit game loop

### Entity Class
- `addComponent<T>()` - Attach component
- `getComponent<T>()` - Retrieve component
- `transform()` - Access transform
- `destroy()` - Mark for destruction

### Input Manager
- `isKeyPressed(key)` - Key just pressed this frame
- `isKeyDown(key)` - Key held down
- `isKeyReleased(key)` - Key just released this frame
- `getMousePosition()` - Mouse coordinates

### Physics World
- `gravity` - Global gravity vector
- `raycast()` - Cast ray for collisions
- `overlapCircle()` - Find entities in radius

## 🤝 Contributing

Contributions are welcome! Areas for improvement:
- Additional paper effects (folding, burning, aging)
- More built-in components
- Tilemap support
- Animation blending
- Shader support for advanced effects

## 📄 License

MIT License - Feel free to use PaperEngine in your projects!

## 🎯 Development Roadmap

See **`ROADMAP.md`** for the complete feature implementation plan with detailed guides!

### Version 0.0.1 (Current)
- ✅ Core engine architecture
- ✅ **Stub implementations for all advanced features**
- ✅ Parallax, shaders, lighting, 3D backgrounds (stubs)
- ✅ Frame pacing, hand-drawn animation (stubs)

### Version 0.5 (Next - 3 months)
- [ ] Full parallax rendering
- [ ] Soft shadow implementation
- [ ] 60 FPS frame pacing
- [ ] Hand-drawn animation wobble
- [ ] Basic stylized lighting

### Version 1.0 (Target - 6-8 months)
- [ ] Paper fold/bend shaders (OpenGL/Vulkan)
- [ ] Advanced lighting (rim, cel, AO)
- [ ] 3D background rendering
- [ ] Rotoscoping pipeline
- [ ] Level editor
- [ ] Complete documentation

**Priority:** Start with parallax, shadows, and frame pacing for maximum impact!

## 🎯 Roadmap

- [ ] Text rendering with SDL_ttf
- [ ] Tilemap/grid system
- [ ] Animation state machines
- [ ] Dialogue system
- [ ] Save/load system
- [ ] Shader effects
- [ ] Editor tools

## 💬 Community

- Discord: [Join our server]
- Twitter: [@PaperEngine]
- Email: dev@paperengine.com

---

**Built with ❤️ for paper-craft game developers**
