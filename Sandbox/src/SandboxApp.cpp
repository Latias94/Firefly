#include <Firefly.h>

class ExampleLayer : public Firefly::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {

    }

    void OnUpdate() override
    {
        FF_INFO("ExampleLayer::Update");
    }

    void OnEvent(Firefly::Event& event) override
    {
        FF_TRACE("{0}", event);
    }
};

class Sandbox : public Firefly::Application
{
public :
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Firefly::ImGuiLayer());
    }

    ~Sandbox()
    {
    }
};

Firefly::Application* Firefly::CreateApplication()
{
    return new Sandbox();
}
