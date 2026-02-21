#include "Renderer.h"
#include "Shader.h"
#include "AdvancedAnimation.h"
#include "Lighting.h"
#include "Background3D.h"
#include "FramePacing.h"
#include <iostream>
#include <cmath>
#include <random>

namespace Paper {

// ========== RENDERER PARALLAX ==========

float Renderer::getParallaxSpeed(int layer) const {
    auto it = parallaxSpeeds.find(layer);
    if (it != parallaxSpeeds.end()) {
        return it->second;
    }
    return 1.0f; // Default: no parallax
}

void Renderer::setParallaxLayer(int layer, float scrollSpeed) {
    parallaxSpeeds[layer] = scrollSpeed;
}

Vector2 Renderer::applyParallax(const Vector2& position, int layer) const {
    float speed = getParallaxSpeed(layer);
    return position - (cameraPosition * speed);
}

// ========== SHADER SYSTEM ==========

Shader::Shader(Type type) : type(type) {}

PaperFoldShader::PaperFoldShader() : Shader(Type::PaperFold) {}

void PaperFoldShader::apply(Texture* texture, const Vector2& position) {
    // v0.0.1: Stub - would deform texture vertices
    std::cout << "PaperFoldShader::apply (stub) - fold at " << foldPosition << std::endl;
}

PaperBendShader::PaperBendShader() : Shader(Type::PaperBend) {}

void PaperBendShader::apply(Texture* texture, const Vector2& position) {
    // v0.0.1: Stub - would apply curved deformation
    std::cout << "PaperBendShader::apply (stub) - bend amount " << bendAmount << std::endl;
}

SoftShadowShader::SoftShadowShader() : Shader(Type::SoftShadow) {}

void SoftShadowShader::apply(Texture* texture, const Vector2& position) {
    // v0.0.1: Stub - would render gaussian blurred shadow
    std::cout << "SoftShadowShader::apply (stub) - blur radius " << blurRadius << std::endl;
}

ShaderManager::ShaderManager() {}
ShaderManager::~ShaderManager() {}

void ShaderManager::addShader(const std::string& name, std::unique_ptr<Shader> shader) {
    shaders[name] = std::move(shader);
}

Shader* ShaderManager::getShader(const std::string& name) {
    auto it = shaders.find(name);
    return (it != shaders.end()) ? it->second.get() : nullptr;
}

void ShaderManager::removeShader(const std::string& name) {
    shaders.erase(name);
}

PaperFoldShader* ShaderManager::createFoldShader(const std::string& name) {
    auto shader = std::make_unique<PaperFoldShader>();
    auto ptr = shader.get();
    addShader(name, std::move(shader));
    return ptr;
}

PaperBendShader* ShaderManager::createBendShader(const std::string& name) {
    auto shader = std::make_unique<PaperBendShader>();
    auto ptr = shader.get();
    addShader(name, std::move(shader));
    return ptr;
}

SoftShadowShader* ShaderManager::createSoftShadowShader(const std::string& name) {
    auto shader = std::make_unique<SoftShadowShader>();
    auto ptr = shader.get();
    addShader(name, std::move(shader));
    return ptr;
}

// ========== ADVANCED ANIMATION ==========

Animation::Animation(const std::string& name) : name(name) {}

void Animation::addFrame(const Rectangle& rect, float duration, const Vector2& offset) {
    AnimationFrame frame;
    frame.sourceRect = rect;
    frame.duration = duration;
    frame.offset = offset;
    frame.rotation = 0.0f;
    frames.push_back(frame);
}

void Animation::addFrameEvent(int frameIndex, const std::string& eventName) {
    if (frameIndex >= 0 && frameIndex < static_cast<int>(frames.size())) {
        frames[frameIndex].eventName = eventName;
    }
}

const AnimationFrame& Animation::getFrame(int index) const {
    return frames[index % frames.size()];
}

AnimationStateMachine::AnimationStateMachine() {}

void AnimationStateMachine::addState(const std::string& name, Animation* animation) {
    State state;
    state.name = name;
    state.animation = animation;
    states[name] = state;
}

void AnimationStateMachine::addTransition(const std::string& from, const std::string& to) {
    auto it = states.find(from);
    if (it != states.end()) {
        it->second.transitions.push_back(to);
    }
}

void AnimationStateMachine::setState(const std::string& name) {
    if (states.find(name) != states.end()) {
        currentState = name;
    }
}

Animation* AnimationStateMachine::getCurrentAnimation() {
    auto it = states.find(currentState);
    return (it != states.end()) ? it->second.animation : nullptr;
}

AdvancedAnimator::AdvancedAnimator() {}

void AdvancedAnimator::update(float deltaTime) {
    if (!playing || paused) return;
    
    if (handDrawnMode) {
        applyHandDrawnWobble();
    }
    
    updateAnimation(deltaTime * playbackSpeed);
}

void AdvancedAnimator::addAnimation(const std::string& name, const Animation& animation) {
    animations[name] = animation;
}

void AdvancedAnimator::play(const std::string& name, bool forceRestart) {
    if (animations.find(name) != animations.end()) {
        if (currentAnimationName != name || forceRestart) {
            currentAnimationName = name;
            currentFrame = 0;
            frameTimer = 0.0f;
        }
        playing = true;
        paused = false;
    }
}

void AdvancedAnimator::stop() {
    playing = false;
    currentFrame = 0;
    frameTimer = 0.0f;
}

void AdvancedAnimator::pause() {
    paused = true;
}

void AdvancedAnimator::resume() {
    paused = false;
}

void AdvancedAnimator::setPlaybackSpeed(float speed) {
    playbackSpeed = speed;
}

void AdvancedAnimator::enableHandDrawnMode(bool enable, float wobble) {
    handDrawnMode = enable;
    wobbleAmount = wobble;
}

void AdvancedAnimator::enableOnionSkinning(bool enable, float alpha) {
    // v0.0.1: Stub
}

void AdvancedAnimator::setFrameEventCallback(FrameEventCallback callback) {
    frameEventCallback = callback;
}

void AdvancedAnimator::updateAnimation(float deltaTime) {
    auto it = animations.find(currentAnimationName);
    if (it == animations.end() || it->second.frames.empty()) return;
    
    Animation& anim = it->second;
    frameTimer += deltaTime;
    
    const AnimationFrame& frame = anim.getFrame(currentFrame);
    if (frameTimer >= frame.duration) {
        frameTimer = 0.0f;
        currentFrame++;
        
        if (currentFrame >= anim.getFrameCount()) {
            if (anim.loop) {
                currentFrame = 0;
            } else {
                currentFrame = anim.getFrameCount() - 1;
                playing = false;
            }
        }
        
        // Trigger event if present
        if (!frame.eventName.empty() && frameEventCallback) {
            triggerFrameEvent(frame.eventName);
        }
    }
}

void AdvancedAnimator::applyHandDrawnWobble() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-wobbleAmount, wobbleAmount);
    
    currentWobble.x = dis(gen);
    currentWobble.y = dis(gen);
}

