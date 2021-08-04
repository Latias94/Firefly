#include "ffpch.h"
#include "Firefly/Core/Application.h"
#include "Firefly/Core/Input.h"
#include "Firefly/Renderer/Renderer.h"
#include <GLFW/glfw3.h>

namespace Firefly
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        FF_PROFILE_FUNCTION();

        FF_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Window::Create();
        m_Window->SetEventCallback(FF_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        FF_PROFILE_FUNCTION();

        Renderer::Shutdown();
    }

    void Application::PushLayer(Layer* layer)
    {
        FF_PROFILE_FUNCTION();

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        FF_PROFILE_FUNCTION();

        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        FF_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        // if event type is close event, call OnWindowClose
        dispatcher.Dispatch<WindowCloseEvent>(FF_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(FF_BIND_EVENT_FN(Application::OnWindowResize));

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
        FF_PROFILE_FUNCTION();

        while (m_Running)
        {
            FF_PROFILE_SCOPE("RunLoop");

            float time = (float) glfwGetTime(); // will be replace with different Platform's Platform::GetTime

            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;

            if (!m_Minimized)
            {
                {
                    FF_PROFILE_SCOPE("LayerStack OnUpdate");

                    for (Layer* layer: m_LayerStack)
                        layer->OnUpdate(timestep);
                }

                m_ImGuiLayer->Begin();
                {
                    FF_PROFILE_SCOPE("LayerStack OnImGuiUpdate");

                    for (Layer* layer : m_LayerStack)
                        layer->OnImGuiRender();
                }
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        FF_PROFILE_FUNCTION();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }
        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}
