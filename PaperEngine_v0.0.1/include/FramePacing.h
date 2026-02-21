#pragma once

#include <chrono>
#include <deque>
#include <functional>

namespace Paper {

/**
 * FramePacer - Ultra-tight frame pacing for buttery smooth animation
 * Handles variable refresh rates, frame prediction, and motion smoothing
 */
class FramePacer {
public:
    FramePacer();
    ~FramePacer();

    struct FrameStats {
        float deltaTime;
        float targetFrameTime;
        float actualFrameTime;
        float jitter;           // Frame time variance
        int fps;
        int droppedFrames;
        float cpuTime;
        float gpuTime;
    };

    // Initialize with target FPS
    void initialize(int targetFPS, bool adaptiveSync = true);
    
    // Frame timing
    void beginFrame();
    void endFrame();
    
    // Frame pacing strategies
    enum class PacingMode {
        VSync,          // Hardware vsync
        Adaptive,       // Adaptive sync (G-Sync/FreeSync)
        Fixed,          // Fixed timestep
        Variable,       // Variable timestep (capped)
        Predictive      // Frame prediction for ultra-low latency
    };
    void setPacingMode(PacingMode mode);
    PacingMode getPacingMode() const { return mode; }

    // Timing queries
    float getDeltaTime() const { return stats.deltaTime; }
    float getSmoothedDeltaTime() const;  // Smoothed for consistent physics
    FrameStats getStats() const { return stats; }
    
    // Frame prediction (for input lag reduction)
    float predictNextFrameTime() const;
    bool shouldSkipFrame() const;  // For maintaining target FPS
    
    // Motion interpolation (between frames)
    float getInterpolationAlpha() const;
    void enableMotionSmoothing(bool enable, int bufferFrames = 2);
    
    // Adaptive quality (dynamic resolution/effects based on performance)
    bool shouldReduceQuality() const;
    float getQualityScale() const;  // 0.5 = half res, 1.0 = full res
    
    // Debug
    void enableDebugOverlay(bool enable) { debugOverlay = enable; }
    bool isDebugOverlayEnabled() const { return debugOverlay; }
    std::string getDebugString() const;

private:
    PacingMode mode;
    int targetFPS;
    bool adaptiveSync;
    
    // Timing
    std::chrono::high_resolution_clock::time_point frameStart;
    std::chrono::high_resolution_clock::time_point lastFrameEnd;
    FrameStats stats;
    
    // Frame history (for smoothing and prediction)
    std::deque<float> frameTimeHistory;
    static constexpr int historySize = 60;
    
    // Motion smoothing
    bool motionSmoothing = false;
    int smoothingBufferSize = 2;
    float interpolationAlpha = 0.0f;
    
    // Adaptive quality
    float currentQualityScale = 1.0f;
    int consecutiveSlowFrames = 0;
    int consecutiveFastFrames = 0;
    
    // Debug
    bool debugOverlay = false;
    
    // Internal methods
    void updateFrameHistory(float frameTime);
    void updateAdaptiveQuality();
    float calculateTargetFrameTime() const;
    void waitForTargetFrameTime(float targetTime);
};

/**
 * SubFrameInterpolation - Interpolates between rendered frames
 * Allows 60fps gameplay to look like 120fps
 */
class SubFrameInterpolation {
public:
    SubFrameInterpolation();

    void update(float deltaTime);
    
    // Enable/disable interpolation
    void setEnabled(bool enabled) { this->enabled = enabled; }
    bool isEnabled() const { return enabled; }
    
    // Interpolation settings
    void setInterpolationFrames(int frames) { interpolationFrames = frames; }
    int getInterpolationFrames() const { return interpolationFrames; }
    
    // Get interpolated position
    Vector2 interpolatePosition(const Vector2& prev, const Vector2& current) const;
    float interpolateRotation(float prev, float current) const;
    
    // Record frame state
    void recordFrameState(const Vector2& position, float rotation);

private:
    bool enabled = true;
    int interpolationFrames = 2;
    float accumulator = 0.0f;
    
    // Frame history
    struct FrameState {
        Vector2 position;
        float rotation;
        float timestamp;
    };
    std::deque<FrameState> stateHistory;
};

/**
 * FrameTimeSmoothing - Smooths out frame time spikes
 */
class FrameTimeSmoothing {
public:
    enum class SmoothingMethod {
        None,
        MovingAverage,
        ExponentialSmoothing,
        Median
    };

    FrameTimeSmoothing(SmoothingMethod method = SmoothingMethod::ExponentialSmoothing);

    float smooth(float rawDeltaTime);
    void setSmoothingMethod(SmoothingMethod method) { this->method = method; }
    void reset();

private:
    SmoothingMethod method;
    std::deque<float> history;
    float smoothedValue = 0.0f;
    float alpha = 0.9f;  // For exponential smoothing
    
    float movingAverage();
    float exponentialSmooth(float value);
    float medianFilter();
};

/**
 * InputLatencyReduction - Reduces input-to-display latency
 */
class InputLatencyReduction {
public:
    InputLatencyReduction();

    // Predict where entity will be based on input
    Vector2 predictPosition(const Vector2& currentPos, 
                           const Vector2& velocity,
                           float predictionTime) const;
    
    // Early input sampling (before frame start)
    void enableEarlyInputSampling(bool enable) { earlyInput = enable; }
    
    // Frame pacing for minimum latency
    void optimizeForLatency(FramePacer* pacer);

private:
    bool earlyInput = true;
    float predictionWeight = 0.5f;
};

} // namespace Paper