void AdvancedAnimator::triggerFrameEvent(const std::string& eventName) {
    if (frameEventCallback) {
        frameEventCallback(eventName);
    }
}

SpriteSheetBuilder::SpriteSheetBuilder(int w, int h) 
    : textureWidth(w), textureHeight(h) {}

Animation SpriteSheetBuilder::buildAnimation(const std::string& name,
                                            int startX, int startY,
                                            int frameWidth, int frameHeight,
                                            int frameCount, float frameDuration,
                                            int columns) {
    Animation anim(name);
    
    if (columns <= 0) {
        columns = textureWidth / frameWidth;
    }
    
    for (int i = 0; i < frameCount; i++) {
        int col = i % columns;
        int row = i / columns;
        Rectangle rect(
            startX + col * frameWidth,
            startY + row * frameHeight,
            frameWidth,
            frameHeight
        );
        anim.addFrame(rect, frameDuration);
    }
    
    return anim;
}

Animation SpriteSheetBuilder::buildCustomAnimation(const std::string& name,
                                                   const std::vector<Rectangle>& frames,
                                                   float frameDuration) {
    Animation anim(name);
    for (const auto& rect : frames) {
        anim.addFrame(rect, frameDuration);
    }
    return anim;
}

Animation SpriteSheetBuilder::buildFromTag(const std::string& name,
                                          const std::string& tag,
                                          float frameDuration) {
    // v0.0.1: Stub - would parse Aseprite JSON
    return Animation(name);
}

