#include "PaperEngine.h"
#include "Renderer.h"
#include "Input.h"
#include "Scene.h"
#include "Resources.h"
#include "Audio.h"
#include "Physics.h"
#include <SDL2/SDL.h>
#include <iostream>

namespace Paper {

Engine* Engine::instance = nullptr;

Engine::Engine() : isRunning(false), isInitialized(false), deltaTime(0.0f), 
                   totalTime(0.0f), currentFPS(0), frameCount(0), fpsTimer(0.0f) {
    instance = this;
}

Engine::~Engine() {
    shutdown();
    instance = nullptr;
}

bool Engine::initialize(const Config& cfg) {
    if (isInitialized) {
        std::cerr << "PaperEngine already initialized!" << std::endl;
        return false;
    }

    config = cfg;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        config.windowTitle.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        config.windowWidth,
        config.windowHeight,
        SDL_WINDOW_SHOWN | (config.fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
    );

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Initialize subsystems
    renderer = std::make_unique<Renderer>();
    if (!renderer->initialize(window)) {
        std::cerr << "Renderer initialization failed!" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    input = std::make_unique<InputManager>();
    sceneManager = std::make_unique<SceneManager>();
    resourceManager = std::make_unique<ResourceManager>();
    audioManager = std::make_unique<AudioManager>();
    physicsWorld = std::make_unique<PhysicsWorld>();

    resourceManager->initialize(renderer.get(), config.assetPath);
    audioManager->initialize();

    isInitialized = true;
    isRunning = true;
    lastFrameTime = std::chrono::high_resolution_clock::now();

    std::cout << "PaperEngine initialized successfully!" << std::endl;
    return true;
}

void Engine::run() {
    if (!isInitialized) {
        std::cerr << "Engine not initialized! Call initialize() first." << std::endl;
        return;
    }

    while (isRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastFrameTime;
        deltaTime = elapsed.count();
        lastFrameTime = currentTime;

        // Cap delta time to prevent spiral of death
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        totalTime += deltaTime;

        processEvents();
        update(deltaTime);
        render();
        calculateFPS();

        // Frame rate limiting (if not using vsync)
        if (!config.vsync && config.targetFPS > 0) {
            float targetFrameTime = 1.0f / config.targetFPS;
            auto frameEndTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> frameTime = frameEndTime - currentTime;
            
            if (frameTime.count() < targetFrameTime) {
                SDL_Delay(static_cast<Uint32>((targetFrameTime - frameTime.count()) * 1000.0f));
            }
        }
    }
}

void Engine::shutdown() {
    if (!isInitialized) return;

    sceneManager.reset();
    audioManager.reset();
    resourceManager.reset();
    physicsWorld.reset();
    input.reset();
    renderer.reset();

    SDL_Quit();

    isInitialized = false;
    std::cout << "PaperEngine shutdown complete." << std::endl;
}

void Engine::quit() {
    isRunning = false;
}

void Engine::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit();
        }
        
        input->processEvent(event);
    }
    
    input->update();
}

void Engine::update(float dt) {
    // Update physics
    physicsWorld->update(dt);
    
    // Update current scene
    sceneManager->update(dt);
}

void Engine::render() {
    renderer->beginFrame();
    sceneManager->render();
    renderer->endFrame();
    renderer->present();
}

void Engine::calculateFPS() {
    frameCount++;
    fpsTimer += deltaTime;
    
    if (fpsTimer >= 1.0f) {
        currentFPS = frameCount;
        frameCount = 0;
        fpsTimer = 0.0f;
    }
}

} // namespace Paper
