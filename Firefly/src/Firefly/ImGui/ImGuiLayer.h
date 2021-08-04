#pragma once

#include "Firefly/Core/Layer.h"
#include "Firefly/Events/ApplicationEvent.h"
#include "Firefly/Events/KeyEvent.h"
#include "Firefly/Events/MouseEvent.h"

namespace Firefly
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnDetach() override;
        virtual void OnAttach() override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
    };
}