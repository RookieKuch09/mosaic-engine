#include "../../include/application/input.hpp"

InputManager::InputManager(WindowData& windowData, EventManager& eventManager)
    : mWindowData(&windowData), mEventManager(&eventManager)
{
}

void InputManager::Update()
{
    EmitKeyEvents();
    EmitMouseEvents();
    EmitCursorEvents();
}

void InputManager::EmitKeyEvents()
{
    const Uint8* currentKeys = SDL_GetKeyboardState(nullptr);

    for (int scancode = 0; scancode < SDL_NUM_SCANCODES; scancode++)
    {
        Key key = static_cast<Key>(scancode);

        ButtonState& state = mKeyStates[key];

        state.IsDown = currentKeys[scancode];

        if (state.IsDown and not state.WasDownLastFrame)
        {
            mEventManager->Emit<KeyInput>({key, Event::Press});
        }
        else if (state.IsDown and state.WasDownLastFrame)
        {
            mEventManager->Emit<KeyInput>({key, Event::Hold});
        }
        else if (state.WasDownLastFrame and not state.IsDown)
        {
            mEventManager->Emit<KeyInput>({key, Event::Release});
        }

        state.WasDownLastFrame = state.IsDown;
    }
}

void InputManager::EmitMouseEvents()
{
    Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);

    auto handleButton = [&](MouseButton button, Uint32 sdlButtonMask)
    {
        bool isDown = mouseState & SDL_BUTTON(sdlButtonMask);
        ButtonState& state = mMouseButtonStates[button];

        if (isDown)
        {
            if (!state.IsDown && !state.WasDownLastFrame)
            {
                mEventManager->Emit<MouseInput>({button, Event::Press});
            }
            else if (state.IsDown && state.WasDownLastFrame)
            {
                mEventManager->Emit<MouseInput>({button, Event::Hold});
            }

            state.IsDown = true;
        }
        else
        {
            if (state.IsDown && state.WasDownLastFrame)
            {
                mEventManager->Emit<MouseInput>({button, Event::Release});
            }

            state.IsDown = false;
        }

        // Update last frame state for next tick
        state.WasDownLastFrame = state.IsDown;
    };

    handleButton(MouseButton::LeftClick, SDL_BUTTON_LEFT);
    handleButton(MouseButton::RightClick, SDL_BUTTON_RIGHT);
    handleButton(MouseButton::MiddleClick, SDL_BUTTON_MIDDLE);
}

void InputManager::EmitCursorEvents()
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    glm::ivec2 screenPos(x, y);

    int width, height;
    SDL_GetWindowSize(mWindowData->Handle, &width, &height);

    glm::fvec2 devicePos = {(2.0 * x / width) - 1.0, 1.0 - (2.0 * y / height)};

    CursorMovement movement{screenPos, devicePos};
    mEventManager->Emit<CursorMovement>(movement);
}