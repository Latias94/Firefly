#pragma once

#include "Firefly/Core/Layer.h"
#include "Firefly/Events/ApplicationEvent.h"
#include "Firefly/Events/KeyEvent.h"
#include "Firefly/Events/MouseEvent.h"

namespace Firefly
{
    class FIREFLY_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnDetach() override;
        virtual void OnAttach() override;

        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
    };
}