#include "../../include/application/input.hpp"
#include "../../include/application/application.hpp"

Mosaic::InputManager::InputManager(ApplicationData& applicationData)
    : mApplicationData(&applicationData)
{
}

void Mosaic::InputManager::Update()
{
    EmitKeyEvents();
    EmitMouseEvents();
    EmitCursorEvents();
}

void Mosaic::InputManager::EmitKeyEvents()
{
    const bool* currentKeys = SDL_GetKeyboardState(nullptr);

    for (std::int32_t scancode = 0; scancode < SDL_SCANCODE_COUNT; scancode++)
    {
        Key key = static_cast<Key>(scancode);

        ButtonState& state = mKeyStates[key];

        state.IsDown = currentKeys[scancode];

        if (state.IsDown and not state.WasDownLastFrame)
        {
            mApplicationData->EventManager.Emit<KeyInput>({key, Event::Press});
        }
        else if (state.IsDown and state.WasDownLastFrame)
        {
            mApplicationData->EventManager.Emit<KeyInput>({key, Event::Hold});
        }
        else if (state.WasDownLastFrame and not state.IsDown)
        {
            mApplicationData->EventManager.Emit<KeyInput>({key, Event::Release});
        }

        state.WasDownLastFrame = state.IsDown;
    }
}

void Mosaic::InputManager::EmitMouseEvents()
{
    Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);

    auto handleButton = [&](MouseButton button, Uint32 sdlButtonMask)
    {
        bool isDown = mouseState bitand SDL_BUTTON_MASK(sdlButtonMask);

        ButtonState& state = mMouseButtonStates[button];

        if (isDown)
        {
            if (not state.IsDown and not state.WasDownLastFrame)
            {
                mApplicationData->EventManager.Emit<MouseInput>({button, Event::Press});
            }
            else if (state.IsDown and state.WasDownLastFrame)
            {
                mApplicationData->EventManager.Emit<MouseInput>({button, Event::Hold});
            }

            state.IsDown = true;
        }
        else
        {
            if (state.IsDown and state.WasDownLastFrame)
            {
                mApplicationData->EventManager.Emit<MouseInput>({button, Event::Release});
            }

            state.IsDown = false;
        }

        state.WasDownLastFrame = state.IsDown;
    };

    handleButton(MouseButton::LeftClick, SDL_BUTTON_LEFT);
    handleButton(MouseButton::RightClick, SDL_BUTTON_RIGHT);
    handleButton(MouseButton::MiddleClick, SDL_BUTTON_MIDDLE);
}

void Mosaic::InputManager::EmitCursorEvents()
{
    float x, y;
    SDL_GetMouseState(&x, &y);

    const glm::uvec2& size = mApplicationData->Window.GetSize();

    glm::uvec2 screenPos(x, y);
    glm::fvec2 devicePos = {(2.0 * x / size.x) - 1.0, 1.0 - (2.0 * y / size.y)};

    CursorMovement movement{screenPos, devicePos};
    mApplicationData->EventManager.Emit<CursorMovement>(movement);
}