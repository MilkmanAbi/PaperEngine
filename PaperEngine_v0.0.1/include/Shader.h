#pragma once

#include "Math.h"
#include "Vector2.h"
#include <string>
#include <memory>

namespace Paper {

class Texture;

/**
 * Shader - Paper-specific visual effects
 * v0.0.1: Basic paper bend/fold simulation using software rendering
 */
class Shader {
public:
    enum class Type {
        None,
        PaperFold,      // Vertical/horizontal paper fold
        PaperBend,      // Curved paper bend
        SoftShadow,     // Gaussian-blurred shadow
        CelShading      // Cartoon/hand-drawn style
    };

    Shader(Type type = Type::None);
    virtual ~Shader() = default;

    Type getType() const { return type; }
    
    virtual void apply(Texture* texture, const Vector2& position) = 0;

protected:
    Type type;
};

/**
 * PaperFoldShader - Simulates paper folding
 */
class PaperFoldShader : public Shader {
public:
    PaperFoldShader();

    float foldPosition = 0.5f;  // 0.0 to 1.0 (where the fold is)
    float foldAngle = 45.0f;     // Degrees of fold
    bool horizontal = false;     // true = horizontal fold, false = vertical
    
    void apply(Texture* texture, const Vector2& position) override;
};

/**
 * PaperBendShader - Curved paper deformation
 */
class PaperBendShader : public Shader {
public:
    PaperBendShader();

    float bendAmount = 0.2f;     // 0.0 to 1.0
    float bendCurvature = 1.0f;  // How curved the bend is
    Vector2 bendDirection{0, 1}; // Direction of bend
    
    void apply(Texture* texture, const Vector2& position) override;
};

/**
 * SoftShadowShader - Gaussian-blurred shadow effect
 */
class SoftShadowShader : public Shader {
public:
    SoftShadowShader();

    float blurRadius = 5.0f;
    Color shadowColor = Color(0, 0, 0, 128);
    Vector2 offset{2, 2};
    
    void apply(Texture* texture, const Vector2& position) override;
};

/**
 * ShaderManager - Manages and applies shaders
 */
class ShaderManager {
public:
    ShaderManager();
    ~ShaderManager();

    // Shader management
    void addShader(const std::string& name, std::unique_ptr<Shader> shader);
    Shader* getShader(const std::string& name);
    void removeShader(const std::string& name);

    // Quick access to common shaders
    PaperFoldShader* createFoldShader(const std::string& name);
    PaperBendShader* createBendShader(const std::string& name);
    SoftShadowShader* createSoftShadowShader(const std::string& name);

private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
};

} // namespace Paper
