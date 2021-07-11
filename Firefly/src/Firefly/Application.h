#pragma once

#include "Core.h"

#include "Window.h"
#include "Firefly/LayerStack.h"
#include "Firefly/Events/Event.h"
#include "Firefly/Events/ApplicationEvent.h"

#include "Firefly/ImGui/ImGuiLayer.h"
#include "Firefly/Renderer/Shader.h"

namespace Firefly
{
    class FIREFLY_API Application
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
        std::unique_ptr<Window> m_Window;
        bool                    m_Running = true;

        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;

        unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
    private:
        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
