# PaperEngine - Build Instructions & Quick Start

## 📦 What You Have

You now have the complete PaperEngine source code! This includes:
- ✅ Full C++ source code (headers + implementation)
- ✅ CMake build system
- ✅ Example game demonstrating all features
- ✅ Complete documentation (API, Architecture, Getting Started)
- ✅ MIT License (free for any use)

## 🔧 Prerequisites

Before building PaperEngine, you need:

### On Linux (Ubuntu/Debian):
```bash
sudo apt-get update
sudo apt-get install build-essential cmake git
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

### On macOS:
```bash
# Install Homebrew if you don't have it: https://brew.sh
brew install cmake sdl2 sdl2_image sdl2_mixer sdl2_ttf
```

### On Windows:
1. Install Visual Studio 2019 or later (Community edition is free)
   - Include "Desktop development with C++" workload
2. Install CMake: https://cmake.org/download/
3. Download SDL2 development libraries: https://www.libsdl.org/download-2.0.php
   - Get the "Development Libraries" (Visual C++)
   - Extract to a known location (e.g., C:\SDL2)
4. Set environment variable: `SDL2_DIR=C:\SDL2` (or your path)

## 🏗️ Building PaperEngine

### Step 1: Extract the Archive
```bash
tar -xzf PaperEngine.tar.gz
cd PaperEngine
```

### Step 2: Create Build Directory
```bash
mkdir build
cd build
```

### Step 3: Configure with CMake
```bash
# Linux/macOS:
cmake ..

# Windows (Visual Studio 2019):
cmake .. -G "Visual Studio 16 2019" -A x64

# Windows (Visual Studio 2022):
cmake .. -G "Visual Studio 17 2022" -A x64
```

### Step 4: Build
```bash
# Linux/macOS:
cmake --build . --config Release

# Or use make directly:
make -j4

# Windows:
cmake --build . --config Release
```

### Step 5: Run the Example
```bash
# Linux/macOS:
./PaperExample

# Windows:
.\Release\PaperExample.exe
```

## 🎮 Example Game Controls

The included example is a simple paper-style platformer:
- **A/D or Arrow Keys**: Move left/right
- **Space/W**: Jump
- **ESC**: Quit

## 📝 Project Structure

```
PaperEngine/
├── include/              # All header files (.h)
│   ├── PaperEngine.h    # Main engine
│   ├── Entity.h         # Game objects
│   ├── Scene.h          # Scene management
│   ├── Components.h     # Built-in components
│   ├── Physics.h        # 2D physics
│   ├── Renderer.h       # Rendering
│   ├── Resources.h      # Asset management
│   ├── Input.h          # Keyboard/mouse
│   ├── Audio.h          # Sound system
│   ├── Vector2.h        # Math types
│   └── Math.h           # Color, Rectangle
│
├── src/                 # Implementation files (.cpp)
│   └── PaperEngine.cpp  # Main engine implementation
│                        # (Other .cpp files to be added)
│
├── examples/            # Example games
│   └── example_game.cpp # Demo platformer
│
├── docs/                # Documentation
│   ├── GETTING_STARTED.md
│   ├── API.md
│   └── ARCHITECTURE.md
│
├── assets/              # Put your game assets here
│                        # (textures, sounds, fonts)
│
├── CMakeLists.txt       # Build configuration
├── README.md            # Overview
├── PROJECT_OVERVIEW.md  # Detailed features
├── LICENSE              # MIT License
└── .gitignore
```

## 🚀 Creating Your First Game

### 1. Create a New Game File

Create `my_game.cpp` in the `examples/` folder:

```cpp
#include "PaperEngine.h"
#include "Scene.h"
#include "Components.h"
#include "Input.h"

using namespace Paper;

class MyGameScene : public Scene {
public:
    MyGameScene() : Scene("MyGame") {
        backgroundColor = Color::paper();
    }

    void onEnter() override {
        // Create your game objects here
        auto player = createEntity("Player");
        player->transform()->position = Vector2(400, 300);
        player->transform()->layer = 1;
        
        auto sprite = player->addComponent<SpriteRenderer>();
        sprite->tint = Color::blue();
        sprite->castShadow = true;
    }