// ========== LIGHTING SYSTEM ==========

Light::Light(Type type) : type(type) {}

void Light::update(float deltaTime) {
    if (flicker) {
        flickerTimer += deltaTime * flickerSpeed;
        float noise = std::sin(flickerTimer) * 0.5f + 0.5f;
        currentIntensity = intensity * (1.0f - flickerAmount * noise);
    } else {
        currentIntensity = intensity;
    }
}

LightingSystem::LightingSystem() {}
LightingSystem::~LightingSystem() {}

void LightingSystem::update(float deltaTime) {
    for (auto* light : lights) {
        light->update(deltaTime);
    }
}

void LightingSystem::render() {
    // v0.0.1: Stub
}

void LightingSystem::registerLight(Light* light) {
    lights.push_back(light);
}

void LightingSystem::unregisterLight(Light* light) {
    lights.erase(std::remove(lights.begin(), lights.end(), light), lights.end());
}

Color LightingSystem::getLightingAt(const Vector2& position, int layer) const {
    return calculateLighting(position, layer);
}

float LightingSystem::getBrightnessAt(const Vector2& position) const {
    // v0.0.1: Stub
    return 1.0f;
}

Color LightingSystem::calculateLighting(const Vector2& position, int layer) const {
    Color result = ambientColor.withAlpha(255);
    // v0.0.1: Stub - would accumulate all lights
    return result;
}

float LightingSystem::calculatePaperLayerShading(int layer) const {
    if (!enablePaperShading) return 1.0f;
    // Lower layers are slightly darker (paper stacking shadow)
    return 1.0f - (layer * 0.05f);
}

PaperShadowCaster::PaperShadowCaster() {}

void PaperShadowCaster::update(float deltaTime) {
    if (castShadow) {
        renderShadow();
    }
}

void PaperShadowCaster::renderShadow() {
    // v0.0.1: Stub
}

CelShader::CelShader() {}

void CelShader::update(float deltaTime) {
    if (wobbleOutline) {
        wobbleTimer += deltaTime * wobbleSpeed;
    }
}

void LightProbe::update(float deltaTime, LightingSystem* lightingSystem, int layer) {
    timer += deltaTime;
    if (timer >= updateInterval) {
        timer = 0.0f;
        cachedLighting = lightingSystem->getLightingAt(position, layer);
    }
}

// ========== 3D BACKGROUND ==========

Background3D::Background3D(Type type) : type(type) {}

void Background3D::update(float deltaTime) {
    if (autoRotate) {
        rotation += rotationSpeed * deltaTime;
    }
}

RotoscopedBackground::RotoscopedBackground() {}

void RotoscopedBackground::update(float deltaTime) {
    if (!playing || frames.empty()) return;
    
    frameTimer += deltaTime;
    if (frameTimer >= frameDuration) {
        frameTimer = 0.0f;
        currentFrame++;
        if (currentFrame >= static_cast<int>(frames.size())) {
            if (loop) {
                currentFrame = 0;
            } else {
                currentFrame = static_cast<int>(frames.size()) - 1;
                playing = false;
            }
        }
    }
}

void RotoscopedBackground::play() { playing = true; }
void RotoscopedBackground::pause() { playing = false; }
void RotoscopedBackground::stop() { playing = false; currentFrame = 0; }
void RotoscopedBackground::setFrame(int frame) { currentFrame = frame; }

PerspectiveGrid::PerspectiveGrid() {}

