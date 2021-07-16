#pragma once

#include "Core.h"

#include "Window.h"
#include "Firefly/LayerStack.h"
#include "Firefly/Events/Event.h"
#include "Firefly/Events/ApplicationEvent.h"
#include "Firefly/Core/Timestep.h"

#include "Firefly/ImGui/ImGuiLayer.h"

namespace Firefly
{
    class Application
    {
    public:
        Application();

        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        inline Window& GetWindow() { return *m_Window; }

        inline static Application& Get() { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        bool                    m_Running = true;

        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
