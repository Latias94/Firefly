#pragma once

#include "Firefly/Core/Input.h"

namespace Firefly
{
    class WindowInput : public Input
    {
    protected:
        virtual bool IsKeyPressedImpl(int keycode);
        virtual bool IsMouseButtonPressedImpl(int button);
        virtual std::pair<float, float> GetMousePositionImpl();
        virtual float GetMouseXImpl();
        virtual float GetMouseYImpl();
    };
}