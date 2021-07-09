#include "ffpch.h"
#include "glad/glad.h" // include before glfw.h in WindowsWindow.h
#include "OpenGLContext.h"

namespace Firefly
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
        FF_CORE_ASSERT(windowHandle, "Window handle is null");
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // Load glad
        FF_CORE_ASSERT(status, "Failed to initialize glad!");
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}