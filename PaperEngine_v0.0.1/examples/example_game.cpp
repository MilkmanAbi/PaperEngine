#include "PaperEngine.h"
#include "Scene.h"
#include "Components.h"
#include "Physics.h"
#include "Input.h"
#include <iostream>

using namespace Paper;

/**
 * Example game scene demonstrating PaperEngine's paper-like aesthetic
 * A simple platformer with paper cutout characters
 */
class GameScene : public Scene {
public:
    GameScene() : Scene("GameScene") {}

    void onEnter() override {
        std::cout << "Entering Game Scene" << std::endl;
        
        // Set paper-like background color
        backgroundColor = Color::paper();

        // Create player entity
        player = createEntity("Player");
        player->transform()->position = Vector2(400, 300);
        player->transform()->layer = 2; // Player on front layer
        
        // Add sprite renderer (would use texture in real game)
        auto sprite = player->addComponent<SpriteRenderer>();
        sprite->tint = Color::blue();
        sprite->castShadow = true;
        
        // Add physics
        auto rb = player->addComponent<Rigidbody>();
        rb->mass = 1.0f;
        rb->drag = 0.1f;
        
        auto collider = player->addComponent<BoxCollider>(32.0f, 48.0f);

        // Create ground
        createGround(0, 550, 1280, 50);
        
        // Create some platforms
        createPlatform(200, 450, 150, 20);
        createPlatform(450, 350, 150, 20);
        createPlatform(700, 250, 150, 20);

        // Create decorative paper elements in background
        createBackgroundPaper(100, 100, 0); // Layer 0 = background
        createBackgroundPaper(900, 150, 0);
        createBackgroundPaper(500, 80, 0);

        std::cout << "Game scene setup complete!" << std::endl;
    }

    void update(float deltaTime) override {
        Scene::update(deltaTime);

        // Player controls
        auto input = Engine::getInstance()->getInput();
        auto rb = player->getComponent<Rigidbody>();

        if (input->isKeyDown(Key::A) || input->isKeyDown(Key::Left)) {
            rb->velocity.x = -200.0f;
        } else if (input->isKeyDown(Key::D) || input->isKeyDown(Key::Right)) {
            rb->velocity.x = 200.0f;
        } else {
            rb->velocity.x *= 0.9f; // Damping
        }

        // Jump
        if (input->isKeyPressed(Key::Space) || input->isKeyPressed(Key::W)) {
            // Check if on ground (simplified - would use collision callbacks in real game)
            if (player->transform()->position.y > 480) {
                rb->addImpulse(Vector2(0, -400));
            }
        }

        // Quit
        if (input->isKeyPressed(Key::Escape)) {
            Engine::getInstance()->quit();
        }

        // Display FPS
        if (frameCount++ % 60 == 0) {
            std::cout << "FPS: " << Engine::getInstance()->getFPS() << std::endl;
        }
    }

private:
    Entity* player;
    int frameCount = 0;

    void createGround(float x, float y, float width, float height) {
        auto ground = createEntity("Ground");
        ground->transform()->position = Vector2(x, y);
        ground->transform()->layer = 1;
        
        auto sprite = ground->addComponent<SpriteRenderer>();
        sprite->tint = Color::cardboard();
        
        auto rb = ground->addComponent<Rigidbody>(Rigidbody::BodyType::Static);
        auto collider = ground->addComponent<BoxCollider>(width, height);
    }

    void createPlatform(float x, float y, float width, float height) {
        auto platform = createEntity("Platform");
        platform->transform()->position = Vector2(x, y);
        platform->transform()->layer = 1;
        
        auto sprite = platform->addComponent<SpriteRenderer>();
        sprite->tint = Color::parchment();
        sprite->castShadow = true;
        sprite->shadowOffset = Vector2(3, 3);
        
        auto rb = platform->addComponent<Rigidbody>(Rigidbody::BodyType::Static);
        auto collider = platform->addComponent<BoxCollider>(width, height);
    }

    void createBackgroundPaper(float x, float y, int layer) {
        auto paper = createEntity("BackgroundPaper");
        paper->transform()->position = Vector2(x, y);
        paper->transform()->layer = layer;
        paper->transform()->rotation = (rand() % 60) - 30; // Random tilt
        
        auto sprite = paper->addComponent<SpriteRenderer>();
        sprite->tint = Color(250, 240, 230, 150); // Semi-transparent paper
        sprite->paperCrumple = 0.3f; // Slightly crumpled
    }
};

/**
 * Main entry point
 */
int main(int argc, char* argv[]) {
    std::cout << "=== PaperEngine Demo ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  A/D or Arrow Keys - Move" << std::endl;
    std::cout << "  Space/W - Jump" << std::endl;
    std::cout << "  ESC - Quit" << std::endl;
    std::cout << "========================" << std::endl;

    // Create engine
    Engine engine;

    // Configure engine
    Engine::Config config;
    config.windowTitle = "PaperEngine Demo - Paper Platformer";
    config.windowWidth = 1280;
    config.windowHeight = 720;
    config.vsync = true;
    config.targetFPS = 60;
    config.assetPath = "assets/";

    // Initialize
    if (!engine.initialize(config)) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }

    // Create and set game scene
    auto gameScene = std::make_unique<GameScene>();
    engine.getSceneManager()->addScene("game", std::move(gameScene));
    engine.getSceneManager()->setActiveScene("game");

    // Run game loop
    engine.run();

    // Cleanup
    engine.shutdown();

    std::cout << "Thanks for playing!" << std::endl;
    return 0;
}
