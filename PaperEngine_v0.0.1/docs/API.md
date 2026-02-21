# PaperEngine API Reference

## Core Classes

### Engine

Main engine class that manages all subsystems.

```cpp
class Engine {
public:
    struct Config {
        std::string windowTitle = "PaperEngine";
        int windowWidth = 1280;
        int windowHeight = 720;
        bool fullscreen = false;
        bool vsync = true;
        int targetFPS = 60;
        std::string assetPath = "assets/";
    };

    bool initialize(const Config& config);
    void run();
    void shutdown();
    void quit();

    // Subsystem access
    Renderer* getRenderer();
    InputManager* getInput();
    SceneManager* getSceneManager();
    ResourceManager* getResourceManager();
    AudioManager* getAudioManager();
    PhysicsWorld* getPhysicsWorld();

    // Time
    float getDeltaTime() const;
    float getTotalTime() const;
    int getFPS() const;

    // Singleton
    static Engine* getInstance();
};
```

### Entity

Game object that holds components.

```cpp
class Entity {
public:
    // Component management
    template<typename T, typename... Args>
    T* addComponent(Args&&... args);
    
    template<typename T>
    T* getComponent();
    
    template<typename T>
    bool hasComponent() const;
    
    template<typename T>
    void removeComponent();

    // Properties
    Transform* transform();
    const std::string& getName() const;
    void setName(const std::string& name);
    bool isActive() const;
    void setActive(bool value);
    
    void destroy();
};
```

### Scene

Container for entities and game logic.

```cpp
class Scene {
public:
    // Lifecycle
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float deltaTime);
    virtual void render();

    // Entity management
    Entity* createEntity(const std::string& name = "Entity");
    void destroyEntity(Entity* entity);
    void clearEntities();
    Entity* findEntityByName(const std::string& name) const;

    // Properties
    Color backgroundColor;
};
```

### Component

Base class for all components.

```cpp
class Component {
public:
    virtual void onAttach();
    virtual void onDetach();
    virtual void update(float deltaTime);
    
    Entity* getEntity() const;
    bool isEnabled() const;
    void setEnabled(bool value);
};
```

## Math Types

### Vector2

```cpp
struct Vector2 {
    float x, y;
    
    Vector2();
    Vector2(float x, float y);
    
    // Operators
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;
    
    // Methods
    float length() const;
    float lengthSquared() const;
    Vector2 normalized() const;
    void normalize();
    float dot(const Vector2& other) const;
    float cross(const Vector2& other) const;
    float distance(const Vector2& other) const;
    Vector2 lerp(const Vector2& other, float t) const;
    
    // Static helpers
    static Vector2 zero();
    static Vector2 one();
    static Vector2 up();
    static Vector2 down();
    static Vector2 left();
    static Vector2 right();
};
```

### Color

```cpp
struct Color {
    unsigned char r, g, b, a;
    
    Color();
    Color(unsigned char r, unsigned char g, unsigned char b, 
          unsigned char a = 255);
    
    // Common colors
    static Color white();
    static Color black();
    static Color red();
    static Color green();
    static Color blue();
    
    // Paper colors
    static Color paper();
    static Color cardboard();
    static Color parchment();
    static Color ink();
    
    Color withAlpha(unsigned char alpha) const;
    Color lerp(const Color& other, float t) const;
};
```

### Rectangle

```cpp
struct Rectangle {
    float x, y, width, height;
    
    Rectangle();
    Rectangle(float x, float y, float width, float height);
    Rectangle(const Vector2& position, const Vector2& size);
    
    // Accessors
    float left() const;
    float right() const;
    float top() const;
    float bottom() const;
    Vector2 position() const;
    Vector2 size() const;
    Vector2 center() const;
    
    // Collision
    bool contains(const Vector2& point) const;
    bool intersects(const Rectangle& other) const;
    Rectangle intersection(const Rectangle& other) const;
};
```

## Built-in Components

### Transform

```cpp
class Transform : public Component {
public:
    Vector2 position;
    float rotation; // Degrees
    Vector2 scale;
    int layer; // Z-order
    
    Vector2 forward() const;
    Vector2 right() const;
};
```

### SpriteRenderer

```cpp
class SpriteRenderer : public Component {
public:
    Texture* texture;
    Color tint;
    Rectangle sourceRect;
    bool flipX;
    bool flipY;
    Vector2 pivot;
    
    // Paper effects
    float paperCrumple;
    float paperTear;
    bool castShadow;
    Vector2 shadowOffset;
    Color shadowColor;
};
```

### Rigidbody

