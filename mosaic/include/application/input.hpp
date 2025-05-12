#pragma once

#include <SDL3/SDL.h>

#include "../utilities/vector.hpp"

#include <unordered_map>

namespace Mosaic
{
    struct ApplicationData;

    enum class Key
    {
        Q = SDL_SCANCODE_Q,
        W = SDL_SCANCODE_W,
        E = SDL_SCANCODE_E,
        R = SDL_SCANCODE_R,
        T = SDL_SCANCODE_T,
        Y = SDL_SCANCODE_Y,
        U = SDL_SCANCODE_U,
        I = SDL_SCANCODE_I,
        O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P,
        A = SDL_SCANCODE_A,
        S = SDL_SCANCODE_S,
        D = SDL_SCANCODE_D,
        F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G,
        H = SDL_SCANCODE_H,
        J = SDL_SCANCODE_J,
        K = SDL_SCANCODE_K,
        L = SDL_SCANCODE_L,
        Z = SDL_SCANCODE_Z,
        X = SDL_SCANCODE_X,
        C = SDL_SCANCODE_C,
        V = SDL_SCANCODE_V,
        B = SDL_SCANCODE_B,
        N = SDL_SCANCODE_N,
        M = SDL_SCANCODE_M,

        Num0 = SDL_SCANCODE_0,
        Num1 = SDL_SCANCODE_1,
        Num2 = SDL_SCANCODE_2,
        Num3 = SDL_SCANCODE_3,
        Num4 = SDL_SCANCODE_4,
        Num5 = SDL_SCANCODE_5,
        Num6 = SDL_SCANCODE_6,
        Num7 = SDL_SCANCODE_7,
        Num8 = SDL_SCANCODE_8,
        Num9 = SDL_SCANCODE_9,

        F1 = SDL_SCANCODE_F1,
        F2 = SDL_SCANCODE_F2,
        F3 = SDL_SCANCODE_F3,
        F4 = SDL_SCANCODE_F4,
        F5 = SDL_SCANCODE_F5,
        F6 = SDL_SCANCODE_F6,
        F7 = SDL_SCANCODE_F7,
        F8 = SDL_SCANCODE_F8,
        F9 = SDL_SCANCODE_F9,
        F10 = SDL_SCANCODE_F10,
        F11 = SDL_SCANCODE_F11,
        F12 = SDL_SCANCODE_F12,

        Up = SDL_SCANCODE_UP,
        Down = SDL_SCANCODE_DOWN,
        Left = SDL_SCANCODE_LEFT,
        Right = SDL_SCANCODE_RIGHT,

        LeftShift = SDL_SCANCODE_LSHIFT,
        RightShift = SDL_SCANCODE_RSHIFT,
        LeftCtrlCmd = SDL_SCANCODE_LCTRL,
        RightCtrlCmd = SDL_SCANCODE_RCTRL,
        LeftAlt = SDL_SCANCODE_LALT,
        RightAlt = SDL_SCANCODE_RALT,
        LeftGUI = SDL_SCANCODE_LGUI,
        RightGUI = SDL_SCANCODE_RGUI,
        LeftBracket = SDL_SCANCODE_LEFTBRACKET,
        RightBracket = SDL_SCANCODE_RIGHTBRACKET,

        Esc = SDL_SCANCODE_ESCAPE,
        Enter = SDL_SCANCODE_RETURN,
        Backspace = SDL_SCANCODE_BACKSPACE,
        Tab = SDL_SCANCODE_TAB,
        CapsLock = SDL_SCANCODE_CAPSLOCK,
        Space = SDL_SCANCODE_SPACE,
        Grave = SDL_SCANCODE_GRAVE,
        Minus = SDL_SCANCODE_MINUS,
        Equals = SDL_SCANCODE_EQUALS,

        Backslash = SDL_SCANCODE_BACKSLASH,
        Semicolon = SDL_SCANCODE_SEMICOLON,
        Apostrophe = SDL_SCANCODE_APOSTROPHE,
        Comma = SDL_SCANCODE_COMMA,
        Period = SDL_SCANCODE_PERIOD,
        Slash = SDL_SCANCODE_SLASH,
        Insert = SDL_SCANCODE_INSERT,
        Delete = SDL_SCANCODE_DELETE,
        Home = SDL_SCANCODE_HOME,
        End = SDL_SCANCODE_END,
        PageUp = SDL_SCANCODE_PAGEUP,
        PageDown = SDL_SCANCODE_PAGEDOWN,
        PrintScreen = SDL_SCANCODE_PRINTSCREEN,
        ScrollLock = SDL_SCANCODE_SCROLLLOCK,
        Pause = SDL_SCANCODE_PAUSE,
        Menu = SDL_SCANCODE_APPLICATION,

        NumLockClear = SDL_SCANCODE_NUMLOCKCLEAR,
        NumpadDivide = SDL_SCANCODE_KP_DIVIDE,
        NumpadMultiply = SDL_SCANCODE_KP_MULTIPLY,
        NumpadMinus = SDL_SCANCODE_KP_MINUS,
        NumpadPlus = SDL_SCANCODE_KP_PLUS,
        NumpadEnter = SDL_SCANCODE_KP_ENTER,
        NumpadPeriod = SDL_SCANCODE_KP_PERIOD,

        Numpad1 = SDL_SCANCODE_KP_1,
        Numpad2 = SDL_SCANCODE_KP_2,
        Numpad3 = SDL_SCANCODE_KP_3,
        Numpad4 = SDL_SCANCODE_KP_4,
        Numpad5 = SDL_SCANCODE_KP_5,
        Numpad6 = SDL_SCANCODE_KP_6,
        Numpad7 = SDL_SCANCODE_KP_7,
        Numpad8 = SDL_SCANCODE_KP_8,
        Numpad9 = SDL_SCANCODE_KP_9,
        Numpad0 = SDL_SCANCODE_KP_0,
    };

    enum class MouseButton
    {
        LeftClick = SDL_BUTTON_LEFT,
        RightClick = SDL_BUTTON_RIGHT,
        MiddleClick = SDL_BUTTON_MIDDLE,
    };

    enum class Event
    {
        Press,
        Hold,
        Release,
    };

    struct KeyInput
    {
        Key Keycode;
        Event Type;
    };

    struct MouseInput
    {
        MouseButton Button;
        Event Type;
    };

    struct CursorMovement
    {
        Vector2<float> ScreenSpacePosition;
        Vector2<float> DeviceCoordPosition;
    };

    struct ButtonState
    {
        bool IsDown = false;
        bool WasDownLastFrame = false;
    };

    class InputManager
    {
    public:
        InputManager(ApplicationData& applicationData);

    private:
        void Update();

        void EmitMouseEvents();
        void EmitCursorEvents();
        void EmitKeyEvents();

        ApplicationData* mApplicationData;

        std::unordered_map<Key, ButtonState> mKeyStates;
        std::unordered_map<MouseButton, ButtonState> mMouseButtonStates;

        friend class Application;
    };
}
