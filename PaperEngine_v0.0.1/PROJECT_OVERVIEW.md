# PaperEngine - Project Overview

## What is PaperEngine?

PaperEngine is a specialized C++ game engine designed for creating 2D games with a distinctive paper-craft aesthetic. Unlike general-purpose game engines, PaperEngine is optimized specifically for games that embrace flat, layered visuals reminiscent of paper cutouts, pop-up books, and paper-craft art.

## Design Philosophy

### 1. **Paper-First Aesthetics**
Every feature is designed with the paper aesthetic in mind:
- Layered rendering system (like stacked paper sheets)
- Built-in shadow effects for depth perception
- Paper-themed color palette
- Particle systems optimized for paper effects (confetti, torn pieces)

### 2. **Performance-Focused**
Written in modern C++17 with efficiency in mind:
- Fast 2D rendering with SDL2
- Optimized physics for 2D games
- Minimal overhead ECS (Entity Component System)
- Smart resource caching

### 3. **Developer-Friendly**
Clean, intuitive API that gets out of your way:
- Component-based architecture
- Clear separation of concerns
- Comprehensive documentation
- Practical examples

### 4. **Advanced but Approachable**
Suitable for both learning and production:
- Simple for basic games
- Powerful for complex projects
- Extensible component system
- Professional-grade architecture

## Core Systems

### Entity Component System (ECS)
Modern, flexible game object architecture where:
- **Entities** are containers for components
- **Components** define behavior and data
- **Systems** process components each frame
- Easy to extend with custom components

### Layered 2D Renderer
Specialized for paper aesthetics:
- Integer-based layer system (background to foreground)
- Automatic depth sorting
- Shadow rendering
- Camera system with shake effects
- Optimized sprite batching

### Physics Engine
Lightweight 2D physics perfect for platformers and puzzle games:
- Rigid body dynamics
- Box and circle colliders
- Collision detection and resolution
- Raycasting
- Spatial queries
- Adjustable gravity

### Resource Management
Intelligent asset handling:
- Texture loading and caching
- Font management (with SDL_ttf integration)
- Automatic resource cleanup
- Asset path management

### Audio System
Simple but powerful audio:
- Sound effects
- Music playback
- Volume control (master, sound, music)
- Looping support

### Input System
Comprehensive input handling:
- Keyboard state (pressed, down, released)
- Mouse buttons and position
- Mouse delta and wheel
- Text input support

## Built-in Components

### Rendering Components
- **SpriteRenderer**: Display textures with effects
- **Animator**: Frame-based sprite animation
- **ParticleSystem**: Paper-themed particle effects
- **Camera**: Camera control with follow and shake

### Physics Components
- **Rigidbody**: Physics simulation (static, kinematic, dynamic)
- **BoxCollider**: Rectangular collision shape
- **CircleCollider**: Circular collision shape

### Custom Components
Easy to create your own:
```cpp
class MyComponent : public Component {
    void update(float deltaTime) override {
        // Your logic here
    }
};
```

## Paper-Specific Features

### 1. Layer System
Simulate stacked paper sheets:
```cpp
entity->transform()->layer = 0;  // Background
entity->transform()->layer = 1;  // Middleground
entity->transform()->layer = 2;  // Foreground
```

### 2. Shadow Effects
Automatic drop shadows for depth:
```cpp
sprite->castShadow = true;
sprite->shadowOffset = Vector2(3, 3);
sprite->shadowColor = Color(0, 0, 0, 100);
```

### 3. Paper Colors
Pre-defined paper-themed colors:
```cpp
Color::paper()      // Cream/beige
Color::cardboard()  // Brown
Color::parchment()  // Aged paper
Color::ink()        // Dark blue-black
```

### 4. Paper Effects
Crumple and tear simulation:
```cpp
sprite->paperCrumple = 0.5f;  // 0=flat, 1=very crumpled
sprite->paperTear = 0.3f;     // 0=intact, 1=very torn
```

### 5. Particle Presets
Paper-themed particle effects:
```cpp
particles->setConfettiMode();    // Celebration
particles->setTornPaperMode();   // Destruction
particles->setDustMode();        // Ambient
```

## Technical Specifications

### Requirements
- C++17 or later
- CMake 3.15+
- SDL2 2.0+
- (Optional) SDL2_image, SDL2_mixer, SDL2_ttf

### Supported Platforms
- Linux (tested on Ubuntu 24.04)
- macOS (10.15+)
- Windows (10/11 with Visual Studio 2019+)

### Performance Characteristics
- **Target Frame Rate**: 60 FPS (configurable)
- **Rendering**: Hardware-accelerated via SDL2
- **Physics**: Sub-step integration for stability
- **Memory**: Minimal allocations during gameplay (uses object pooling internally)

