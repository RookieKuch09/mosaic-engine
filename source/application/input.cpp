#include "application/input.hpp"
#include "application/events.hpp"

namespace Mosaic::Internal
{
    InputManager::InputManager(EventManager& eventManager)
        : mEventManager(eventManager)
    {
        mEventManager.Subscribe(this, &InputManager::OnWindowResize);
    }

    void InputManager::Update()
    {
        EmitKeyEvents();
        EmitMouseEvents();
        EmitCursorEvents();
    }

    void InputManager::EmitKeyEvents()
    {
        const bool* currentKeys = SDL_GetKeyboardState(nullptr);

        for (Types::UI32 scancode = 0; scancode < SDL_SCANCODE_COUNT; scancode++)
        {
            InputKey key = FromKeyScancode(static_cast<SDL_Scancode>(scancode));

            ButtonState& state = mKeyStates[key];

            state.IsDown = currentKeys[scancode];

            if (state.IsDown and not state.WasDownLastFrame)
            {
                mEventManager.Emit<KeyInputEvent>({key, InputEventType::Press});
            }
            else if (state.IsDown and state.WasDownLastFrame)
            {
                mEventManager.Emit<KeyInputEvent>({key, InputEventType::Hold});
            }
            else if (state.WasDownLastFrame and not state.IsDown)
            {
                mEventManager.Emit<KeyInputEvent>({key, InputEventType::Release});
            }

            state.WasDownLastFrame = state.IsDown;
        }
    }

    void InputManager::EmitMouseEvents()
    {
        Types::UI32 mouseState = SDL_GetMouseState(nullptr, nullptr);

        auto handleButton = [&](InputMouseButton button, Types::UI32 sdlButtonMask)
        {
            bool isDown = mouseState bitand SDL_BUTTON_MASK(sdlButtonMask);

            ButtonState& state = mMouseButtonStates[button];

            if (isDown)
            {
                if (not state.IsDown and not state.WasDownLastFrame)
                {
                    mEventManager.Emit<MouseInputEvent>({button, InputEventType::Press});
                }
                else if (state.IsDown and state.WasDownLastFrame)
                {
                    mEventManager.Emit<MouseInputEvent>({button, InputEventType::Hold});
                }

                state.IsDown = true;
            }
            else
            {
                if (state.IsDown and state.WasDownLastFrame)
                {
                    mEventManager.Emit<MouseInputEvent>({button, InputEventType::Release});
                }

                state.IsDown = false;
            }

            state.WasDownLastFrame = state.IsDown;
        };

        handleButton(InputMouseButton::LeftClick, SDL_BUTTON_LEFT);
        handleButton(InputMouseButton::RightClick, SDL_BUTTON_RIGHT);
        handleButton(InputMouseButton::MiddleClick, SDL_BUTTON_MIDDLE);
    }

    void InputManager::EmitCursorEvents()
    {
        Types::F32 x, y;
        SDL_GetMouseState(&x, &y);

        Types::F32 normx = x / static_cast<float>(mWindowSize.X);
        Types::F32 normy = y / static_cast<float>(mWindowSize.Y);

        Types::F32 posx = (normx * 2.0f) - 1.0f;
        Types::F32 posy = 1.0f - (normy * 2.0f);

        Types::Vec2<float> screenPos(x, y);
        Types::Vec2<float> devicePos(posx, posy);

        CursorMovementEvent movement{.ScreenSpacePosition = screenPos, .DeviceCoordPosition = devicePos};
        mEventManager.Emit<CursorMovementEvent>(movement);
    }

    void InputManager::OnWindowResize(const Windowing::WindowResizeEvent& event)
    {
        mWindowSize = event.Size;
    }

