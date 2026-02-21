#pragma once

#include <memory>
#include <string>
#include <chrono>

namespace Paper {

// Forward declarations
class Renderer;
class InputManager;
class SceneManager;
class ResourceManager;
class AudioManager;
class PhysicsWorld;

/**
 * PaperEngine - Main engine class
 * Handles initialization, main loop, and subsystem coordination
 */
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

    Engine();
    ~Engine();

    // Core functions
    bool initialize(const Config& config);
    void run();
    void shutdown();
    void quit();

    // Subsystem access
    Renderer* getRenderer() { return renderer.get(); }
    InputManager* getInput() { return input.get(); }
    SceneManager* getSceneManager() { return sceneManager.get(); }
    ResourceManager* getResourceManager() { return resourceManager.get(); }
    AudioManager* getAudioManager() { return audioManager.get(); }
    PhysicsWorld* getPhysicsWorld() { return physicsWorld.get(); }

    // Time management
    float getDeltaTime() const { return deltaTime; }
    float getTotalTime() const { return totalTime; }
    int getFPS() const { return currentFPS; }

    // Singleton access
    static Engine* getInstance() { return instance; }

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    void calculateFPS();

    static Engine* instance;

    Config config;
    bool isRunning;
    bool isInitialized;

    // Subsystems
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<InputManager> input;
    std::unique_ptr<SceneManager> sceneManager;
    std::unique_ptr<ResourceManager> resourceManager;
    std::unique_ptr<AudioManager> audioManager;
    std::unique_ptr<PhysicsWorld> physicsWorld;

    // Timing
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    float deltaTime;
    float totalTime;
    int currentFPS;
    int frameCount;
    float fpsTimer;
};

} // namespace Paper
