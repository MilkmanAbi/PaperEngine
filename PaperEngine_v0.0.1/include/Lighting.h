#pragma once

#include "Entity.h"
#include "Math.h"
#include "Vector2.h"
#include <vector>
#include <memory>

namespace Paper {

/**
 * Light - Stylized lighting for paper aesthetic
 * Not physically accurate - designed for artistic paper look
 */
class Light : public Component {
public:
    enum class Type {
        Point,      // Radial light from a point
        Spot,       // Directional cone
        Ambient,    // Global illumination
        Rim         // Edge highlighting (paper outline)
    };

    Light(Type type = Type::Point);

    void update(float deltaTime) override;

    // Light properties
    Type type;
    Color color = Color::white();
    float intensity = 1.0f;
    float radius = 200.0f;       // For point/spot lights
    float innerRadius = 100.0f;  // Spot light inner cone
    Vector2 direction{0, -1};    // For spot lights
    
    // Paper-specific
    bool castPaperShadows = true;
    float paperShadowSoftness = 5.0f;
    bool affectPaperFolds = true;  // Light affects fold shading
    
    // Animation
    bool flicker = false;
    float flickerSpeed = 5.0f;
    float flickerAmount = 0.2f;

private:
    float flickerTimer = 0.0f;
    float currentIntensity = 1.0f;
};

/**
 * LightingSystem - Manages all lights and applies them
 */
class LightingSystem {
public:
    LightingSystem();
    ~LightingSystem();

    void update(float deltaTime);
    void render();

    // Lighting configuration
    Color ambientColor = Color(100, 100, 120);  // Cool ambient for paper
    float ambientIntensity = 0.3f;
    bool enableShadows = true;
    bool enablePaperShading = true;  // Layer-based shading

    // Register lights
    void registerLight(Light* light);
    void unregisterLight(Light* light);

    // Query lighting at position
    Color getLightingAt(const Vector2& position, int layer) const;
    float getBrightnessAt(const Vector2& position) const;

    // Paper-specific lighting
    void setPaperLayerShading(bool enable) { enablePaperShading = enable; }
    void setRimLightIntensity(float intensity) { rimLightIntensity = intensity; }

private:
    std::vector<Light*> lights;
    float rimLightIntensity = 0.5f;

    Color calculateLighting(const Vector2& position, int layer) const;
    float calculatePaperLayerShading(int layer) const;
};

/**
 * PaperShadowCaster - Component that casts stylized paper shadows
 */
class PaperShadowCaster : public Component {
public:
    PaperShadowCaster();

    void update(float deltaTime) override;

    // Shadow properties
    bool castShadow = true;
    float shadowLength = 10.0f;     // Distance shadow projects
    float shadowSoftness = 5.0f;    // Blur amount
    Color shadowTint = Color(0, 0, 0, 100);
    bool dynamicShadow = true;      // Respond to lights
    
    // Paper-specific
    float paperThickness = 2.0f;    // Affects shadow appearance
    bool contactShadow = true;      // Dark shadow where paper meets surface

private:
    void renderShadow();
};

/**
 * CelShader - Cartoon/hand-drawn lighting effect
 */
class CelShader : public Component {
public:
    CelShader();

    void update(float deltaTime) override;

    // Cel shading parameters
    int shadingSteps = 3;           // Number of discrete shading levels
    float outlineWidth = 2.0f;      // Thickness of outlines
    Color outlineColor = Color::ink();
    bool outlineEnabled = true;
    
    // Hand-drawn wobble
    bool wobbleOutline = true;
    float wobbleAmount = 0.5f;
    float wobbleSpeed = 10.0f;

private:
    float wobbleTimer = 0.0f;
};

/**
 * LightProbe - Samples lighting at a point (for optimization)
 */
struct LightProbe {
    Vector2 position;
    Color cachedLighting;
    float updateInterval = 0.1f;
    float timer = 0.0f;
    
    void update(float deltaTime, LightingSystem* lightingSystem, int layer);
};

} // namespace Paper
