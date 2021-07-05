#pragma once

#include "Firefly/Window.h"
#include "GLFW/glfw3.h"

struct GLFWwindow;

namespace Firefly
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps &props);

        virtual ~WindowsWindow();
        virtual void OnUpdate() override;

        inline virtual uint32_t GetWidth() const override { return m_Data.Width; }

        inline virtual uint32_t GetHeight() const override { return m_Data.Height; }

        inline void SetEventCallback(const EventCallbackFn &callback) override
        {
            m_Data.EventCallback = callback;
        };

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const override { return m_Window; }

    private:
        virtual void Init(const WindowProps &props);
        virtual void Shutdown();

    private:
        GLFWwindow *m_Window;

        struct WindowData
        {
            std::string Title;
            uint32_t    Width, Height;
            bool        VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}