```cpp
class Rigidbody : public Component {
public:
    enum class BodyType { Static, Kinematic, Dynamic };
    
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
    float drag;
    float gravityScale;
    float restitution;
    
    BodyType bodyType;
    bool useGravity;
    bool freezeRotation;
    bool freezeX;
    bool freezeY;
    
    void addForce(const Vector2& force);
    void addImpulse(const Vector2& impulse);
    void setVelocity(const Vector2& vel);
};
```

### BoxCollider

```cpp
class BoxCollider : public Collider {
public:
    Vector2 size;
    Vector2 offset;
    bool isTrigger;
    
    bool intersects(const Collider* other) const override;
    Rectangle getBounds() const override;
};
```

### CircleCollider

```cpp
class CircleCollider : public Collider {
public:
    float radius;
    Vector2 offset;
    bool isTrigger;
    
    bool intersects(const Collider* other) const override;
    Rectangle getBounds() const override;
};
```

### ParticleSystem

```cpp
class ParticleSystem : public Component {
public:
    enum class EmitterShape { Point, Circle, Rectangle };
    
    // Emitter
    EmitterShape shape;
    Vector2 shapeSize;
    int maxParticles;
    float emissionRate;
    
    // Particles
    float lifetime;
    float lifetimeVariation;
    Vector2 startVelocity;
    Vector2 velocityVariation;
    Vector2 gravity;
    Color startColor;
    Color endColor;
    float startAlpha;
    float endAlpha;
    Vector2 startScale;
    Vector2 endScale;
    float rotationSpeed;
    float rotationVariation;
    
    void emit(int count);
    void clear();
    
    // Presets
    void setConfettiMode();
    void setTornPaperMode();
    void setDustMode();
};
```

### Animator

```cpp
class Animator : public Component {
public:
    void addAnimation(const std::string& name, 
                     const std::vector<Rectangle>& frames,
                     float frameTime, 
                     bool loop = true);
    void play(const std::string& name);
    void stop();
    void pause();
    void resume();
    
    bool isPlaying() const;
    const std::string& getCurrentAnimation() const;
};
```

### Camera

```cpp
class Camera : public Component {
public:
    float zoom;
    bool smoothFollow;
    float followSpeed;
    Vector2 offset;
    Rectangle bounds;
    
    void shake(float intensity, float duration);
};
```

## Input System

### InputManager

```cpp
class InputManager {
public:
    enum class MouseButton { Left, Middle, Right };
    
    // Keyboard
    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyReleased(SDL_Scancode key) const;
    
    // Mouse
    bool isMouseButtonPressed(MouseButton button) const;
    bool isMouseButtonDown(MouseButton button) const;
    bool isMouseButtonReleased(MouseButton button) const;
    Vector2 getMousePosition() const;
    Vector2 getMouseDelta() const;
    int getMouseWheelDelta() const;
    
    // Text input
    void startTextInput();
    void stopTextInput();
    bool isTextInputActive() const;
    const std::string& getTextInput() const;
};
```

### Key Constants

```cpp
namespace Key {
    constexpr SDL_Scancode A, B, C, ..., Z;
    constexpr SDL_Scancode Space, Enter, Escape, Tab, Backspace;
    constexpr SDL_Scancode Up, Down, Left, Right;
    constexpr SDL_Scancode LShift, RShift, LCtrl, RCtrl;
}
```

## Physics System

### PhysicsWorld

```cpp
class PhysicsWorld {
public:
    Vector2 gravity;
    int velocityIterations;
    int positionIterations;
    
    void update(float deltaTime);
    void checkCollisions();
    
    // Raycasting
    struct RaycastHit {
        Entity* entity;
        Vector2 point;
        Vector2 normal;
        float distance;
    };
    
    bool raycast(const Vector2& origin, 
                const Vector2& direction,
                float maxDistance,
                RaycastHit& hit);
    
    std::vector<RaycastHit> raycastAll(const Vector2& origin,
                                       const Vector2& direction,
                                       float maxDistance);
    
    // Queries
    std::vector<Entity*> overlapCircle(const Vector2& center, 
                                       float radius);
    std::vector<Entity*> overlapRectangle(const Rectangle& rect);
};
```

## Resource Management

### ResourceManager

```cpp
class ResourceManager {
public:
    void initialize(Renderer* renderer, const std::string& assetPath);
    
    // Textures
    Texture* loadTexture(const std::string& name, 
                        const std::string& filepath);
    Texture* getTexture(const std::string& name);
    void unloadTexture(const std::string& name);
    
    // Fonts
    Font* loadFont(const std::string& name,
                  const std::string& filepath,
                  int ptSize);
    Font* getFont(const std::string& name);
    void unloadFont(const std::string& name);
    
    std::string getAssetPath(const std::string& filename) const;
    void clearAll();
};
```

