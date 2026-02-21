#pragma once

#include "Entity.h"
#include "Math.h"
#include "Vector2.h"
#include <vector>
#include <string>

namespace Paper {

class Texture;

/**
 * Background3D - 3D background rendered behind 2D paper elements
 * Supports pre-rendered 3D or simple perspective tricks
 */
class Background3D : public Component {
public:
    enum class Type {
        Static,         // Fixed 3D background image
        Parallax3D,     // Multiple layers with perspective
        Rotoscoped,     // Video/animated 3D background
        Skybox          // 360-degree background
    };

    Background3D(Type type = Type::Static);

    void update(float deltaTime) override;

    Type type;
    Texture* texture = nullptr;
    std::vector<Texture*> layers;  // For parallax3D
    
    // Perspective settings
    float fov = 60.0f;
    float perspectiveStrength = 0.5f;
    Vector2 vanishingPoint{0.5f, 0.5f};  // Normalized screen space
    
    // Camera follow
    bool followCamera = true;
    float followSpeed = 0.3f;  // Slower = more depth
    float followScale = 1.0f;   // How much camera affects position
    
    // Rotation (for skybox)
    float rotation = 0.0f;
    bool autoRotate = false;
    float rotationSpeed = 5.0f;

private:
    Vector2 currentOffset{0, 0};
    void renderStatic();
    void renderParallax3D();
    void renderSkybox();
};

/**
 * RotoscopedBackground - Animated 3D background
 * Plays video or frame sequence behind 2D gameplay
 */
class RotoscopedBackground : public Component {
public:
    RotoscopedBackground();

    void update(float deltaTime) override;

    // Animation
    std::vector<Texture*> frames;
    int currentFrame = 0;
    float frameDuration = 1.0f / 30.0f;  // 30 FPS default
    bool loop = true;
    bool playing = true;
    
    // Blending with paper
    float opacity = 1.0f;
    Color tint = Color::white();
    bool applyPaperFilter = true;  // Make it look more paper-like
    
    void play();
    void pause();
    void stop();
    void setFrame(int frame);

private:
    float frameTimer = 0.0f;
};

/**
 * PerspectiveGrid - Helper for 3D-looking perspective grid
 */
class PerspectiveGrid : public Component {
public:
    PerspectiveGrid();

    void update(float deltaTime) override;

    // Grid settings
    int gridSize = 50;          // Cell size
    int width = 20;             // Grid width in cells
    int depth = 20;             // Grid depth in cells
    Color gridColor = Color(100, 100, 100, 100);
    float lineWidth = 1.0f;
    
    // Perspective
    float perspectiveStrength = 1.0f;
    float horizonY = 0.5f;      // Where horizon line appears
    
    // Animation
    bool scroll = false;
    float scrollSpeed = 50.0f;
    Vector2 scrollDirection{0, 1};

private:
    float scrollOffset = 0.0f;
    void renderGrid();
};

/**
 * Background3DManager - Manages 3D backgrounds and layering
 */
class Background3DManager {
public:
    Background3DManager();
    ~Background3DManager();

    void update(float deltaTime);
    void render();

    // Background management
    void addBackground(Background3D* background, int layer);
    void removeBackground(Background3D* background);
    void clearBackgrounds();

    // Layer control
    void setLayerDepth(int layer, float depth);  // Affects parallax
    float getLayerDepth(int layer) const;

    // Camera integration
    void setCameraPosition(const Vector2& position);
    Vector2 getCameraPosition() const { return cameraPosition; }

private:
    struct BackgroundLayer {
        Background3D* background;
        int layer;
        float depth;
    };

    std::vector<BackgroundLayer> backgrounds;
    Vector2 cameraPosition{0, 0};

    void sortBackgrounds();
};

/**
 * Depth2D - Makes 2D sprites appear 3D-positioned
 * Projects 2D position based on virtual depth
 */
struct Depth2D {
    float depth = 0.0f;         // Distance from camera (0 = at camera, 1+ = far)
    Vector2 position3D{0, 0};   // Virtual 3D position
    
    // Convert to screen position
    Vector2 toScreenPosition(const Vector2& cameraPos, float perspectiveStrength) const;
    
    // Scale based on depth (further = smaller)
    float getDepthScale() const;
};

/**
 * DepthSprite - Sprite with depth positioning
 */
class DepthSprite : public Component {
public:
    DepthSprite();

    void update(float deltaTime) override;

    Depth2D depth;
    bool autoScale = true;      // Scale based on depth
    bool autoSort = true;       // Sort by depth for layering
    float minScale = 0.1f;      // Minimum scale at far depth
    float maxScale = 2.0f;      // Maximum scale at near depth

private:
    SpriteRenderer* sprite = nullptr;
};

} // namespace Paper
