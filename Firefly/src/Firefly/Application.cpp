#include "ffpch.h"
#include "Application.h"

#include "glad/glad.h"

#include "Input.h"

namespace Firefly
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        FF_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(FF_BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
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
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer: m_LayerStack)
                layer->OnUpdate();

//            auto[x, y] = Input::GetMousePosition();
//            FF_CORE_TRACE("{0}, {1}", x, y);

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
