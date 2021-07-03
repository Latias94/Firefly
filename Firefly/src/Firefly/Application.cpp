#include "ffpch.h"
#include "Application.h"

#include "glad/glad.h"

namespace Firefly
{
    #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        // if event type is close event, call OnWindowClose
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        FF_CORE_TRACE(e);
    }


    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