void PerspectiveGrid::update(float deltaTime) {
    if (scroll) {
        scrollOffset += scrollSpeed * deltaTime;
    }
}

Background3DManager::Background3DManager() {}
Background3DManager::~Background3DManager() {}

void Background3DManager::update(float deltaTime) {
    for (auto& bg : backgrounds) {
        bg.background->update(deltaTime);
    }
}

void Background3DManager::render() {
    // v0.0.1: Stub
}

void Background3DManager::addBackground(Background3D* background, int layer) {
    backgrounds.push_back({background, layer, 1.0f});
    sortBackgrounds();
}

void Background3DManager::removeBackground(Background3D* background) {
    backgrounds.erase(
        std::remove_if(backgrounds.begin(), backgrounds.end(),
            [background](const BackgroundLayer& bl) { return bl.background == background; }),
        backgrounds.end()
    );
}

void Background3DManager::clearBackgrounds() {
    backgrounds.clear();
}

void Background3DManager::setLayerDepth(int layer, float depth) {
    for (auto& bg : backgrounds) {
        if (bg.layer == layer) {
            bg.depth = depth;
        }
    }
}

float Background3DManager::getLayerDepth(int layer) const {
    for (const auto& bg : backgrounds) {
        if (bg.layer == layer) {
            return bg.depth;
        }
    }
    return 1.0f;
}

void Background3DManager::setCameraPosition(const Vector2& position) {
    cameraPosition = position;
}

void Background3DManager::sortBackgrounds() {
    std::sort(backgrounds.begin(), backgrounds.end(),
        [](const BackgroundLayer& a, const BackgroundLayer& b) {
            return a.layer < b.layer;
        });
}

Vector2 Depth2D::toScreenPosition(const Vector2& cameraPos, float perspectiveStrength) const {
    float scale = getDepthScale();
    Vector2 offset = (position3D - cameraPos) * perspectiveStrength * scale;
    return position3D + offset;
}

float Depth2D::getDepthScale() const {
    return 1.0f / (1.0f + depth);
}

DepthSprite::DepthSprite() {}

void DepthSprite::update(float deltaTime) {
    // v0.0.1: Stub
}

// ========== FRAME PACING ==========

FramePacer::FramePacer() : mode(PacingMode::VSync), targetFPS(60), adaptiveSync(true) {}
FramePacer::~FramePacer() {}

void FramePacer::initialize(int fps, bool adaptive) {
    targetFPS = fps;
    adaptiveSync = adaptive;
}

void FramePacer::beginFrame() {
    frameStart = std::chrono::high_resolution_clock::now();
}

void FramePacer::endFrame() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - frameStart;
    float frameTime = elapsed.count();
    
    updateFrameHistory(frameTime);
    updateAdaptiveQuality();
    
    stats.actualFrameTime = frameTime;
    stats.deltaTime = frameTime;
    stats.fps = static_cast<int>(1.0f / frameTime);
    
    lastFrameEnd = now;
}

void FramePacer::setPacingMode(PacingMode m) {
    mode = m;
}

float FramePacer::getSmoothedDeltaTime() const {
    if (frameTimeHistory.empty()) return stats.deltaTime;
    
    float sum = 0.0f;
    for (float t : frameTimeHistory) {
        sum += t;
    }
    return sum / frameTimeHistory.size();
}

float FramePacer::predictNextFrameTime() const {
    // v0.0.1: Simple average
    return getSmoothedDeltaTime();
}

bool FramePacer::shouldSkipFrame() const {
    return false; // v0.0.1: No frame skipping
}

float FramePacer::getInterpolationAlpha() const {
    return interpolationAlpha;
}

void FramePacer::enableMotionSmoothing(bool enable, int bufferFrames) {
    motionSmoothing = enable;
    smoothingBufferSize = bufferFrames;
}

bool FramePacer::shouldReduceQuality() const {
    return consecutiveSlowFrames > 10;
}

float FramePacer::getQualityScale() const {
    return currentQualityScale;
}

