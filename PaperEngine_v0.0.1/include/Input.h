#pragma once

#include "Vector2.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include <unordered_set>

namespace Paper {

/**
 * InputManager - Handles keyboard and mouse input
 */
class InputManager {
public:
    enum class MouseButton {
        Left = SDL_BUTTON_LEFT,
        Middle = SDL_BUTTON_MIDDLE,
        Right = SDL_BUTTON_RIGHT
    };

    InputManager();
    ~InputManager();

    void update();
    void processEvent(const SDL_Event& event);

    // Keyboard input
    bool isKeyPressed(SDL_Scancode key) const;
    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyReleased(SDL_Scancode key) const;

    // Mouse input
    bool isMouseButtonPressed(MouseButton button) const;
    bool isMouseButtonDown(MouseButton button) const;
    bool isMouseButtonReleased(MouseButton button) const;

    Vector2 getMousePosition() const { return mousePosition; }
    Vector2 getMouseDelta() const { return mouseDelta; }
    int getMouseWheelDelta() const { return mouseWheelDelta; }

    // Text input
    void startTextInput();
    void stopTextInput();
    bool isTextInputActive() const { return textInputActive; }
    const std::string& getTextInput() const { return textInput; }

private:
    // Keyboard state
    std::unordered_set<SDL_Scancode> keysPressed;
    std::unordered_set<SDL_Scancode> keysDown;
    std::unordered_set<SDL_Scancode> keysReleased;

    // Mouse state
    std::unordered_set<MouseButton> mouseButtonsPressed;
    std::unordered_set<MouseButton> mouseButtonsDown;
    std::unordered_set<MouseButton> mouseButtonsReleased;

    Vector2 mousePosition{0, 0};
    Vector2 lastMousePosition{0, 0};
    Vector2 mouseDelta{0, 0};
    int mouseWheelDelta = 0;

    // Text input
    bool textInputActive = false;
    std::string textInput;
};

// Common key aliases for easier access
namespace Key {
    constexpr SDL_Scancode A = SDL_SCANCODE_A;
    constexpr SDL_Scancode B = SDL_SCANCODE_B;
    constexpr SDL_Scancode C = SDL_SCANCODE_C;
    constexpr SDL_Scancode D = SDL_SCANCODE_D;
    constexpr SDL_Scancode E = SDL_SCANCODE_E;
    constexpr SDL_Scancode F = SDL_SCANCODE_F;
    constexpr SDL_Scancode G = SDL_SCANCODE_G;
    constexpr SDL_Scancode H = SDL_SCANCODE_H;
    constexpr SDL_Scancode I = SDL_SCANCODE_I;
    constexpr SDL_Scancode J = SDL_SCANCODE_J;
    constexpr SDL_Scancode K = SDL_SCANCODE_K;
    constexpr SDL_Scancode L = SDL_SCANCODE_L;
    constexpr SDL_Scancode M = SDL_SCANCODE_M;
    constexpr SDL_Scancode N = SDL_SCANCODE_N;
    constexpr SDL_Scancode O = SDL_SCANCODE_O;
    constexpr SDL_Scancode P = SDL_SCANCODE_P;
    constexpr SDL_Scancode Q = SDL_SCANCODE_Q;
    constexpr SDL_Scancode R = SDL_SCANCODE_R;
    constexpr SDL_Scancode S = SDL_SCANCODE_S;
    constexpr SDL_Scancode T = SDL_SCANCODE_T;
    constexpr SDL_Scancode U = SDL_SCANCODE_U;
    constexpr SDL_Scancode V = SDL_SCANCODE_V;
    constexpr SDL_Scancode W = SDL_SCANCODE_W;
    constexpr SDL_Scancode X = SDL_SCANCODE_X;
    constexpr SDL_Scancode Y = SDL_SCANCODE_Y;
    constexpr SDL_Scancode Z = SDL_SCANCODE_Z;
    
    constexpr SDL_Scancode Space = SDL_SCANCODE_SPACE;
    constexpr SDL_Scancode Enter = SDL_SCANCODE_RETURN;
    constexpr SDL_Scancode Escape = SDL_SCANCODE_ESCAPE;
    constexpr SDL_Scancode Tab = SDL_SCANCODE_TAB;
    constexpr SDL_Scancode Backspace = SDL_SCANCODE_BACKSPACE;
    
    constexpr SDL_Scancode Up = SDL_SCANCODE_UP;
    constexpr SDL_Scancode Down = SDL_SCANCODE_DOWN;
    constexpr SDL_Scancode Left = SDL_SCANCODE_LEFT;
    constexpr SDL_Scancode Right = SDL_SCANCODE_RIGHT;
    
    constexpr SDL_Scancode LShift = SDL_SCANCODE_LSHIFT;
    constexpr SDL_Scancode RShift = SDL_SCANCODE_RSHIFT;
    constexpr SDL_Scancode LCtrl = SDL_SCANCODE_LCTRL;
    constexpr SDL_Scancode RCtrl = SDL_SCANCODE_RCTRL;
    constexpr SDL_Scancode LAlt = SDL_SCANCODE_LALT;
    constexpr SDL_Scancode RAlt = SDL_SCANCODE_RALT;
}

} // namespace Paper
