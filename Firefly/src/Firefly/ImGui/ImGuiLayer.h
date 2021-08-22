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

        void OnDetach() override;
        void OnAttach() override;
        void OnEvent(Event& e) override;

        void Begin();
        void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }

    private:
        bool  m_BlockEvents = true;
        float m_Time        = 0.0f;
    };
}