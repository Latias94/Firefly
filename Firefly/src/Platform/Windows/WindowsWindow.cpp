#include "ffpch.h"
#include "glad/glad.h" // include before glfw.h in WindowsWindow.h
#include "WindowsWindow.h"

namespace Firefly
{

    static bool s_GLFWInitialized = false;

    Window *Window::Create(const WindowProps &props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps &props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps &props)
    {
        m_Data.Title  = props.Title;
        m_Data.Width  = props.Width;
        m_Data.Height = props.Height;

        FF_CORE_INFO("Creating window {0}, ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            // TODO: glfwTerminate on system shutdown
            int success = glfwInit();
            FF_ASSERT(success, "Could not initialize GLFW!");

            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(props.Width, props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // Load glad
        FF_CORE_ASSERT(status, "Failed to initialize glad!");
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
    }

    void WindowsWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const { return m_Data.VSync; }
}