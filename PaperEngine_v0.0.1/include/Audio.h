#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <SDL2/SDL.h>

namespace Paper {

/**
 * AudioClip - Represents a sound effect or music track
 */
class AudioClip {
public:
    AudioClip();
    ~AudioClip();

    bool loadFromFile(const std::string& filepath);
    void free();

    // Playback control
    void play(int loops = 0); // 0 = once, -1 = infinite
    void pause();
    void resume();
    void stop();

    void setVolume(float volume); // 0.0 to 1.0
    float getVolume() const { return volume; }

    bool isPlaying() const;

private:
    void* audioData; // SDL_mixer chunk/music pointer
    float volume = 1.0f;
    bool isMusic = false;
};

/**
 * AudioManager - Manages audio playback and resources
 */
class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool initialize();
    void shutdown();

    // Audio clip management
    AudioClip* loadSound(const std::string& name, const std::string& filepath);
    AudioClip* loadMusic(const std::string& name, const std::string& filepath);
    AudioClip* getSound(const std::string& name);
    AudioClip* getMusic(const std::string& name);

    // Quick playback
    void playSound(const std::string& name, float volume = 1.0f);
    void playMusic(const std::string& name, bool loop = true, float volume = 1.0f);
    void stopMusic();

    // Global volume control
    void setMasterVolume(float volume);
    void setSoundVolume(float volume);
    void setMusicVolume(float volume);

    float getMasterVolume() const { return masterVolume; }
    float getSoundVolume() const { return soundVolume; }
    float getMusicVolume() const { return musicVolume; }

private:
    std::unordered_map<std::string, std::unique_ptr<AudioClip>> sounds;
    std::unordered_map<std::string, std::unique_ptr<AudioClip>> music;

    float masterVolume = 1.0f;
    float soundVolume = 1.0f;
    float musicVolume = 1.0f;

    bool isInitialized = false;
};

} // namespace Paper