    void update(float deltaTime) override {
        Scene::update(deltaTime);
        
        // Your game logic here
    }
};

int main() {
    Engine engine;
    
    Engine::Config config;
    config.windowTitle = "My Paper Game";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    
    engine.initialize(config);
    
    auto scene = std::make_unique<MyGameScene>();
    engine.getSceneManager()->addScene("game", std::move(scene));
    engine.getSceneManager()->setActiveScene("game");
    
    engine.run();
    return 0;
}
```

### 2. Update CMakeLists.txt

Add your game to the bottom of `CMakeLists.txt`:

```cmake
# Your game executable
add_executable(MyGame examples/my_game.cpp)
target_link_libraries(MyGame PRIVATE PaperEngine)
```

### 3. Rebuild

```bash
cd build
cmake ..
cmake --build . --config Release
./MyGame  # or .\Release\MyGame.exe on Windows
```

## 📚 Next Steps

1. **Read the Documentation**:
   - Start with `docs/GETTING_STARTED.md`
   - Reference `docs/API.md` when coding
   - Understand the architecture in `docs/ARCHITECTURE.md`

2. **Study the Example**:
   - Open `examples/example_game.cpp`
   - See how entities, components, and scenes work
   - Experiment by modifying it

3. **Create Custom Components**:
   ```cpp
   class MyComponent : public Component {
   public:
       void update(float deltaTime) override {
           // Your behavior here
       }
   };
   ```

4. **Add Your Assets**:
   - Put textures in `assets/textures/`
   - Put sounds in `assets/sounds/`
   - Load them with ResourceManager

5. **Join the Community**:
   - GitHub: Report issues, contribute
   - Discord: Ask questions, share projects

## 🐛 Troubleshooting

### "SDL2 not found" Error
- **Linux**: Install with `sudo apt-get install libsdl2-dev`
- **macOS**: Install with `brew install sdl2`
- **Windows**: Make sure SDL2_DIR environment variable is set

### Linking Errors
- Make sure all SDL2 libraries are installed (SDL2, SDL2_image, SDL2_mixer, SDL2_ttf)
- On Windows, ensure you downloaded the correct architecture (x64)

### Black Screen
- Check that your scene creates entities
- Verify entity layers are set
- Make sure sprite components are added

### Performance Issues
- Build in Release mode: `cmake --build . --config Release`
- Enable VSync in engine config
- Reduce particle counts

## 💡 Tips for Paper Aesthetics

1. **Use Layers Effectively**:
   - Background: layer = 0
   - Game objects: layer = 1
   - Player: layer = 2
   - UI: layer = 10

2. **Enable Shadows**:
   ```cpp
   sprite->castShadow = true;
   sprite->shadowOffset = Vector2(3, 3);
   ```

3. **Use Paper Colors**:
   ```cpp
   Color::paper()      // Cream background
   Color::cardboard()  // Brown objects
   Color::ink()        // Text/outlines
   ```

4. **Add Subtle Rotation**:
   ```cpp
   entity->transform()->rotation = 5.0f; // Slight tilt
   ```

5. **Use Particle Effects**:
   ```cpp
   auto particles = entity->addComponent<ParticleSystem>();
   particles->setConfettiMode();
   particles->emit(50);
   ```

## 🎯 Example Projects to Try

1. **Platformer**: Character jumping between paper platforms
2. **Puzzle Game**: Sliding paper pieces to solve puzzles
3. **Flying Game**: Paper airplane dodging obstacles
4. **Adventure Game**: Top-down exploration in paper world
5. **Tower Defense**: Paper enemies and defenses

## 📖 Additional Resources

- Full API Reference: `docs/API.md`
- Architecture Details: `docs/ARCHITECTURE.md`
- Getting Started Guide: `docs/GETTING_STARTED.md`
- Project Overview: `PROJECT_OVERVIEW.md`

## 🤝 Contributing

Want to improve PaperEngine? We welcome:
- Bug fixes
- New features
- Documentation improvements
- Example games
- Performance optimizations

## 📄 License

PaperEngine is MIT licensed - free for any use!
See the LICENSE file for full details.

---

**Happy game development! 🎮📄**

Need help? Check the docs or reach out to the community!
