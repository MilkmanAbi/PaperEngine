#pragma once

#include "Entity.h"
#include "Resources.h"
#include "Math.h"

namespace Paper {

/**
 * SpriteRenderer - Renders a texture/sprite
 */
class SpriteRenderer : public Component {
public:
    SpriteRenderer(Texture* texture = nullptr);

    void update(float deltaTime) override;

    Texture* texture = nullptr;
    Color tint = Color::white();
    Rectangle sourceRect; // Empty = use full texture
    bool flipX = false;
    bool flipY = false;
    Vector2 pivot{0.5f, 0.5f}; // Center by default (0,0 = top-left, 1,1 = bottom-right)

    // Paper-specific effects
    float paperCrumple = 0.0f; // 0 = flat, 1 = very crumpled
    float paperTear = 0.0f;    // 0 = intact, 1 = very torn
    bool castShadow = false;
    Vector2 shadowOffset{2, 2};
    Color shadowColor = Color(0, 0, 0, 100);
};

/**
 * Animator - Handles sprite animation
 */
class Animator : public Component {
public:
    struct Animation {
        std::string name;
        std::vector<Rectangle> frames;
        float frameTime; // Duration of each frame
        bool loop;
    };

    Animator();

    void update(float deltaTime) override;

    void addAnimation(const std::string& name, const std::vector<Rectangle>& frames, 
                     float frameTime, bool loop = true);
    void play(const std::string& name);
    void stop();
    void pause();
    void resume();

    bool isPlaying() const { return playing; }
    const std::string& getCurrentAnimation() const { return currentAnimationName; }

private:
    std::unordered_map<std::string, Animation> animations;
    std::string currentAnimationName;
    int currentFrame = 0;
    float frameTimer = 0.0f;
    bool playing = false;
    bool paused = false;

    SpriteRenderer* spriteRenderer = nullptr;
};

/**
 * ParticleSystem - Paper-themed particle effects (confetti, torn paper, etc.)
 */
class ParticleSystem : public Component {
public:
    struct Particle {
        Vector2 position;
        Vector2 velocity;
        float lifetime;
        float maxLifetime;
        Color color;
        float rotation;
        float rotationSpeed;
        Vector2 scale;
        float alpha;
    };

    enum class EmitterShape {
        Point,
        Circle,
        Rectangle
    };

    ParticleSystem();

    void update(float deltaTime) override;

    void emit(int count);
    void clear();

    // Emitter properties
    EmitterShape shape = EmitterShape::Point;
    Vector2 shapeSize{10, 10};
    
    // Particle properties
    int maxParticles = 100;
    float emissionRate = 10.0f; // Particles per second
    float lifetime = 2.0f;
    float lifetimeVariation = 0.5f;
    
    Vector2 startVelocity{0, -100};
    Vector2 velocityVariation{50, 50};
    Vector2 gravity{0, 100};
    
    Color startColor = Color::white();
    Color endColor = Color::white();
    float startAlpha = 1.0f;
    float endAlpha = 0.0f;
    
    Vector2 startScale{1, 1};
    Vector2 endScale{0.5f, 0.5f};
    
    float rotationSpeed = 0.0f;
    float rotationVariation = 90.0f;

    // Paper particle presets
    void setConfettiMode();
    void setTornPaperMode();
    void setDustMode();

private:
    std::vector<Particle> particles;
    float emissionTimer = 0.0f;

    void updateParticle(Particle& p, float deltaTime);
    Particle createParticle();
};

/**
 * Camera - Camera component for following entities
 */
class Camera : public Component {
public:
    Camera();

    void update(float deltaTime) override;

    float zoom = 1.0f;
    bool smoothFollow = true;
    float followSpeed = 5.0f;
    Vector2 offset{0, 0};
    Rectangle bounds; // Optional world bounds

    void shake(float intensity, float duration);

private:
    bool isShaking = false;
    float shakeIntensity = 0.0f;
    float shakeDuration = 0.0f;
    float shakeTimer = 0.0f;
};

} // namespace Paper