### Texture

```cpp
class Texture {
public:
    bool loadFromFile(const std::string& filepath, 
                     Renderer* renderer);
    void free();
    
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(unsigned char alpha);
    void setColor(unsigned char r, unsigned char g, unsigned char b);
    
    int getWidth() const;
    int getHeight() const;
    Vector2 getSize() const;
};
```

## Audio System

### AudioManager

```cpp
class AudioManager {
public:
    bool initialize();
    void shutdown();
    
    // Loading
    AudioClip* loadSound(const std::string& name,
                        const std::string& filepath);
    AudioClip* loadMusic(const std::string& name,
                        const std::string& filepath);
    AudioClip* getSound(const std::string& name);
    AudioClip* getMusic(const std::string& name);
    
    // Playback
    void playSound(const std::string& name, float volume = 1.0f);
    void playMusic(const std::string& name, 
                  bool loop = true,
                  float volume = 1.0f);
    void stopMusic();
    
    // Volume
    void setMasterVolume(float volume);
    void setSoundVolume(float volume);
    void setMusicVolume(float volume);
    
    float getMasterVolume() const;
    float getSoundVolume() const;
    float getMusicVolume() const;
};
```

### AudioClip

```cpp
class AudioClip {
public:
    bool loadFromFile(const std::string& filepath);
    void free();
    
    void play(int loops = 0); // 0=once, -1=infinite
    void pause();
    void resume();
    void stop();
    
    void setVolume(float volume); // 0.0 to 1.0
    float getVolume() const;
    bool isPlaying() const;
};
```

## Renderer

### Renderer

```cpp
class Renderer {
public:
    enum class BlendMode { None, Alpha, Additive, Multiply };
    
    bool initialize(SDL_Window* window);
    
    void beginFrame();
    void endFrame();
    void present();
    
    // Drawing
    void drawTexture(Texture* texture,
                    const Vector2& position,
                    int layer = 0,
                    const Color& tint = Color::white(),
                    float rotation = 0.0f,
                    const Vector2& scale = Vector2::one());
    
    void drawRectangle(const Rectangle& rect,
                      const Color& color,
                      int layer = 0,
                      bool filled = true);
    
    void drawCircle(const Vector2& center,
                   float radius,
                   const Color& color,
                   int layer = 0,
                   bool filled = true);
    
    void drawLine(const Vector2& start,
                 const Vector2& end,
                 const Color& color,
                 int layer = 0,
                 float thickness = 1.0f);
    
    // Paper effects
    void drawWithShadow(Texture* texture,
                       const Vector2& position,
                       int layer = 0,
                       const Vector2& shadowOffset = Vector2(2, 2),
                       const Color& shadowColor = Color(0,0,0,100));
    
    // Camera
    void setCamera(const Vector2& position, float zoom = 1.0f);
    Vector2 getCameraPosition() const;
    float getCameraZoom() const;
    Vector2 screenToWorld(const Vector2& screenPos) const;
    Vector2 worldToScreen(const Vector2& worldPos) const;
    
    // State
    void setBlendMode(BlendMode mode);
    void setDrawColor(const Color& color);
    void clear(const Color& color);
};
```

## Scene Management

### SceneManager

```cpp
class SceneManager {
public:
    void update(float deltaTime);
    void render();
    
    void addScene(const std::string& name, 
                 std::unique_ptr<Scene> scene);
    void setActiveScene(const std::string& name);
    Scene* getActiveScene() const;
    Scene* getScene(const std::string& name) const;
    
    void removeScene(const std::string& name);
    void clearScenes();
};
```

## Usage Examples

### Creating a Custom Component

```cpp
class HealthComponent : public Component {
public:
    int maxHealth = 100;
    int currentHealth = 100;
    
    void takeDamage(int amount) {
        currentHealth -= amount;
        if (currentHealth <= 0) {
            getEntity()->destroy();
        }
    }
    
    void heal(int amount) {
        currentHealth = std::min(currentHealth + amount, maxHealth);
    }
    
    void update(float deltaTime) override {
        // Update logic
    }
};
```

### Using the API

```cpp
// Initialize engine
Engine engine;
Engine::Config config;
config.windowTitle = "My Game";
engine.initialize(config);

// Create scene
auto scene = std::make_unique<MyScene>();
engine.getSceneManager()->addScene("main", std::move(scene));
engine.getSceneManager()->setActiveScene("main");

// Create entity
auto player = scene->createEntity("Player");
player->transform()->position = Vector2(100, 100);
player->addComponent<SpriteRenderer>();
player->addComponent<Rigidbody>();
player->addComponent<BoxCollider>(32, 48);

// Game loop
engine.run();
```