### Architecture Patterns
- **ECS**: Entity Component System
- **Singleton**: Engine instance
- **Resource Caching**: Textures and audio
- **Command Pattern**: Render queue
- **Observer**: Component lifecycle hooks

## Project Structure

```
PaperEngine/
├── include/              # Public API headers
│   ├── PaperEngine.h    # Main engine class
│   ├── Entity.h         # Entity and Transform
│   ├── Scene.h          # Scene management
│   ├── Components.h     # Built-in components
│   ├── Physics.h        # Physics system
│   ├── Renderer.h       # Rendering system
│   ├── Resources.h      # Resource management
│   ├── Input.h          # Input handling
│   ├── Audio.h          # Audio system
│   ├── Vector2.h        # Math utilities
│   └── Math.h           # Color, Rectangle
│
├── src/                 # Implementation files
│   ├── PaperEngine.cpp
│   └── ...
│
├── examples/            # Example games
│   └── example_game.cpp
│
├── docs/                # Documentation
│   ├── GETTING_STARTED.md
│   ├── ARCHITECTURE.md
│   └── API.md
│
├── assets/              # Game assets directory
│
├── CMakeLists.txt       # Build configuration
├── README.md            # Project overview
├── LICENSE              # MIT License
└── .gitignore
```

## Use Cases

### Perfect For:
- **2D Platformers** with paper cutout characters
- **Puzzle Games** with paper-craft aesthetics
- **Adventure Games** in paper-style worlds
- **Educational Games** with clean, friendly visuals
- **Indie Games** seeking a unique art style
- **Game Jams** for rapid prototyping

### Not Suitable For:
- 3D games (use Unity, Unreal, or Godot)
- Realistic graphics (PaperEngine is stylized)
- Network multiplayer (no built-in networking)
- Mobile games (desktop-focused)

## Comparison to Other Engines

| Feature | PaperEngine | Unity | Godot | SDL2 |
|---------|-------------|-------|-------|------|
| Paper Focus | ✅ Yes | ❌ No | ❌ No | ❌ No |
| C++ API | ✅ Yes | ❌ No | ✅ Yes | ✅ Yes |
| ECS | ✅ Yes | ✅ Yes | ❌ No | ❌ No |
| 2D Physics | ✅ Built-in | ✅ Yes | ✅ Yes | ❌ No |
| Learning Curve | ⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐ |
| Performance | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| Flexibility | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐⭐ |

## Roadmap

### Version 1.0 (Current)
- ✅ Core ECS architecture
- ✅ 2D rendering with layers
- ✅ Physics system
- ✅ Input handling
- ✅ Resource management
- ✅ Audio system
- ✅ Built-in components
- ✅ Example game

### Version 1.1 (Planned)
- [ ] SDL_ttf text rendering
- [ ] Tilemap support
- [ ] Animation state machines
- [ ] Better collision callbacks
- [ ] Profiling tools

### Version 1.2 (Future)
- [ ] Particle editor
- [ ] Scene editor
- [ ] Asset pipeline
- [ ] Shader support
- [ ] Mobile platform support

### Version 2.0 (Vision)
- [ ] Visual scripting
- [ ] Network replication
- [ ] 3D paper layers
- [ ] Advanced paper physics (folding, tearing)

## Getting Involved

### Contributing
We welcome contributions! Areas we need help:
- Documentation improvements
- Bug fixes
- Example games
- Feature implementations
- Performance optimizations

### Community
- **Discord**: [Join our community]
- **GitHub**: [Report issues and contribute]
- **Twitter**: [@PaperEngine] - Follow for updates

### Support
- **Email**: support@paperengine.com
- **Discussions**: GitHub Discussions
- **Wiki**: Community-maintained wiki

## License

PaperEngine is released under the MIT License, which means:
- ✅ Free for commercial use
- ✅ Free for personal projects
- ✅ Modify and distribute freely
- ✅ No royalties or licensing fees
- ⚠️ Provided "as-is" without warranty

## Credits

**Created by**: PaperEngine Team
**Built with**: SDL2, C++17, CMake
**Inspired by**: Paper Mario, Tearaway, Kirby's Epic Yarn

Special thanks to all contributors and the SDL community!

---

## Quick Links

- 📚 [Documentation](docs/)
- 🎮 [Examples](examples/)
- 🔧 [API Reference](docs/API.md)
- 🏗️ [Architecture](docs/ARCHITECTURE.md)
- 🚀 [Getting Started](docs/GETTING_STARTED.md)

**Ready to start creating paper-craft games? Let's go! 🎨📄**