std::string FramePacer::getDebugString() const {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), 
        "FPS: %d | Frame: %.2fms | Quality: %.0f%%",
        stats.fps, stats.actualFrameTime * 1000.0f, currentQualityScale * 100.0f);
    return std::string(buffer);
}

void FramePacer::updateFrameHistory(float frameTime) {
    frameTimeHistory.push_back(frameTime);
    if (frameTimeHistory.size() > historySize) {
        frameTimeHistory.pop_front();
    }
}

void FramePacer::updateAdaptiveQuality() {
    float targetTime = 1.0f / targetFPS;
    if (stats.actualFrameTime > targetTime * 1.2f) {
        consecutiveSlowFrames++;
        consecutiveFastFrames = 0;
    } else if (stats.actualFrameTime < targetTime * 0.8f) {
        consecutiveFastFrames++;
        consecutiveSlowFrames = 0;
    }
    
    // Adjust quality
    if (consecutiveSlowFrames > 5 && currentQualityScale > 0.5f) {
        currentQualityScale -= 0.1f;
    } else if (consecutiveFastFrames > 30 && currentQualityScale < 1.0f) {
        currentQualityScale += 0.1f;
    }
}

float FramePacer::calculateTargetFrameTime() const {
    return 1.0f / targetFPS;
}

void FramePacer::waitForTargetFrameTime(float targetTime) {
    // v0.0.1: Stub
}

SubFrameInterpolation::SubFrameInterpolation() {}

void SubFrameInterpolation::update(float deltaTime) {
    accumulator += deltaTime;
}

Vector2 SubFrameInterpolation::interpolatePosition(const Vector2& prev, const Vector2& current) const {
    if (!enabled || stateHistory.size() < 2) return current;
    return prev.lerp(current, accumulator);
}

float SubFrameInterpolation::interpolateRotation(float prev, float current) const {
    if (!enabled) return current;
    return prev + (current - prev) * accumulator;
}

void SubFrameInterpolation::recordFrameState(const Vector2& position, float rotation) {
    FrameState state;
    state.position = position;
    state.rotation = rotation;
    state.timestamp = accumulator;
    stateHistory.push_back(state);
    
    if (stateHistory.size() > static_cast<size_t>(interpolationFrames + 1)) {
        stateHistory.pop_front();
    }
}

FrameTimeSmoothing::FrameTimeSmoothing(SmoothingMethod m) : method(m) {}

float FrameTimeSmoothing::smooth(float rawDeltaTime) {
    history.push_back(rawDeltaTime);
    if (history.size() > 10) {
        history.pop_front();
    }
    
    switch (method) {
        case SmoothingMethod::None:
            return rawDeltaTime;
        case SmoothingMethod::MovingAverage:
            return movingAverage();
        case SmoothingMethod::ExponentialSmoothing:
            return exponentialSmooth(rawDeltaTime);
        case SmoothingMethod::Median:
            return medianFilter();
    }
    return rawDeltaTime;
}

void FrameTimeSmoothing::reset() {
    history.clear();
    smoothedValue = 0.0f;
}

float FrameTimeSmoothing::movingAverage() {
    float sum = 0.0f;
    for (float v : history) sum += v;
    return sum / history.size();
}

float FrameTimeSmoothing::exponentialSmooth(float value) {
    smoothedValue = alpha * smoothedValue + (1.0f - alpha) * value;
    return smoothedValue;
}

float FrameTimeSmoothing::medianFilter() {
    std::vector<float> sorted(history.begin(), history.end());
    std::sort(sorted.begin(), sorted.end());
    return sorted[sorted.size() / 2];
}

InputLatencyReduction::InputLatencyReduction() {}

Vector2 InputLatencyReduction::predictPosition(const Vector2& currentPos,
                                               const Vector2& velocity,
                                               float predictionTime) const {
    return currentPos + velocity * predictionTime * predictionWeight;
}

void InputLatencyReduction::optimizeForLatency(FramePacer* pacer) {
    pacer->setPacingMode(FramePacer::PacingMode::Predictive);
}

} // namespace Paper
