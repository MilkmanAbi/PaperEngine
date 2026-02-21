#pragma once

#include "Entity.h"
#include "Math.h"
#include "Vector2.h"
#include <vector>
#include <string>
#include <functional>

namespace Paper {

class Texture;

/**
 * AnimationFrame - Single frame with timing and optional events
 */
struct AnimationFrame {
    Rectangle sourceRect;
    float duration;         // Frame duration in seconds
    Vector2 offset;         // Frame-specific offset for hand-drawn wobble
    float rotation;         // Frame-specific rotation
    std::string eventName;  // Optional event to trigger
};

/**
 * Animation - Sequence of frames with metadata
 */
class Animation {
public:
    Animation(const std::string& name = "");

    std::string name;
    std::vector<AnimationFrame> frames;
    bool loop = true;
    bool pingPong = false;  // Play forward then backward
    float playbackSpeed = 1.0f;

    // Hand-drawn animation features
    bool handDrawnWobble = false;   // Add slight per-frame jitter
    float wobbleAmount = 1.0f;      // Pixel offset variation
    bool onionSkinning = false;     // Show previous frame ghosted
    float onionSkinAlpha = 0.3f;

    void addFrame(const Rectangle& rect, float duration, const Vector2& offset = Vector2::zero());
    void addFrameEvent(int frameIndex, const std::string& eventName);
    
    int getFrameCount() const { return static_cast<int>(frames.size()); }
    const AnimationFrame& getFrame(int index) const;
};

/**
 * AnimationStateMachine - State-based animation control
 */
class AnimationStateMachine {
public:
    struct State {
        std::string name;
        Animation* animation;
        std::vector<std::string> transitions; // States we can transition to
        std::function<bool()> canExit;        // Optional exit condition
    };

    AnimationStateMachine();

    void addState(const std::string& name, Animation* animation);
    void addTransition(const std::string& from, const std::string& to);
    void setState(const std::string& name);
    
    const std::string& getCurrentState() const { return currentState; }
    Animation* getCurrentAnimation();

private:
    std::unordered_map<std::string, State> states;
    std::string currentState;
};

/**
 * AdvancedAnimator - Enhanced animator with state machine and hand-drawn support
 */
class AdvancedAnimator : public Component {
public:
    AdvancedAnimator();

    void update(float deltaTime) override;

    // Animation control
    void addAnimation(const std::string& name, const Animation& animation);
    void play(const std::string& name, bool forceRestart = false);
    void stop();
    void pause();
    void resume();
    void setPlaybackSpeed(float speed);

    // State machine
    AnimationStateMachine* getStateMachine() { return &stateMachine; }
    
    // Hand-drawn features
    void enableHandDrawnMode(bool enable, float wobbleAmount = 1.0f);
    void enableOnionSkinning(bool enable, float alpha = 0.3f);
    
    // Events
    using FrameEventCallback = std::function<void(const std::string&)>;
    void setFrameEventCallback(FrameEventCallback callback);

    // State
    bool isPlaying() const { return playing; }
    bool isPaused() const { return paused; }
    const std::string& getCurrentAnimation() const { return currentAnimationName; }
    int getCurrentFrame() const { return currentFrame; }
    float getPlaybackSpeed() const { return playbackSpeed; }

private:
    std::unordered_map<std::string, Animation> animations;
    AnimationStateMachine stateMachine;
    
    std::string currentAnimationName;
    int currentFrame = 0;
    float frameTimer = 0.0f;
    bool playing = false;
    bool paused = false;
    float playbackSpeed = 1.0f;
    
    // Hand-drawn
    bool handDrawnMode = false;
    float wobbleAmount = 1.0f;
    Vector2 currentWobble{0, 0};
    
    // Events
    FrameEventCallback frameEventCallback;
    
    SpriteRenderer* spriteRenderer = nullptr;

    void updateAnimation(float deltaTime);
    void applyHandDrawnWobble();
    void triggerFrameEvent(const std::string& eventName);
};

/**
 * SpriteSheetBuilder - Helper for creating animations from sprite sheets
 */
class SpriteSheetBuilder {
public:
    SpriteSheetBuilder(int textureWidth, int textureHeight);

    // Grid-based sprite sheets
    Animation buildAnimation(const std::string& name,
                           int startX, int startY,
                           int frameWidth, int frameHeight,
                           int frameCount,
                           float frameDuration,
                           int columns = -1); // -1 = auto-calculate

    // Custom frame positions
    Animation buildCustomAnimation(const std::string& name,
                                  const std::vector<Rectangle>& frames,
                                  float frameDuration);

    // Aseprite-style tag support (future: parse .json)
    Animation buildFromTag(const std::string& name,
                          const std::string& tag,
                          float frameDuration);

private:
    int textureWidth;
    int textureHeight;
};

} // namespace Paper
