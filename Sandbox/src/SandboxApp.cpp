#include <Firefly.h>
#include <Firefly/Core/EntryPoint.h>

#include "Layers/Sandbox2D.h"
#include "Layers/ExampleLayer.h"

class Sandbox : public Firefly::Application
{
public :
    Sandbox()
    {
//        PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }

    ~Sandbox()
    {
    }
};

Firefly::Application* Firefly::CreateApplication()
{
    return new Sandbox();
}
