#pragma once

#include "Math.h"
#include "Vector2.h"
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

namespace Paper {

// Forward declarations
class Texture;
class Font;

/**
 * Renderer - Handles all 2D rendering with paper-like layering support
 */
class Renderer {
public:
    enum class BlendMode {
        None,
        Alpha,
        Additive,
        Multiply
    };

    struct RenderCommand {
        int layer;
        float depth; // Within same layer for sub-sorting
        std::function<void()> execute;

        bool operator<(const RenderCommand& other) const {
            if (layer != other.layer) return layer < other.layer;
            return depth < other.depth;
        }
    };

    Renderer();
    ~Renderer();

    bool initialize(SDL_Window* window);
    void shutdown();

    void beginFrame();
    void endFrame();
    void present();

    // Drawing commands (batched by layer)
    void drawTexture(Texture* texture, const Vector2& position, int layer = 0,
                    const Color& tint = Color::white(), float rotation = 0.0f,
                    const Vector2& scale = Vector2::one());
    
    void drawTextureRect(Texture* texture, const Rectangle& srcRect, 
                        const Rectangle& destRect, int layer = 0,
                        const Color& tint = Color::white(), float rotation = 0.0f);

    void drawRectangle(const Rectangle& rect, const Color& color, int layer = 0, bool filled = true);
    void drawCircle(const Vector2& center, float radius, const Color& color, int layer = 0, bool filled = true);
    void drawLine(const Vector2& start, const Vector2& end, const Color& color, int layer = 0, float thickness = 1.0f);
    void drawText(Font* font, const std::string& text, const Vector2& position, 
                 const Color& color, int layer = 0);

    // Paper-specific effects
    void drawPaperTexture(Texture* texture, const Vector2& position, int layer = 0,
                         float crumple = 0.0f, float tear = 0.0f);
    void drawWithShadow(Texture* texture, const Vector2& position, int layer = 0,
                       const Vector2& shadowOffset = Vector2(2, 2), 
                       const Color& shadowColor = Color(0, 0, 0, 100));

    // Parallax system
    struct ParallaxLayer {
        int layer;
        float scrollSpeed; // 1.0 = normal, 0.5 = half speed (far), 2.0 = double (near)
        Vector2 offset;
    };
    void setParallaxLayer(int layer, float scrollSpeed);
    float getParallaxSpeed(int layer) const;
    
    // Camera and viewport
    void setCamera(const Vector2& position, float zoom = 1.0f);
    Vector2 getCameraPosition() const { return cameraPosition; }
    float getCameraZoom() const { return cameraZoom; }
    
    Vector2 screenToWorld(const Vector2& screenPos) const;
    Vector2 worldToScreen(const Vector2& worldPos) const;

    // Rendering state
    void setBlendMode(BlendMode mode);
    void setDrawColor(const Color& color);
    void clear(const Color& color);

    // Viewport
    Rectangle getViewport() const;
    void setViewport(const Rectangle& rect);

    SDL_Renderer* getSDLRenderer() { return sdlRenderer; }

private:
    SDL_Renderer* sdlRenderer;
    SDL_Window* window;
    
    Vector2 cameraPosition{0, 0};
    float cameraZoom = 1.0f;
    
    std::vector<RenderCommand> renderQueue;
    BlendMode currentBlendMode = BlendMode::Alpha;
    
    // Parallax system
    std::unordered_map<int, float> parallaxSpeeds; // layer -> scroll speed

    void executeRenderQueue();
    void applyBlendMode(BlendMode mode);
    Vector2 applyParallax(const Vector2& position, int layer) const;
};

} // namespace Paper
