#pragma once

#include "Firefly/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"

struct GLFWwindow;

namespace Firefly
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* m_WindowHandle;
    };
}