    InputKey InputManager::FromKeyScancode(SDL_Scancode scancode)
    {
        switch (scancode)
        {
            case SDL_SCANCODE_Q:
                return InputKey::Q;
            case SDL_SCANCODE_W:
                return InputKey::W;
            case SDL_SCANCODE_E:
                return InputKey::E;
            case SDL_SCANCODE_R:
                return InputKey::R;
            case SDL_SCANCODE_T:
                return InputKey::T;
            case SDL_SCANCODE_Y:
                return InputKey::Y;
            case SDL_SCANCODE_U:
                return InputKey::U;
            case SDL_SCANCODE_I:
                return InputKey::I;
            case SDL_SCANCODE_O:
                return InputKey::O;
            case SDL_SCANCODE_P:
                return InputKey::P;
            case SDL_SCANCODE_A:
                return InputKey::A;
            case SDL_SCANCODE_S:
                return InputKey::S;
            case SDL_SCANCODE_D:
                return InputKey::D;
            case SDL_SCANCODE_F:
                return InputKey::F;
            case SDL_SCANCODE_G:
                return InputKey::G;
            case SDL_SCANCODE_H:
                return InputKey::H;
            case SDL_SCANCODE_J:
                return InputKey::J;
            case SDL_SCANCODE_K:
                return InputKey::K;
            case SDL_SCANCODE_L:
                return InputKey::L;
            case SDL_SCANCODE_Z:
                return InputKey::Z;
            case SDL_SCANCODE_X:
                return InputKey::X;
            case SDL_SCANCODE_C:
                return InputKey::C;
            case SDL_SCANCODE_V:
                return InputKey::V;
            case SDL_SCANCODE_B:
                return InputKey::B;
            case SDL_SCANCODE_N:
                return InputKey::N;
            case SDL_SCANCODE_M:
                return InputKey::M;

            case SDL_SCANCODE_0:
                return InputKey::Num0;
            case SDL_SCANCODE_1:
                return InputKey::Num1;
            case SDL_SCANCODE_2:
                return InputKey::Num2;
            case SDL_SCANCODE_3:
                return InputKey::Num3;
            case SDL_SCANCODE_4:
                return InputKey::Num4;
            case SDL_SCANCODE_5:
                return InputKey::Num5;
            case SDL_SCANCODE_6:
                return InputKey::Num6;
            case SDL_SCANCODE_7:
                return InputKey::Num7;
            case SDL_SCANCODE_8:
                return InputKey::Num8;
            case SDL_SCANCODE_9:
                return InputKey::Num9;

            case SDL_SCANCODE_F1:
                return InputKey::F1;
            case SDL_SCANCODE_F2:
                return InputKey::F2;
            case SDL_SCANCODE_F3:
                return InputKey::F3;
            case SDL_SCANCODE_F4:
                return InputKey::F4;
            case SDL_SCANCODE_F5:
                return InputKey::F5;
            case SDL_SCANCODE_F6:
                return InputKey::F6;
            case SDL_SCANCODE_F7:
                return InputKey::F7;
            case SDL_SCANCODE_F8:
                return InputKey::F8;
            case SDL_SCANCODE_F9:
                return InputKey::F9;
            case SDL_SCANCODE_F10:
                return InputKey::F10;
            case SDL_SCANCODE_F11:
                return InputKey::F11;
            case SDL_SCANCODE_F12:
                return InputKey::F12;

            case SDL_SCANCODE_UP:
                return InputKey::Up;
            case SDL_SCANCODE_DOWN:
                return InputKey::Down;
            case SDL_SCANCODE_LEFT:
                return InputKey::Left;
            case SDL_SCANCODE_RIGHT:
                return InputKey::Right;

            case SDL_SCANCODE_LSHIFT:
                return InputKey::LeftShift;
            case SDL_SCANCODE_RSHIFT:
                return InputKey::RightShift;
            case SDL_SCANCODE_LCTRL:
                return InputKey::LeftCtrlCmd;
            case SDL_SCANCODE_RCTRL:
                return InputKey::RightCtrlCmd;
            case SDL_SCANCODE_LALT:
                return InputKey::LeftAlt;
            case SDL_SCANCODE_RALT:
                return InputKey::RightAlt;
            case SDL_SCANCODE_LGUI:
                return InputKey::LeftGUI;
            case SDL_SCANCODE_RGUI:
                return InputKey::RightGUI;
            case SDL_SCANCODE_LEFTBRACKET:
                return InputKey::LeftBracket;
            case SDL_SCANCODE_RIGHTBRACKET:
                return InputKey::RightBracket;

            case SDL_SCANCODE_ESCAPE:
                return InputKey::Esc;
            case SDL_SCANCODE_RETURN:
                return InputKey::Enter;
            case SDL_SCANCODE_BACKSPACE:
                return InputKey::Backspace;
            case SDL_SCANCODE_TAB:
                return InputKey::Tab;
            case SDL_SCANCODE_CAPSLOCK:
                return InputKey::CapsLock;
            case SDL_SCANCODE_SPACE:
                return InputKey::Space;
            case SDL_SCANCODE_GRAVE:
                return InputKey::Grave;
            case SDL_SCANCODE_MINUS:
                return InputKey::Minus;
            case SDL_SCANCODE_EQUALS:
                return InputKey::Equals;

            case SDL_SCANCODE_BACKSLASH:
                return InputKey::Backslash;
            case SDL_SCANCODE_SEMICOLON:
                return InputKey::Semicolon;
            case SDL_SCANCODE_APOSTROPHE:
                return InputKey::Apostrophe;
            case SDL_SCANCODE_COMMA:
                return InputKey::Comma;
            case SDL_SCANCODE_PERIOD:
                return InputKey::Period;
            case SDL_SCANCODE_SLASH:
                return InputKey::Slash;
            case SDL_SCANCODE_INSERT:
                return InputKey::Insert;
            case SDL_SCANCODE_DELETE:
                return InputKey::Delete;
            case SDL_SCANCODE_HOME:
                return InputKey::Home;
            case SDL_SCANCODE_END:
                return InputKey::End;
            case SDL_SCANCODE_PAGEUP:
                return InputKey::PageUp;
            case SDL_SCANCODE_PAGEDOWN:
                return InputKey::PageDown;
            case SDL_SCANCODE_PRINTSCREEN:
                return InputKey::PrintScreen;
            case SDL_SCANCODE_SCROLLLOCK:
                return InputKey::ScrollLock;
            case SDL_SCANCODE_PAUSE:
                return InputKey::Pause;
            case SDL_SCANCODE_APPLICATION:
                return InputKey::Menu;

            case SDL_SCANCODE_NUMLOCKCLEAR:
                return InputKey::NumLockClear;
            case SDL_SCANCODE_KP_DIVIDE:
                return InputKey::NumpadDivide;
            case SDL_SCANCODE_KP_MULTIPLY:
                return InputKey::NumpadMultiply;
            case SDL_SCANCODE_KP_MINUS:
                return InputKey::NumpadMinus;
            case SDL_SCANCODE_KP_PLUS:
                return InputKey::NumpadPlus;
            case SDL_SCANCODE_KP_ENTER:
                return InputKey::NumpadEnter;
            case SDL_SCANCODE_KP_PERIOD:
                return InputKey::NumpadPeriod;

            case SDL_SCANCODE_KP_0:
                return InputKey::Numpad0;
            case SDL_SCANCODE_KP_1:
                return InputKey::Numpad1;
            case SDL_SCANCODE_KP_2:
                return InputKey::Numpad2;
            case SDL_SCANCODE_KP_3:
                return InputKey::Numpad3;
            case SDL_SCANCODE_KP_4:
                return InputKey::Numpad4;
            case SDL_SCANCODE_KP_5:
                return InputKey::Numpad5;
            case SDL_SCANCODE_KP_6:
                return InputKey::Numpad6;
            case SDL_SCANCODE_KP_7:
                return InputKey::Numpad7;
            case SDL_SCANCODE_KP_8:
                return InputKey::Numpad8;
            case SDL_SCANCODE_KP_9:
                return InputKey::Numpad9;

            default:
                return InputKey::Unknown;
        }
    }
}
