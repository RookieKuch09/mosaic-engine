#include "application/input.hpp"
#include "application/application.hpp"

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

    const Vector2<std::uint32_t>& size = mApplicationData->Window.GetSize();

    float normx = x / static_cast<float>(size.X);
    float normy = y / static_cast<float>(size.Y);

    float posx = (normx * 2.0f) - 1.0f;
    float posy = 1.0f - (normy * 2.0f);

    Vector2<float> screenPos(x, y);
    Vector2<float> devicePos(posx, posy);

    CursorMovement movement{.ScreenSpacePosition = screenPos, .DeviceCoordPosition = devicePos};
    mApplicationData->EventManager.Emit<CursorMovement>(movement);
}