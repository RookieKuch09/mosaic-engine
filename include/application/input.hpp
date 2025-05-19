#pragma once

#include <SDL3/SDL.h>

#include "application/window.hpp"
#include "utilities/vector.hpp"

#include <unordered_map>

namespace Mosaic::Internal
{
    class EventManager;

    enum class InputKey
    {
        Q,
        W,
        E,
        R,
        T,
        Y,
        U,
        I,
        O,
        P,
        A,
        S,
        D,
        F,
        G,
        H,
        J,
        K,
        L,
        Z,
        X,
        C,
        V,
        B,
        N,
        M,

        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        Up,
        Down,
        Left,
        Right,

        LeftShift,
        RightShift,
        LeftCtrlCmd,
        RightCtrlCmd,
        LeftAlt,
        RightAlt,
        LeftGUI,
        RightGUI,
        LeftBracket,
        RightBracket,

        Esc,
        Enter,
        Backspace,
        Tab,
        CapsLock,
        Space,
        Grave,
        Minus,
        Equals,

        Backslash,
        Semicolon,
        Apostrophe,
        Comma,
        Period,
        Slash,
        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,
        PrintScreen,
        ScrollLock,
        Pause,
        Menu,

        NumLockClear,
        NumpadDivide,
        NumpadMultiply,
        NumpadMinus,
        NumpadPlus,
        NumpadEnter,
        NumpadPeriod,

        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,
        Numpad0,

        Unknown,
    };

    enum class InputMouseButton
    {
        LeftClick,
        RightClick,
        MiddleClick,
        Unknown,
    };

    enum class InputEventType
    {
        Press,
        Hold,
        Release,
    };

    struct KeyInputEvent
    {
        InputKey Keycode;
        InputEventType Type;
    };

    struct MouseInputEvent
    {
        InputMouseButton Button;
        InputEventType Type;
    };

    struct CursorMovementEvent
    {
        Types::Vector2<Types::Float32> ScreenSpacePosition;
        Types::Vector2<Types::Float32> DeviceCoordPosition;
    };

    struct ButtonState
    {
        bool IsDown = false;
        bool WasDownLastFrame = false;
    };

    class InputManager
    {
    public:
        InputManager(EventManager& eventManager);

    private:
        void Update();

        void EmitMouseEvents();
        void EmitCursorEvents();
        void EmitKeyEvents();

        InputKey FromKeyScancode(SDL_Scancode scancode);

        std::unordered_map<InputKey, ButtonState> mKeyStates;
        std::unordered_map<InputMouseButton, ButtonState> mMouseButtonStates;

        Types::Vector2<Types::UInt32> mWindowSize;

        void OnWindowResize(const Windowing::WindowResizeEvent& event);

        EventManager& mEventManager;

        friend class Application;
    };
}
