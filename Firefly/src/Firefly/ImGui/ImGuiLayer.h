#pragma once

#include "Firefly/Layer.h"

namespace Firefly
{
    class FIREFLY_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();

        void OnUpdate();
        void OnEvent(Event& event);

    private:
        float m_Time = 0.0f;
    };
}