#include "ffpch.h"
#include "Application.h"

#include "Firefly/Renderer/Renderer.h"

#include "Input.h"
#include <GLFW/glfw3.h>

namespace Firefly
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        FF_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(FF_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer * layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer * layer)
    {
        m_LayerStack.PushOverlay(layer);
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        // if event type is close event, call OnWindowClose
        dispatcher.Dispatch<WindowCloseEvent>(FF_BIND_EVENT_FN(Application::OnWindowClose));

//        FF_CORE_TRACE(e);

        // Event handled by engine first, then handled by client
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }


    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float) glfwGetTime(); // will be replace with different Platform's Platform::GetTime

            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for (Layer* layer: m_LayerStack)
                layer->OnUpdate(timestep);

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
