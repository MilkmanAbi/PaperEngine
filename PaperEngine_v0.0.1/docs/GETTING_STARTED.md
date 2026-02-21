# Getting Started with PaperEngine

This guide will walk you through creating your first paper-style game with PaperEngine.

## Installation

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
sudo apt-get install cmake build-essential
```

### macOS
```bash
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf cmake
```

### Windows
1. Download SDL2 development libraries from https://www.libsdl.org/
2. Install Visual Studio 2019 or later with C++ support
3. Set SDL2_DIR environment variable to your SDL2 installation

## Your First Game

Let's create a simple paper airplane game where you control a paper plane!

### Step 1: Project Setup

```cpp
#include "PaperEngine.h"
#include "Scene.h"
#include "Components.h"
#include "Input.h"

using namespace Paper;
```

### Step 2: Create Your Scene

```cpp
class PlaneScene : public Scene {
public:
    PlaneScene() : Scene("PlaneScene") {
        backgroundColor = Color(135, 206, 235); // Sky blue
    }

    void onEnter() override {
        // Create paper plane
        plane = createEntity("Plane");
        plane->transform()->position = Vector2(640, 360);
        plane->transform()->layer = 2;
        
        auto sprite = plane->addComponent<SpriteRenderer>();
        sprite->tint = Color::white();
        sprite->castShadow = true;
        
        auto rb = plane->addComponent<Rigidbody>();
        rb->useGravity = false; // Plane controls its own movement
        rb->drag = 0.5f;
        
        // Create clouds in background
        createCloud(200, 150);
        createCloud(600, 100);
        createCloud(1000, 180);
    }

    void update(float deltaTime) override {
        Scene::update(deltaTime);
        
        auto input = Engine::getInstance()->getInput();
        auto rb = plane->getComponent<Rigidbody>();
        
        // Control plane with arrow keys
        Vector2 force(0, 0);
        if (input->isKeyDown(Key::Up)) force.y = -500;
        if (input->isKeyDown(Key::Down)) force.y = 500;
        if (input->isKeyDown(Key::Left)) force.x = -500;
        if (input->isKeyDown(Key::Right)) force.x = 500;
        
        rb->addForce(force);
        
        // Tilt plane based on velocity
        plane->transform()->rotation = rb->velocity.x * 0.05f;
    }

private:
    Entity* plane;
    
    void createCloud(float x, float y) {
        auto cloud = createEntity("Cloud");
        cloud->transform()->position = Vector2(x, y);
        cloud->transform()->layer = 0; // Background
        
        auto sprite = cloud->addComponent<SpriteRenderer>();
        sprite->tint = Color(255, 255, 255, 180);
    }
};
```

### Step 3: Main Function

```cpp
int main() {
    Engine engine;
    
    Engine::Config config;
    config.windowTitle = "Paper Plane Adventure";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    config.vsync = true;
    
    if (!engine.initialize(config)) {
        return -1;
    }
    
    auto scene = std::make_unique<PlaneScene>();
    engine.getSceneManager()->addScene("plane", std::move(scene));
    engine.getSceneManager()->setActiveScene("plane");
    
    engine.run();
    return 0;
}
```

### Step 4: Build and Run

```bash
mkdir build && cd build
cmake ..
cmake --build .
./YourGame
```

## Common Patterns

### Creating Interactive Objects

```cpp
void createButton(const Vector2& pos, const std::string& label) {
    auto button = createEntity("Button");
    button->transform()->position = pos;
    button->transform()->layer = 10; // UI layer
    
    auto sprite = button->addComponent<SpriteRenderer>();
    sprite->tint = Color::cardboard();
    
    // Add interaction logic in update()
}
```

### Spawning Enemies

```cpp
Entity* spawnEnemy(const Vector2& position) {
    auto enemy = createEntity("Enemy");
    enemy->transform()->position = position;
    enemy->transform()->layer = 1;
    
    auto sprite = enemy->addComponent<SpriteRenderer>();
    sprite->tint = Color::red();
    
    auto rb = enemy->addComponent<Rigidbody>();
    rb->velocity = Vector2(-100, 0); // Move left
    
    auto collider = enemy->addComponent<BoxCollider>(32, 32);
    
    return enemy;
}
```

### Score System

```cpp
class ScoreComponent : public Component {
public:
    int score = 0;
    
    void addScore(int points) {
        score += points;
        std::cout << "Score: " << score << std::endl;
    }
};

// Usage
auto scoreManager = createEntity("ScoreManager");
auto score = scoreManager->addComponent<ScoreComponent>();
score->addScore(100);
```

### Camera Follow

```cpp
void createFollowCamera(Entity* target) {
    auto cameraEntity = createEntity("Camera");
    auto camera = cameraEntity->addComponent<Camera>();
    camera->smoothFollow = true;
    camera->followSpeed = 5.0f;
    
    // In update(), set camera position to target
}
```

## Tips for Paper Aesthetics

1. **Textures**: Use high-quality paper textures with visible grain
2. **Shadows**: Always enable shadows for depth (2-4px offset)
3. **Colors**: Stick to muted, paper-like colors - avoid pure whites and blacks
4. **Animation**: Keep it simple and snappy, like stop-motion
5. **Particles**: Use torn paper and confetti for visual feedback

## Next Steps

- Read the full API documentation
- Check out the advanced examples
- Join our community Discord
- Experiment with particle systems
- Try creating custom components

## Troubleshooting

**SDL2 not found:**
- Make sure SDL2 is installed and in your PATH
- On Windows, set SDL2_DIR environment variable

**Black screen:**
- Check that your scene has entities
- Verify sprite layers are set correctly
- Ensure camera is positioned properly

**Low FPS:**
- Reduce particle count
- Optimize collision checks
- Use object pooling for frequently spawned entities

## Resources

- [API Reference](API.md)
- [Example Projects](examples/)
- [Discord Community](https://discord.gg/paperengine)
- [Video Tutorials](https://youtube.com/paperengine)

Happy game making! 🎮📄
