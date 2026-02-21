#pragma once

#include "Math.h"
#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include <unordered_map>

namespace Paper {

class Renderer;

/**
 * Texture - Represents a 2D image/sprite
 */
class Texture {
public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& filepath, Renderer* renderer);
    bool loadFromSurface(SDL_Surface* surface, Renderer* renderer);
    void free();

    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(unsigned char alpha);
    void setColor(unsigned char r, unsigned char g, unsigned char b);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Vector2 getSize() const { return Vector2(static_cast<float>(width), static_cast<float>(height)); }

    SDL_Texture* getSDLTexture() { return texture; }

private:
    SDL_Texture* texture;
    int width;
    int height;
};

/**
 * Font - Text rendering support
 */
class Font {
public:
    Font();
    ~Font();

    bool loadFromFile(const std::string& filepath, int ptSize);
    void free();

    // For now, placeholder - would integrate SDL_ttf
    void* getFontData() { return nullptr; }
    int getSize() const { return size; }

private:
    void* fontData; // TTF_Font* when SDL_ttf is integrated
    int size;
};

/**
 * ResourceManager - Centralized asset loading and caching
 */
class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    void initialize(Renderer* renderer, const std::string& assetPath);
    void shutdown();

    // Texture management
    Texture* loadTexture(const std::string& name, const std::string& filepath);
    Texture* getTexture(const std::string& name);
    void unloadTexture(const std::string& name);

    // Font management
    Font* loadFont(const std::string& name, const std::string& filepath, int ptSize);
    Font* getFont(const std::string& name);
    void unloadFont(const std::string& name);

    // Asset path helpers
    std::string getAssetPath(const std::string& filename) const;
    void setAssetPath(const std::string& path) { assetPath = path; }

    void clearAll();

private:
    Renderer* renderer;
    std::string assetPath;
    
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<Font>> fonts;
};

} // namespace Paper
