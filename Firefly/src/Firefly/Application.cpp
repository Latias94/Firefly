#include "Application.h"

#include "Firefly/Events/ApplicationEvent.h"
#include "Firefly/Log.h"

namespace Firefly
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);
        FF_TRACE(e);
        while (true);
    }
}
