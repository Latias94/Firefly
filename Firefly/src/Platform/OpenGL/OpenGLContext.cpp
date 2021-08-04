#include "ffpch.h"
#include "glad/glad.h" // include before glfw.h in WindowsWindow.h
#include "Platform/OpenGL/OpenGLContext.h"

namespace Firefly
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
        FF_CORE_ASSERT(windowHandle, "Window handle is null");
    }

    void OpenGLContext::Init()
    {
        FF_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // Load glad
        FF_CORE_ASSERT(status, "Failed to initialize glad!");

        FF_CORE_INFO("OpenGL Info:");
        FF_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        FF_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        FF_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
    }

    void OpenGLContext::SwapBuffers()
    {
        FF_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}