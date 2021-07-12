#pragma once

#include "Core.h"

#include "Window.h"
#include "Firefly/LayerStack.h"
#include "Firefly/Events/Event.h"
#include "Firefly/Events/ApplicationEvent.h"

#include "Firefly/ImGui/ImGuiLayer.h"
#include "Firefly/Renderer/Shader.h"
#include "Firefly/Renderer/Buffer.h"
#include "Firefly/Renderer/VertexArray.h"

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

        std::shared_ptr<Shader>       m_Shader;
        std::shared_ptr<VertexArray>  m_VertexArray;

        std::shared_ptr<Shader>      m_BlueShader;
        std::shared_ptr<VertexArray> m_SquareVA;

    private:
        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
